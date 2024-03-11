/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

#if LCD_SSD1963

void setup_scr_Model_Training(lv_ui *ui){

	//Write codes Model_Training
	ui->Model_Training = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->Model_Training, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->Model_Training, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_imgModelTrainNXP
	ui->Model_Training_imgModelTrainNXP = lv_img_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_imgModelTrainNXP, 678, 419);
	lv_obj_set_size(ui->Model_Training_imgModelTrainNXP, 100, 37);
	lv_obj_set_scrollbar_mode(ui->Model_Training_imgModelTrainNXP, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training_imgModelTrainNXP. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Model_Training_imgModelTrainNXP, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Model_Training_imgModelTrainNXP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Model_Training_imgModelTrainNXP, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Model_Training_imgModelTrainNXP, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Model_Training_imgModelTrainNXP,&_logo_nxp_alpha_100x37);
	lv_img_set_pivot(ui->Model_Training_imgModelTrainNXP, 50,50);
	lv_img_set_angle(ui->Model_Training_imgModelTrainNXP, 0);

	//Write codes Model_Training_labelModelTrainCurFunc
	ui->Model_Training_labelModelTrainCurFunc = lv_label_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_labelModelTrainCurFunc, 38, 43);
	lv_obj_set_size(ui->Model_Training_labelModelTrainCurFunc, 216, 43);
	lv_obj_set_scrollbar_mode(ui->Model_Training_labelModelTrainCurFunc, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Model_Training_labelModelTrainCurFunc, "States Monitor");
	lv_label_set_long_mode(ui->Model_Training_labelModelTrainCurFunc, LV_LABEL_LONG_WRAP);

	//Set style for Model_Training_labelModelTrainCurFunc. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_labelModelTrainCurFunc, lv_color_make(0x52, 0xe3, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_labelModelTrainCurFunc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_labelModelTrainCurFunc, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_labelModelTrainCurFunc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_labelModelTrainCurFunc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_labelModelTrainCurFunc, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_labelModelTrainCurFunc, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_labelModelTrainCurFunc, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_labelModelTrainCurFunc, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_labelModelTrainCurFunc, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_btnModelTrainReturn
	ui->Model_Training_btnModelTrainReturn = lv_btn_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_btnModelTrainReturn, 496, 340);
	lv_obj_set_size(ui->Model_Training_btnModelTrainReturn, 112, 50);
	lv_obj_set_scrollbar_mode(ui->Model_Training_btnModelTrainReturn, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training_btnModelTrainReturn. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_btnModelTrainReturn, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_btnModelTrainReturn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_btnModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_btnModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Model_Training_btnModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_btnModelTrainReturn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_btnModelTrainReturn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Model_Training_btnModelTrainReturn_label = lv_label_create(ui->Model_Training_btnModelTrainReturn);
	lv_label_set_text(ui->Model_Training_btnModelTrainReturn_label, "Return");
	lv_obj_set_style_pad_all(ui->Model_Training_btnModelTrainReturn, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Model_Training_btnModelTrainReturn_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Model_Training_labelModelTrainInfo
	ui->Model_Training_labelModelTrainInfo = lv_label_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_labelModelTrainInfo, 38, 117);
	lv_obj_set_size(ui->Model_Training_labelModelTrainInfo, 216, 43);
	lv_obj_set_scrollbar_mode(ui->Model_Training_labelModelTrainInfo, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Model_Training_labelModelTrainInfo, "progress");
	lv_label_set_long_mode(ui->Model_Training_labelModelTrainInfo, LV_LABEL_LONG_WRAP);

	//Set style for Model_Training_labelModelTrainInfo. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_labelModelTrainInfo, lv_color_make(0x52, 0xe3, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_labelModelTrainInfo, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_labelModelTrainInfo, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_labelModelTrainInfo, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_labelModelTrainInfo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_labelModelTrainInfo, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_labelModelTrainInfo, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_labelModelTrainInfo, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_labelModelTrainInfo, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_labelModelTrainInfo, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_labelModelTrainStat
	ui->Model_Training_labelModelTrainStat = lv_label_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_labelModelTrainStat, 419, 74);
	lv_obj_set_size(ui->Model_Training_labelModelTrainStat, 216, 43);
	lv_obj_set_scrollbar_mode(ui->Model_Training_labelModelTrainStat, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Model_Training_labelModelTrainStat, "Status");
	lv_label_set_long_mode(ui->Model_Training_labelModelTrainStat, LV_LABEL_LONG_WRAP);

	//Set style for Model_Training_labelModelTrainStat. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_labelModelTrainStat, lv_color_make(0x52, 0xe3, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_labelModelTrainStat, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_labelModelTrainStat, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_labelModelTrainStat, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_labelModelTrainStat, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_labelModelTrainStat, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_labelModelTrainStat, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_labelModelTrainStat, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_labelModelTrainStat, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_labelModelTrainStat, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_barModelTrainProgress
	ui->Model_Training_barModelTrainProgress = lv_bar_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_barModelTrainProgress, 138, 227);
	lv_obj_set_size(ui->Model_Training_barModelTrainProgress, 525, 26);
	lv_obj_set_scrollbar_mode(ui->Model_Training_barModelTrainProgress, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training_barModelTrainProgress. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_barModelTrainProgress, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_barModelTrainProgress, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_barModelTrainProgress, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_barModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_barModelTrainProgress, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_barModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_barModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_barModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for Model_Training_barModelTrainProgress. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_barModelTrainProgress, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_barModelTrainProgress, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_barModelTrainProgress, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_anim_time(ui->Model_Training_barModelTrainProgress, 1000, 0);
	lv_bar_set_mode(ui->Model_Training_barModelTrainProgress, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->Model_Training_barModelTrainProgress, 38, LV_ANIM_OFF);

	//Write codes Model_Training_btnModelTrainStart
	ui->Model_Training_btnModelTrainStart = lv_btn_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_btnModelTrainStart, 192, 340);
	lv_obj_set_size(ui->Model_Training_btnModelTrainStart, 112, 50);
	lv_obj_set_scrollbar_mode(ui->Model_Training_btnModelTrainStart, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training_btnModelTrainStart. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_btnModelTrainStart, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_btnModelTrainStart, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_btnModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_btnModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Model_Training_btnModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_btnModelTrainStart, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_btnModelTrainStart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Model_Training_btnModelTrainStart_label = lv_label_create(ui->Model_Training_btnModelTrainStart);
	lv_label_set_text(ui->Model_Training_btnModelTrainStart_label, "Start");
	lv_obj_set_style_pad_all(ui->Model_Training_btnModelTrainStart, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Model_Training_btnModelTrainStart_label, LV_ALIGN_CENTER, 0, 0);
    
    
    ui->Model_Training_Cont = lv_obj_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_Cont, 100, 20);
	lv_obj_set_size(ui->Model_Training_Cont, 600, 300);
    
    ui->Model_Training_Cont_Label = lv_label_create(ui->Model_Training_Cont);
	lv_label_set_text(ui->Model_Training_Cont_Label, "Param");
	lv_obj_align(ui->Model_Training_Cont_Label, LV_ALIGN_TOP_MID, 0, 0);
    
    ui->Model_Training_SliderModelTrainGamma = lv_slider_create(ui->Model_Training_Cont);
	lv_obj_set_pos(ui->Model_Training_SliderModelTrainGamma, 50, 112);
	lv_obj_set_size(ui->Model_Training_SliderModelTrainGamma, 500, 16);
	lv_slider_set_range(ui->Model_Training_SliderModelTrainGamma,0, 400);
	lv_slider_set_value(ui->Model_Training_SliderModelTrainGamma,100,false);
    
    ui->Model_Training_SliderModelTrainGamma_Label = lv_label_create(ui->Model_Training_Cont);
	lv_obj_set_pos(ui->Model_Training_SliderModelTrainGamma_Label, 250, 67);
	lv_obj_set_size(ui->Model_Training_SliderModelTrainGamma_Label, 100, 32); 
    lv_label_set_text(ui->Model_Training_SliderModelTrainGamma_Label, "Gamma:0100");
    lv_obj_set_style_text_align(ui->Model_Training_SliderModelTrainGamma_Label, LV_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    
  
    ui->Model_Training_SliderModelTrainNu = lv_slider_create(ui->Model_Training_Cont);
	lv_obj_set_pos(ui->Model_Training_SliderModelTrainNu, 50, 208);
	lv_obj_set_size(ui->Model_Training_SliderModelTrainNu, 500, 16);
	lv_slider_set_range(ui->Model_Training_SliderModelTrainNu,0, 100);
	lv_slider_set_value(ui->Model_Training_SliderModelTrainNu,10,false);  
    
    ui->Model_Training_SliderModelTrainNu_Label = lv_label_create(ui->Model_Training_Cont);
	lv_obj_set_pos(ui->Model_Training_SliderModelTrainNu_Label, 250, 162);
	lv_obj_set_size(ui->Model_Training_SliderModelTrainNu_Label, 100, 32);
    lv_label_set_text(ui->Model_Training_SliderModelTrainNu_Label, "Nu:0.10");
    lv_obj_set_style_text_align(ui->Model_Training_SliderModelTrainNu_Label, LV_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    
    ui->Model_Training_btnModelTrainParamSet = lv_btn_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_btnModelTrainParamSet, 344, 340);
	lv_obj_set_size(ui->Model_Training_btnModelTrainParamSet, 112, 50);
    
    ui->Model_Training_btnModelTrainParamSet_Label = lv_label_create(ui->Model_Training_btnModelTrainParamSet);
    lv_label_set_text(ui->Model_Training_btnModelTrainParamSet_Label, "ParamSet");
    lv_obj_set_style_text_align(ui->Model_Training_btnModelTrainParamSet_Label, LV_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    events_param_set(ui);
    events_slider_nu_value_changed(ui);
    events_slider_gamma_value_changed(ui);
    
    // the cont is hidden at first
    lv_obj_add_flag(ui->Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
    ui->Model_Training_Cont_Hidden = 1;
    
	//Init events for screen
	events_init_Model_Training(ui);
    events_init_Start(ui);
}
#else

void setup_scr_Model_Training(lv_ui *ui){

	//Write codes Model_Training
	ui->Model_Training = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->Model_Training, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->Model_Training, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	//Write codes Model_Training_labelModelTrainCurFunc
	ui->Model_Training_labelModelTrainCurFunc = lv_label_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_labelModelTrainCurFunc, 38, 43);
	lv_obj_set_size(ui->Model_Training_labelModelTrainCurFunc, 216, 43);
	lv_obj_set_scrollbar_mode(ui->Model_Training_labelModelTrainCurFunc, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Model_Training_labelModelTrainCurFunc, "States Monitor");
	lv_label_set_long_mode(ui->Model_Training_labelModelTrainCurFunc, LV_LABEL_LONG_WRAP);

	//Set style for Model_Training_labelModelTrainCurFunc. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_labelModelTrainCurFunc, lv_color_make(0x52, 0xe3, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_labelModelTrainCurFunc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_labelModelTrainCurFunc, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_labelModelTrainCurFunc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_labelModelTrainCurFunc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_labelModelTrainCurFunc, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_labelModelTrainCurFunc, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_labelModelTrainCurFunc, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_labelModelTrainCurFunc, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_labelModelTrainCurFunc, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_labelModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_btnModelTrainReturn
	ui->Model_Training_btnModelTrainReturn = lv_btn_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_btnModelTrainReturn, 496, 340);
	lv_obj_set_size(ui->Model_Training_btnModelTrainReturn, 112, 50);
	lv_obj_set_scrollbar_mode(ui->Model_Training_btnModelTrainReturn, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training_btnModelTrainReturn. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_btnModelTrainReturn, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_btnModelTrainReturn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_btnModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_btnModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_btnModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Model_Training_btnModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_btnModelTrainReturn, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_btnModelTrainReturn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_btnModelTrainReturn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Model_Training_btnModelTrainReturn_label = lv_label_create(ui->Model_Training_btnModelTrainReturn);
	lv_label_set_text(ui->Model_Training_btnModelTrainReturn_label, "Return");
	lv_obj_set_style_pad_all(ui->Model_Training_btnModelTrainReturn, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Model_Training_btnModelTrainReturn_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Model_Training_labelModelTrainInfo
	ui->Model_Training_labelModelTrainInfo = lv_label_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_labelModelTrainInfo, 38, 117);
	lv_obj_set_size(ui->Model_Training_labelModelTrainInfo, 216, 43);
	lv_obj_set_scrollbar_mode(ui->Model_Training_labelModelTrainInfo, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Model_Training_labelModelTrainInfo, "progress");
	lv_label_set_long_mode(ui->Model_Training_labelModelTrainInfo, LV_LABEL_LONG_WRAP);

	//Set style for Model_Training_labelModelTrainInfo. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_labelModelTrainInfo, lv_color_make(0x52, 0xe3, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_labelModelTrainInfo, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_labelModelTrainInfo, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_labelModelTrainInfo, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_labelModelTrainInfo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_labelModelTrainInfo, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_labelModelTrainInfo, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_labelModelTrainInfo, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_labelModelTrainInfo, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_labelModelTrainInfo, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_labelModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_labelModelTrainStat
	ui->Model_Training_labelModelTrainStat = lv_label_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_labelModelTrainStat, 419, 74);
	lv_obj_set_size(ui->Model_Training_labelModelTrainStat, 216, 43);
	lv_obj_set_scrollbar_mode(ui->Model_Training_labelModelTrainStat, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Model_Training_labelModelTrainStat, "Status");
	lv_label_set_long_mode(ui->Model_Training_labelModelTrainStat, LV_LABEL_LONG_WRAP);

	//Set style for Model_Training_labelModelTrainStat. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_labelModelTrainStat, lv_color_make(0x52, 0xe3, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_labelModelTrainStat, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_labelModelTrainStat, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_labelModelTrainStat, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_labelModelTrainStat, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_labelModelTrainStat, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_labelModelTrainStat, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Model_Training_labelModelTrainStat, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_labelModelTrainStat, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Model_Training_labelModelTrainStat, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Model_Training_labelModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Model_Training_barModelTrainProgress
	ui->Model_Training_barModelTrainProgress = lv_bar_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_barModelTrainProgress, 138, 227);
	lv_obj_set_size(ui->Model_Training_barModelTrainProgress, 525, 26);
	lv_obj_set_scrollbar_mode(ui->Model_Training_barModelTrainProgress, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training_barModelTrainProgress. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_barModelTrainProgress, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_barModelTrainProgress, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_barModelTrainProgress, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_barModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_barModelTrainProgress, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_barModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_barModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_barModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for Model_Training_barModelTrainProgress. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_barModelTrainProgress, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_barModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_barModelTrainProgress, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_barModelTrainProgress, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_anim_time(ui->Model_Training_barModelTrainProgress, 1000, 0);
	lv_bar_set_mode(ui->Model_Training_barModelTrainProgress, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->Model_Training_barModelTrainProgress, 38, LV_ANIM_OFF);

	//Write codes Model_Training_btnModelTrainStart
	ui->Model_Training_btnModelTrainStart = lv_btn_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_btnModelTrainStart, 192, 340);
	lv_obj_set_size(ui->Model_Training_btnModelTrainStart, 112, 50);
	lv_obj_set_scrollbar_mode(ui->Model_Training_btnModelTrainStart, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training_btnModelTrainStart. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Model_Training_btnModelTrainStart, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Model_Training_btnModelTrainStart, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Model_Training_btnModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Model_Training_btnModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Model_Training_btnModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Model_Training_btnModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Model_Training_btnModelTrainStart, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Model_Training_btnModelTrainStart, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Model_Training_btnModelTrainStart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Model_Training_btnModelTrainStart_label = lv_label_create(ui->Model_Training_btnModelTrainStart);
	lv_label_set_text(ui->Model_Training_btnModelTrainStart_label, "Start");
	lv_obj_set_style_pad_all(ui->Model_Training_btnModelTrainStart, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Model_Training_btnModelTrainStart_label, LV_ALIGN_CENTER, 0, 0);
    
    
    ui->Model_Training_Cont = lv_obj_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_Cont, 100, 20);
	lv_obj_set_size(ui->Model_Training_Cont, 600, 300);
    
    ui->Model_Training_Cont_Label = lv_label_create(ui->Model_Training_Cont);
	lv_label_set_text(ui->Model_Training_Cont_Label, "Param");
	lv_obj_align(ui->Model_Training_Cont_Label, LV_ALIGN_TOP_MID, 0, 0);
    
    ui->Model_Training_SliderModelTrainGamma = lv_slider_create(ui->Model_Training_Cont);
	lv_obj_set_pos(ui->Model_Training_SliderModelTrainGamma, 50, 112);
	lv_obj_set_size(ui->Model_Training_SliderModelTrainGamma, 500, 16);
	lv_slider_set_range(ui->Model_Training_SliderModelTrainGamma,0, 400);
	lv_slider_set_value(ui->Model_Training_SliderModelTrainGamma,100,false);
    
    ui->Model_Training_SliderModelTrainGamma_Label = lv_label_create(ui->Model_Training_Cont);
	lv_obj_set_pos(ui->Model_Training_SliderModelTrainGamma_Label, 250, 67);
	lv_obj_set_size(ui->Model_Training_SliderModelTrainGamma_Label, 200, 32); 
    lv_label_set_text(ui->Model_Training_SliderModelTrainGamma_Label, "Gam:0100");
    lv_obj_set_style_text_align(ui->Model_Training_SliderModelTrainGamma_Label, LV_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    
  
    ui->Model_Training_SliderModelTrainNu = lv_slider_create(ui->Model_Training_Cont);
	lv_obj_set_pos(ui->Model_Training_SliderModelTrainNu, 50, 208);
	lv_obj_set_size(ui->Model_Training_SliderModelTrainNu, 500, 16);
	lv_slider_set_range(ui->Model_Training_SliderModelTrainNu,0, 100);
	lv_slider_set_value(ui->Model_Training_SliderModelTrainNu,10,false);  
    
    ui->Model_Training_SliderModelTrainNu_Label = lv_label_create(ui->Model_Training_Cont);
	lv_obj_set_pos(ui->Model_Training_SliderModelTrainNu_Label, 250, 162);
	lv_obj_set_size(ui->Model_Training_SliderModelTrainNu_Label, 100, 32);
    lv_label_set_text(ui->Model_Training_SliderModelTrainNu_Label, "Nu:0.10");
    lv_obj_set_style_text_align(ui->Model_Training_SliderModelTrainNu_Label, LV_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    
    ui->Model_Training_btnModelTrainParamSet = lv_btn_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_btnModelTrainParamSet, 344, 340);
	lv_obj_set_size(ui->Model_Training_btnModelTrainParamSet, 112, 50);
    lv_obj_set_style_text_font(ui->Model_Training_btnModelTrainParamSet, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Model_Training_btnModelTrainParamSet, LV_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    
    ui->Model_Training_btnModelTrainParamSet_Label = lv_label_create(ui->Model_Training_btnModelTrainParamSet);
    lv_label_set_text(ui->Model_Training_btnModelTrainParamSet_Label, "Param");
	lv_obj_align(ui->Model_Training_btnModelTrainParamSet_Label, LV_ALIGN_CENTER, 0, 0);
    
    // put the log here, to use undef
    //Write codes Model_Training_imgModelTrainNXP
	ui->Model_Training_imgModelTrainNXP = lv_img_create(ui->Model_Training);
	lv_obj_set_pos(ui->Model_Training_imgModelTrainNXP, 678 - 30, 419);
    #undef lv_obj_set_size
	lv_obj_set_size(ui->Model_Training_imgModelTrainNXP, 100, 37);
	lv_obj_set_scrollbar_mode(ui->Model_Training_imgModelTrainNXP, LV_SCROLLBAR_MODE_OFF);

	//Set style for Model_Training_imgModelTrainNXP. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Model_Training_imgModelTrainNXP, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Model_Training_imgModelTrainNXP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Model_Training_imgModelTrainNXP, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Model_Training_imgModelTrainNXP, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Model_Training_imgModelTrainNXP,&_logo_nxp_alpha_100x37);
	lv_img_set_pivot(ui->Model_Training_imgModelTrainNXP, 50,50);
	lv_img_set_angle(ui->Model_Training_imgModelTrainNXP, 0);
  
    events_param_set(ui);
    events_slider_nu_value_changed(ui);
    events_slider_gamma_value_changed(ui);
    
    // the cont is hidden at first
    lv_obj_add_flag(ui->Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
    ui->Model_Training_Cont_Hidden = 1;
    
	//Init events for screen
	events_init_Model_Training(ui);
    events_init_Start(ui);
}
#endif