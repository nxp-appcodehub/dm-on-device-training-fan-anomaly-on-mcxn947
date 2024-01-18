/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_lpi2c.h"

/* Private */
#include "ctp_impl.h"

void ctp_impl_init(void) {
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1);

    lpi2c_master_config_t i2c_config;
    LPI2C_MasterGetDefaultConfig(&i2c_config);

    i2c_config.baudRate_Hz = 400000U;

    LPI2C_MasterInit(LPI2C2, &i2c_config, CLOCK_GetLPFlexCommClkFreq(2));
}

gt911_ret_t ctp_impl_xfer(void *handle, gt911_i2c_xfer_t *xfer) {
    lpi2c_master_transfer_t master_xfer = {
        .slaveAddress   = 0x5D,
        .subaddress     = 0x00UL,
        .subaddressSize = 0,
        .flags          = kLPI2C_TransferDefaultFlag,
    };

    if (xfer->tx_data && xfer->tx_len) {
        master_xfer.direction = kLPI2C_Write;
        master_xfer.data      = xfer->tx_data;
        master_xfer.dataSize  = xfer->tx_len;

        if (LPI2C_MasterTransferBlocking(LPI2C2, &master_xfer) != kStatus_Success) {
            return GT911_FAIL;
        }
    }

    if (xfer->rx_data && xfer->rx_len) {
        master_xfer.direction = kLPI2C_Read;
        master_xfer.data      = xfer->rx_data;
        master_xfer.dataSize  = xfer->rx_len;

        if (LPI2C_MasterTransferBlocking(LPI2C2, &master_xfer) != kStatus_Success) {
            return GT911_FAIL;
        }
    }

    return GT911_SUCCESS;
}