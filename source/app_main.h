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

#define INC_TRAIN           1
#define APP_FEATURE_DIM     2
#define APP_FEATURE_SIZE (APP_FEATURE_DIM * sizeof(float))
#define APP_FEATURE_CNT     60
#define APP_FEATURE_RETRAIN_CNT_MAX   (10 * INC_TRAIN) /* support the INC_TTRAIN */

#define APP_SENSOR_TASK_READ_ITEM   (16)
#define APP_SENSOR_TASK_READ_SIZE    (APP_SENSOR_TASK_READ_ITEM*6)

#define APP_SENSOR_DATA_DIVIDER     50.0f   
#define APP_FEATURE_RMS_DIVIDER     800.0f

#define APP_SAMP_RATE_HZ   400
#define APP_SAMP_PERIOD_MS  5 // (1000 / APP_SAMP_RATE_HZ)
#define APP_FFT_LEN     512
#define APP_FFT_STRIDE  64
#define APP_FFT_MAX_FREQ_FILTER 200
#define APP_FFT_DATATYPE    int16_t
#define APP_FFT_DATATYPE_LEN    2// sizeof(int16_t)
#define APP_FFT_MSB_PRE_DIV     64
#define APP_EMA_DECAY_FACTOR_X100   50

#define APP_FFT_X_WEI	0.4
#define APP_FFT_Y_WEI	0.4
#define APP_FFT_Z_WEI	0.2

#define APP_HEALTH_ABS_MAX  10.0f
#define APP_ABNORMAL_LEVEL  0.0f
#define APP_SWITCH_BAND     0.8f

#define DEFAULT_GAMMA 40
#define DEFAULT_NU	0.06f

#define ALERT_LED_RED_GPIO   (GPIO3)
#define ALERT_LED_RED_PIN   (20u)

#define ALERT_LED_GREEN_GPIO   (GPIO3)
#define ALERT_LED_GREEN_PIN   (21u)

#define INIT_LED_PINS(led, output) \
    GPIO_PinWrite(ALERT_LED_## led ##_GPIO, ALERT_LED_## led ##_PIN, output); \
    ALERT_LED_## led ##_GPIO->PDDR |= (1U << ALERT_LED_## led ##_PIN)                         /*!< Enable target LED_RED */

#define LED_ON(led) \
    GPIO_PinWrite(ALERT_LED_## led ##_GPIO, ALERT_LED_## led ##_PIN, LOGIC_LED_ON);

#define LED_OFF(led) \
    GPIO_PinWrite(ALERT_LED_## led ##_GPIO, ALERT_LED_## led ##_PIN, LOGIC_LED_OFF);

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
    bool sensor_error;
    bool inc_train; // flag indicates the INC-Train

    int rfftInFillCnt;
    APP_FFT_DATATYPE rfftInBuf[3][APP_FFT_LEN];
    APP_FFT_DATATYPE rfftInBufHanning[3][APP_FFT_LEN];
    APP_FFT_DATATYPE rfftOutBuf[3][APP_FFT_LEN * 2];
    int32_t currentMeans[3];
    int16_t dcEMAs[3];    // moving EMA of DC offsets


}AppLogic_t;


extern AppLogic_t g_app;


extern void sensor_init(void);

// Get anomaly detection model. Returns NULL if model is not existing or ready,
// returning NULL triggers Training Screen
extern void* get_ad_model(void);
extern void* ad_train(float features[][APP_FEATURE_DIM], int featureCnt,float gamma, float nu);
extern float ad_predict(void* pvModel, float feature[APP_FEATURE_DIM]);
extern uint8_t ad_is_on(void* pvModel, float feature[APP_FEATURE_CNT]);

void app_sensor_task(void* parameters);
void app_algo_task(void* parameters);

#define SENSOR_SAMPLING_PERIOD  50

#endif
