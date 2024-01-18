/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/* FreeRTOS */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

/* LVGL */
#include "lvgl.h"

/* LCD driver */
#include "lcd_impl_flexio.h"
#include "st7796_lcd.h"

/* CTP driver */
#include "ctp_impl.h"
#include "gt911_ctp.h"

#include "fsl_lpi2c.h"
#include "fsl_ft5406.h"

/* Debug console */
#include "fsl_debug_console.h"

#define APP_LVGL_BUF_SIZE (LCD_PANEL_WIDTH * 16)

static void app_lvgl_task(void *parameters);
static void app_lvgl_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
static void app_lvgl_ctp_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data);

static lcd_impl_flexio_t s_lcd_impl;

static st7796_lcd_t s_lcd = {
    .config =
    #if LCD_SSD1963
         {
            .direction = ST7796_DIR_0,
            .pix_fmt   = ST7796_RGB565,
            .bgr_mode  = 0,
            .inversion = 0,
            .mirrored  = 0,
        },   
    #else
        {
            .direction = ST7796_DIR_90,
            .pix_fmt   = ST7796_RGB565,
            .bgr_mode  = 1,
            .inversion = 0, 
            .mirrored  = 1,
        },
    #endif
    .cb =
        {
            .reset_cb      = lcd_impl_reset,
            .write_cmd_cb  = lcd_impl_write_cmd,
            .write_data_cb = lcd_impl_write_data,
        },
    .user_data = &s_lcd_impl,
};

static gt911_t s_ctp = {
    .ops =
        {
            .xfer = ctp_impl_xfer,
        },
};

ft5406_handle_t touchHandle;

static SemaphoreHandle_t s_lvgl_semphr;

static lv_disp_draw_buf_t s_disp_buf;
static lv_color_t         s_disp_buf_color_1[APP_LVGL_BUF_SIZE];
static lv_color_t         s_disp_buf_color_2[APP_LVGL_BUF_SIZE];
static lv_disp_drv_t      s_disp_drv;
static lv_indev_drv_t     s_indev_drv;

int app_lvgl_init(void) {
    lv_init();

    lcd_impl_init(&s_lcd_impl);
    st7796_lcd_init(&s_lcd);

    ctp_impl_init();
#if GT911
    if (gt911_ctp_init(&s_ctp) != GT911_SUCCESS) {
        PRINTF("CTP initialization failed.\r\n");
    }
#else
    /* Initialize the touch handle. */
    if ((FT5406_Init(&touchHandle, LPI2C2)) != kStatus_Success)
    {
        PRINTF("Touch panel init failed\n");
        assert(0);
    }
#endif
    lv_disp_draw_buf_init(&s_disp_buf, s_disp_buf_color_1, s_disp_buf_color_2, APP_LVGL_BUF_SIZE);
    lv_disp_drv_init(&s_disp_drv);

    s_disp_drv.draw_buf  = &s_disp_buf;
    s_disp_drv.flush_cb  = app_lvgl_flush_cb;
    s_disp_drv.hor_res   = LCD_PANEL_WIDTH;
    s_disp_drv.ver_res   = LCD_PANEL_HEIGHT;
    s_disp_drv.user_data = &s_lcd;

    lv_disp_drv_register(&s_disp_drv);

    lv_indev_drv_init(&s_indev_drv);

    s_indev_drv.type    = LV_INDEV_TYPE_POINTER;
    s_indev_drv.read_cb = app_lvgl_ctp_read_cb;

    lv_indev_drv_register(&s_indev_drv);

    s_lvgl_semphr = xSemaphoreCreateBinary();
    if (s_lvgl_semphr == NULL) {
        return -2;
    }

    xSemaphoreGive(s_lvgl_semphr);

    if (xTaskCreate(app_lvgl_task, "LVT", 1024, NULL, 3, NULL) != pdPASS) {
        return -1;
    }

    return 0;
}

bool app_lvgl_lock_acquire(uint32_t block_msec) {
    if (xSemaphoreTake(s_lvgl_semphr, pdMS_TO_TICKS(block_msec)) != pdPASS) {
        return false;
    }

    return true;
}

void app_lvgl_lock_release(void) {
    xSemaphoreGive(s_lvgl_semphr);
}

uint32_t app_lvgl_ms_ticks(void) {
    return xTaskGetTickCount();
}

static void app_lvgl_task(void *parameters) {
    TickType_t tick_start;
    TickType_t tick_end;
    TickType_t delay_target;

    for (;;) {
        tick_start = xTaskGetTickCount();

        if (app_lvgl_lock_acquire(10000)) {
            lv_timer_handler();
            app_lvgl_lock_release();
        }

        tick_end = xTaskGetTickCount();

        if (tick_end - tick_start >= pdMS_TO_TICKS(LV_DISP_DEF_REFR_PERIOD)) {
            /* The task takes too long to finish, use minimum delay target */
            delay_target = 1;
        } else {
            delay_target = pdMS_TO_TICKS(LV_DISP_DEF_REFR_PERIOD) - (tick_end - tick_start);
        }

        vTaskDelay(delay_target);
    }
}

static void app_lvgl_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    st7796_lcd_t *lcd = (st7796_lcd_t *)disp_drv->user_data;

    /* In eDMA async mode, this is returned immediately. See impl. */
    st7796_lcd_load(lcd, (uint8_t *)color_p, area->x1, area->x2, area->y1, area->y2);
    lv_disp_flush_ready(disp_drv);
}

static void app_lvgl_ctp_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data) {
#if GT911
		gt911_input_t ctp_input;

		if (gt911_ctp_read(&s_ctp, &ctp_input) != GT911_SUCCESS) {
			PRINTF("Failed to read touch data\r\n");
		}

		/**
		 * GT911 supports 5 points tracking, we only tracks ID #0.
		 *
		 */

		bool found_track = false;

		for (uint8_t i = 0; i < ctp_input.num_pos; i++) {
			/* Found track ID #0 */
			if (ctp_input.pos[i].id == 0) {
				if (s_lcd.config.direction == ST7796_DIR_90) {
					data->state   = LV_INDEV_STATE_PRESSED;
//					data->point.x = s_ctp.pos_y_max - ctp_input.pos[i].pos_y;
//					data->point.y = ctp_input.pos[i].pos_x;
                    
                    # warning "Need to add a more find way to do, all the group, rotate + flip"
                    data->point.y = s_ctp.pos_x_max - ctp_input.pos[i].pos_x;
					data->point.x = ctp_input.pos[i].pos_y;

					found_track = true;
				}
			}
		}
	    /* No track #0 found... */
	    if (!found_track) {
	        data->state = LV_INDEV_STATE_RELEASED;
	    }
#else
		touch_event_t touch_event;
		static int touch_x = 0;
		static int touch_y = 0;

		data->state = LV_INDEV_STATE_RELEASED;

		if (kStatus_Success == FT5406_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y))
		{
			if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
			{
				data->state = LV_INDEV_STATE_PRESSED;
			}
		}

		/*Set the last pressed coordinates*/
		data->point.x = touch_x;
		data->point.y = touch_y;
#endif
}
