/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"
#include "app_custom_ui.h"


void setup_scr_Model_Training(lv_ui *ui)
{
	//Write codes Model_Training
	ui->Model_Training = lv_obj_create(NULL);
	lv_obj_set_size(ui->Model_Training, 480, 320);
	lv_obj_set_scrollbar_mode(ui->Model_Training, LV_SCROLLBAR_MODE_OFF);

	//Write style for Model_Training, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_main_stop(ui->Model_Training, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_stop(ui->Model_Training, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_label_1
	ui->Model_Training_label_1 = lv_label_create(ui->Model_Training);
	lv_label_set_text(ui->Model_Training_label_1, "Trainer");
	lv_label_set_long_mode(ui->Model_Training_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Model_Training_label_1, 132, 18);
	lv_obj_set_size(ui->Model_Training_label_1, 216, 42);

	//Write style for Model_Training_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Model_Training_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_label_1, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Model_Training_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_inc_sw
	ui->Model_Training_inc_sw = lv_switch_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_inc_sw, 181, 112);
	lv_obj_set_size(ui->Model_Training_inc_sw, 40, 20);

	//Write style for Model_Training_inc_sw, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_inc_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_inc_sw, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_inc_sw, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_inc_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_inc_sw, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_inc_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Model_Training_inc_sw, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->Model_Training_inc_sw, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->Model_Training_inc_sw, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_inc_sw, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->Model_Training_inc_sw, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for Model_Training_inc_sw, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_inc_sw, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_inc_sw, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_inc_sw, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_inc_sw, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_inc_sw, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes Model_Training_inc_label
	ui->Model_Training_inc_label = lv_label_create(ui->Model_Training);
	lv_label_set_text(ui->Model_Training_inc_label, "Inc Training");
	lv_label_set_long_mode(ui->Model_Training_inc_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Model_Training_inc_label, 48, 114);
	lv_obj_set_size(ui->Model_Training_inc_label, 113, 16);

	//Write style for Model_Training_inc_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Model_Training_inc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_inc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_inc_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_inc_label, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Model_Training_inc_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_inc_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_inc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_inc_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_inc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_inc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_inc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_inc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_inc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_inc_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_btnModelTrainStart
	ui->Model_Training_btnModelTrainStart = lv_btn_create(ui->Model_Training);
	ui->Model_Training_btnModelTrainStart_label = lv_label_create(ui->Model_Training_btnModelTrainStart);
	lv_label_set_text(ui->Model_Training_btnModelTrainStart_label, "Start");
	lv_label_set_long_mode(ui->Model_Training_btnModelTrainStart_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Model_Training_btnModelTrainStart_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Model_Training_btnModelTrainStart, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->Model_Training_btnModelTrainStart_label, LV_PCT(100));
	lv_obj_set_pos(ui->Model_Training_btnModelTrainStart, 40, 223);
	lv_obj_set_size(ui->Model_Training_btnModelTrainStart, 84, 42);

	//Write style for Model_Training_btnModelTrainStart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_btnModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_btnModelTrainStart, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_btnModelTrainStart, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_btnModelTrainStart, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_btnModelTrainStart, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_btnModelTrainStart, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Model_Training_btnModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_btnModelTrainStart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_barModelTrainProgress
	ui->Model_Training_barModelTrainProgress = lv_bar_create(ui->Model_Training);
	lv_obj_set_style_anim_time(ui->Model_Training_barModelTrainProgress, 1000, 0);
	lv_bar_set_mode(ui->Model_Training_barModelTrainProgress, LV_BAR_MODE_NORMAL);
	lv_bar_set_range(ui->Model_Training_barModelTrainProgress, 0, 100);
	lv_bar_set_value(ui->Model_Training_barModelTrainProgress, 0, LV_ANIM_OFF);
	lv_obj_set_pos(ui->Model_Training_barModelTrainProgress, 40, 150);
	lv_obj_set_size(ui->Model_Training_barModelTrainProgress, 400, 20);

	//Write style for Model_Training_barModelTrainProgress, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_barModelTrainProgress, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_barModelTrainProgress, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_barModelTrainProgress, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_barModelTrainProgress, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_barModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Model_Training_barModelTrainProgress, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_barModelTrainProgress, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_barModelTrainProgress, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_barModelTrainProgress, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_barModelTrainProgress, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write codes Model_Training_btnModelTrainParam
	ui->Model_Training_btnModelTrainParam = lv_btn_create(ui->Model_Training);
	ui->Model_Training_btnModelTrainParam_label = lv_label_create(ui->Model_Training_btnModelTrainParam);
	lv_label_set_text(ui->Model_Training_btnModelTrainParam_label, "Param");
	lv_label_set_long_mode(ui->Model_Training_btnModelTrainParam_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Model_Training_btnModelTrainParam_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Model_Training_btnModelTrainParam, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->Model_Training_btnModelTrainParam_label, LV_PCT(100));
	lv_obj_set_pos(ui->Model_Training_btnModelTrainParam, 198, 223);
	lv_obj_set_size(ui->Model_Training_btnModelTrainParam, 84, 42);

	//Write style for Model_Training_btnModelTrainParam, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_btnModelTrainParam, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_btnModelTrainParam, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_btnModelTrainParam, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_btnModelTrainParam, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_btnModelTrainParam, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_btnModelTrainParam, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_btnModelTrainParam, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_btnModelTrainParam, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Model_Training_btnModelTrainParam, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_btnModelTrainParam, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_btnModelTrainReturn
	ui->Model_Training_btnModelTrainReturn = lv_btn_create(ui->Model_Training);
	ui->Model_Training_btnModelTrainReturn_label = lv_label_create(ui->Model_Training_btnModelTrainReturn);
	lv_label_set_text(ui->Model_Training_btnModelTrainReturn_label, "Return");
	lv_label_set_long_mode(ui->Model_Training_btnModelTrainReturn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Model_Training_btnModelTrainReturn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Model_Training_btnModelTrainReturn, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->Model_Training_btnModelTrainReturn_label, LV_PCT(100));
	lv_obj_set_pos(ui->Model_Training_btnModelTrainReturn, 356, 223);
	lv_obj_set_size(ui->Model_Training_btnModelTrainReturn, 84, 42);

	//Write style for Model_Training_btnModelTrainReturn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_btnModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_btnModelTrainReturn, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_btnModelTrainReturn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_btnModelTrainReturn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_btnModelTrainReturn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_btnModelTrainReturn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Model_Training_btnModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_btnModelTrainReturn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_Model_Training_Cont
	ui->Model_Training_Model_Training_Cont = lv_obj_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_Model_Training_Cont, 90, 26);
	lv_obj_set_size(ui->Model_Training_Model_Training_Cont, 300, 235);
	lv_obj_set_scrollbar_mode(ui->Model_Training_Model_Training_Cont, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->Model_Training_Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui->Model_Training_Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);

	//Write style for Model_Training_Model_Training_Cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Model_Training_Model_Training_Cont, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Model_Training_Model_Training_Cont, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Model_Training_Model_Training_Cont, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Model_Training_Model_Training_Cont, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_Model_Training_Cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_Model_Training_Cont, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_Model_Training_Cont, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_Model_Training_Cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_Model_Training_Cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_Model_Training_Cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_Model_Training_Cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_Model_Training_Cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_Model_Training_Cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_ModelTrainGamma
	ui->Model_Training_ModelTrainGamma = lv_slider_create(ui->Model_Training_Model_Training_Cont);
	lv_slider_set_range(ui->Model_Training_ModelTrainGamma, 0, 400);
	lv_slider_set_mode(ui->Model_Training_ModelTrainGamma, LV_SLIDER_MODE_NORMAL);
	lv_slider_set_value(ui->Model_Training_ModelTrainGamma, 50, LV_ANIM_OFF);
	lv_obj_set_pos(ui->Model_Training_ModelTrainGamma, 40, 92);
	lv_obj_set_size(ui->Model_Training_ModelTrainGamma, 220, 8);

	//Write style for Model_Training_ModelTrainGamma, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_ModelTrainGamma, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_ModelTrainGamma, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_ModelTrainGamma, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_ModelTrainGamma, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_outline_width(ui->Model_Training_ModelTrainGamma, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_ModelTrainGamma, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Model_Training_ModelTrainGamma, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_ModelTrainGamma, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_ModelTrainGamma, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_ModelTrainGamma, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_ModelTrainGamma, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style for Model_Training_ModelTrainGamma, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_ModelTrainGamma, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_ModelTrainGamma, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_ModelTrainGamma, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_ModelTrainGamma, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes Model_Training_Model_Training_Cont_Label
	ui->Model_Training_Model_Training_Cont_Label = lv_label_create(ui->Model_Training_Model_Training_Cont);
	lv_label_set_text(ui->Model_Training_Model_Training_Cont_Label, "Parameter Setting");
	lv_label_set_long_mode(ui->Model_Training_Model_Training_Cont_Label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Model_Training_Model_Training_Cont_Label, 24, 19);
	lv_obj_set_size(ui->Model_Training_Model_Training_Cont_Label, 246, 32);

	//Write style for Model_Training_Model_Training_Cont_Label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Model_Training_Model_Training_Cont_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_Model_Training_Cont_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_Model_Training_Cont_Label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_Model_Training_Cont_Label, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Model_Training_Model_Training_Cont_Label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_Model_Training_Cont_Label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_Model_Training_Cont_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_Model_Training_Cont_Label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_Model_Training_Cont_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_Model_Training_Cont_Label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_Model_Training_Cont_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_Model_Training_Cont_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_Model_Training_Cont_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_Model_Training_Cont_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_ModelTrainGamma_Label
	ui->Model_Training_ModelTrainGamma_Label = lv_label_create(ui->Model_Training_Model_Training_Cont);
	lv_label_set_text(ui->Model_Training_ModelTrainGamma_Label, "Gamma:050");
	lv_label_set_long_mode(ui->Model_Training_ModelTrainGamma_Label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Model_Training_ModelTrainGamma_Label, 43, 72);
	lv_obj_set_size(ui->Model_Training_ModelTrainGamma_Label, 215, 32);

	//Write style for Model_Training_ModelTrainGamma_Label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Model_Training_ModelTrainGamma_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_ModelTrainGamma_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_ModelTrainGamma_Label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_ModelTrainGamma_Label, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Model_Training_ModelTrainGamma_Label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_ModelTrainGamma_Label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_ModelTrainGamma_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_ModelTrainGamma_Label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_ModelTrainGamma_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_ModelTrainGamma_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_ModelTrainGamma_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_ModelTrainGamma_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_ModelTrainGamma_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_ModelTrainGamma_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_Model_Training_SliderModelTrainNu
	ui->Model_Training_Model_Training_SliderModelTrainNu = lv_slider_create(ui->Model_Training_Model_Training_Cont);
	lv_slider_set_range(ui->Model_Training_Model_Training_SliderModelTrainNu, 0, 100);
	lv_slider_set_mode(ui->Model_Training_Model_Training_SliderModelTrainNu, LV_SLIDER_MODE_NORMAL);
	lv_slider_set_value(ui->Model_Training_Model_Training_SliderModelTrainNu, 20, LV_ANIM_OFF);
	lv_obj_set_pos(ui->Model_Training_Model_Training_SliderModelTrainNu, 41, 149);
	lv_obj_set_size(ui->Model_Training_Model_Training_SliderModelTrainNu, 220, 8);

	//Write style for Model_Training_Model_Training_SliderModelTrainNu, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_Model_Training_SliderModelTrainNu, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_Model_Training_SliderModelTrainNu, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_Model_Training_SliderModelTrainNu, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_Model_Training_SliderModelTrainNu, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_outline_width(ui->Model_Training_Model_Training_SliderModelTrainNu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_Model_Training_SliderModelTrainNu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Model_Training_Model_Training_SliderModelTrainNu, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_Model_Training_SliderModelTrainNu, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_Model_Training_SliderModelTrainNu, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_Model_Training_SliderModelTrainNu, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_Model_Training_SliderModelTrainNu, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style for Model_Training_Model_Training_SliderModelTrainNu, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_Model_Training_SliderModelTrainNu, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_Model_Training_SliderModelTrainNu, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_Model_Training_SliderModelTrainNu, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_Model_Training_SliderModelTrainNu, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes Model_Training_SliderModelTrainNu_Label
	ui->Model_Training_SliderModelTrainNu_Label = lv_label_create(ui->Model_Training_Model_Training_Cont);
	lv_label_set_text(ui->Model_Training_SliderModelTrainNu_Label, "Nu:0.10");
	lv_label_set_long_mode(ui->Model_Training_SliderModelTrainNu_Label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Model_Training_SliderModelTrainNu_Label, 44, 127);
	lv_obj_set_size(ui->Model_Training_SliderModelTrainNu_Label, 212, 32);

	//Write style for Model_Training_SliderModelTrainNu_Label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Model_Training_SliderModelTrainNu_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_SliderModelTrainNu_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_SliderModelTrainNu_Label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_SliderModelTrainNu_Label, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Model_Training_SliderModelTrainNu_Label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_SliderModelTrainNu_Label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_SliderModelTrainNu_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_SliderModelTrainNu_Label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_SliderModelTrainNu_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_SliderModelTrainNu_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_SliderModelTrainNu_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_SliderModelTrainNu_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_SliderModelTrainNu_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_SliderModelTrainNu_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_Exit
	ui->Model_Training_Exit = lv_btn_create(ui->Model_Training_Model_Training_Cont);
	ui->Model_Training_Exit_label = lv_label_create(ui->Model_Training_Exit);
	lv_label_set_text(ui->Model_Training_Exit_label, "Exit");
	lv_label_set_long_mode(ui->Model_Training_Exit_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Model_Training_Exit_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Model_Training_Exit, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->Model_Training_Exit_label, LV_PCT(100));
	lv_obj_set_pos(ui->Model_Training_Exit, 111, 185);
	lv_obj_set_size(ui->Model_Training_Exit, 78, 23);

	//Write style for Model_Training_Exit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Model_Training_Exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_Exit, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Model_Training_Exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Model_Training_Exit, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Model_Training_Exit, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Model_Training_Exit, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_Exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_Exit, lv_color_hex(0x12548b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_Exit, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Model_Training_Exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_Exit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_img_1
	ui->Model_Training_img_1 = lv_img_create(ui->Model_Training);
	lv_obj_add_flag(ui->Model_Training_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Model_Training_img_1, &_NXP_Logo_RGB_Colour_alpha_120x40);
	lv_img_set_pivot(ui->Model_Training_img_1, 50,50);
	lv_img_set_angle(ui->Model_Training_img_1, 0);
	lv_obj_set_pos(ui->Model_Training_img_1, 10, 10);
	lv_obj_set_size(ui->Model_Training_img_1, 120, 40);

	//Write style for Model_Training_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->Model_Training_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of Model_Training.
	Train_win_setup();

	//Update current screen layout.
	lv_obj_update_layout(ui->Model_Training);

	//Init events for screen.
	events_init_Model_Training(ui);
}
