/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#include "app_custom_ui.h"
#include "app_main.h"
#include "svm_test.h"

lv_timer_t *g_pTmrFanRotate;
lv_timer_t *g_pTmrPgsBar;
lv_chart_series_t* g_pSeriesAry[3];
bool is_in_MainScr = false;


lv_img_dsc_t svm_toy = {
  .header.always_zero = 0,
  .header.w = 160,
  .header.h = 160,

  .header.cf = LV_IMG_CF_TRUE_COLOR,
  .data = NULL,
  .data_size = 160 * 160 * LV_COLOR_SIZE,
};


void ShowHealthWaveform(float health);

// define a simple function to format the value
void format_int(uint8_t *buf, uint32_t value, uint32_t max_bits){
    switch(max_bits){
        case 4:
            *buf++ = (value / 1000) + '0';
            value %= 1000;
        case 3:
            *buf++ = (value / 100) + '0';
            value %= 100;
        case 2:
            *buf++ = (value / 10) + '0';
            value %= 10;
        case 1:
            *buf++ = (value) + '0';
        default:
            break;
    }
}

void format_decimal(uint8_t *buf, float value){
    uint32_t sign_bit = 0;
    if(value < 0){
        buf[sign_bit] = '-';
        sign_bit = 1;
        value = -value;
    }
    format_int(buf+sign_bit, value, 2);
    buf[sign_bit+2] = '.';
    format_int(buf+sign_bit+2+1, ((value - (uint32_t)value)*1000), 3);
}

void format_nu(uint8_t *buf, uint32_t value){
    // the nu must always be 0-1
    buf[0] = '0';
    buf[1] = '.';
    format_int(buf+2, value, 2);
}

void format_gamma(uint8_t *buf, uint32_t value){
    format_int(buf, value, 4);
}

void ShowProgress(int val)
{
	lv_obj_t *pBar = guider_ui.Model_Training_barModelTrainProgress;
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
    	lv_img_set_src(pFan,&Main_Main_img_statusfan_abnormal);
    } else {
    	lv_img_set_src(pFan,&Main_Main_img_statusfan_normal_on);
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
    lv_img_set_pivot(guider_ui.Main_Main_img_status, 80,80);
    g_pTmrFanRotate = lv_timer_create(OnTmr_FanRotate, 150, guider_ui.Main_Main_img_status);
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

void Train_win_param_btn_cb()
{
	lv_ui* ui = &guider_ui;
	{
		lv_obj_clear_flag(ui->Model_Training_Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
		uint8_t text_buf[8] = "Nu:";
		format_nu(text_buf+3, lv_slider_get_value(ui->Model_Training_Model_Training_SliderModelTrainNu)/2);
		lv_label_set_text(ui->Model_Training_SliderModelTrainNu_Label, (const char*)text_buf);
		uint8_t text_buf2[13] = "Gamma:";
		format_gamma(text_buf2+6, lv_slider_get_value(ui->Model_Training_ModelTrainGamma));
		lv_label_set_text(ui->Model_Training_ModelTrainGamma_Label, (const char*)text_buf2);
	}
}

void SetChartInitData(void)
{
    lv_ui* ui = &guider_ui;
	lv_chart_series_t * Main_Main_chart_1_0 = lv_chart_add_series(ui->Main_Main_chart_1, lv_color_make(0x00, 0x00, 0x00), LV_CHART_AXIS_PRIMARY_Y);
	// lv_chart_series_t * Main_chart_1_1 = lv_chart_add_series(ui->Main_chart_1, lv_color_make(0xcc, 0x2d, 0x2d), LV_CHART_AXIS_PRIMARY_Y);
    // lv_chart_series_t * Main_chart_1_2 = lv_chart_add_series(ui->Main_chart_1, lv_color_make(0x37, 0xa5, 0x18), LV_CHART_AXIS_PRIMARY_Y);
    g_pSeriesAry[0] = Main_Main_chart_1_0;
    // g_pSeriesAry[1] = Main_chart_1_1;
    // g_pSeriesAry[2] = Main_chart_1_2;
    lv_chart_set_point_count(ui->Main_Main_chart_1, 20);
    lv_chart_set_div_line_count(ui->Main_Main_chart_1, 5, 4);
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



void Main_win_train_btn_cb()
{
	lv_obj_t * act_scr = lv_scr_act();
	lv_disp_t * d = lv_obj_get_disp(act_scr);
	if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
	{
		if (guider_ui.Model_Training_del == true) {
			if (g_pTmrFanRotate)
				lv_timer_del(g_pTmrFanRotate);
			g_pTmrFanRotate = NULL;
			setup_scr_Model_Training(&guider_ui);
			// checking the g_app.pADModel is NULL
			if(g_app.pADModel == NULL){
				lv_obj_add_flag(guider_ui.Model_Training_inc_sw, LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_flag(guider_ui.Model_Training_inc_label, LV_OBJ_FLAG_HIDDEN);

			}else{
				lv_obj_clear_flag(guider_ui.Model_Training_inc_label, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(guider_ui.Model_Training_inc_sw, LV_OBJ_FLAG_HIDDEN);
			}

			lv_obj_t *pBar = guider_ui.Model_Training_barModelTrainProgress;
			lv_bar_set_range(pBar, 0, APP_FEATURE_CNT);
			lv_bar_set_value(pBar, 0, LV_ANIM_OFF);


		}
		lv_scr_load_anim(guider_ui.Model_Training, LV_SCR_LOAD_ANIM_NONE, 100, 100, true);
		SetupPgsBar();
		guider_ui.Main_del = true;
		is_in_MainScr = false;
	}
}

void Train_win_Inc_sw_cb(int state)
{
	g_app.inc_train = state;
}

void Train_win_start_btn_cb()
{
	g_app.appState = kAppCollecting;
	g_app.featureNdx = 0;
	g_app.nu = lv_slider_get_value(guider_ui.Model_Training_Model_Training_SliderModelTrainNu) / 200.0f;
	g_app.gamma = lv_slider_get_value(guider_ui.Model_Training_ModelTrainGamma);
}

void Train_win_return_btn_cb()
{
	lv_obj_t * act_scr = lv_scr_act();
	lv_disp_t * d = lv_obj_get_disp(act_scr);
	if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
	{
		if (guider_ui.Main_del == true) {
			if (g_pTmrPgsBar)
				lv_timer_del(g_pTmrPgsBar);
			g_pTmrPgsBar = NULL;
			setup_scr_Main(&guider_ui);
		}
		lv_scr_load_anim(guider_ui.Main, LV_SCR_LOAD_ANIM_NONE, 100, 100, true);

		guider_ui.Model_Training_del = true;
	}
	is_in_MainScr = true;
}

void Train_win_show_error_log(char *log)
{
	if (is_in_MainScr)
	{
		lv_label_set_text(guider_ui.Main_error_log, log);
	}
}
void Train_win_param_setup()
{
	lv_slider_set_value(guider_ui.Model_Training_Model_Training_SliderModelTrainNu, g_app.nu * 200.0f, LV_ANIM_OFF);
	lv_slider_set_value(guider_ui.Model_Training_ModelTrainGamma, g_app.gamma, LV_ANIM_OFF);
	Train_win_param_btn_cb();

}

void Train_win_setup()
{
	lv_slider_set_value(guider_ui.Model_Training_Model_Training_SliderModelTrainNu, g_app.nu * 200.0f, LV_ANIM_OFF);
	lv_slider_set_value(guider_ui.Model_Training_ModelTrainGamma, g_app.gamma, LV_ANIM_OFF);
	Train_win_param_btn_cb();
	lv_obj_add_flag(guider_ui.Model_Training_Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
}


void Train_win_param_nu_changed_cb()
{
	g_app.nu = lv_slider_get_value(guider_ui.Model_Training_Model_Training_SliderModelTrainNu) / 200.0f;
	uint8_t text_buf[8] = "Nu:";
	format_nu(text_buf+3, lv_slider_get_value(guider_ui.Model_Training_Model_Training_SliderModelTrainNu) / 2);
	lv_label_set_text(guider_ui.Model_Training_SliderModelTrainNu_Label, (const char*)text_buf);

}

void Train_win_param_gamma_changed_cb()
{
	g_app.gamma = lv_slider_get_value(guider_ui.Model_Training_ModelTrainGamma);
	uint8_t text_buf2[13] = "Gamma:";
	format_gamma(text_buf2+6, lv_slider_get_value(guider_ui.Model_Training_ModelTrainGamma));
	lv_label_set_text(guider_ui.Model_Training_ModelTrainGamma_Label, (const char*)text_buf2);
}


void ShowHealthWaveform(float health) {
	lv_ui* ui = &guider_ui;
	float stdHealth = (health + APP_HEALTH_ABS_MAX) / (2.0f * APP_HEALTH_ABS_MAX);

    uint32_t uHealth = (uint32_t)(stdHealth * 100.0f + 0.5f);

    lv_chart_set_next_value(ui->Main_Main_chart_1, g_pSeriesAry[0], uHealth);

    uint8_t buffer[7+3+1+3+1] = "Health:"; // one sign bit, 2 int, 1., 3 decimal

    format_decimal(buffer+7, health);
    lv_label_set_text(ui->Main_Main_chart_Label, (const char*)buffer);
}

void Setup_main_windows()
{
	SetupFanRotating();
	SetChartInitData();
	is_in_MainScr = true;

	if (g_app.sensor_error)
		Train_win_show_error_log("Sensor init failed, Re-plugin the USB cable");

	if (g_app.appState == kAppWaitForReturn )
		algo_update_heatmap_after_train();
	else if (g_app.appState == kAppPredicting)
		algo_update_heatmap();
	g_app.appState = kAppPredicting;
}

void update_image_widget(lv_obj_t *img_obj, lv_img_dsc_t* img_dsc){
   if(is_in_MainScr)
   {
       if((((lv_img_t *)(img_obj))->src) == NULL){
           lv_img_set_src(img_obj,img_dsc);
       }else{
           lv_obj_invalidate(img_obj);
       }
   }
}


void draw_svm_toy_update(){
   update_image_widget(guider_ui.Main_Main_svm_toy, &svm_toy);
}

extern void draw_svm_toy_map(void* model, void* buffer_toy, uint32_t toy_w, uint32_t toy_h);
void main_win_heatmap_update(void *model, void* img_data)
{
	if(is_in_MainScr && model != NULL)
	{
		svm_toy.data = img_data;
		draw_svm_toy_map(model, img_data, 160, 160);

		lv_img_set_src(guider_ui.Main_Main_svm_toy, &svm_toy);
		lv_img_cache_invalidate_src(lv_img_get_src(guider_ui.Main_Main_svm_toy));
		lv_obj_invalidate(guider_ui.Main_Main_svm_toy);
		lv_task_handler();
	}
}

extern void draw_points_on_svm_toy(void* buffer_toy, uint32_t toy_w, uint32_t toy_h, void* _features, lv_color_t col, bool del_old);
void main_win_draw_current_feature_points(float* features)
{
	if(is_in_MainScr)
	{
		int cx = features[0] * 160;
		int cy = features[1] * 160;
		if (cx >= 160)
			cx = 159;
		if (cy >= 160)
			cy = 159;
		int x = (int)(cx + 32 - 2);
		int y = (int)(cy + 80 - 2);


		lv_obj_set_pos(guider_ui.Main_point, x, y);
	}
}

