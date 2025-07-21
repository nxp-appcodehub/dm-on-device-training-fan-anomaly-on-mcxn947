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
#include "fsl_flash.h"
#include "fsl_flash_ffr.h"
#include "app_main.h"
#include "app_lvgl.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define I2C_RELEASE_SDA_PORT  PORT4
#define I2C_RELEASE_SCL_PORT  PORT4
#define I2C_RELEASE_SDA_GPIO  GPIO4
#define I2C_RELEASE_SDA_PIN   0U
#define I2C_RELEASE_SCL_GPIO  GPIO4
#define I2C_RELEASE_SCL_PIN   1U
#define I2C_RELEASE_BUS_COUNT 100U

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_lvgl_initialized = false;
lv_ui guider_ui;
AppLogic_t g_app;

static flash_config_t s_flashDriver;
#ifdef __ARMCC_VERSION
extern uint8_t Image$$ER_m_svm$$Base[];
#define m_svm_start (uint32_t)Image$$ER_m_svm$$Base

#elif defined(__GNUC__)
extern uint8_t __base_m_svm_model[];
#define m_svm_start __base_m_svm_model
#endif
static SemaphoreHandle_t s_lvgl_semphr;

extern void  svm_get_model_param_gamma_nu(void *model, float *gamma, float  *nu);
extern uint32_t svm_model_valid(void* model_ptr);
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if LV_USE_LOG
static void print_cb(const char *buf)
{
    PRINTF("\r%s\n", buf);
}
#endif

static uint32_t get_idle_time_cb(void)
{
    return (getIdleTaskTime() / 1000);
}

static void reset_idle_time_cb(void)
{
    resetIdleTaskTime();
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

static void AppTask(void *param)
{
#if LV_USE_LOG
    lv_log_register_print_cb(print_cb);
#endif

    TickType_t tick_start;
	TickType_t tick_end;
	TickType_t delay_target;

    lv_timer_register_get_idle_cb(get_idle_time_cb);
    lv_timer_register_reset_idle_cb(reset_idle_time_cb);
    lv_port_pre_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    s_lvgl_initialized = true;

    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);

    for (;;)
    {
    	tick_start = xTaskGetTickCount();

		//if (app_lvgl_lock_acquire(10000))
		{
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

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);

__WEAK void* get_ad_model(void) {
    return svm_model_valid((void*)m_svm_start) ? (void*)m_svm_start : NULL;
}

uint32_t get_flash_address(){
    return (uint32_t)m_svm_start;
}

uint32_t save_model_to_flash(void* model, uint32_t model_len){

    taskENTER_CRITICAL();
    // total 8K sector erase, the model must be little than this
    FLASH_Erase(&s_flashDriver, (uint32_t)m_svm_start, 0x2000, kFLASH_ApiEraseKey);

    for(int i = 0; i < model_len; i += 256){
        FLASH_Program(&s_flashDriver, (uint32_t)m_svm_start + i, (uint8_t*)model + i, 256);
    }
    taskEXIT_CRITICAL();
    return (uint32_t)m_svm_start;
}

static void BOARD_InitSmartDMA(void)
{
    RESET_ClearPeripheralReset(kMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_FlexioToSmartDma);

    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX0);

    SMARTDMA_InitWithoutFirmware();

    NVIC_EnableIRQ(SMARTDMA_IRQn);
    NVIC_SetPriority(SMARTDMA_IRQn, 3);
}


static void i2c_release_bus_delay(void)
{
    SDK_DelayAtLeastUs(100U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux        = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1U;

    CLOCK_EnableClock(kCLOCK_Port4);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();


    CLOCK_EnableClock(kCLOCK_Port3);
	PORT_SetPinConfig(PORT3, 2, &i2c_pin_config);
	PORT_SetPinConfig(PORT3, 3, &i2c_pin_config);

	GPIO_PinInit(GPIO3, 3, &pin_config);
	GPIO_PinInit(GPIO3, 2, &pin_config);

	/* Drive SDA low first to simulate a start */
	GPIO_PinWrite(GPIO3, 2, 0U);
	i2c_release_bus_delay();

	/* Send 9 pulses on SCL and keep SDA high */
	for (i = 0; i < 9; i++)
	{
		GPIO_PinWrite(GPIO3, 3, 0U);
		i2c_release_bus_delay();

		GPIO_PinWrite(GPIO3, 2, 1U);
		i2c_release_bus_delay();

		GPIO_PinWrite(GPIO3, 3, 1U);
		i2c_release_bus_delay();
		i2c_release_bus_delay();
	}

	/* Send stop */
	GPIO_PinWrite(GPIO3, 3, 0U);
	i2c_release_bus_delay();

	GPIO_PinWrite(GPIO3, 2, 0U);
	i2c_release_bus_delay();

	GPIO_PinWrite(GPIO3, 3, 1U);
	i2c_release_bus_delay();

	GPIO_PinWrite(GPIO3, 2, 1U);
	i2c_release_bus_delay();
}


/*!
 * @brief Main function
 */
int main(void)
{
    BaseType_t stat;

    /* Init board hardware. */
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO 12M to FLEXCOMM2 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    CLOCK_AttachClk(kPLL0_to_FLEXIO);
    BOARD_InitBootClocks();
    BOARD_I2C_ReleaseBus();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    DEMO_InitUsTimer();
    
    // Init the LED for show
	CLOCK_EnableClock(kCLOCK_Gpio3);
	INIT_LED_PINS(RED, LOGIC_LED_OFF);
	INIT_LED_PINS(GREEN, LOGIC_LED_OFF);
	LED_RED_INIT(0);
	LED_GREEN_INIT(0);
	LED_GREEN_ON();
	FLASH_Init(&s_flashDriver);
	FFR_Init(&s_flashDriver);
	PRINTF("Anomaly detection demo.\r\n");

    /* Init smartdma. */
    BOARD_InitSmartDMA();

   	s_lvgl_semphr = xSemaphoreCreateBinary();
	if (s_lvgl_semphr == NULL) {
		return -2;
	}

	xSemaphoreGive(s_lvgl_semphr);

	g_app.pADModel = get_ad_model();
	g_app.qh_NewSample = xQueueCreate(1, APP_FEATURE_SIZE);
	g_app.health = APP_HEALTH_ABS_MAX * 0.5f;
	if (g_app.pADModel != NULL)
	{
		svm_get_model_param_gamma_nu(g_app.pADModel, &g_app.gamma, &g_app.nu);
	}
	else
	{
		g_app.gamma = DEFAULT_GAMMA;
		g_app.nu = DEFAULT_NU;
	}

    stat = xTaskCreate(AppTask, "lvgl", 1024, NULL, 3, NULL);
    if (pdPASS != stat)
    {
        PRINTF("Failed to create lvgl task");
        while (1)
            ;
    }

    if (xTaskCreate(app_sensor_task, "SENS", 4096, NULL, configMAX_PRIORITIES-1, NULL) != pdPASS) {
		PRINTF("Failed to create sensor task.\r\n");
		while (1) {}
	}

	if (xTaskCreate(app_algo_task, "ALGO", 0x1000, NULL, configMAX_PRIORITIES-1, NULL) != pdPASS) {
		PRINTF("Failed to create sensor task.\r\n");
		while (1) {}
	}

    vTaskStartScheduler();

    for (;;)
    {
    } /* should never get here */
}

/*!
 * @brief Malloc failed hook.
 */
void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc failed. Increase the heap size.");

    for (;;)
        ;
}

/*!
 * @brief FreeRTOS tick hook.
 */
void vApplicationTickHook(void)
{
    if (s_lvgl_initialized)
    {
        //lv_tick_inc(1);
    }
}

/*!
 * @brief Stack overflow hook.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)pcTaskName;
    (void)xTask;

    for (;;)
        ;
}
