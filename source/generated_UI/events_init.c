/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "app_main.h"
void events_init(lv_ui *ui)
{
}
extern lv_timer_t *g_pTmrFanRotate;
extern lv_timer_t *g_pTmrPgsBar;


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


void format_nu(uint8_t *buf, uint32_t value){
    // the nu must always be 0-1
    buf[0] = '0';
    buf[1] = '.';
    format_int(buf+2, value, 2);
}

void format_gamma(uint8_t *buf, uint32_t value){
    format_int(buf, value, 4);
}

extern void SetupPgsBar(void);

static void Main_btnMainSwScr_event_handler(lv_event_t *e)
{	// press "Train" button in main screen
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
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
                lv_slider_set_value(guider_ui.Model_Training_SliderModelTrainNu, g_app.nu * 100.0f, LV_ANIM_OFF);
                lv_slider_set_value(guider_ui.Model_Training_SliderModelTrainGamma, g_app.gamma, LV_ANIM_OFF);                              
                lv_obj_t *pBar = guider_ui.Model_Training_barModelTrainProgress;
                lv_bar_set_range(pBar, 0, APP_FEATURE_CNT);
                lv_bar_set_value(pBar, 0, LV_ANIM_OFF);
                
                
			}
			lv_scr_load_anim(guider_ui.Model_Training, LV_SCR_LOAD_ANIM_NONE, 100, 100, true);
            SetupPgsBar();
			guider_ui.Main_del = true;
            guider_ui.is_in_MainScr = false;
		}
	}
		break;
	default:
		break;
	}
}

void events_init_Main(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Main_btnMainSwScr, Main_btnMainSwScr_event_handler, LV_EVENT_ALL, ui);
}
/*
static void ANModel_Training_btnANModelTrainReturn_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
#if 0
		lv_obj_t * act_scr = lv_scr_act();
		lv_disp_t * d = lv_obj_get_disp(act_scr);
		if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
		{
			if (guider_ui.Main_del == true)
				setup_scr_Main(&guider_ui);
			lv_scr_load_anim(guider_ui.Main, LV_SCR_LOAD_ANIM_NONE, 100, 100, true);
			guider_ui.ANModel_Training_del = true;
         }
#else
        lv_obj_del(lv_scr_act());
        setup_scr_ANModel_Training(&guider_ui);
        lv_disp_load_scr(guider_ui.ANModel_Training);
        g_app.appState = kAppWaitForCollect; 
#endif
		
	}
		break;
	default:
		break;
	}
}
*/

/*
void events_init_ANModel_Training(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->ANModel_Training_btnANModelTrainReturn, ANModel_Training_btnANModelTrainReturn_event_handler, LV_EVENT_ALL, ui);
}
*/
extern void SetupMainScrUserLogic(void);

// "Return" button in Train screen is clicked
static void Model_Training_btnModelTrainReturn_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
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
            SetupMainScrUserLogic();
			guider_ui.Model_Training_del = true;
            
		}
	}
		break;
	default:
		break;
	}
}

void events_init_Model_Training(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Model_Training_btnModelTrainReturn, Model_Training_btnModelTrainReturn_event_handler, LV_EVENT_ALL, ui);
}

static void OnTrainingStart(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
			g_app.appState = kAppCollecting;
            g_app.featureNdx = 0;
            g_app.nu = lv_slider_get_value(guider_ui.Model_Training_SliderModelTrainNu) / 100.0f;
            g_app.gamma = lv_slider_get_value(guider_ui.Model_Training_SliderModelTrainGamma);
			break;
        default:
            break;
	};
}

void events_init_Start(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Model_Training_btnModelTrainStart, OnTrainingStart, LV_EVENT_ALL, ui);
}

static void OnClickedParamSet(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);
    
	switch (code)
	{
		case LV_EVENT_CLICKED:
			if(ui->Model_Training_Cont_Hidden) { 
                lv_obj_clear_flag(ui->Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
                uint8_t text_buf[8] = "Nu:";
                format_nu(text_buf+3, lv_slider_get_value(ui->Model_Training_SliderModelTrainNu));
                lv_label_set_text(ui->Model_Training_SliderModelTrainNu_Label, (const char*)text_buf);   
                uint8_t text_buf2[13] = "Gam:";         
                format_gamma(text_buf2+4, lv_slider_get_value(ui->Model_Training_SliderModelTrainGamma));
                lv_label_set_text(ui->Model_Training_SliderModelTrainGamma_Label, (const char*)text_buf2);                  
            }
            else lv_obj_add_flag(ui->Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
            ui->Model_Training_Cont_Hidden ^= 1;

			break;
        default:
            break;
	};
}

void events_param_set(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Model_Training_btnModelTrainParamSet, OnClickedParamSet, LV_EVENT_ALL, ui);
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


static void OnSliderNuValueChanged(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);
    
    uint8_t text_buf[8] = "Nu:";
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED:
			format_nu(text_buf+3, lv_slider_get_value(ui->Model_Training_SliderModelTrainNu));
            g_app.nu = lv_slider_get_value(ui->Model_Training_SliderModelTrainNu) / 100.0f;
            lv_label_set_text(ui->Model_Training_SliderModelTrainNu_Label, (const char*)text_buf);
            break;
        default:
            break;
	};
}
void events_slider_nu_value_changed(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Model_Training_SliderModelTrainNu, OnSliderNuValueChanged, LV_EVENT_ALL, ui);
}

static void OnSliderGammaValueChanged(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);
    
    uint8_t text_buf[13] = "Gam:";
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED:
            g_app.gamma = lv_slider_get_value(ui->Model_Training_SliderModelTrainGamma);
			format_gamma(text_buf+4, lv_slider_get_value(ui->Model_Training_SliderModelTrainGamma));
            lv_label_set_text(ui->Model_Training_SliderModelTrainGamma_Label, (const char*)text_buf);
            break;
        default:
            break;
	};
}
void events_slider_gamma_value_changed(lv_ui *ui)
{
   lv_obj_add_event_cb(ui->Model_Training_SliderModelTrainGamma, OnSliderGammaValueChanged, LV_EVENT_ALL, ui); 
}
