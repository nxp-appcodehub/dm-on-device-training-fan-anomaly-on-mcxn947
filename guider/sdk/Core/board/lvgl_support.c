/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lvgl_support.h"
#include "lvgl.h"
#if defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#include "board.h"
#include "fsl_gpio.h"

#if BOARD_USE_FLEXIO_SMARTDMA
#include "fsl_dbi_flexio_smartdma.h"
#else
#include "fsl_dbi_flexio_edma.h"
#endif
#include "fsl_flexio_mculcd.h"
#include "fsl_lpi2c.h"
#include "fsl_port.h"

#if BOARD_LCD_S035
#include "fsl_st7796s.h"
#include "fsl_gt911.h"
#else
#include "fsl_ssd1963.h"
#include "fsl_ft5406_rt.h"
#endif

#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Port Me. Start */
#if BOARD_LCD_S035
#else
#define BOARD_SSD1963_XTAL_FREQ     10000000U
#define BOARD_SSD1963_PCLK_FREQ     30000000U
#define BOARD_SSD1963_HSW           48U
#define BOARD_SSD1963_HFP           40U
#define BOARD_SSD1963_HBP           0U
#define BOARD_SSD1963_VSW           3U
#define BOARD_SSD1963_VFP           13U
#define BOARD_SSD1963_VBP           18U
#define BOARD_SSD1963_POLARITY_FLAG 0U
#endif

/* Macros for FlexIO interfacing the LCD */
#define BOARD_FLEXIO              FLEXIO0
#define BOARD_FLEXIO_CLOCK_FREQ   CLOCK_GetFlexioClkFreq()
#define BOARD_FLEXIO_BAUDRATE_BPS 160000000U

/* Macros for FlexIO shifter, timer, and pins. */
#define BOARD_FLEXIO_WR_PIN           1
#define BOARD_FLEXIO_RD_PIN           0
#define BOARD_FLEXIO_DATA_PIN_START   16
#define BOARD_FLEXIO_TX_START_SHIFTER 0
#define BOARD_FLEXIO_RX_START_SHIFTER 0
#define BOARD_FLEXIO_TX_END_SHIFTER   7
#define BOARD_FLEXIO_RX_END_SHIFTER   7
#define BOARD_FLEXIO_TIMER            0

/* Macros for the touch touch controller. */
#define BOARD_TOUCH_I2C            LPI2C2
#define BOARD_TOUCH_I2C_CLOCK_FREQ CLOCK_GetLPFlexCommClkFreq(2u)
#define BOARD_TOUCH_I2C_BAUDRATE   100000U
/* Port Me. End */

#define DEMO_MS_TO_TICK(ms) ((ms * configTICK_RATE_HZ / 1000) + 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitLcd(void);

static void DEMO_InitLcdClock(void);

static status_t DEMO_InitLcdController(void);

static void DEMO_FlushDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

static void DEMO_InitTouch(void);

static void DEMO_ReadTouch(lv_indev_drv_t *drv, lv_indev_data_t *data);

static void DEMO_SetCSPin(bool set);

static void DEMO_SetRSPin(bool set);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if BOARD_LCD_S035
static gt911_handle_t touchHandle;
#else
static ft5406_rt_handle_t touchHandle;
#endif

#if defined(SDK_OS_FREE_RTOS)
static SemaphoreHandle_t s_memWriteDone;
#else
static volatile bool s_memWriteDone;
#endif

#if BOARD_LCD_S035
/* ST7796S LCD controller handle. */
st7796s_handle_t lcdHandle;
#else
/* SSD1963 LCD controller handle. */
ssd1963_handle_t lcdHandle;
#endif

/* DBI XFER handle. */
#if BOARD_USE_FLEXIO_SMARTDMA
dbi_flexio_smartdma_xfer_handle_t g_dbiFlexioXferHandle;
#else
dbi_flexio_edma_xfer_handle_t g_dbiFlexioXferHandle;
#endif
/* The FlexIO MCU LCD device. */
FLEXIO_MCULCD_Type flexioLcdDev = {
    .flexioBase          = BOARD_FLEXIO,
    .busType             = kFLEXIO_MCULCD_8080,
    .dataPinStartIndex   = BOARD_FLEXIO_DATA_PIN_START,
    .ENWRPinIndex        = BOARD_FLEXIO_WR_PIN,
    .RDPinIndex          = BOARD_FLEXIO_RD_PIN,
    .txShifterStartIndex = BOARD_FLEXIO_TX_START_SHIFTER,
    .txShifterEndIndex   = BOARD_FLEXIO_TX_END_SHIFTER,
    .rxShifterStartIndex = BOARD_FLEXIO_RX_START_SHIFTER,
    .rxShifterEndIndex   = BOARD_FLEXIO_RX_END_SHIFTER,
    .timerIndex          = BOARD_FLEXIO_TIMER,
    .setCSPin            = DEMO_SetCSPin,
    .setRSPin            = DEMO_SetRSPin,
    .setRDWRPin          = NULL /* Not used in 8080 mode. */
};

SDK_ALIGN(static uint8_t s_frameBuffer[2][LCD_VIRTUAL_BUF_SIZE * LCD_FB_BYTE_PER_PIXEL], 4);

/*******************************************************************************
 * Code
 ******************************************************************************/
static void DEMO_SetCSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, (uint8_t)set);
}

static void DEMO_SetRSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, (uint8_t)set);
}

static void DEMO_DbiMemoryDoneCallback(status_t status, void *userData)
{
#if defined(SDK_OS_FREE_RTOS)
    BaseType_t taskAwake = pdFALSE;

    xSemaphoreGiveFromISR(s_memWriteDone, &taskAwake);

    portYIELD_FROM_ISR(taskAwake);
#else
    s_memWriteDone = true;
#endif
}

#if BOARD_LCD_S035
void DEMO_LCD_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_TOUCH_I2C, BOARD_TOUCH_I2C_CLOCK_FREQ);
}

status_t DEMO_LCD_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_TOUCH_I2C, deviceAddress, subAddress, subaddressSize, (uint8_t *)txBuff, txBuffSize);
}

status_t DEMO_LCD_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_TOUCH_I2C, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}

void DEMO_TouchDelayMs(uint32_t delayMs)
{
#if defined(SDK_OS_FREE_RTOS)
    vTaskDelay(pdMS_TO_TICKS(delayMs));
#else
    SDK_DelayAtLeastUs(delayMs * 1000, CLOCK_GetCoreSysClkFreq());
#endif
}

void DEMO_TouchConfigIntPin(gt911_int_pin_mode_t mode)
{
    port_pin_config_t int_config = {/* Internal pull-up/down resistor is disabled */
                                    kPORT_PullDown,
                                    /* Low internal pull resistor value is selected. */
                                    kPORT_LowPullResistor,
                                    /* Fast slew rate is configured */
                                    kPORT_FastSlewRate,
                                    /* Passive input filter is disabled */
                                    kPORT_PassiveFilterDisable,
                                    /* Open drain output is disabled */
                                    kPORT_OpenDrainDisable,
                                    /* Low drive strength is configured */
                                    kPORT_LowDriveStrength,
                                    /* Pin is configured as GPIO */
                                    kPORT_MuxAlt0,
                                    /* Digital input enabled */
                                    kPORT_InputBufferEnable,
                                    /* Digital input is not inverted */
                                    kPORT_InputNormal,
                                    /* Pin Control Register fields [15:0] are not locked */
                                    kPORT_UnlockRegister};

    switch (mode)
    {
        case kGT911_IntPinPullUp:
            int_config.pullSelect = kPORT_PullUp;
            break;
        case kGT911_IntPinPullDown:
            int_config.pullSelect = kPORT_PullDown;
            break;
        case kGT911_IntPinInput:
            int_config.pullSelect = kPORT_PullDisable;
            break;
        default:
            break;
    };

    PORT_SetPinConfig(BOARD_LCD_INT_PORT, BOARD_LCD_INT_PIN, &int_config);
}

void DEMO_TouchConfigResetPin(bool pullUp)
{
    /*
     * As touch controller and display controller shares the same reset pin,
     * we do not do actual reset / address configuration here. Please check below for
     * the relationship between RST pin and INT pin.
     *
     */
}
#endif

/* Clear the LCD controller video memory content. */
static void DEMO_ClearLcd(void)
{
    int32_t leftLinesToClear = LCD_HEIGHT;
    int32_t curLinesToClear;
    int32_t startLine = 0;

    while (leftLinesToClear > 0)
    {
        curLinesToClear =
            (leftLinesToClear > (2 * LCD_VIRTUAL_BUF_HEIGHT)) ? (2 * LCD_VIRTUAL_BUF_HEIGHT) : leftLinesToClear;

#if BOARD_LCD_S035
        ST7796S_SelectArea(&lcdHandle, 0, startLine, LCD_WIDTH - 1, startLine + curLinesToClear - 1);
#else
        SSD1963_SelectArea(&lcdHandle, 0, startLine, LCD_WIDTH - 1, startLine + curLinesToClear - 1);
#endif

#if !defined(SDK_OS_FREE_RTOS)
        s_memWriteDone = false;
#endif

#if BOARD_LCD_S035
        ST7796S_WritePixels(&lcdHandle, (uint16_t *)s_frameBuffer, curLinesToClear * LCD_WIDTH);
#else
        SSD1963_WriteMemory(&lcdHandle, (const uint8_t *)s_frameBuffer,
                            curLinesToClear * LCD_WIDTH * LCD_FB_BYTE_PER_PIXEL);
#endif

#if defined(SDK_OS_FREE_RTOS)
        if (xSemaphoreTake(s_memWriteDone, portMAX_DELAY) != pdTRUE)
        {
            PRINTF("Wait semaphore error: s_memWriteDone\r\n");
            assert(0);
        }
#else
        while (false == s_memWriteDone)
        {
        }
#endif

        startLine += curLinesToClear;
        leftLinesToClear -= curLinesToClear;
    }
}

status_t DEMO_InitLcdController(void)
{
    status_t status;

    flexio_mculcd_config_t flexioMcuLcdConfig;

#if BOARD_LCD_S035
    const st7796s_config_t st7796sConfig = {.driverPreset    = kST7796S_DriverPresetLCDPARS035,
                                            .pixelFormat     = kST7796S_PixelFormatRGB565,
                                            .orientationMode = kST7796S_Orientation270,
                                            .teConfig        = kST7796S_TEDisabled,
                                            .invertDisplay   = true,
                                            .flipDisplay     = true,
                                            .bgrFilter       = true};
#else
    const ssd1963_config_t ssd1963Config = {.pclkFreq_Hz    = BOARD_SSD1963_PCLK_FREQ,
                                            .pixelInterface = kSSD1963_RGB565,
                                            .panelDataWidth = kSSD1963_PanelData24Bit,
                                            .polarityFlags  = BOARD_SSD1963_POLARITY_FLAG,
                                            .panelWidth     = LCD_WIDTH,
                                            .panelHeight    = LCD_HEIGHT,
                                            .hsw            = BOARD_SSD1963_HSW,
                                            .hfp            = BOARD_SSD1963_HFP,
                                            .hbp            = BOARD_SSD1963_HBP,
                                            .vsw            = BOARD_SSD1963_VSW,
                                            .vfp            = BOARD_SSD1963_VFP,
                                            .vbp            = BOARD_SSD1963_VBP};
#endif

    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

#if BOARD_USE_FLEXIO_SMARTDMA
    flexio_mculcd_smartdma_config_t flexioEzhConfig = {
        .inputPixelFormat = kFLEXIO_MCULCD_RGB565,
        .outputPixelFormat = kFLEXIO_MCULCD_RGB565,
    };
#endif

    /* Set SSD1963 CS, RS, and reset pin to output. */
    GPIO_PinInit(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, &pinConfig);
    GPIO_PinInit(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, &pinConfig);
    GPIO_PinInit(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, &pinConfig);

    /* Initialize the flexio MCU LCD. */
    /*
     * flexioMcuLcdConfig.enable = true;
     * flexioMcuLcdConfig.enableInDoze = false;
     * flexioMcuLcdConfig.enableInDebug = true;
     * flexioMcuLcdConfig.enableFastAccess = true;
     * flexioMcuLcdConfig.baudRate_Bps = 96000000U;
     */
    FLEXIO_MCULCD_GetDefaultConfig(&flexioMcuLcdConfig);
    flexioMcuLcdConfig.baudRate_Bps = BOARD_FLEXIO_BAUDRATE_BPS;

    status = FLEXIO_MCULCD_Init(&flexioLcdDev, &flexioMcuLcdConfig, BOARD_FLEXIO_CLOCK_FREQ);

    if (kStatus_Success != status)
    {
        return status;
    }

#if BOARD_USE_FLEXIO_SMARTDMA
    /* Create the DBI XFER handle. */
    status = DBI_FLEXIO_SMARTDMA_CreateXferHandle(&g_dbiFlexioXferHandle, &flexioLcdDev, &flexioEzhConfig);
#else
    /* Create the DBI XFER handle. Because DMA transfer is not used, so don't
     * need to create DMA handle.
     */
    status = DBI_FLEXIO_EDMA_CreateXferHandle(&g_dbiFlexioXferHandle, &flexioLcdDev, NULL, NULL);
#endif

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Reset the SSD1963 LCD controller. */
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 0);

#if BOARD_LCD_S035
    /* Required for GT911 I2C address mode 0 */
    DEMO_TouchConfigIntPin(kGT911_IntPinPullDown);
#endif

#if defined(SDK_OS_FREE_RTOS)
    vTaskDelay(DEMO_MS_TO_TICK(1)); /* Delay at least 10ns. */
#else
    SDK_DelayAtLeastUs(1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 1);

#if BOARD_LCD_S035
    DEMO_TouchConfigIntPin(kGT911_IntPinInput);
#endif

#if defined(SDK_OS_FREE_RTOS)
    vTaskDelay(DEMO_MS_TO_TICK(5)); /* Delay at 5ms. */
#else
    SDK_DelayAtLeastUs(5000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif

#if BOARD_USE_FLEXIO_SMARTDMA
#if BOARD_LCD_S035
    status = ST7796S_Init(&lcdHandle, &st7796sConfig, &g_dbiFlexioSmartdmaXferOps, &g_dbiFlexioXferHandle);
#else
    status = SSD1963_Init(&lcdHandle, &ssd1963Config, &g_dbiFlexioSmartdmaXferOps, &g_dbiFlexioXferHandle,
                          BOARD_SSD1963_XTAL_FREQ);
#endif
#else
#if BOARD_LCD_S035
    status = ST7796S_Init(&lcdHandle, &st7796sConfig, &g_dbiFlexioEdmaXferOps, &g_dbiFlexioXferHandle);
#else
    status = SSD1963_Init(&lcdHandle, &ssd1963Config, &g_dbiFlexioEdmaXferOps, &g_dbiFlexioXferHandle,
                          BOARD_SSD1963_XTAL_FREQ);
#endif
#endif

    if (status == kStatus_Success)
    {
#if BOARD_LCD_S035
        ST7796S_SetMemoryDoneCallback(&lcdHandle, DEMO_DbiMemoryDoneCallback, NULL);
#else
        SSD1963_SetMemoryDoneCallback(&lcdHandle, DEMO_DbiMemoryDoneCallback, NULL);
#endif

        /* Clear the SSD1963 video ram. */
        DEMO_ClearLcd();

#if BOARD_LCD_S035
        ST7796S_EnableDisplay(&lcdHandle, true);
#else
        SSD1963_StartDisplay(&lcdHandle);

        SSD1963_SetBackLight(&lcdHandle, 128);
#endif
    }
    else
    {
        PRINTF("LCD controller initialization failed.\r\n");
    }

    return status;
}

static void DEMO_InitLcdClock(void)
{
}

static void DEMO_InitLcd(void)
{
#if defined(SDK_OS_FREE_RTOS)
    s_memWriteDone = xSemaphoreCreateBinary();
    if (NULL == s_memWriteDone)
    {
        PRINTF("Frame semaphore create failed\r\n");
        assert(0);
    }
#else
    s_memWriteDone = false;
#endif

    DEMO_InitLcdClock();
    DEMO_InitLcdController();
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
static void DEMO_FlushDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lv_coord_t x1 = area->x1;
    lv_coord_t y1 = area->y1;
    lv_coord_t x2 = area->x2;
    lv_coord_t y2 = area->y2;

    int32_t length = (x2 - x1 + 1) * (y2 - y1 + 1) * LCD_FB_BYTE_PER_PIXEL;

#if BOARD_LCD_S035
    ST7796S_SelectArea(&lcdHandle, x1, y1, x2, y2);
#else
    SSD1963_SelectArea(&lcdHandle, x1, y1, x2, y2);
#endif

#if !defined(SDK_OS_FREE_RTOS)
    s_memWriteDone = false;
#endif

#if BOARD_LCD_S035
    ST7796S_WritePixels(&lcdHandle, (uint16_t *)color_p, length / LCD_FB_BYTE_PER_PIXEL);
#else
    SSD1963_WriteMemory(&lcdHandle, (const uint8_t *)color_p, length);
#endif

#if defined(SDK_OS_FREE_RTOS)
    if (xSemaphoreTake(s_memWriteDone, portMAX_DELAY) != pdTRUE)
    {
        PRINTF("Wait semaphore error: s_memWriteDone\r\n");
        assert(0);
    }
#else
    while (!s_memWriteDone)
    {
    }
#endif

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

void lv_port_pre_init(void)
{
}

void lv_port_disp_init(void)
{
    static lv_disp_draw_buf_t disp_buf;

    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));
    lv_disp_draw_buf_init(&disp_buf, (void *)s_frameBuffer[0], (void *)s_frameBuffer[1], LCD_VIRTUAL_BUF_SIZE);

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    DEMO_InitLcd();

    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);   /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = DEMO_FlushDisplay;

    /*Set a display buffer*/
    disp_drv.draw_buf = &disp_buf;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

void lv_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad */
    DEMO_InitTouch();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = DEMO_ReadTouch;
    lv_indev_drv_register(&indev_drv);
}

/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
#if BOARD_LCD_S035
    DEMO_LCD_I2C_Init();

    gt911_config_t touchConfig = {.I2C_SendFunc     = DEMO_LCD_I2C_Send,
                                  .I2C_ReceiveFunc  = DEMO_LCD_I2C_Receive,
                                  .timeDelayMsFunc  = DEMO_TouchDelayMs,
                                  .intPinFunc       = DEMO_TouchConfigIntPin,
                                  .pullResetPinFunc = DEMO_TouchConfigResetPin,
                                  .touchPointNum    = 5,
                                  .i2cAddrMode      = kGT911_I2cAddrMode0,
                                  .intTrigMode      = kGT911_IntFallingEdge};

    GT911_Init(&touchHandle, &touchConfig);
#else
    lpi2c_master_config_t masterConfig;

    /*
     * masterConfig.enableMaster = true;
     * masterConfig.baudRate_Bps = 100000U;
     * masterConfig.enableTimeout = false;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = BOARD_TOUCH_I2C_BAUDRATE;

    /* Initialize the I2C master peripheral */
    LPI2C_MasterInit(BOARD_TOUCH_I2C, &masterConfig, BOARD_TOUCH_I2C_CLOCK_FREQ);

    /* Initialize the touch handle. */
    FT5406_RT_Init(&touchHandle, BOARD_TOUCH_I2C);
#endif
}

/* Will be called by the library to read the touchpad */
static void DEMO_ReadTouch(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
#if BOARD_LCD_S035
    touch_point_t tp[5];

    uint8_t tp_count = 5;
    GT911_GetMultiTouch(&touchHandle, &tp_count, tp);

    /**
     * GT911 supports 5 points tracking, we only tracks ID #0.
     *
     */

    bool found_track = false;

    for (uint8_t i = 0; i < tp_count; i++)
    {
        /* Found track ID #0 */
        if (tp[i].touchID == 0)
        {
            data->state = LV_INDEV_STATE_PRESSED;

            switch (lcdHandle.orientationMode)
            {
                case kST7796S_Orientation0:
                    data->point.x = (int16_t)tp[i].x;
                    data->point.y = (int16_t)tp[i].y;
                    break;
                case kST7796S_Orientation90:
                    data->point.x = (int16_t)(touchHandle.resolutionY - tp[i].y);
                    data->point.y = (int16_t)tp[i].x;
                    break;
                case kST7796S_Orientation180:
                    data->point.x = (int16_t)(touchHandle.resolutionX - tp[i].x);
                    data->point.y = (int16_t)(touchHandle.resolutionY - tp[i].y);
                    break;
                case kST7796S_Orientation270:
                    data->point.x = (int16_t)tp[i].y;
                    data->point.y = (int16_t)(touchHandle.resolutionX - tp[i].x);
                    break;
                default:
                    break;
            }

            found_track = true;
            break;
        }
    }

    /* No track #0 found... */
    if (!found_track)
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
#else
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;

    data->state = LV_INDEV_STATE_REL;

    if (kStatus_Success == FT5406_RT_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y))
    {
        if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
        {
            data->state = LV_INDEV_STATE_PR;
        }
    }

    /*Set the last pressed coordinates*/
    data->point.y = touch_y;
    data->point.x = touch_x;
#endif
}
