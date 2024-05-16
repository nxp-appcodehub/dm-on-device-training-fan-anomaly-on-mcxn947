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

#include "arm_math.h"
#include "arm_const_structs.h"

#include "app_main.h"
#include "app_custom_ui.h"
#include <imu_ops.h>
#include "ringbuffer.h"

volatile float g_dbgFeature[APP_FEATURE_DIM] = {0.7f, 0.4f};
volatile float g_rmsDiv = APP_FEATURE_RMS_DIVIDER;
float hanning_ary[APP_FFT_LEN] = {0};
uint16_t ring_buffer[1];//[2 * 3 * APP_FEATURE_CNT];
ringbuffer_t ringbuffer_handler;

void delay_ms(uint32_t ms){
	vTaskDelay(ms);
}

void hanning_window(float *window, int length) {

    for (int n = 0; n < length; n++) {
        window[n] = 0.5 * (1 - cosf(2 * 3.141593f * n / (length - 1)));
    }

}

void hanming_window(float *window, int length) {
	float a0 = 0.54;
	float a1 = 0.46;
	for (int n = 0; n < length; n++) {
		window[n] = a0 - a1 * cosf(2.0 * 3.141593f * n / (length - 1)) +
					 a1 * cosf(4.0 * 3.141593f * n / (length - 1)) / 2.0;
	}
}
#define CAPTURE_FFT 0
void app_sensor_task(void* parameters)
{
	TickType_t tick_start;
	TickType_t tick_end;
	TickType_t delay_target;
	TickType_t fft_tick;
	int cnt = 0;
	APP_FFT_DATATYPE fft_buffer[APP_FFT_LEN];
	if(IMU_Init()){
		PRINTF("IMU Sensor init failed! \r\n");
		g_app.sensor_error = 1;

		while(1)
		{
			vTaskSuspend(NULL);
		}
	}
	g_app.sensor_error = 0;
	PRINTF("IMU Sensor init sucess! \r\n");

	ringbuffer_init(&ringbuffer_handler, (uint8_t*)ring_buffer, sizeof(ring_buffer));
	hanning_window(hanning_ary, APP_FFT_LEN);

	arm_rfft_instance_q15 s;
	arm_rfft_init_q15(&s, APP_FFT_LEN, 0, 1/*bit reversal*/);

	int16_t rawBuf[APP_SENSOR_TASK_READ_SIZE/6][3];
	int taskPeriod_ms = APP_SENSOR_TASK_READ_ITEM * 1000 / APP_SAMP_RATE_HZ / 2 + 1 ;
	g_app.currentMeans[0] = g_app.currentMeans[1] = g_app.currentMeans[2] = 0;
#if !CAPTURE_FFT
	for (;;)
	{
		tick_start = xTaskGetTickCount();
		int fifoCnt = IMU_Get_Fifo_Cnt();
		int readSize = 0;
		// calc how many bytes to read
		int maxItemCnt = APP_SENSOR_TASK_READ_ITEM;
		int itemToRead = APP_FFT_LEN - g_app.rfftInFillCnt;
		if (itemToRead > maxItemCnt)
			itemToRead = maxItemCnt;
		int bytesToRead = itemToRead * 6;


#if 0
		int16_t avg_msbval[3] = {0};
		if (fifoCnt * 6 >= bytesToRead) {
			readSize = IMU_ReadSensorData(rawBuf[0], fifoCnt, bytesToRead);
			for (int i=0; i<readSize / 6; i++) {
				for (int j=0; j<3; j++) {
					avg_msbval[j] += rawBuf[i][j];

				}

			}

			for (int j=0; j<3; j++) {
				g_app.currentMeans[j] += avg_msbval[j] / (readSize / 6) ;
				g_app.rfftInBuf[j][g_app.rfftInFillCnt] = avg_msbval[j] / (readSize / 6);

			}
			g_app.rfftInFillCnt++;
		}

#else
		int16_t msbVal;
		if (fifoCnt * 6 >= bytesToRead) {
			readSize = IMU_ReadSensorData(rawBuf[0], fifoCnt, bytesToRead);
			for (int i=0; i<readSize / 6; i++) {
				for (int j=0; j<3; j++) {
					msbVal = rawBuf[i][j];
					msbVal; // /= APP_FFT_MSB_PRE_DIV;
					g_app.currentMeans[j] += msbVal;
					g_app.rfftInBuf[j][g_app.rfftInFillCnt] = msbVal;
				}
				g_app.rfftInFillCnt++;
			}
		}
#endif
		if (g_app.rfftInFillCnt == APP_FFT_LEN) {
			fft_tick =  xTaskGetTickCount();
			float rmsAry[3];
			float top1FreqAry[3];

			// calc dc offset EMAs for Ax, Ay, Az
			for (int j=0; j<3; j++) {
				// calc current mean
				g_app.currentMeans[j] /= APP_FFT_LEN;
				g_app.dcEMAs[j] = (
					g_app.dcEMAs[j] * APP_EMA_DECAY_FACTOR_X100 +
					g_app.currentMeans[j] * (100 - APP_EMA_DECAY_FACTOR_X100)) / 100;
				if (cnt == 0)
					g_app.dcEMAs[j] = g_app.currentMeans[j];

				// the rfft will populate the inBuf, and also the substract, so do a backend to it first
				int16_t rfftInBackend[APP_FFT_LEN - APP_FFT_STRIDE];
				memcpy(rfftInBackend, g_app.rfftInBuf[j] + APP_FFT_STRIDE, sizeof(int16_t) * (APP_FFT_LEN - APP_FFT_STRIDE));
				// subtract EMA
				rmsAry[j] = 0.0f;
				float ftmp;
				for (int i=0; i<APP_FFT_LEN; i++) {
					g_app.rfftInBuf[j][i] -= g_app.dcEMAs[j];
					//hanning window
					fft_buffer[i] = g_app.rfftInBuf[j][i] * hanning_ary[i];
					ftmp = (float)(g_app.rfftInBuf[j][i]) / g_rmsDiv;

					rmsAry[j] += ftmp * ftmp;
				}
				arm_sqrt_f32(rmsAry[j] / APP_FFT_LEN, rmsAry + j);
				// do FFT in q15
				arm_rfft_q15(&s, fft_buffer, g_app.rfftOutBuf[j]);

				// find top1 freq
				int16_t maxFreqNdx = 0, maxFreqVal = -32768;
				int16_t stmp;
				for (int i=1*2; i<APP_FFT_LEN; i+=2/*skip imag part*/) {
					stmp = sqrtf(g_app.rfftOutBuf[j][i] * g_app.rfftOutBuf[j][i] +
							    g_app.rfftOutBuf[j][i+1]*g_app.rfftOutBuf[j][i+1]);
					if (stmp > APP_FFT_MAX_FREQ_FILTER)
						stmp = 0;
					if (stmp > maxFreqVal) {
						maxFreqVal = stmp;
						maxFreqNdx = i >> 1;    // skip imag part
					}
				}
				top1FreqAry[j] = (float)(APP_SAMP_RATE_HZ / 2) * maxFreqNdx / (APP_FFT_LEN / 2);
				g_app.currentMeans[j] = 0;
				// copy back, we have chance to copy only once, merge to line:215, but will need a 3-buffer, forget it.
				memcpy(g_app.rfftInBuf[j] + APP_FFT_STRIDE, rfftInBackend, sizeof(int16_t) * (APP_FFT_LEN - APP_FFT_STRIDE));
			}
			g_dbgFeature[1] = /*rmsAry[0];*/(rmsAry[0] + rmsAry[1] + rmsAry[2]) / 3.0f;
			PRINTF("top1_x=%f,top1_y=%f,top1_z=%f\r\n",top1FreqAry[0]/(APP_SAMP_RATE_HZ / 2),
					top1FreqAry[1]/(APP_SAMP_RATE_HZ / 2),top1FreqAry[2]/(APP_SAMP_RATE_HZ / 2));
			float alph = 0.75f;
			g_dbgFeature[0] = g_dbgFeature[0] * alph + ((top1FreqAry[0]*APP_FFT_X_WEI + top1FreqAry[1]*APP_FFT_Y_WEI +
					top1FreqAry[2]*APP_FFT_Z_WEI) /(APP_SAMP_RATE_HZ / 2)) * (1 - alph);
			xQueueSend(g_app.qh_NewSample, (void*)g_dbgFeature, portMAX_DELAY);
			// slide the data to left
			for (int j=0; j<3; j++) {
				memcpy(g_app.rfftInBuf[j], g_app.rfftInBuf[j]+APP_FFT_STRIDE,
					(APP_FFT_LEN - APP_FFT_STRIDE) * APP_FFT_DATATYPE_LEN);
				for(int i=0;i<(APP_FFT_LEN - APP_FFT_STRIDE);i++){
					g_app.currentMeans[j] += g_app.rfftInBuf[j][i];
				}
			}
			g_app.rfftInFillCnt = APP_FFT_LEN - APP_FFT_STRIDE;
			cnt++;

		}

		tick_end = xTaskGetTickCount();
		if (tick_end - tick_start >= pdMS_TO_TICKS(taskPeriod_ms)) {
			/* The task takes too long to finish, use minimum delay target */
			delay_target = 1;
		} else {
			delay_target = pdMS_TO_TICKS(taskPeriod_ms) - (tick_end - tick_start);
		}

		vTaskDelay(delay_target);
	}
#else
	while(1)
	{
		float rmsAry[3];
		float top1FreqAry[3];
		tick_start = xTaskGetTickCount();
		int fifoCnt = IMU_Get_Fifo_Cnt();
		int readSize = 0;
		// calc how many bytes to read
		int maxItemCnt = APP_SENSOR_TASK_READ_ITEM;
		int itemToRead = APP_FFT_LEN - g_app.rfftInFillCnt;
		if (itemToRead > maxItemCnt)
			itemToRead = maxItemCnt;
		int bytesToRead = itemToRead * 6;

		int16_t msbVal;
		if (fifoCnt * 6 >= bytesToRead) {
			readSize = IMU_ReadSensorData(rawBuf[0], fifoCnt, bytesToRead);
			for (int i=0; i<readSize / 6; i++) {
				for (int j=0; j<3; j++) {
					msbVal = rawBuf[i][j];
					msbVal; // /= APP_FFT_MSB_PRE_DIV;
					g_app.currentMeans[j] += msbVal;
					g_app.rfftInBuf[j][g_app.rfftInFillCnt] = msbVal;

				}
				g_app.rfftInFillCnt++;
			}
		}

		if (g_app.rfftInFillCnt == APP_FFT_LEN) {


			for (int j=0; j<3; j++) {
				// calc current mean
				g_app.currentMeans[j] /= APP_FFT_LEN;
				g_app.dcEMAs[j] = (
					g_app.dcEMAs[j] * APP_EMA_DECAY_FACTOR_X100 +
					g_app.currentMeans[j] * (100 - APP_EMA_DECAY_FACTOR_X100)) / 100;
				if (cnt == 0)
					g_app.dcEMAs[j] = g_app.currentMeans[j];

				// the rfft will populate the inBuf, and also the substract, so do a backend to it first
				int16_t rfftInBackend[APP_FFT_LEN - APP_FFT_STRIDE];
				memcpy(rfftInBackend, g_app.rfftInBuf[j] + APP_FFT_STRIDE, sizeof(int16_t) * (APP_FFT_LEN - APP_FFT_STRIDE));
				// subtract EMA
				rmsAry[j] = 0.0f;
				float ftmp;
				for (int i=0; i<APP_FFT_LEN; i++) {
					//g_app.rfftInBuf[j][i] -= g_app.dcEMAs[j];
					//hanning window
					fft_buffer[i] = g_app.rfftInBuf[j][i] * hanning_ary[i];
					ftmp = (float)(g_app.rfftInBuf[j][i] - g_app.dcEMAs[j]) / g_rmsDiv;

					rmsAry[j] += ftmp * ftmp;
				}
				arm_sqrt_f32(rmsAry[j] / APP_FFT_LEN, rmsAry + j);
				// do FFT in q15
				//arm_rfft_q15(&s, g_app.rfftInBuf[j], g_app.rfftOutBuf[j]);
				arm_rfft_q15(&s, fft_buffer, g_app.rfftOutBuf[j]);
				// copy back, we have chance to copy only once, merge to line:215, but will need a 3-buffer, forget it.
				memcpy(g_app.rfftInBuf[j] + APP_FFT_STRIDE, rfftInBackend, sizeof(int16_t) * (APP_FFT_LEN - APP_FFT_STRIDE));
				float stmp = 0.0f, maxFreqVal=0.0f;
				int maxFreqNdx;
				for (int i=1*2; i<APP_FFT_LEN; i+=2/*skip imag part*/) {
					stmp = sqrtf(g_app.rfftOutBuf[j][i] * g_app.rfftOutBuf[j][i] +
								g_app.rfftOutBuf[j][i+1]*g_app.rfftOutBuf[j][i+1]);
					//stmp = g_app.rfftOutBuf[j][i];

					if (stmp < 0)
						stmp = -stmp;
					if (stmp > APP_FFT_MAX_FREQ_FILTER)
						stmp = 0;
					if (stmp > maxFreqVal) {
						maxFreqVal = stmp;

						maxFreqNdx = i >> 1;    // skip imag part
					}
				}
				top1FreqAry[j] = (float)(APP_SAMP_RATE_HZ / 2) * maxFreqNdx / (APP_FFT_LEN / 2) / (APP_SAMP_RATE_HZ / 2);

				g_app.currentMeans[j] = 0;
			}
			for (int j=0; j<3; j++) {
				memcpy(g_app.rfftInBuf[j], g_app.rfftInBuf[j]+APP_FFT_STRIDE,
					(APP_FFT_LEN - APP_FFT_STRIDE) * APP_FFT_DATATYPE_LEN);
				for(int i=0;i<(APP_FFT_LEN - APP_FFT_STRIDE);i++){
					g_app.currentMeans[j] += g_app.rfftInBuf[j][i];
				}
			}
			g_app.rfftInFillCnt = APP_FFT_LEN - APP_FFT_STRIDE;
			cnt++;
			float stmp0,stmp1,stmp3;
#if 0
			for (int i=0;i<APP_FFT_LEN*2;i+=2)
			{
				stmp0 = sqrtf(g_app.rfftOutBuf[0][i] * g_app.rfftOutBuf[0][i] + g_app.rfftOutBuf[0][i+1]*g_app.rfftOutBuf[0][i+1]);
				stmp1 = sqrtf(g_app.rfftOutBuf[1][i] * g_app.rfftOutBuf[1][i] + g_app.rfftOutBuf[1][i+1]*g_app.rfftOutBuf[1][i+1]);
				stmp3 = sqrtf(g_app.rfftOutBuf[2][i] * g_app.rfftOutBuf[2][i] + g_app.rfftOutBuf[2][i+1]*g_app.rfftOutBuf[2][i+1]);
				PRINTF("fft_x=%f,fft_y=%f,fft_z=%f\r\n",stmp0,stmp1,stmp3);
			}
#endif
			PRINTF("top1_x=%f,top1_y=%f,top1_z=%f\r\n",top1FreqAry[0],top1FreqAry[1],top1FreqAry[2]);
			PRINTF("\r\n");

		}

		vTaskDelay(5);
	}
#endif
}
