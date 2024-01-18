/**
 * @file lv_demo_widgets.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_demo_widgets.h"
#include "app_main.h"
#if LV_USE_DEMO_WIDGETS

#if LV_MEM_CUSTOM == 0 && LV_MEM_SIZE < (38ul * 1024ul)
    #error Insufficient memory for lv_demo_widgets. Please set LV_MEM_SIZE to at least 38KB (38ul * 1024ul).  48KB is recommended.
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
} disp_size_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void profile_create(lv_obj_t * parent);
static void analytics_create(lv_obj_t * parent);
static void shop_create(lv_obj_t * parent);
static void color_changer_create(lv_obj_t * parent);

static lv_obj_t * create_meter_box(lv_obj_t * parent, const char * title, const char * text1, const char * text2,
                                   const char * text3);
static lv_obj_t * create_shop_item(lv_obj_t * parent, const void * img_src, const char * name, const char * category,
                                   const char * price);

static void color_changer_event_cb(lv_event_t * e);
static void color_event_cb(lv_event_t * e);
static void ta_event_cb(lv_event_t * e);
static void birthday_event_cb(lv_event_t * e);
static void calendar_event_cb(lv_event_t * e);
static void slider_event_cb(lv_event_t * e);
static void chart_event_cb(lv_event_t * e);
static void shop_chart_event_cb(lv_event_t * e);
static void meter1_indic1_anim_cb(void * var, int32_t v);
static void meter1_indic2_anim_cb(void * var, int32_t v);
static void meter1_indic3_anim_cb(void * var, int32_t v);
static void meter2_timer_cb(lv_timer_t * timer);
static void meter3_anim_cb(void * var, int32_t v);

/**********************
 *  STATIC VARIABLES
 **********************/
static disp_size_t disp_size;

static lv_obj_t * tv;
static lv_obj_t * calendar;
static lv_style_t style_text_muted;
static lv_style_t style_title;
static lv_style_t style_icon;
static lv_style_t style_bullet;

static lv_obj_t * meter1;
static lv_obj_t * meter2;
static lv_obj_t * meter3;

static lv_obj_t * chart1;
static lv_obj_t * chart2;
static lv_obj_t * chart3;

static lv_chart_series_t * ser1;
static lv_chart_series_t * ser2;
static lv_chart_series_t * ser3;
static lv_chart_series_t * ser4;

static const lv_font_t * font_large;
static const lv_font_t * font_normal;

static uint32_t session_desktop = 1000;
static uint32_t session_tablet = 1000;
static uint32_t session_mobile = 1000;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
#include "FreeRTOS.h"
#include "timers.h"

uint32_t seed;
void srand(uint32_t new_seed){
	seed = new_seed;
}

uint32_t rand(){
	seed = 1664525 * seed + 1013904223;
	return (uint32_t)(seed & 0xFFFF);
}

volatile uint32_t rect_x, rect_y;
void x_inc_event_handler(lv_event_t *event)
{
	lv_event_code_t code = lv_event_get_code(event);
	lv_obj_t *rect = lv_event_get_user_data(event);
    if(code == LV_EVENT_CLICKED) {
    	srand(lv_tick_get());
    	uint16_t rect_x_y = rand() & 0xFFFF;
    	rect_x = rect_x_y & 0xFF;
    	rect_y = rect_x_y >> 8;
    	lv_obj_set_pos(rect, rect_x, rect_y);
    }
}

extern uint16_t pic_start[];
extern uint16_t pic_end[];

static lv_img_dsc_t my_image[3];
static lv_obj_t* my_image_obj[3];

#define IMG_OBJ_W  (128)
#define IMG_OBJ_H  (99 / 3)
#define FRAMES (25 * 3)
uint16_t camera_buffer_simulate[2][IMG_OBJ_W*IMG_OBJ_H];
#define LV_IMAGE_VAL \
	(lv_img_dsc_t){ \
	    .header.always_zero = 0, \
	    .header.w = IMG_OBJ_W, \
	    .header.h = IMG_OBJ_H, \
	    .data_size = IMG_OBJ_W * IMG_OBJ_H * 2, \
	    .header.cf = LV_IMG_CF_TRUE_COLOR, \
	    .data = NULL,  \
	}; \

#define DECLARE_DEF_LV_IMAGE(name, n) \
	name [n] = LV_IMAGE_VAL; \
	name ##_obj[n] = lv_img_create(cont); \
	lv_img_set_src(name ##_obj[n], &(name [n])); \
	lv_obj_set_pos(name ##_obj[n], 100, 100 + n * (IMG_OBJ_H)); \
	lv_obj_set_style_pad_all(name ##_obj[n], 0, 0); \
	lv_obj_add_flag(name ##_obj[n], LV_OBJ_FLAG_OVERFLOW_VISIBLE); \
	lv_obj_set_size(name ##_obj[n], IMG_OBJ_W, IMG_OBJ_H); \
	lv_obj_add_flag(name ##_obj[n], LV_OBJ_FLAG_IGNORE_LAYOUT | LV_LAYER_TYPE_NONE); \
	lv_obj_set_style_bg_color(name ##_obj[n], lv_color_make(0xFF, 0, 0), 0);

void vTimerCallback(TimerHandle_t xTimer){
	static uint32_t idx = 0;
	static uint32_t start_idx = 0;
	static uint32_t frame_idx = 0;
	uint32_t start_off = start_idx;
	if((++start_idx) > (FRAMES - 1)){
		// re-start
		start_idx = 0;
	}
//	uint32_t start_off = (idx + start_idx) % 3;
//	if(idx == 0) start_idx += 1;
//	uint16_t* pic_start_ptr = pic_start + start_off * IMG_OBJ_W * IMG_OBJ_H;
//	uint16_t* img_obj_buf = camera_buffer_simulate[frame_idx];
//	//frame_idx ^= 1;
//    if (app_lvgl_lock_acquire(10000)) {
//		memcpy(img_obj_buf, pic_start_ptr, sizeof(camera_buffer_simulate[0]));
//		my_image[idx].data = (const uint8_t*)img_obj_buf;
//
//    	lv_img_set_src(my_image_obj[idx], &(my_image[idx]));
//        app_lvgl_lock_release();
//    }
//	if((++idx) > 2){
//		idx = 0;
//	}
}

static TimerHandle_t xTimer;
void start_event_handler(lv_event_t *event)
{
	lv_event_code_t code = lv_event_get_code(event);
	lv_obj_t *label = lv_event_get_user_data(event);
    if(code == LV_EVENT_CLICKED) {
    	lv_label_set_text(label, "Stop");
    	xTimerStart(xTimer, 0);
    }
}

#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
lv_ui guider_ui;

lv_timer_t *g_pTmrFanRotate;
lv_timer_t *g_pTmrPgsBar;
lv_chart_series_t* g_pSeriesAry[3];

void ShowHealthWaveform(float health) {
	lv_ui* ui = &guider_ui;
	float stdHealth = (health + APP_HEALTH_ABS_MAX) / (2.0f * APP_HEALTH_ABS_MAX);

    uint32_t uHealth = (uint32_t)(stdHealth * 100.0f + 0.5f);

    lv_chart_set_next_value(ui->Main_chart_1, g_pSeriesAry[0], uHealth);
    
    uint8_t buffer[7+3+1+3+1] = "Health:"; // one sign bit, 2 int, 1., 3 decimal
    extern void format_decimal(uint8_t *buf, float value);
    format_decimal(buffer+7, health);
    lv_label_set_text(ui->Main_chart_Label, (const char*)buffer);
}

void ShowProgress(int val)
{
	lv_obj_t *pBar = &guider_ui.Model_Training_barModelTrainProgress;
	lv_bar_set_value(pBar, g_app.featureNdx, LV_ANIM_OFF);
}

void OnTmr_FanRotate(lv_timer_t* pTmr) 
{
    lv_ui* ui = &guider_ui;   
	// note: angle unit is 0.1 degree
    lv_obj_t* pFan = (lv_obj_t*)pTmr->user_data;
    if (g_app.fanState == kFanOff) {
        return;
    }
    uint16_t angle = lv_img_get_angle(pFan);
    
    if (g_app.fanState == kFanErr) {
    	lv_img_set_src(pFan,&_fan_abnormal_alpha_160x160);
    } else {
    	lv_img_set_src(pFan,&_fan_normal_on_alpha_160x160);
    }
    angle = (angle + 150) % 3600;
    lv_img_set_angle(pFan, angle);

    if (g_app.isNewFeature) {
        g_app.isNewFeature = 0;
        if (g_app.appState == kAppPredicting)
            ShowHealthWaveform(g_app.health);

    }
}

void SetupFanRotating(void)
{
    lv_img_set_pivot(guider_ui.Main_img_status, 80,80);
    g_pTmrFanRotate = lv_timer_create(OnTmr_FanRotate, 100, guider_ui.Main_img_status);
}

void OnTmr_PgsBar(lv_timer_t* pTmr) 
{
    lv_ui* ui = &guider_ui;

    lv_bar_set_value(ui->Model_Training_barModelTrainProgress, g_app.featureNdx, LV_ANIM_OFF);
}

void SetupPgsBar(void)
{
    g_pTmrPgsBar = lv_timer_create(OnTmr_PgsBar, 100, guider_ui.Model_Training_barModelTrainProgress);
}



void SetChartInitData(void)
{
    lv_ui* ui = &guider_ui;   
	lv_chart_series_t * Main_chart_1_0 = lv_chart_add_series(ui->Main_chart_1, lv_color_make(0x00, 0x00, 0x00), LV_CHART_AXIS_PRIMARY_Y);
	// lv_chart_series_t * Main_chart_1_1 = lv_chart_add_series(ui->Main_chart_1, lv_color_make(0xcc, 0x2d, 0x2d), LV_CHART_AXIS_PRIMARY_Y);
    // lv_chart_series_t * Main_chart_1_2 = lv_chart_add_series(ui->Main_chart_1, lv_color_make(0x37, 0xa5, 0x18), LV_CHART_AXIS_PRIMARY_Y);
    g_pSeriesAry[0] = Main_chart_1_0;
    // g_pSeriesAry[1] = Main_chart_1_1;
    // g_pSeriesAry[2] = Main_chart_1_2;
    lv_chart_set_point_count(ui->Main_chart_1, 20);
    lv_chart_set_div_line_count(ui->Main_chart_1, 5, 4);
    // lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_0, 1);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_0, 20);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_0, 30);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_0, 40);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_0, 5);
	
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_1, 0);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_1, 15);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_1, 16);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_1, 18);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_1, 6);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_1, 23);

	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_2, 0);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_2, 7);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_2, 9);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_2, 11);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_2, 16);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_2, 16);
	// lv_chart_set_next_value(ui->Main_chart_1, Main_chart_1_2, 31);
    
}


void SetupMainScrUserLogic(void)
{
    g_app.appState = kAppPredicting;
    SetupFanRotating();
    SetChartInitData();
}

uint32_t get_slider_value(uint32_t value_type){
    switch(value_type){
        // gamma
        case 0:
            return lv_slider_get_value(guider_ui.Model_Training_SliderModelTrainGamma);
        case 1:
            return lv_slider_get_value(guider_ui.Model_Training_SliderModelTrainNu);
        default:
            break;
    }
    return 0;
}

void lv_demo_widgets(void)
{
#if 1
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);
    
    SetupMainScrUserLogic();
    
#else
		if(LV_HOR_RES <= 320) disp_size = DISP_SMALL;
		else if(LV_HOR_RES < 720) disp_size = DISP_MEDIUM;
		else disp_size = DISP_LARGE;

		font_large = LV_FONT_DEFAULT;
		font_normal = LV_FONT_DEFAULT;

		lv_coord_t tab_h;
		if(disp_size == DISP_LARGE) {
			tab_h = 70;
	#if LV_FONT_MONTSERRAT_24
			font_large     = &lv_font_montserrat_24;
	#else
			LV_LOG_WARN("LV_FONT_MONTSERRAT_24 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
	#endif
	#if LV_FONT_MONTSERRAT_16
			font_normal    = &lv_font_montserrat_16;
	#else
			LV_LOG_WARN("LV_FONT_MONTSERRAT_16 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
	#endif
		}
		else if(disp_size == DISP_MEDIUM) {
			tab_h = 45;
	#if LV_FONT_MONTSERRAT_20
			font_large     = &lv_font_montserrat_20;
	#else
			LV_LOG_WARN("LV_FONT_MONTSERRAT_20 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
	#endif
	#if LV_FONT_MONTSERRAT_14
			font_normal    = &lv_font_montserrat_14;
	#else
			LV_LOG_WARN("LV_FONT_MONTSERRAT_14 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
	#endif
		}
		else {   /* disp_size == DISP_SMALL */
			tab_h = 45;
	#if LV_FONT_MONTSERRAT_18
			font_large     = &lv_font_montserrat_18;
	#else
			LV_LOG_WARN("LV_FONT_MONTSERRAT_18 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
	#endif
	#if LV_FONT_MONTSERRAT_12
			font_normal    = &lv_font_montserrat_12;
	#else
			LV_LOG_WARN("LV_FONT_MONTSERRAT_12 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
	#endif
		}

	#if LV_USE_THEME_DEFAULT
		lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
							  font_normal);
	#endif

		lv_style_init(&style_text_muted);
		lv_style_set_text_opa(&style_text_muted, LV_OPA_50);

		lv_style_init(&style_title);
		lv_style_set_text_font(&style_title, font_large);

		lv_style_init(&style_icon);
		lv_style_set_text_color(&style_icon, lv_theme_get_color_primary(NULL));
		lv_style_set_text_font(&style_icon, font_large);

	//    lv_style_init(&style_bullet);
	//    lv_style_set_border_width(&style_bullet, 0);
	//    lv_style_set_radius(&style_bullet, LV_RADIUS_CIRCLE);

		lv_obj_t * cont = lv_scr_act();

		lv_obj_t * rect = lv_obj_create(cont);

		lv_obj_set_size(rect, 80, 80);
		lv_obj_set_pos(rect, 50, 100);

		lv_obj_t * btn = lv_btn_create(cont);
		lv_obj_set_size(btn, 80, 30);
		lv_obj_set_pos(btn, 380, 20);

		lv_obj_t * x_inc_label = lv_label_create(btn);
		lv_label_set_text(x_inc_label, "Trigger");

		lv_obj_add_event_cb(btn, x_inc_event_handler, LV_EVENT_ALL, rect);

		DECLARE_DEF_LV_IMAGE(my_image, 0);
		DECLARE_DEF_LV_IMAGE(my_image, 1);
		DECLARE_DEF_LV_IMAGE(my_image, 2);


		// create a soft-timer
		xTimer = xTimerCreate("MyTimer", pdMS_TO_TICKS(11), pdTRUE, NULL, vTimerCallback);

		lv_obj_t * btn2 = lv_btn_create(cont);
		lv_obj_set_size(btn2, 80, 30);
		lv_obj_set_pos(btn2, 380, 60);

		lv_obj_t * btn2_label = lv_label_create(btn2);
		lv_label_set_text(btn2_label, "Start");

		lv_obj_add_event_cb(btn2, start_event_handler, LV_EVENT_ALL, btn2_label);

		/* 刷新屏幕 */
		//lv_scr_load(lv_scr_act());
#endif
}
#endif
