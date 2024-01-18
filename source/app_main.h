/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_MAIN_H_
#define _APP_MAIN_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

typedef enum {
    kAppPredicting, // in main screen, predicting data
    kAppWaitForCollect, // in train screen, wait for "Start" button
    kAppCollecting, // in train screen, collecting data
    kAppWaitForReturn,
}AppState_e;

typedef enum {
    kFanOff,
    kFanOn,
    kFanErr,
}FanState_e;

#define APP_FEATURE_DIM     2
#define APP_FEATURE_SIZE (APP_FEATURE_DIM * sizeof(float))
#define APP_FEATURE_CNT     30

#define APP_SENSOR_TASK_READ_ITEM   (4)
#define APP_SENSOR_TASK_READ_SIZE    (APP_SENSOR_TASK_READ_ITEM*6)

#define APP_SENSOR_DATA_DIVIDER     50.0f   
#define APP_FEATURE_RMS_DIVIDER     4000.0f

#define APP_SAMP_RATE_HZ   200
#define APP_SAMP_PERIOD_MS  5 // (1000 / APP_SAMP_RATE_HZ)
#define APP_FFT_LEN     256
#define APP_FFT_STRIDE  64
#define APP_FFT_DATATYPE    int16_t
#define APP_FFT_DATATYPE_LEN    2// sizeof(int16_t)
#define APP_FFT_MSB_PRE_DIV     64
#define APP_EMA_DECAY_FACTOR_X100   50

#define APP_HEALTH_ABS_MAX  10.0f
#define APP_ABNORMAL_LEVEL  0.0f
#define APP_SWITCH_BAND     0.8f


typedef struct tagAppLogic
{
    AppState_e appState;
    FanState_e fanState;
    void* pADModel;
    QueueHandle_t qh_NewSample;    // new sample calculated, sensor task sends to algo task
    float health;   // if heath < APP_ABNORMAL_THRESHOLD then s
    float gamma;
    float nu;
    bool isNewFeature;
    int featureNdx;

    int rfftInFillCnt;
    APP_FFT_DATATYPE rfftInBuf[3][APP_FFT_LEN];
    APP_FFT_DATATYPE rfftOutBuf[3][APP_FFT_LEN * 2];
    int32_t currentMeans[3];
    int16_t dcEMAs[3];    // moving EMA of DC offsets


}AppLogic_t;

#ifndef _MAIN_C_
extern 
#endif
AppLogic_t g_app;


extern void sensor_init(void);

// Get anomaly detection model. Returns NULL if model is not existing or ready,
// returning NULL triggers Training Screen
extern void* get_ad_model(void);
extern void* ad_train(float features[][APP_FEATURE_DIM], int featureCnt);
extern float ad_predict(void* pvModel, float feature[APP_FEATURE_DIM]);
extern uint8_t ad_is_on(void* pvModel, float feature[APP_FEATURE_CNT]);

#define SENSOR_SAMPLING_PERIOD  50

#endif
