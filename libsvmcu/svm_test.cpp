#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "fsl_common.h"
#include "svm.h"
#include "fsl_debug_console.h"
#include "svm_test.h"
#include "FreeRTOS.h"
#include "task.h"

#undef lv_obj_set_size
#undef lv_obj_set_pos

#if defined(__cplusplus)
extern "C" {
#endif

enum {
    SLIDER_GAMMA = 0,
    SLIDER_NU,
};

extern uint32_t get_slider_value(uint32_t value_type);
svm_model* model;

uint32_t svm_model_valid(void* model_ptr){
    svm_model* model = ((svm_model*)model_ptr);
    uint32_t model_len = svm_model_total_size( model );
    if(model->model_magic == SVM_MODEL_MAGIC){
        uint32_t model_tail_magic = *(uint32_t*)(&(((uint8_t*)model_ptr)[model_len]));
        return (model_tail_magic == SVM_MODEL_MAGIC);
    }
    return 0;
}

#ifdef __ARMCC_VERSION
extern uint8_t Image$$RW_m_sramx_start$$Limit[];
extern uint8_t Image$$RW_m_sramx_start$$Base[];

#define SVM_pArena Image$$RW_m_sramx_start$$Base
#define SVM_pArenaSize (Image$$RW_m_sramx_start$$Limit - Image$$RW_m_sramx_start$$Base)

#else
extern uint8_t __base_SRAMX[];
extern uint8_t __top_SRAMX[];
#define SVM_pArena __base_SRAMX
#define SVM_pArenaSize (__top_SRAMX - __base_SRAMX)
#endif

void* svm_train_mode( float train_data[][FEATURE_NUMS],int XLEN)
{

    svm_parameter param;
    int i, j;

    // default values
    param.svm_type = ONE_CLASS;
    param.kernel_type = RBF;
    param.degree = 3;
    param.gamma = get_slider_value(SLIDER_GAMMA);
    param.coef0 = 0;
    param.nu = get_slider_value(SLIDER_NU) / 100.f;
    param.cacheSize = SVM_CFG_CACHE_SIZE;
    param.C = 1;
    param.eps = 1e-3f;
    param.p = 0.1f;
    param.shrinking = 0; // 1;
#if SVM_CFG_FEATURE_PROBABILITY != 0
    param.probability = 0;
#endif
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;

    svm_problem prob;

    prob.n_pts = XLEN;;
    prob.y =  ( SVM_REAL* ) malloc( prob.n_pts * sizeof( SVM_REAL ) );
    prob.x = ( svm_node** ) malloc( prob.n_pts * sizeof( svm_node* ) );
    memset( prob.x, 0, prob.n_pts * sizeof( svm_node* ) );
    if( param.kernel_type == PRECOMPUTED )
    {
    }
    else
    {
        if( param.gamma == 0 ) param.gamma = 0.5f;
        prob.x = ( svm_node** ) malloc( prob.n_pts * sizeof( svm_node* ) );
        memset( prob.x, 0, prob.n_pts * sizeof( svm_node* ) );

        int i, j;
        for ( i = 0; i < prob.n_pts; i++ )
        {
            prob.y[i] = 1;
            prob.x[i] = ( svm_node* )&train_data[i][0];

        }
        // build model & classify
        prob.vecDim = FEATURE_NUMS;
        model = svm_train( &prob, &param, /*isCopySV=*/1,
                           SVM_CFG_CACHE_SIZE + prob.n_pts * 6 * sizeof( SVM_REAL ) * 16, (void*)SVM_pArena);
    }
    model->model_magic = SVM_MODEL_MAGIC;
    model->model_tag = xTaskGetTickCount();
    
    extern uint32_t save_model_to_flash(void* model, uint32_t model_len);
    extern uint32_t get_flash_address();
    
    size_t totalSize = svm_model_total_size( model );
    
    char* pBuf;
    pBuf = ( char* ) malloc ( totalSize + sizeof(model->model_magic));
    *((uint32_t*)(pBuf+totalSize)) = model->model_magic;
    
    svm_model *model_buf = svm_copy_model_to_buffer(pBuf, totalSize, (char*)get_flash_address(), model );
    svm_model *model_on_flash = (svm_model *)save_model_to_flash(model_buf, totalSize + sizeof(model->model_magic));
    
    free ( pBuf );
    free ( prob.y );
    free ( prob.x );
    return (void*)model_on_flash;
}
float svm_data_pre(float pre_data[][FEATURE_NUMS], int Test_Len)
{
        size_t totalSize = svm_model_total_size( model );
        // rocky: do a test of svm_copy_model as if it is loaded from flash
        char* pBuf;
        pBuf = ( char* ) malloc ( totalSize );
        char* pFlash_1;
        pFlash_1 = ( char* ) malloc ( totalSize );

        svm_model *model_buf = svm_copy_model_to_buffer( pBuf, totalSize, pFlash_1, model );
        memcpy( pFlash_1, pBuf, totalSize );  // mimic flash programming
        svm_model *model2 = ( svm_model* )pFlash_1;
//
        model2->pStk = model->pStk;
        model2->pArena = model->pArena;

        svm_node x[FEATURE_NUMS];
        for (int i = 0; i < Test_Len; i++ )
        {
            for ( int j = 0; j < FEATURE_NUMS; j++ )

                x[j].value = pre_data[i][j];

            SVM_REAL d = svm_predict( model2, x );
            if(Test_Len == 1){
                return d;
            }
        }

       //
        free ( pBuf );
        free ( pFlash_1 );
        return 0;

}

float svm_model_pre(void* model, float pre_data[FEATURE_NUMS])
{
    svm_node x[FEATURE_NUMS];
    for ( int j = 0; j < FEATURE_NUMS; j++ )

        x[j].value = pre_data[j];

    SVM_REAL d = svm_predict( (svm_model *)model, x );
    
    return d;
}

void destory_model(void)
{
  svm_free_and_destroy_model( &model );
}

#define SetPixel(buffer, x, y, col)  *(((lv_color_t*)(buffer)) + (y) * toy_w + (x)) = (col);
#define GetPixel(buffer, x, y)  *(((lv_color_t*)(buffer)) + (y) * toy_w + (x))
lv_color_t colors[] =
{
  lv_color_make(0,0,0),
  lv_color_make(80,40,40),
  lv_color_make(40,72,40),
  lv_color_make(40,40,80),
  lv_color_make(65,65,65),
  lv_color_make(255,95,95),
  lv_color_make(95,255,95),
  lv_color_make(95,95,255),
  lv_color_make(200,200,200),
};

#define fabs(x) ((x) < 0 ? (-x) : (x))
extern lv_img_dsc_t svm_toy;
typedef struct {
    float r;       // a fraction between 0 and 1
    float g;       // a fraction between 0 and 1
    float b;       // a fraction between 0 and 1
} rgb_t;

typedef struct {
    float h;       // angle in degrees
    float s;       // a fraction between 0 and 1
    float v;       // a fraction between 0 and 1
} hsv_t;

void HSVtoRGB(float H/*0-360*/, float S/*0-100*/, float V/*0-100*/, int* pR, int* pG, int* pB)
{
    hsv_t in = {H, S / 100.0f, V / 100.0f};
    
    double      hh, p, q, t, ff;
    long        i;
    rgb_t         out;

    hh = in.h;
    if(hh >= 360.0f) hh = 0;
    hh /= 60.0f;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    pR[0] = (int)(out.r * 255.5f);
    pG[0] = (int)(out.g * 255.5f);
    pB[0] = (int)(out.b * 255.5f);
}

#define MyFMod(_X, _Y) ((_X) - (int)((_X) / (float)(_Y)) * (float)(_Y))
void HSVtoRGB_old(float H/*0-360*/, float S/*0-100*/, float V/*0-100*/, int* pR, int* pG, int* pB) {
	if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
		return;
	}
	float s = S / 100.0f;
	float v = V / 100.0f;
	float C = s * v;
	float rgn = H / 60.0f;
	float X = C * (1 - fabs(MyFMod(rgn, 2.0f) - 1.0f));
	float m = v - C;
	float r, g, b;
	if (H >= 0 && H < 60) {
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}
	pR[0] = (r + m) * 255.5f;
	pG[0] = (g + m) * 255.5f;
	pB[0] = (b + m) * 255.5f;
}


void draw_svm_toy_map(void* model, void* buffer_toy, uint32_t toy_w, uint32_t toy_h){
    svm_node x[FEATURE_NUMS];

    for (int i = 0; i < toy_w; i++)
    {
        for (int j = 0; j < toy_h; j++) {
            
            x[0].value = (SVM_REAL)i / toy_w; // powf((SVM_REAL)i / toy_w, 2.0f);
            x[1].value = (SVM_REAL)j / toy_h; // powf((SVM_REAL)j / toy_h, 2.0f);
            SVM_REAL d = svm_predict((svm_model *)model, x);
            if (((svm_model *)model)->param.svm_type == ONE_CLASS) {
              lv_color_t col;
              #if 0  
              int bri;
                if (d > 0) {
                    if (d <= 0.5f) {
                      bri = (int)(fabs(d) * 384) + 64;
                      if (bri > 255)
                          bri = 255;
                      col = lv_color_make(bri / 2, bri, bri / 2);
                    } else {
                      bri = (int)(fabs(d - 0.5f) * 512);
                      if (bri > 255)
                          bri = 255;
                      col = lv_color_make(128, 255 - bri / 4, 128 + bri / 2);
                    }
                }
                else {
                    if (-d <= 0.5f) {
                      bri = (int)(fabs(d) * 384) + 64;
                      if (bri > 255)
                          bri = 255;
                      col = lv_color_make(bri, bri / 2, bri / 2);
                    } else {
                      bri = (int)(fabs(-d - 0.5f) * 512);
                      if (bri > 255)
                          bri = 255;
                      col = lv_color_make(255 - bri / 4, 128, 128 + bri / 2);
                    }
                }
                #else
                int r, g, b;
                float hsv_h;
                if (d > 0) {
                    hsv_h = 75.0f + d * 360.0f;
                    if (hsv_h > 240)
                      hsv_h = 240;
                } else {
                    hsv_h = 60.0f + d * 72.0f;
                    if (hsv_h < 0) {
                        if (hsv_h < -90)
                            hsv_h = -90;
                        hsv_h += 360;
                    }
                }
                HSVtoRGB(hsv_h, 95.0f, 80.0f, &r, &g, &b);
                col = lv_color_make(r, g, b);
                #endif
                SetPixel(buffer_toy, i, j, col);
//                lv_color_t* buf = ((lv_color_t*)(buffer_toy))  + j * toy_w + i ;
//                *(buf) = col;            
           }else {
                        SetPixel(buffer_toy, i, j, colors[(int)d]);
                    }
                }
            }
}

#define draw_cross(buffer, cx, cy, col) \
{ \
    SetPixel(buffer_toy, cx-1, cy-1, col); \
    SetPixel(buffer_toy, cx-1, cy+1, col); \
    SetPixel(buffer_toy, cx-1, cy, col); \
    SetPixel(buffer_toy, cx, cy-1, col); \
    SetPixel(buffer_toy, cx, cy, col); \
    SetPixel(buffer_toy, cx, cy+1, col); \
    SetPixel(buffer_toy, cx+1, cy-1, col); \
    SetPixel(buffer_toy, cx+1, cy, col); \
    SetPixel(buffer_toy, cx+1, cy+1, col); \
}

void update_image_widget(lv_obj_t *img_obj, lv_img_dsc_t* img_dsc){
   if(guider_ui.is_in_MainScr)
   {
       if((((lv_img_t *)(img_obj))->src) == NULL){
           lv_img_set_src(img_obj,img_dsc);
       }else{
           lv_obj_invalidate(img_obj);
       }  
   }
}

#if 1
// when there is only one feature, need do full-image refresh
void draw_points_on_svm_toy(void* buffer_toy, uint32_t toy_w, uint32_t toy_h, svm_features_t* features, lv_color_t col, bool del_old){

    static uint32_t cx_old, cy_old;
    static lv_color_t color_old[9]; // store the back color
    
    static uint32_t point_now;
    float f1 ,f2;
    for(int i=0;i<(features->col);i++){
        f1 = features->p_feature[0 + i*2];
        f2 = features->p_feature[1 + i*2];
       uint32_t cx = (uint32_t)(f1 * toy_w)/*(powf(f1, 0.5f) * toy_w)*/;
       uint32_t cy = (uint32_t)(f2 * toy_h)/*(powf(f2, 0.5f) * toy_h)*/;
       // clip the coordinates to leave 1 pixel's margin to the edges.
       if (cx < 1) cx = 1;
       if (cx >= toy_w - 2)
           cx = toy_w - 2;
       if (cy < 1) cy = 1;
       if (cy >= toy_h - 2)
           cy = toy_h - 2;
       
       if(del_old){
           if(point_now == 1){
               // delete the pre-point
               int j = 0;
               for (int x1=-1; x1<2; x1++){
                    for (int y1=-1; y1<2; y1++) {
                        SetPixel(buffer_toy, cx_old+x1, cy_old+y1, color_old[j++]); 
                    }
               }
               point_now = 0;
               
           }
           point_now += 1;
           
           if((features->col) == 1){
                // need store the back color of the cur_cx, cur_cy
               int j = 0;
               for (int x1=-1; x1<2; x1++){
                    for (int y1=-1; y1<2; y1++) {
                        color_old[j++] = GetPixel(buffer_toy, cx+x1, cy+y1);
                    }
               }
                cx_old = cx, cy_old = cy;
           }
       }
              
       draw_cross(buffer_toy, cx, cy, col);        
    }   
}
#else
// when there is only one feature, treate their as image-obj, and refresh them
void draw_points_on_svm_toy(void* buffer_toy, uint32_t toy_w, uint32_t toy_h, svm_features_t* features, lv_color_t col, bool del_old){

    static uint32_t cx_old, cy_old;
    /*
        0 1 2
        3 4 5
        6 7 8
    */
    static lv_color_t color_pre[9]; // store the back color need to del
    static lv_color_t color_cur[9];  // store the current color need to draw
    static lv_obj_t *pre_feature_img, *cur_feature_img;
    static lv_img_dsc_t pre_feature_img_dsc, cur_feature_img_dsc;
    
   // we use the img-obj, so need to do this first to make sure not in the deleted-main wind
   if(guider_ui.is_in_MainScr)
   {
        uint32_t base_x = guider_ui.Main_svm_toy->coords.x1;
        uint32_t base_y = guider_ui.Main_svm_toy->coords.y1;
        
        static uint32_t point_now;
        
        #define img_obj_wh 4
        if((features->col) == 1){
           if(pre_feature_img == NULL){
               pre_feature_img = lv_img_create(guider_ui.Main);
               lv_obj_set_size(pre_feature_img, img_obj_wh, img_obj_wh);
               pre_feature_img_dsc = (lv_img_dsc_t){
                  .header = (lv_img_header_t){
                                .cf=LV_IMG_CF_TRUE_COLOR,
                                .always_zero = 0,
                                .w = 3,
                                .h = 3
                            },
                  .data_size = 3 * 3 * LV_COLOR_SIZE,
                  .data = (const uint8_t*)color_pre,
                };    
                
           }  
           if(cur_feature_img == NULL){
               cur_feature_img = lv_img_create(guider_ui.Main);
               lv_obj_set_size(cur_feature_img, img_obj_wh, img_obj_wh);
               cur_feature_img_dsc = pre_feature_img_dsc;
               cur_feature_img_dsc.data = (const uint8_t*)color_cur;
               lv_img_set_src(cur_feature_img,&cur_feature_img_dsc);
           } 
        }
        
        for(int i=0;i<(features->col);i++){
           uint32_t cx = (uint32_t)((features->p_feature[0 + i*2]) * toy_w);
           uint32_t cy = (uint32_t)((features->p_feature[1 + i*2]) * toy_h);
           // clip the coordinates to leave 1 pixel's margin to the edges.
           if (cx < 1) cx = 1;
           if (cx >= toy_w - 2)
               cx = toy_w - 2;
           if (cy < 1) cy = 1;
           if (cy >= toy_h - 2)
               cy = toy_h - 2;
           
           if(del_old){
               if(point_now == 1){
                   // delete the pre-point          
                   point_now = 0; 
                   // 1. invalidate the old_point_area
                   lv_obj_set_pos(pre_feature_img, base_x + cx_old - 1, base_y + cy_old - 1);
                   update_image_widget((lv_obj_t *)pre_feature_img, (lv_img_dsc_t*)&pre_feature_img_dsc);
               }
               point_now += 1;
               
               if((features->col) == 1){
                    // need store the back color of the cur_cx, cur_cy
                    color_pre[0] = GetPixel(buffer_toy, cx-1, cy-1); 
                    color_pre[1] = GetPixel(buffer_toy, cx, cy-1); 
                    color_pre[2] = GetPixel(buffer_toy, cx+1, cy-1); 
                    color_pre[3] = GetPixel(buffer_toy, cx-1, cy); 
                    color_pre[4] = GetPixel(buffer_toy, cx, cy); 
                    color_pre[5] = GetPixel(buffer_toy, cx+1, cy); 
                    color_pre[6] = GetPixel(buffer_toy, cx-1, cy+1); 
                    color_pre[7] = GetPixel(buffer_toy, cx, cy+1); 
                    color_pre[8] = GetPixel(buffer_toy, cx+1, cy+1); 
                    cx_old = cx, cy_old = cy;
                    
                    // update the cur_col buffer
                    color_cur[0] = color_pre[0]; 
                    color_cur[1] = col; 
                    color_cur[2] = color_pre[2]; 
                    color_cur[3] = col; 
                    color_cur[4] = col; 
                    color_cur[5] = col; 
                    color_cur[6] = color_pre[6]; 
                    color_cur[7] = col; 
                    color_cur[8] = color_pre[8];
                    
                    lv_obj_set_pos(cur_feature_img, base_x + cx-1, base_y + cy-1);
                    update_image_widget((lv_obj_t *)cur_feature_img, (lv_img_dsc_t*)&cur_feature_img_dsc);
                   
                   // refresh two operation already, return
                   return;
               }
           }
                  
           draw_cross(buffer_toy, cx, cy, col);       
        }  
    }else{
        // re-init these
        pre_feature_img = NULL;
        cur_feature_img = NULL;
    }        
}
#endif

void draw_svm_toy_update(){
   update_image_widget(guider_ui.Main_svm_toy, &svm_toy);
}

#if defined(__cplusplus)
}
#endif


