// SPDX-License-Identifier: MIT
// Copyright 2023 NXP
/*
 * custom.h
 *
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

typedef enum
{
  AnomalyDetectMode,
  StateMonitorMode
}WorkMode;

void custom_init(lv_ui *ui);
void widgets_move();

#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
