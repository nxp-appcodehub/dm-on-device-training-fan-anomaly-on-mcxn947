/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __APP_CUSTOM_UI_H
#define __APP_CUSTOM_UI_H



extern lv_img_dsc_t svm_toy;

void Main_win_train_btn_cb();
void Train_win_start_btn_cb();
void Train_win_param_setup();
void Train_win_setup();
void Setup_main_windows();
void Train_win_param_btn_cb();
void Train_win_show_error_log(char *log);
void main_win_heatmap_update(void *model, void* img_data);
void main_win_draw_current_feature_points(float* features);
void main_win_update_heatmap_points(void* features, void* img_data, lv_color_t col, bool del_old);
void algo_update_heatmap_after_train();
void algo_update_heatmap();
void update_image_widget(lv_obj_t *img_obj, lv_img_dsc_t* img_dsc);
void Train_win_Inc_sw_cb(int state);
#endif
