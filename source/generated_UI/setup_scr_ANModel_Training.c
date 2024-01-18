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


void setup_scr_ANModel_Training(lv_ui *ui){
#if 0
	//Write codes ANModel_Training
	ui->ANModel_Training = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->ANModel_Training, LV_SCROLLBAR_MODE_OFF);

	//Set style for ANModel_Training. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->ANModel_Training, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ANModel_Training, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ANModel_Training, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ANModel_Training, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes ANModel_Training_imgANModelTrainNXP
	ui->ANModel_Training_imgANModelTrainNXP = lv_img_create(ui->ANModel_Training);
	lv_obj_set_pos(ui->ANModel_Training_imgANModelTrainNXP, 678, 419);
	lv_obj_set_size(ui->ANModel_Training_imgANModelTrainNXP, 100, 37);
	lv_obj_set_scrollbar_mode(ui->ANModel_Training_imgANModelTrainNXP, LV_SCROLLBAR_MODE_OFF);

	//Set style for ANModel_Training_imgANModelTrainNXP. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->ANModel_Training_imgANModelTrainNXP, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->ANModel_Training_imgANModelTrainNXP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->ANModel_Training_imgANModelTrainNXP, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->ANModel_Training_imgANModelTrainNXP, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->ANModel_Training_imgANModelTrainNXP,&_logo_nxp_alpha_100x37);
	lv_img_set_pivot(ui->ANModel_Training_imgANModelTrainNXP, 50,50);
	lv_img_set_angle(ui->ANModel_Training_imgANModelTrainNXP, 0);

	//Write codes ANModel_Training_labelANModelTrainCurFunc
	ui->ANModel_Training_labelANModelTrainCurFunc = lv_label_create(ui->ANModel_Training);
	lv_obj_set_pos(ui->ANModel_Training_labelANModelTrainCurFunc, 38, 43);
	lv_obj_set_size(ui->ANModel_Training_labelANModelTrainCurFunc, 216, 43);
	lv_obj_set_scrollbar_mode(ui->ANModel_Training_labelANModelTrainCurFunc, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->ANModel_Training_labelANModelTrainCurFunc, "Anomaly Detect");
	lv_label_set_long_mode(ui->ANModel_Training_labelANModelTrainCurFunc, LV_LABEL_LONG_WRAP);

	//Set style for ANModel_Training_labelANModelTrainCurFunc. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ANModel_Training_labelANModelTrainCurFunc, lv_color_make(0x52, 0xe3, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ANModel_Training_labelANModelTrainCurFunc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ANModel_Training_labelANModelTrainCurFunc, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->ANModel_Training_labelANModelTrainCurFunc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->ANModel_Training_labelANModelTrainCurFunc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->ANModel_Training_labelANModelTrainCurFunc, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->ANModel_Training_labelANModelTrainCurFunc, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->ANModel_Training_labelANModelTrainCurFunc, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->ANModel_Training_labelANModelTrainCurFunc, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->ANModel_Training_labelANModelTrainCurFunc, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->ANModel_Training_labelANModelTrainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes ANModel_Training_btnANModelTrainReturn
	ui->ANModel_Training_btnANModelTrainReturn = lv_btn_create(ui->ANModel_Training);
	lv_obj_set_pos(ui->ANModel_Training_btnANModelTrainReturn, 496, 340);
	lv_obj_set_size(ui->ANModel_Training_btnANModelTrainReturn, 112, 50);
	lv_obj_set_scrollbar_mode(ui->ANModel_Training_btnANModelTrainReturn, LV_SCROLLBAR_MODE_OFF);

	//Set style for ANModel_Training_btnANModelTrainReturn. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ANModel_Training_btnANModelTrainReturn, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ANModel_Training_btnANModelTrainReturn, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ANModel_Training_btnANModelTrainReturn, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ANModel_Training_btnANModelTrainReturn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ANModel_Training_btnANModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->ANModel_Training_btnANModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->ANModel_Training_btnANModelTrainReturn, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->ANModel_Training_btnANModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->ANModel_Training_btnANModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->ANModel_Training_btnANModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->ANModel_Training_btnANModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->ANModel_Training_btnANModelTrainReturn, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->ANModel_Training_btnANModelTrainReturn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->ANModel_Training_btnANModelTrainReturn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->ANModel_Training_btnANModelTrainReturn, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->ANModel_Training_btnANModelTrainReturn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->ANModel_Training_btnANModelTrainReturn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->ANModel_Training_btnANModelTrainReturn_label = lv_label_create(ui->ANModel_Training_btnANModelTrainReturn);
	lv_label_set_text(ui->ANModel_Training_btnANModelTrainReturn_label, "Return");
	lv_obj_set_style_pad_all(ui->ANModel_Training_btnANModelTrainReturn, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->ANModel_Training_btnANModelTrainReturn_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes ANModel_Training_labelANModelTrainInfo
	ui->ANModel_Training_labelANModelTrainInfo = lv_label_create(ui->ANModel_Training);
	lv_obj_set_pos(ui->ANModel_Training_labelANModelTrainInfo, 38, 117);
	lv_obj_set_size(ui->ANModel_Training_labelANModelTrainInfo, 216, 43);
	lv_obj_set_scrollbar_mode(ui->ANModel_Training_labelANModelTrainInfo, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->ANModel_Training_labelANModelTrainInfo, "Information");
	lv_label_set_long_mode(ui->ANModel_Training_labelANModelTrainInfo, LV_LABEL_LONG_WRAP);

	//Set style for ANModel_Training_labelANModelTrainInfo. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ANModel_Training_labelANModelTrainInfo, lv_color_make(0x52, 0xe3, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ANModel_Training_labelANModelTrainInfo, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ANModel_Training_labelANModelTrainInfo, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->ANModel_Training_labelANModelTrainInfo, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->ANModel_Training_labelANModelTrainInfo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->ANModel_Training_labelANModelTrainInfo, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->ANModel_Training_labelANModelTrainInfo, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->ANModel_Training_labelANModelTrainInfo, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->ANModel_Training_labelANModelTrainInfo, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->ANModel_Training_labelANModelTrainInfo, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->ANModel_Training_labelANModelTrainInfo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes ANModel_Training_labelANModelTrainStat
	ui->ANModel_Training_labelANModelTrainStat = lv_label_create(ui->ANModel_Training);
	lv_obj_set_pos(ui->ANModel_Training_labelANModelTrainStat, 419, 74);
	lv_obj_set_size(ui->ANModel_Training_labelANModelTrainStat, 216, 43);
	lv_obj_set_scrollbar_mode(ui->ANModel_Training_labelANModelTrainStat, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->ANModel_Training_labelANModelTrainStat, "Status");
	lv_label_set_long_mode(ui->ANModel_Training_labelANModelTrainStat, LV_LABEL_LONG_WRAP);

	//Set style for ANModel_Training_labelANModelTrainStat. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ANModel_Training_labelANModelTrainStat, lv_color_make(0x52, 0xe3, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ANModel_Training_labelANModelTrainStat, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ANModel_Training_labelANModelTrainStat, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->ANModel_Training_labelANModelTrainStat, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->ANModel_Training_labelANModelTrainStat, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->ANModel_Training_labelANModelTrainStat, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->ANModel_Training_labelANModelTrainStat, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->ANModel_Training_labelANModelTrainStat, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->ANModel_Training_labelANModelTrainStat, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->ANModel_Training_labelANModelTrainStat, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->ANModel_Training_labelANModelTrainStat, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes ANModel_Training_barANModelTrainProgress
	ui->ANModel_Training_barANModelTrainProgress = lv_bar_create(ui->ANModel_Training);
	lv_obj_set_pos(ui->ANModel_Training_barANModelTrainProgress, 138, 227);
	lv_obj_set_size(ui->ANModel_Training_barANModelTrainProgress, 525, 26);
	lv_obj_set_scrollbar_mode(ui->ANModel_Training_barANModelTrainProgress, LV_SCROLLBAR_MODE_OFF);

	//Set style for ANModel_Training_barANModelTrainProgress. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ANModel_Training_barANModelTrainProgress, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ANModel_Training_barANModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ANModel_Training_barANModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ANModel_Training_barANModelTrainProgress, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ANModel_Training_barANModelTrainProgress, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->ANModel_Training_barANModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->ANModel_Training_barANModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->ANModel_Training_barANModelTrainProgress, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->ANModel_Training_barANModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->ANModel_Training_barANModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->ANModel_Training_barANModelTrainProgress, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for ANModel_Training_barANModelTrainProgress. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ANModel_Training_barANModelTrainProgress, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ANModel_Training_barANModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ANModel_Training_barANModelTrainProgress, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ANModel_Training_barANModelTrainProgress, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ANModel_Training_barANModelTrainProgress, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_anim_time(ui->ANModel_Training_barANModelTrainProgress, 1000, 0);
	lv_bar_set_mode(ui->ANModel_Training_barANModelTrainProgress, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->ANModel_Training_barANModelTrainProgress, 50, LV_ANIM_OFF);

	//Write codes ANModel_Training_btnANModelTrainStart
	ui->ANModel_Training_btnANModelTrainStart = lv_btn_create(ui->ANModel_Training);
	lv_obj_set_pos(ui->ANModel_Training_btnANModelTrainStart, 192, 340);
	lv_obj_set_size(ui->ANModel_Training_btnANModelTrainStart, 112, 50);
	lv_obj_set_scrollbar_mode(ui->ANModel_Training_btnANModelTrainStart, LV_SCROLLBAR_MODE_OFF);

	//Set style for ANModel_Training_btnANModelTrainStart. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ANModel_Training_btnANModelTrainStart, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ANModel_Training_btnANModelTrainStart, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ANModel_Training_btnANModelTrainStart, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ANModel_Training_btnANModelTrainStart, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ANModel_Training_btnANModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->ANModel_Training_btnANModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->ANModel_Training_btnANModelTrainStart, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->ANModel_Training_btnANModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->ANModel_Training_btnANModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->ANModel_Training_btnANModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->ANModel_Training_btnANModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->ANModel_Training_btnANModelTrainStart, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->ANModel_Training_btnANModelTrainStart, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->ANModel_Training_btnANModelTrainStart, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->ANModel_Training_btnANModelTrainStart, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->ANModel_Training_btnANModelTrainStart, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->ANModel_Training_btnANModelTrainStart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->ANModel_Training_btnANModelTrainStart_label = lv_label_create(ui->ANModel_Training_btnANModelTrainStart);
	lv_label_set_text(ui->ANModel_Training_btnANModelTrainStart_label, "Start");
	lv_obj_set_style_pad_all(ui->ANModel_Training_btnANModelTrainStart, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->ANModel_Training_btnANModelTrainStart_label, LV_ALIGN_CENTER, 0, 0);
#endif
	//Init events for screen
	events_init_ANModel_Training(ui);
}
