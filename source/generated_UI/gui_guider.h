/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *Main;
	bool Main_del;
	lv_obj_t *Main_imgMainNXP;
	lv_obj_t *Main_labelMainCurFunc;
	lv_obj_t *Main_btnMainSwScr;
	lv_obj_t *Main_btnMainSwScr_label;
	lv_obj_t *Main_img_status;
	lv_obj_t *Main_chart_1;
	// lv_obj_t *ANModel_Training;
	// bool ANModel_Training_del;
	// lv_obj_t *ANModel_Training_imgANModelTrainNXP;
	// lv_obj_t *ANModel_Training_labelANModelTrainCurFunc;
	// lv_obj_t *ANModel_Training_btnANModelTrainReturn;
	// lv_obj_t *ANModel_Training_btnANModelTrainReturn_label;
	// lv_obj_t *ANModel_Training_labelANModelTrainInfo;
	// lv_obj_t *ANModel_Training_labelANModelTrainStat;
	// lv_obj_t *ANModel_Training_barANModelTrainProgress;
	// lv_obj_t *ANModel_Training_btnANModelTrainStart;
	// lv_obj_t *ANModel_Training_btnANModelTrainStart_label;
	lv_obj_t *Model_Training;
	bool Model_Training_del;
	lv_obj_t *Model_Training_imgModelTrainNXP;
	lv_obj_t *Model_Training_labelModelTrainCurFunc;
	lv_obj_t *Model_Training_btnModelTrainReturn;
	lv_obj_t *Model_Training_btnModelTrainReturn_label;
	lv_obj_t *Model_Training_labelModelTrainInfo;
	lv_obj_t *Model_Training_labelModelTrainStat;
	lv_obj_t *Model_Training_barModelTrainProgress;
	lv_obj_t *Model_Training_btnModelTrainStart;
	lv_obj_t *Model_Training_btnModelTrainStart_label;
    
    /* New Add one */
    bool Model_Training_Cont_Hidden;
    lv_obj_t *Model_Training_Cont;
    lv_obj_t *Model_Training_Cont_Label;
    lv_obj_t *Model_Training_SliderModelTrainGamma; // 0-N
    lv_obj_t *Model_Training_SliderModelTrainNu; // 0-1
    lv_obj_t *Model_Training_SliderModelTrainGamma_Label; 
    lv_obj_t *Model_Training_SliderModelTrainNu_Label;   

    lv_obj_t *Model_Training_btnModelTrainParamSet;
    lv_obj_t *Model_Training_btnModelTrainParamSet_Label;
    
    lv_obj_t *Main_chart_Label;
    
	lv_obj_t *screen_1;
	bool screen_1_del;
	lv_obj_t *screen_1_cont_1;
	lv_obj_t *screen_1_cb_2;
	lv_obj_t *screen_1_cb_1;
	lv_obj_t *screen_1_img_1;
    
    lv_obj_t *Main_svm_toy;
    bool is_in_MainScr;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_Main(lv_ui *ui);
void setup_scr_ANModel_Training(lv_ui *ui);
void setup_scr_Model_Training(lv_ui *ui);
void setup_scr_screen_1(lv_ui *ui);
LV_IMG_DECLARE(_logo_nxp_alpha_100x37);
LV_IMG_DECLARE(_fan_normal_on_alpha_160x160);
LV_IMG_DECLARE(_fan_abnormal_alpha_160x160);

// redifine the lv_obj_set_pos, lv_obj_set_size
#define DEFAULT_LCD_WIDTH    (800)
#define DEFAULT_LCD_HEIGHT   (480)

#define LCD_PANEL_WIDTH      (480)
#define LCD_PANEL_HEIGHT     (320)

#define lv_obj_set_size(obj, w, h) \
    lv_obj_set_size(obj, (lv_coord_t)(LCD_PANEL_WIDTH * 1.0f / DEFAULT_LCD_WIDTH * w), \
                        (lv_coord_t)(LCD_PANEL_HEIGHT * 1.0f / DEFAULT_LCD_HEIGHT * h));

#define lv_obj_set_pos(obj, x, y) \
    lv_obj_set_pos(obj, (lv_coord_t)(LCD_PANEL_WIDTH * 1.0f / DEFAULT_LCD_WIDTH * x), \
                        (lv_coord_t)(LCD_PANEL_HEIGHT * 1.0f / DEFAULT_LCD_HEIGHT * y));
                        
#ifdef __cplusplus
}
#endif
#endif