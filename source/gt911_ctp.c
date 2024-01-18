/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stddef.h>

/* Private */
#include "gt911_ctp.h"

#define GT911_REG_RT_CMD            0x8040U
#define GT911_REG_RT_CMD_SW_RST_Pos 2U
#define GT911_REG_RT_CMD_SW_RST_Msk (1U << GT911_REG_RT_CMD_SW_RST_Pos)
#define GT911_REG_RT_CMD_READ_Pos   0U
#define GT911_REG_RT_CMD_READ_Msk   (1U << GT911_REG_RT_CMD_READ_Pos)

#define GT911_REG_CONFIG_VERSION 0x8047U

#define GT911_REG_MODULE_SW1         0x804DU
#define GT911_REG_MODULE_SW1_INT_Pos 0U
#define GT911_REG_MODULE_SW1_INT_Msk (3U << GT911_REG_MODULE_SW1_INT_Pos)

#define GT911_REG_PRODUCT_ID 0x8140U

#define GT911_REG_COORD            0x814EU
#define GT911_REG_COORD_STATUS_Pos 7U
#define GT911_REG_COORD_STATUS_Msk (1U << GT911_REG_COORD_STATUS_Pos)

#define GT911_REG_POINT0 0x814FU

static gt911_ret_t gt911_ctp_read_reg(gt911_t *ctp, uint16_t reg, uint8_t *data, uint16_t len);
static gt911_ret_t gt911_ctp_write_reg(gt911_t *ctp, uint16_t reg, uint8_t data);

static gt911_ret_t gt911_ctp_sw_reset(gt911_t *ctp);
static gt911_ret_t gt911_ctp_read_config(gt911_t *ctp);
static gt911_ret_t gt911_ctp_config_interrupt(gt911_t *ctp);

gt911_ret_t gt911_ctp_init(gt911_t *ctp) {
    if (ctp->ops.reset) {
        if (ctp->ops.reset(ctp->user_data) != GT911_SUCCESS) {
            return GT911_FAIL;
        }
    }

    if (gt911_ctp_sw_reset(ctp) != GT911_SUCCESS) {
        return GT911_FAIL;
    }
    
    // need delay after the reset
    extern void vTaskDelay( const uint32_t xTicksToDelay );
    vTaskDelay(5);
    if (gt911_ctp_read_config(ctp) != GT911_SUCCESS) {
        return GT911_FAIL;
    }

    if (gt911_ctp_config_interrupt(ctp) != GT911_SUCCESS) {
        return GT911_FAIL;
    }

    return GT911_SUCCESS;
}

gt911_ret_t gt911_ctp_read(gt911_t *ctp, gt911_input_t *input) {
    uint8_t rx_data[40];

    if (gt911_ctp_read_reg(ctp, GT911_REG_COORD, rx_data, 1) != GT911_SUCCESS) {
        return GT911_FAIL;
    }

    if ((rx_data[0] & GT911_REG_COORD_STATUS_Msk) == 0) {
        input->num_pos = 0U;

        return GT911_SUCCESS;
    }

    input->num_pos = rx_data[0] & 0x0F;

    if (gt911_ctp_read_reg(ctp, GT911_REG_POINT0, rx_data, 40) != GT911_SUCCESS) {
        return GT911_FAIL;
    }

    for (uint8_t i = 0; i < input->num_pos; i++) {
        uint8_t point_offset = 8 * i; /* Each point has 8 bytes */

        input->pos[i].id    = rx_data[point_offset];                                         /* 0x00: Track ID */
        input->pos[i].pos_x = rx_data[point_offset + 1] | (rx_data[point_offset + 2] << 8U); /* 0x01-0x02: X coord */
        input->pos[i].pos_y = rx_data[point_offset + 3] | (rx_data[point_offset + 4] << 8U); /* 0x03-0x04: Y coord */
        input->pos[i].size  = rx_data[point_offset + 5] | (rx_data[point_offset + 6] << 8U); /* 0x05-0x06: Size*/
    }

    /* Clear buffer status latch, ready for new data */
    gt911_ctp_write_reg(ctp, GT911_REG_COORD, 0x00);

    return GT911_SUCCESS;
}

static gt911_ret_t gt911_ctp_read_reg(gt911_t *ctp, uint16_t reg, uint8_t *data, uint16_t len) {
    uint8_t tx_data[2] = {(reg >> 8U), (reg & 0xFFU)};

    gt911_i2c_xfer_t xfer = {
        .tx_data = tx_data,
        .rx_data = data,
        .tx_len  = 2,
        .rx_len  = len,
    };

    return ctp->ops.xfer(ctp->user_data, &xfer);
}

static gt911_ret_t gt911_ctp_write_reg(gt911_t *ctp, uint16_t reg, uint8_t data) {
    uint8_t tx_data[3] = {(reg >> 8U), (reg & 0xFFU), data};

    gt911_i2c_xfer_t xfer = {
        .tx_data = tx_data,
        .rx_data = NULL,
        .tx_len  = 3,
        .rx_len  = 0,
    };

    return ctp->ops.xfer(ctp->user_data, &xfer);
}

/**
 * @brief Send software reset command to CTP
 *
 * @param ctp
 * @return gt911_ret_t
 */
static gt911_ret_t gt911_ctp_sw_reset(gt911_t *ctp) {
    return gt911_ctp_write_reg(ctp, GT911_REG_RT_CMD, (GT911_REG_RT_CMD_SW_RST_Msk));
}

static gt911_ret_t gt911_ctp_read_config(gt911_t *ctp) {
    uint8_t rx_data[7];

    if (gt911_ctp_read_reg(ctp, GT911_REG_CONFIG_VERSION, rx_data, 7) != GT911_SUCCESS) {
        return GT911_FAIL;
    }

    ctp->fw_version = rx_data[0];                      /* 0x8047, Config Version */
    ctp->pos_x_max  = rx_data[1] | (rx_data[2] << 8U); /* 0x8048-0x8049, Maximum X */
    ctp->pos_y_max  = rx_data[3] | (rx_data[4] << 8U); /* 0x804A-0x804B, Maximum Y */
    ctp->pos_max    = rx_data[5] & 0x0FU;              /* 0x804C, Maximum positions */

    return GT911_SUCCESS;
}

static gt911_ret_t gt911_ctp_config_interrupt(gt911_t *ctp) {
    uint8_t mod_sw1 = 0x00U;

    if (gt911_ctp_read_reg(ctp, GT911_REG_MODULE_SW1, &mod_sw1, 0x01) != GT911_SUCCESS) {
        return GT911_FAIL;
    }

    mod_sw1 &= ~(GT911_REG_MODULE_SW1_INT_Msk);
    mod_sw1 |= (ctp->int_mode & GT911_REG_MODULE_SW1_INT_Msk);

    return gt911_ctp_write_reg(ctp, GT911_REG_MODULE_SW1, mod_sw1);
}