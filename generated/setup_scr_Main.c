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


void setup_scr_Main(lv_ui *ui)
{
	//Write codes Main
	ui->Main = lv_obj_create(NULL);
	lv_obj_set_size(ui->Main, 480, 320);
	lv_obj_set_scrollbar_mode(ui->Main, LV_SCROLLBAR_MODE_OFF);

	//Write style for Main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main, lv_color_hex(0xFFFFFF), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main, lv_color_hex(0x2F92DA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_main_stop(ui->Main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_stop(ui->Main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_labelMainCurFunc
	ui->Main_labelMainCurFunc = lv_label_create(ui->Main);
	lv_label_set_text(ui->Main_labelMainCurFunc, "Anomaly Detect");
	lv_label_set_long_mode(ui->Main_labelMainCurFunc, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Main_labelMainCurFunc, 144, 16);
	lv_obj_set_size(ui->Main_labelMainCurFunc, 192, 32);

	//Write style for Main_labelMainCurFunc, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_labelMainCurFunc, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_labelMainCurFunc, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_labelMainCurFunc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_labelMainCurFunc, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_labelMainCurFunc, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Main_img_status
	ui->Main_Main_img_status = lv_animimg_create(ui->Main);
	lv_animimg_set_src(ui->Main_Main_img_status, (const void **) Main_Main_img_status_imgs, 2, false);
	lv_animimg_set_duration(ui->Main_Main_img_status, 30*2);
	lv_animimg_set_repeat_count(ui->Main_Main_img_status, 3);
	lv_img_set_src(ui->Main_Main_img_status, Main_Main_img_status_imgs[0]);
	lv_obj_set_pos(ui->Main_Main_img_status, 308, 44);
	lv_obj_set_size(ui->Main_Main_img_status, 160, 160);

	//Write codes Main_Main_chart_1
	ui->Main_Main_chart_1 = lv_chart_create(ui->Main);
	lv_chart_set_type(ui->Main_Main_chart_1, LV_CHART_TYPE_LINE);
	lv_chart_set_div_line_count(ui->Main_Main_chart_1, 5, 4);
	lv_chart_set_point_count(ui->Main_Main_chart_1, 5);
	lv_chart_set_range(ui->Main_Main_chart_1, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
	lv_chart_set_range(ui->Main_Main_chart_1, LV_CHART_AXIS_SECONDARY_Y, 0, 100);
	lv_obj_set_pos(ui->Main_Main_chart_1, 212, 212);
	lv_obj_set_size(ui->Main_Main_chart_1, 244, 104);
	lv_obj_set_scrollbar_mode(ui->Main_Main_chart_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for Main_Main_chart_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Main_Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_Main_chart_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_Main_chart_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_Main_chart_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_Main_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Main_Main_chart_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->Main_Main_chart_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Main_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Main_Main_chart_1, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Main_Main_chart_1, lv_color_hex(0x151212), LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Main_chart_1, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Main_chart_1, 255, LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->Main_Main_chart_1, 2, LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Main_chart_1, lv_color_hex(0xe8e8e8), LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Main_chart_1, 255, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes Main_Main_chart_Label
	ui->Main_Main_chart_Label = lv_label_create(ui->Main);
	lv_label_set_text(ui->Main_Main_chart_Label, "Health:0.00");
	lv_label_set_long_mode(ui->Main_Main_chart_Label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Main_Main_chart_Label, 212, 198);
	lv_obj_set_size(ui->Main_Main_chart_Label, 200, 16);

	//Write style for Main_Main_chart_Label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Main_Main_chart_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_Main_chart_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_Main_chart_Label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Main_chart_Label, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Main_chart_Label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_Main_chart_Label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_Main_chart_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Main_chart_Label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_Main_chart_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_Main_chart_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_Main_chart_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_Main_chart_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_Main_chart_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Main_chart_Label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Main_svm_toy
	ui->Main_Main_svm_toy = lv_img_create(ui->Main);
	lv_obj_add_flag(ui->Main_Main_svm_toy, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_pivot(ui->Main_Main_svm_toy, 50,50);
	lv_img_set_angle(ui->Main_Main_svm_toy, 0);
	lv_obj_set_pos(ui->Main_Main_svm_toy, 32, 80);
	lv_obj_set_size(ui->Main_Main_svm_toy, 160, 160);

	//Write style for Main_Main_svm_toy, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->Main_Main_svm_toy, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Main_btnMainSwScr
	ui->Main_Main_btnMainSwScr = lv_btn_create(ui->Main);
	ui->Main_Main_btnMainSwScr_label = lv_label_create(ui->Main_Main_btnMainSwScr);
	lv_label_set_text(ui->Main_Main_btnMainSwScr_label, "Train");
	lv_label_set_long_mode(ui->Main_Main_btnMainSwScr_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Main_Main_btnMainSwScr_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Main_Main_btnMainSwScr, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->Main_Main_btnMainSwScr_label, LV_PCT(100));
	lv_obj_set_pos(ui->Main_Main_btnMainSwScr, 16, 261);
	lv_obj_set_size(ui->Main_Main_btnMainSwScr, 100, 50);

	//Write style for Main_Main_btnMainSwScr, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Main_Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_Main_btnMainSwScr, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_Main_btnMainSwScr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_Main_btnMainSwScr, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_Main_btnMainSwScr, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Main_btnMainSwScr, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Main_btnMainSwScr, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_img_1
	ui->Main_img_1 = lv_img_create(ui->Main);
	lv_obj_add_flag(ui->Main_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Main_img_1, &_NXP_Logo_RGB_Colour_alpha_120x40);
	lv_img_set_pivot(ui->Main_img_1, 50,50);
	lv_img_set_angle(ui->Main_img_1, 0);
	lv_obj_set_pos(ui->Main_img_1, 10, 10);
	lv_obj_set_size(ui->Main_img_1, 120, 40);

	//Write style for Main_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->Main_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_label_1
	ui->Main_label_1 = lv_label_create(ui->Main);
	lv_label_set_text(ui->Main_label_1, "RMS\n|\n|\nV");
	lv_label_set_long_mode(ui->Main_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Main_label_1, 12, 80);
	lv_obj_set_size(ui->Main_label_1, 12, 79);

	//Write style for Main_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_label_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_label_2
	ui->Main_label_2 = lv_label_create(ui->Main);
	lv_label_set_text(ui->Main_label_2, "FFT Top1 -->");
	lv_label_set_long_mode(ui->Main_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Main_label_2, 32, 62);
	lv_obj_set_size(ui->Main_label_2, 121, 16);

	//Write style for Main_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_label_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_point
	ui->Main_point = lv_img_create(ui->Main);
	lv_obj_add_flag(ui->Main_point, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Main_point, &_draw_dot_alpha_8x8);
	lv_img_set_pivot(ui->Main_point, 50,50);
	lv_img_set_angle(ui->Main_point, 0);
	lv_obj_set_pos(ui->Main_point, 220, 190);
	lv_obj_set_size(ui->Main_point, 8, 8);

	//Write style for Main_point, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->Main_point, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_error_log
	ui->Main_error_log = lv_label_create(ui->Main);
	lv_label_set_text(ui->Main_error_log, "");
	lv_label_set_long_mode(ui->Main_error_log, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Main_error_log, 190, 65);
	lv_obj_set_size(ui->Main_error_log, 116, 111);

	//Write style for Main_error_log, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Main_error_log, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_error_log, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_error_log, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_error_log, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_error_log, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_error_log, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_error_log, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_error_log, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_error_log, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_error_log, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_error_log, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_error_log, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_error_log, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_error_log, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of Main.
	Setup_main_windows();

	//Update current screen layout.
	lv_obj_update_layout(ui->Main);

	//Init events for screen.
	events_init_Main(ui);
}
