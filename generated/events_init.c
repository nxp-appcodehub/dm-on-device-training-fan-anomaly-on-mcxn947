/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


#include "app_custom_ui.h"

extern void Train_win_param_gamma_changed_cb();
extern void Train_win_param_nu_changed_cb();
extern void Train_win_return_btn_cb();
static void Main_Main_btnMainSwScr_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		Main_win_train_btn_cb();
		break;
	}
	default:
		break;
	}
}
void events_init_Main(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Main_Main_btnMainSwScr, Main_Main_btnMainSwScr_event_handler, LV_EVENT_ALL, ui);
}
static void Model_Training_inc_sw_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_t * status_obj = lv_event_get_target(e);
		int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? 1 : 0;
		switch(status) {
		case 0:
		{
			Train_win_Inc_sw_cb(0);
			break;
		}
		case 1:
		{
			Train_win_Inc_sw_cb(1);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}
static void Model_Training_btnModelTrainStart_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		Train_win_start_btn_cb();
		break;
	}
	default:
		break;
	}
}
static void Model_Training_btnModelTrainParam_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_PRESSED:
	{
		Train_win_param_btn_cb();
		break;
	}
	case LV_EVENT_CLICKED:
	{
		Train_win_param_setup();
		break;
	}
	default:
		break;
	}
}
static void Model_Training_btnModelTrainReturn_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		Train_win_return_btn_cb();
		break;
	}
	default:
		break;
	}
}
static void Model_Training_ModelTrainGamma_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_VALUE_CHANGED:
	{
		Train_win_param_gamma_changed_cb();
		break;
	}
	default:
		break;
	}
}
static void Model_Training_Model_Training_SliderModelTrainNu_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_VALUE_CHANGED:
	{
		Train_win_param_nu_changed_cb();
		break;
	}
	default:
		break;
	}
}
static void Model_Training_Exit_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_PRESSED:
	{
		lv_obj_add_flag(guider_ui.Model_Training_Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
void events_init_Model_Training(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Model_Training_inc_sw, Model_Training_inc_sw_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Model_Training_btnModelTrainStart, Model_Training_btnModelTrainStart_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Model_Training_btnModelTrainParam, Model_Training_btnModelTrainParam_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Model_Training_btnModelTrainReturn, Model_Training_btnModelTrainReturn_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Model_Training_ModelTrainGamma, Model_Training_ModelTrainGamma_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Model_Training_Model_Training_SliderModelTrainNu, Model_Training_Model_Training_SliderModelTrainNu_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Model_Training_Exit, Model_Training_Exit_event_handler, LV_EVENT_ALL, ui);
}

void events_init(lv_ui *ui)
{

}
