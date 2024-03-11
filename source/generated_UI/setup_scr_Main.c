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
void setup_scr_Main(lv_ui *ui){

	//Write codes Main
	ui->Main = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->Main, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->Main, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_imgMainNXP
	ui->Main_imgMainNXP = lv_img_create(ui->Main);
	lv_obj_set_pos(ui->Main_imgMainNXP, 678, 419);
	lv_obj_set_size(ui->Main_imgMainNXP, 100, 37);
	lv_obj_set_scrollbar_mode(ui->Main_imgMainNXP, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_imgMainNXP. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Main_imgMainNXP, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Main_imgMainNXP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Main_imgMainNXP, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Main_imgMainNXP, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Main_imgMainNXP,&_logo_nxp_alpha_100x37);
	lv_img_set_pivot(ui->Main_imgMainNXP, 50,50);
	lv_img_set_angle(ui->Main_imgMainNXP, 0);

	//Write codes Main_labelMainCurFunc
	ui->Main_labelMainCurFunc = lv_label_create(ui->Main);
	lv_obj_set_pos(ui->Main_labelMainCurFunc, 280, 9);
	lv_obj_set_size(ui->Main_labelMainCurFunc, 216, 43);
	lv_obj_set_scrollbar_mode(ui->Main_labelMainCurFunc, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Main_labelMainCurFunc, "Anomaly Detect");
	lv_label_set_long_mode(ui->Main_labelMainCurFunc, LV_LABEL_LONG_WRAP);

	//Set style for Main_labelMainCurFunc. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_labelMainCurFunc, lv_color_make(0xda, 0xf6, 0xfd), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_labelMainCurFunc, lv_color_make(0x2F, 0xCA, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_labelMainCurFunc, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_labelMainCurFunc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_labelMainCurFunc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_labelMainCurFunc, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_labelMainCurFunc, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_labelMainCurFunc, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_labelMainCurFunc, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_labelMainCurFunc, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_btnMainSwScr
	ui->Main_btnMainSwScr = lv_btn_create(ui->Main);
	lv_obj_set_pos(ui->Main_btnMainSwScr, 666, 340);
	lv_obj_set_size(ui->Main_btnMainSwScr, 112, 50);
	lv_obj_set_scrollbar_mode(ui->Main_btnMainSwScr, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_btnMainSwScr. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_btnMainSwScr, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_btnMainSwScr, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_btnMainSwScr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_btnMainSwScr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_btnMainSwScr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_btnMainSwScr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_btnMainSwScr, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_btnMainSwScr, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_btnMainSwScr, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Main_btnMainSwScr_label = lv_label_create(ui->Main_btnMainSwScr);
	lv_label_set_text(ui->Main_btnMainSwScr_label, "Model Train");
	lv_obj_set_style_pad_all(ui->Main_btnMainSwScr, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Main_btnMainSwScr_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Main_img_status
	ui->Main_img_status = lv_img_create(ui->Main);
	lv_obj_set_pos(ui->Main_img_status, 594, 100);
	lv_obj_set_size(ui->Main_img_status, 160, 160);
	lv_obj_set_scrollbar_mode(ui->Main_img_status, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_img_status. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Main_img_status, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Main_img_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Main_img_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Main_img_status, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Main_img_status,&_fan_normal_on_alpha_160x160);
	lv_img_set_pivot(ui->Main_img_status, 80,80);
	lv_img_set_angle(ui->Main_img_status, 0);

	//Write codes Main_chart_1
	ui->Main_chart_1 = lv_chart_create(ui->Main);
	lv_obj_set_pos(ui->Main_chart_1, 97, 100);
	lv_obj_set_size(ui->Main_chart_1, 480, 160);
	lv_obj_set_scrollbar_mode(ui->Main_chart_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_chart_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_chart_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_chart_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_chart_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_chart_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_chart_1, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_chart_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_chart_1, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->Main_chart_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_chart_set_type(ui->Main_chart_1, LV_CHART_TYPE_LINE);
	lv_chart_set_range(ui->Main_chart_1,LV_CHART_AXIS_PRIMARY_Y, 0, 100);
	lv_chart_set_div_line_count(ui->Main_chart_1, 5, 4);
	lv_chart_set_point_count(ui->Main_chart_1, 7);
    
    ui->Main_chart_Label = lv_label_create(ui->Main);
    lv_obj_set_pos(ui->Main_chart_Label, 287, 68);
	lv_obj_set_size(ui->Main_chart_Label, 200, 32);
    lv_label_set_text(ui->Main_chart_Label, "Health:0.00");
    
    extern const LV_ATTRIBUTE_LARGE_CONST lv_img_dsc_t svm_toy;
    //memset((void*)svm_toy.data, 0x0, 160*160*sizeof(lv_color_t));
    ui->Main_svm_toy = lv_img_create(ui->Main);
	lv_obj_set_pos(ui->Main_svm_toy, 418, 280);
	lv_obj_set_size(ui->Main_svm_toy, 160, 160);
    //lv_img_set_src(ui->Main_svm_toy,&svm_toy);
	//Init events for screen
    ui->is_in_MainScr = true;
	events_init_Main(ui);
}
#else

#undef lv_obj_set_pos
#undef lv_obj_set_size

void setup_scr_Main(lv_ui *ui){

	//Write codes Main
	ui->Main = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->Main, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->Main, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_imgMainNXP
	ui->Main_imgMainNXP = lv_img_create(ui->Main);
	lv_obj_set_pos(ui->Main_imgMainNXP, 8, 8);
	lv_obj_set_size(ui->Main_imgMainNXP, 100, 37);
	lv_obj_set_scrollbar_mode(ui->Main_imgMainNXP, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_imgMainNXP. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Main_imgMainNXP, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Main_imgMainNXP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Main_imgMainNXP, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Main_imgMainNXP, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Main_imgMainNXP,&_logo_nxp_alpha_100x37);
	lv_img_set_pivot(ui->Main_imgMainNXP, 50,50);
	lv_img_set_angle(ui->Main_imgMainNXP, 0);

	//Write codes Main_labelMainCurFunc
	ui->Main_labelMainCurFunc = lv_label_create(ui->Main);
	lv_obj_set_pos(ui->Main_labelMainCurFunc, 175, 8);
	lv_obj_set_size(ui->Main_labelMainCurFunc, 220, 38);
	lv_obj_set_scrollbar_mode(ui->Main_labelMainCurFunc, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Main_labelMainCurFunc, "Anomaly Detect");
	lv_label_set_long_mode(ui->Main_labelMainCurFunc, LV_LABEL_LONG_WRAP);

	//Set style for Main_labelMainCurFunc. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_labelMainCurFunc, lv_color_make(0xda, 0xf6, 0xfd), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_labelMainCurFunc, lv_color_make(0x2F, 0xCA, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_labelMainCurFunc, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_labelMainCurFunc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_labelMainCurFunc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_labelMainCurFunc, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_labelMainCurFunc, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_labelMainCurFunc, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_labelMainCurFunc, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_labelMainCurFunc, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_btnMainSwScr
	ui->Main_btnMainSwScr = lv_btn_create(ui->Main);
	lv_obj_set_pos(ui->Main_btnMainSwScr, 16, 260);
	lv_obj_set_size(ui->Main_btnMainSwScr, 110, 48);
	lv_obj_set_scrollbar_mode(ui->Main_btnMainSwScr, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_btnMainSwScr. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_btnMainSwScr, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_btnMainSwScr, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_btnMainSwScr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_btnMainSwScr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_btnMainSwScr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_btnMainSwScr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_btnMainSwScr, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_btnMainSwScr, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_btnMainSwScr, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Main_btnMainSwScr_label = lv_label_create(ui->Main_btnMainSwScr);
	lv_label_set_text(ui->Main_btnMainSwScr_label, "Train");
	lv_obj_set_style_pad_all(ui->Main_btnMainSwScr, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Main_btnMainSwScr_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Main_img_status
	ui->Main_img_status = lv_img_create(ui->Main);
	lv_obj_set_pos(ui->Main_img_status, 308, 44);
	lv_obj_set_size(ui->Main_img_status, 160, 160);
	lv_obj_set_scrollbar_mode(ui->Main_img_status, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_img_status. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Main_img_status, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Main_img_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Main_img_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Main_img_status, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Main_img_status,&_fan_normal_on_alpha_160x160);
	lv_img_set_pivot(ui->Main_img_status, 80,80);
	lv_img_set_angle(ui->Main_img_status, 0);

	//Write codes Main_chart_1
	ui->Main_chart_1 = lv_chart_create(ui->Main);
	lv_obj_set_pos(ui->Main_chart_1, 212, 212);
	lv_obj_set_size(ui->Main_chart_1, 244, 104);
	lv_obj_set_scrollbar_mode(ui->Main_chart_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_chart_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_chart_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_chart_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_chart_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_chart_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_chart_1, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_chart_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_chart_1, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->Main_chart_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_chart_set_type(ui->Main_chart_1, LV_CHART_TYPE_LINE);
	lv_chart_set_range(ui->Main_chart_1,LV_CHART_AXIS_PRIMARY_Y, 0, 100);
	lv_chart_set_div_line_count(ui->Main_chart_1, 5, 4);
	lv_chart_set_point_count(ui->Main_chart_1, 7);
    
    ui->Main_chart_Label = lv_label_create(ui->Main);
    lv_obj_set_pos(ui->Main_chart_Label, 212, 212 - 16);
	lv_obj_set_size(ui->Main_chart_Label, 200, 32);
    lv_label_set_text(ui->Main_chart_Label, "Health:0.00");
    
    extern const LV_ATTRIBUTE_LARGE_CONST lv_img_dsc_t svm_toy;
    //memset((void*)svm_toy.data, 0x0, 160*160*sizeof(lv_color_t));
    ui->Main_svm_toy = lv_img_create(ui->Main);
	lv_obj_set_pos(ui->Main_svm_toy, 16, 80);
	lv_obj_set_size(ui->Main_svm_toy, 160, 160);
    //lv_img_set_src(ui->Main_svm_toy,&svm_toy);
	//Init events for screen
    ui->is_in_MainScr = true;
	events_init_Main(ui);
}
#endif
