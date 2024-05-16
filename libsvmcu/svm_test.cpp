#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "fsl_common.h"
#include "svm.h"
#include "fsl_debug_console.h"
#include "fsl_smartdma_mcxn.h"
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

#define SVM_pArena (Image$$RW_m_sramx_start$$Base + SMARTDMA_DISPLAY_FIRMWARE_SIZE)
#define SVM_pArenaSize (Image$$RW_m_sramx_start$$Limit - Image$$RW_m_sramx_start$$Base - SMARTDMA_DISPLAY_FIRMWARE_SIZE)

#else
extern uint8_t __base_SRAMX[];
extern uint8_t __top_SRAMX[];
#define SVM_pArena (__base_SRAMX + SMARTDMA_DISPLAY_FIRMWARE_SIZE)
#define SVM_pArenaSize (__top_SRAMX - __base_SRAMX - SMARTDMA_DISPLAY_FIRMWARE_SIZE)
#endif

void  svm_get_model_param_gamma_nu(void *model, float *gamma, float  *nu)
{
	svm_model* model_prob = (svm_model* )model;
	*gamma = model_prob->param.gamma;
	*nu = model_prob->param.nu;
}

void* svm_train_mode( float train_data[][FEATURE_NUMS],int XLEN,float gamma, float nu)
{

    svm_parameter param;
    int i, j;

    // default values
    param.svm_type = ONE_CLASS;
    param.kernel_type = RBF;
    param.degree = 3;
    param.gamma = gamma;//get_slider_value(SLIDER_GAMMA);
    param.coef0 = 0;
    param.nu = nu;//get_slider_value(SLIDER_NU) / 100.f;
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
    prob.y =  ( SVM_REAL* ) pvPortMalloc( prob.n_pts * sizeof( SVM_REAL ) );
    prob.x = ( svm_node** ) pvPortMalloc( prob.n_pts * sizeof( svm_node* ) );
    memset( prob.x, 0, prob.n_pts * sizeof( svm_node* ) );
    if( param.kernel_type == PRECOMPUTED )
    {
    }
    else
    {
        if( param.gamma == 0 ) param.gamma = 0.5f;
        prob.x = ( svm_node** ) pvPortMalloc( prob.n_pts * sizeof( svm_node* ) );
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
    pBuf = ( char* ) pvPortMalloc ( totalSize + sizeof(model->model_magic));
    if (pBuf == NULL)
    {
    	PRINTF("Malloc failed in %s",__func__);
    	return 0;
    }
    *((uint32_t*)(pBuf+totalSize)) = model->model_magic;
    
    svm_model *model_buf = svm_copy_model_to_buffer(pBuf, totalSize, (char*)get_flash_address(), model );
    svm_model *model_on_flash = (svm_model *)save_model_to_flash(model_buf, totalSize + sizeof(model->model_magic));
    
    vPortFree ( pBuf );
    vPortFree ( prob.y );
    vPortFree ( prob.x );
    return (void*)model_on_flash;
}
float svm_data_pre(float pre_data[][FEATURE_NUMS], int Test_Len)
{
        size_t totalSize = svm_model_total_size( model );
        // rocky: do a test of svm_copy_model as if it is loaded from flash
        char* pBuf;
        pBuf = ( char* ) pvPortMalloc ( totalSize );
        char* pFlash_1;
        pFlash_1 = ( char* ) pvPortMalloc ( totalSize );

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
        vPortFree ( pBuf );
        vPortFree ( pFlash_1 );
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
	struct svm_node x[FEATURE_NUMS];

    for (int i = 0; i < toy_w; i++)
    {
        for (int j = 0; j < toy_h; j++) {

            x[0].value = (SVM_REAL)i / toy_w; // powf((SVM_REAL)i / toy_w, 2.0f);
            x[1].value = (SVM_REAL)j / toy_h; // powf((SVM_REAL)j / toy_h, 2.0f);
            SVM_REAL d = svm_predict((struct svm_model *)model, x);
            if (((struct svm_model *)model)->param.svm_type == ONE_CLASS) {
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



#if defined(__cplusplus)
}
#endif


