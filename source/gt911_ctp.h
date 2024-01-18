/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef GT911_CTP_H
#define GT911_CTP_H

#include <stdint.h>

typedef enum {
    GT911_SUCCESS = 0U,
    GT911_FAIL,
} gt911_ret_t;

typedef enum {
    GT911_INT_MODE_IRQ_RISE = 0x00U,
    GT911_INT_MODE_IRQ_FALL = 0x01U,
    GT911_INT_MODE_POLL     = 0x03U,
} gt911_int_mode_t;

typedef struct {
    uint8_t  id;
    uint16_t pos_x;
    uint16_t pos_y;
    uint16_t size;
} gt911_point_t;

typedef struct {
    uint8_t       num_pos;
    gt911_point_t pos[5];
} gt911_input_t;

typedef struct {
    uint8_t *tx_data;
    uint8_t *rx_data;
    uint16_t tx_len;
    uint16_t rx_len;
} gt911_i2c_xfer_t;

typedef gt911_ret_t (*gt911_ops_reset_t)(void *handle);
typedef gt911_ret_t (*gt911_ops_i2c_xfer_t)(void *handle, gt911_i2c_xfer_t *xfer);

typedef struct {
    gt911_ops_reset_t    reset;
    gt911_ops_i2c_xfer_t xfer;
} gt911_ops_t;

typedef struct {
    uint16_t         pos_x_max;
    uint16_t         pos_y_max;
    uint8_t          pos_max;
    uint8_t          fw_version;
    gt911_int_mode_t int_mode;
    gt911_ops_t      ops;
    void            *user_data;
} gt911_t;

gt911_ret_t gt911_ctp_init(gt911_t *ctp);
gt911_ret_t gt911_ctp_read(gt911_t *ctp, gt911_input_t *input);

#endif