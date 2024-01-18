/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "lcd_impl_flexio.h"

#ifdef LCD_IMPL_FLEXIO

#define LCD_FLEXIO      FLEXIO0
#define LCD_FLEXIO_FREQ 150000000
 /* *Slightly* overclocked to 25MHz, use 256000000 for 16MHz. */
 /* Note baud is bit per second in SDK's view, so this is multiplied by 16 on a 16-bit bus. */
#define LCD_FLEXIO_BAUD 400000000

#define SET_GPIO(x, v) GPIO_PinWrite(BOARD_INITLCDFXIOPINS_LCD_##x##_GPIO, BOARD_INITLCDFXIOPINS_LCD_##x##_GPIO_PIN, v)

static void lcd_impl_set_cs_pin(bool set);
static void lcd_impl_set_rs_pin(bool set);
#if LCD_IMPL_FLEXIO_EDMA
static void lcd_impl_dma_callback(FLEXIO_MCULCD_Type *base, flexio_mculcd_edma_handle_t *handle, status_t status,
                                  void *userData);
#endif

void lcd_impl_init(lcd_impl_flexio_t *impl) {
    flexio_mculcd_config_t flexio_cfg;

    SET_GPIO(RS, 1U);
    SET_GPIO(RST, 1U);
    SET_GPIO(BLK, 1U);
#if LCD_IMPL_FLEXIO_EDMA
    edma_config_t edma_cfg;

    CLOCK_EnableClock(kCLOCK_Dma0);

    EDMA_GetDefaultConfig(&edma_cfg);
    EDMA_Init(DMA0, &edma_cfg);

    NVIC_SetPriority(EDMA_0_CH0_IRQn, 5);

    EDMA_EnableChannelRequest(DMA0, 0);
    EDMA_SetChannelMux(DMA0, 0, kDma0RequestMuxFlexIO0ShiftRegister0Request);

    impl->semaphore = xSemaphoreCreateBinary();
    if (impl->semaphore == NULL) {
        vTaskSuspend(NULL);
    }

    xSemaphoreGive(impl->semaphore);

#endif

    CLOCK_AttachClk(kPLL0_to_FLEXIO);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, CLOCK_GetPll0OutFreq() / LCD_FLEXIO_FREQ);

    FLEXIO_MCULCD_GetDefaultConfig(&flexio_cfg);

    flexio_cfg.baudRate_Bps  = LCD_FLEXIO_BAUD;
    flexio_cfg.enableInDebug = true;
    flexio_cfg.enableInDoze  = true;

    impl->dev.flexioBase = LCD_FLEXIO;
    impl->dev.busType    = kFLEXIO_MCULCD_8080;

    impl->dev.timerIndex = 0U;

    impl->dev.txShifterStartIndex = 0;
    impl->dev.txShifterEndIndex   = 7;
    impl->dev.rxShifterStartIndex = 0;
    impl->dev.rxShifterEndIndex   = 7;

    impl->dev.dataPinStartIndex = 16;
    impl->dev.RDPinIndex        = 0;
    impl->dev.ENWRPinIndex      = 1;

    impl->dev.setCSPin = lcd_impl_set_cs_pin;
    impl->dev.setRSPin = lcd_impl_set_rs_pin;

    FLEXIO_MCULCD_Init(&impl->dev, &flexio_cfg, CLOCK_GetFlexioClkFreq());

#if LCD_IMPL_FLEXIO_EDMA
    EDMA_CreateHandle(&impl->tx_handle, DMA0, 0);
    FLEXIO_MCULCD_TransferCreateHandleEDMA(&impl->dev, &impl->dma_hdl, lcd_impl_dma_callback, (void *)impl,
                                           &impl->tx_handle, NULL);
#endif
}

st7796_ret_t lcd_impl_reset(void *handle) {
    SDK_DelayAtLeastUs(50 * 1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    SET_GPIO(RST, 0U);
    SDK_DelayAtLeastUs(50 * 1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    SET_GPIO(RST, 1U);
    SDK_DelayAtLeastUs(50 * 1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    return ST7796_OK;
}

st7796_ret_t lcd_impl_write_cmd(void *handle, uint8_t *cmd, uint8_t len) {
    lcd_impl_flexio_t *impl = (lcd_impl_flexio_t *)handle;

#if LCD_IMPL_FLEXIO_EDMA
    xSemaphoreTake(impl->semaphore, portMAX_DELAY);
#endif

    FLEXIO_MCULCD_StartTransfer(&impl->dev);

    FLEXIO_MCULCD_WriteCommandBlocking(&impl->dev, (uint32_t)cmd[0]);

    if (len > 1) {
        for (uint8_t i = 1; i < len; i++) {
            uint16_t data = cmd[i]; /* Zero-extend */
            FLEXIO_MCULCD_WriteDataArrayBlocking(&impl->dev, &data, 2);
        }
    }

    FLEXIO_MCULCD_StopTransfer(&impl->dev);

#if LCD_IMPL_FLEXIO_EDMA
    xSemaphoreGive(impl->semaphore);
#endif

    return ST7796_OK;
}

st7796_ret_t lcd_impl_write_data(void *handle, uint8_t *data, uint32_t len) {
    lcd_impl_flexio_t *impl = (lcd_impl_flexio_t *)handle;

#if LCD_IMPL_FLEXIO_EDMA
    edma_handle_t               tx_handle;

    xSemaphoreTake(impl->semaphore, portMAX_DELAY);
	FLEXIO_MCULCD_WriteDataEDMA(&impl->dev, &impl->dma_hdl, data, len);
#else
    FLEXIO_MCULCD_StartTransfer(&impl->dev);
    FLEXIO_MCULCD_WriteDataArrayBlocking(&impl->dev, data, len);
    FLEXIO_MCULCD_StopTransfer(&impl->dev);
#endif

    return ST7796_OK;
}

static void lcd_impl_set_cs_pin(bool set) {
    if (set) {
        SET_GPIO(CS, 1);
    } else {
        SET_GPIO(CS, 0);
    }
}

static void lcd_impl_set_rs_pin(bool set) {
    if (set) {
        SET_GPIO(RS, 1);
    } else {
        SET_GPIO(RS, 0);
    }
}

#if LCD_IMPL_FLEXIO_EDMA
static void lcd_impl_dma_callback(FLEXIO_MCULCD_Type *base, flexio_mculcd_edma_handle_t *handle, status_t status,
                                  void *userData) {
    lcd_impl_flexio_t *impl = (lcd_impl_flexio_t *)userData;

    BaseType_t higher_priority_woken = pdFALSE;

    xSemaphoreGiveFromISR(impl->semaphore, &higher_priority_woken);
    portYIELD_FROM_ISR(higher_priority_woken);
}
#endif

#endif
