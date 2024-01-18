/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef APP_LVGL_H
#define APP_LVGL_H

#include <stdint.h>

int app_lvgl_init(void);
uint32_t app_lvgl_ms_ticks(void);

bool app_lvgl_lock_acquire(uint32_t block_msec);
void app_lvgl_lock_release(void);

#endif