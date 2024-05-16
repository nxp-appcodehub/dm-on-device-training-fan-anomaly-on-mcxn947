/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *Main;
	bool Main_del;
	lv_obj_t *Main_labelMainCurFunc;
	lv_obj_t *Main_Main_img_status;
	lv_obj_t *Main_Main_chart_1;
	lv_obj_t *Main_Main_chart_Label;
	lv_obj_t *Main_Main_svm_toy;
	lv_obj_t *Main_Main_btnMainSwScr;
	lv_obj_t *Main_Main_btnMainSwScr_label;
	lv_obj_t *Main_img_1;
	lv_obj_t *Main_label_1;
	lv_obj_t *Main_label_2;
	lv_obj_t *Main_point;
	lv_obj_t *Main_error_log;
	lv_obj_t *Model_Training;
	bool Model_Training_del;
	lv_obj_t *Model_Training_label_1;
	lv_obj_t *Model_Training_inc_sw;
	lv_obj_t *Model_Training_inc_label;
	lv_obj_t *Model_Training_btnModelTrainStart;
	lv_obj_t *Model_Training_btnModelTrainStart_label;
	lv_obj_t *Model_Training_barModelTrainProgress;
	lv_obj_t *Model_Training_btnModelTrainParam;
	lv_obj_t *Model_Training_btnModelTrainParam_label;
	lv_obj_t *Model_Training_btnModelTrainReturn;
	lv_obj_t *Model_Training_btnModelTrainReturn_label;
	lv_obj_t *Model_Training_Model_Training_Cont;
	lv_obj_t *Model_Training_ModelTrainGamma;
	lv_obj_t *Model_Training_Model_Training_Cont_Label;
	lv_obj_t *Model_Training_ModelTrainGamma_Label;
	lv_obj_t *Model_Training_Model_Training_SliderModelTrainNu;
	lv_obj_t *Model_Training_SliderModelTrainNu_Label;
	lv_obj_t *Model_Training_Exit;
	lv_obj_t *Model_Training_Exit_label;
	lv_obj_t *Model_Training_img_1;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_move_animation(void * var, int32_t duration, int32_t delay, int32_t x_end, int32_t y_end, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_scale_animation(void * var, int32_t duration, int32_t delay, int32_t width, int32_t height, lv_anim_path_cb_t path_cb,
                        uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                        lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_zoom_animation(void * var, int32_t duration, int32_t delay, int32_t zoom, lv_anim_path_cb_t path_cb,
                           uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                           lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_rotate_animation(void * var, int32_t duration, int32_t delay, lv_coord_t x, lv_coord_t y, int32_t rotate,
                   lv_anim_path_cb_t path_cb, uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time,
                   uint32_t playback_delay, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);


extern lv_ui guider_ui;


void setup_scr_Main(lv_ui *ui);
void setup_scr_Model_Training(lv_ui *ui);
#include "extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(Main_Main_img_statusfan_normal_on);
LV_IMG_DECLARE(Main_Main_img_statusfan_abnormal);
LV_IMG_DECLARE(_NXP_Logo_RGB_Colour_alpha_120x40);
LV_IMG_DECLARE(_draw_dot_alpha_8x8);
LV_IMG_DECLARE(_NXP_Logo_RGB_Colour_alpha_120x40);

LV_FONT_DECLARE(lv_font_montserratMedium_18)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_montserratMedium_14)


#ifdef __cplusplus
}
#endif
#endif
