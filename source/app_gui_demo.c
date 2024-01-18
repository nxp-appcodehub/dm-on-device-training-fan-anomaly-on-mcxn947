/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "lvgl.h"
#include "lv_demos.h"
/* clang-format on */

/* App */
#include "app_lvgl.h"

void app_gui_demo_init(void) {
    if (app_lvgl_lock_acquire(1000)) {
        lv_demo_widgets();
        app_lvgl_lock_release();
    }
}