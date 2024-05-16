/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "lvgl_support.h"
#include "pin_mux.h"
#include "board.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "lvgl_demo_utils.h"
#include "lvgl_freertos.h"

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_smartdma.h"
#include "fsl_inputmux_connections.h"
#include "fsl_inputmux.h"

#include "app_main.h"

#include <imu_ops.h>
#include "ringbuffer.h"
#include "svm_test.h"
#include "app_custom_ui.h"

#ifdef __ARMCC_VERSION
extern uint8_t Image$$RW_m_sramx_start$$Limit[];
extern uint8_t Image$$RW_m_sramx_start$$Base[];

#define SVM_pArena (Image$$RW_m_sramx_start$$Base + SMARTDMA_DISPLAY_FIRMWARE_SIZE)

#elif defined(__GNUC__)
extern uint8_t __base_SRAMX[];
extern uint8_t __top_SRAMX[];
#define SVM_pArena (__base_SRAMX  + SMARTDMA_DISPLAY_FIRMWARE_SIZE)
#endif


extern void ShowProgress(int val);
float g_features[APP_FEATURE_CNT + APP_FEATURE_RETRAIN_CNT_MAX][APP_FEATURE_DIM];
volatile uint8_t g_dbgShowNdx; // 0 = health, 1 = rms, 2 = top1

__WEAK void ad_get_support_vector(void* model, void* SVs, int32_t* svs_cnt) {
    svm_model_get_SVs(model, SVs, svs_cnt);
}

__WEAK void* ad_train(float features[][APP_FEATURE_DIM], int featureCnt,float gamma, float nu) {
    return svm_train_mode(features, featureCnt,gamma, nu);
}

volatile float g_modelRetPlaceHolder;
__WEAK float ad_predict(void* pvModel, float feature[APP_FEATURE_DIM]) {
    return svm_model_pre(pvModel, feature);
}

volatile float g_rmsThreshold = 0.0f;
__WEAK uint8_t ad_is_on(void* pvModel, float feature[APP_FEATURE_CNT]) {
    if (feature[0] >= g_rmsThreshold)
        return 1;
    return 0;
}


int $Sub$$_sys_open(const char *name, int openmode)
{
    return 1;
}

void app_algo_task(void* parameters)
{
	TickType_t tick_start;
	TickType_t tick_end;
	TickType_t delay_target;
	float feature[APP_FEATURE_DIM];

	g_app.featureNdx = 0;
	int fanState;
	int deSlideAcc = 0;
	for (;;) {

		xQueueReceive(g_app.qh_NewSample, feature, portMAX_DELAY);
		g_app.isNewFeature = 1;
		switch (g_app.appState) {
		case kAppCollecting:
			//deSlideAcc += APP_FFT_STRIDE;
			//if (deSlideAcc >= APP_FFT_LEN)
			{
				//deSlideAcc -= APP_FFT_LEN;
				memcpy(g_features + g_app.featureNdx, feature, APP_FEATURE_SIZE);
				g_app.featureNdx++;
				// ShowProgress(g_app.featureNdx);
				if (g_app.featureNdx == APP_FEATURE_CNT) {
					// check inc_train and then re-train, adding the SVs to the tail
					int32_t pre_svs_cnt = 0;
					if(g_app.inc_train){
						ad_get_support_vector(g_app.pADModel, &(g_features[APP_FEATURE_CNT]), &pre_svs_cnt);
						g_app.inc_train = 0;
					}
					g_app.pADModel = ad_train(g_features, APP_FEATURE_CNT + pre_svs_cnt, g_app.gamma, g_app.nu);
					deSlideAcc = 0;
					if (g_app.pADModel) {
						g_app.appState = kAppWaitForReturn;

					}
				}
			}
			break;
		case kAppPredicting:
			if (g_app.pADModel) {
				fanState = g_app.fanState;
				float ret = 0;
				if (ad_is_on(g_app.pADModel, feature)) {
					// draw the new one into the svm_toy
					svm_features_t features = {
						.col = 1,
						.row = APP_FEATURE_DIM,
						.p_feature = (float*)feature,
					};
					if(svm_toy.data == NULL){

						main_win_heatmap_update(g_app.pADModel,SVM_pArena);

					}
					if(svm_toy.data){
						main_win_draw_current_feature_points(feature);
					}

					ret = ad_predict(g_app.pADModel, feature);
					float retSign = -1.0f;
					if(ret > 0){
						retSign = 1.0f;
						ret *= 25;
					} else {
						ret *= 1;
					}
					g_app.health = (g_app.health + ret + 0.5 * retSign);

#if 0

					if (g_dbgShowNdx == 0)
						g_app.health = (g_app.health + ret + 0.5 * retSign);// * 0.85f;
					else if (g_dbgShowNdx == 1)
						g_app.health = feature[0] * 10;
					else if (g_dbgShowNdx == 2)
						g_app.health = feature[1] * 10;
#endif
					if (g_app.health > APP_HEALTH_ABS_MAX)
						g_app.health = APP_HEALTH_ABS_MAX;
					if (g_app.health < -APP_HEALTH_ABS_MAX)
						g_app.health = -APP_HEALTH_ABS_MAX;
					if (g_app.fanState == kFanOn) {
						if (g_app.health < APP_ABNORMAL_LEVEL - APP_SWITCH_BAND)
							g_app.fanState = kFanErr;
					} else if (g_app.fanState == kFanErr) {
						if (g_app.health > APP_ABNORMAL_LEVEL + APP_SWITCH_BAND)
							g_app.fanState = kFanOn;
					} else {
						g_app.fanState = kFanOn;
					}
					if(fanState == kFanErr){
						LED_OFF(GREEN);
						LED_ON(RED);

						LED_GREEN_OFF();
						LED_RED_ON();

					}else{
						LED_OFF(RED);
						LED_ON(GREEN);

						LED_RED_OFF();
						LED_GREEN_ON();
					}

				} else {
					g_app.fanState = kFanOff;
				}
			}
		default:
			break;
		}
	}
}

void algo_update_heatmap_after_train()
{
	main_win_heatmap_update(g_app.pADModel,SVM_pArena);

	svm_features_t features = {
		.col = APP_FEATURE_CNT + 0,
		.row = APP_FEATURE_DIM,
		.p_feature = (float*)g_features,
	};

}

void algo_update_heatmap()
{
	main_win_heatmap_update(g_app.pADModel,SVM_pArena);
}
