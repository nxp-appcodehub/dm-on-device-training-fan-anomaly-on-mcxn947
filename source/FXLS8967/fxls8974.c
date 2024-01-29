/*
 * fxls8974.c
 *
 *  Created on: 2024年1月18日
 *      Author: nxa04576
 */
#include <imu_ops.h>
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"

/* Debug console */
#include "fsl_debug_console.h"
#include "fxls8974.h"
#include "fsl_lpi2c.h"


#define MULTI_READ	0
#define I2C_MASTER_BASE (LPI2C7_BASE)
#define LPI2C_MASTER_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_Fro12M)
#define I2C_MASTER ((LPI2C_Type *)I2C_MASTER_BASE)
#define LPI2C_BAUDRATE               400000U
#define FXLS8974_I2C_ADDR 0x18

extern void delay_ms(uint32_t);

void IMU_FXLS8974_IIC_Init()
{
    /* attach FRO 12M to FLEXCOMM0 */
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);
	CLOCK_EnableClock(kCLOCK_LPFlexComm7);
	CLOCK_EnableClock(kCLOCK_LPI2c7);
	CLOCK_SetClkDiv(kCLOCK_DivFlexcom7Clk, 1u);

		lpi2c_master_config_t masterConfig;
    /* init i2c */
    LPI2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = LPI2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);
}


uint8_t IMU_FXLS8974_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	lpi2c_master_transfer_t transfer = {
		.direction = kLPI2C_Write,
		.slaveAddress = addr,
		.subaddress = reg,
		.subaddressSize = sizeof(reg),
		.data = buf,
		.dataSize = len,
	};
	LPI2C_MasterTransferBlocking(I2C_MASTER, &transfer);
	return 0;
}

uint8_t IMU_FXLS8974_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	lpi2c_master_transfer_t transfer = {
		.direction = kLPI2C_Read,
		.slaveAddress = addr,
		.subaddress = reg,
		.subaddressSize = sizeof(reg),
		.data = buf,
		.dataSize = len,
	};
	LPI2C_MasterTransferBlocking(I2C_MASTER, &transfer);
	return 0;
}

uint8_t IMU_FXLS8974_Write_Byte(uint8_t reg,uint8_t data)
{
	IMU_FXLS8974_Write_Len(FXLS8974_I2C_ADDR, reg, 1, &data);
	return 0;
}

uint8_t IMU_FXLS8974_Read_Byte(uint8_t reg)
{
	uint8_t res;
	IMU_FXLS8974_Read_Len(FXLS8974_I2C_ADDR, reg, 1, &res);
	return res;
}

void IMU_FXLS8974_Flush()
{
	uint8_t standby = (IMU_FXLS8974_Read_Byte(FXLS8974_SENS_CONFIG1) & ~FXLS8974_SENS_CONFIG1_ACTIVE_MASK) | FXLS8974_SENS_CONFIG1_ACTIVE_STANDBY;
	IMU_FXLS8974_Write_Byte(FXLS8974_SENS_CONFIG1,standby);
	IMU_FXLS8974_Write_Byte(FXLS8974_BUF_CONFIG2,FXLS8974_BUF_CONFIG2_BUF_FLUSH_EN);
	uint8_t active = (IMU_FXLS8974_Read_Byte(FXLS8974_SENS_CONFIG1) & ~FXLS8974_SENS_CONFIG1_ACTIVE_MASK) | FXLS8974_SENS_CONFIG1_ACTIVE_ACTIVE;
	IMU_FXLS8974_Write_Byte(FXLS8974_SENS_CONFIG1,active);
}

uint8_t IMU_FXLS8974_Init(void)
{
	IMU_FXLS8974_IIC_Init();

	IMU_FXLS8974_Write_Byte(FXLS8974_SENS_CONFIG1,FXLS8974_SENS_CONFIG1_RST_RST);

	delay_ms(10);

	uint8_t whoami = IMU_FXLS8974_Read_Byte(FXLS8974_WHO_AM_I);

	if ((FXLS8964_WHOAMI_VALUE == whoami) || (FXLS8967_WHOAMI_VALUE == whoami))
	{
		PRINTF("\r\n Successfully Initialized Gemini with WHO_AM_I = 0x%X\r\n", whoami);
	}
	else if ((FXLS8974_WHOAMI_VALUE == whoami) || (FXLS8968_WHOAMI_VALUE == whoami))
	{
		PRINTF("\r\n Successfully Initialized Timandra with WHO_AM_I = 0x%X\r\n", whoami);
	}
	else if (FXLS8962_WHOAMI_VALUE == whoami)
	{
		PRINTF("\r\n Successfully Initialized Newstein with WHO_AM_I = 0x%X\r\n", whoami);
	}
	else
	{
		PRINTF("\r\n Bad WHO_AM_I = 0x%X\r\n", whoami);
		return 1;
	}

	/*  Start config */
	uint8_t standby = (0x00 & ~FXLS8974_SENS_CONFIG1_ACTIVE_MASK) | FXLS8974_SENS_CONFIG1_ACTIVE_STANDBY;
	IMU_FXLS8974_Write_Byte(FXLS8974_SENS_CONFIG1,standby);
	/* Set Full-scale range as 2G. */
	IMU_FXLS8974_Write_Byte(FXLS8974_SENS_CONFIG1,FXLS8974_SENS_CONFIG1_FSR_2G);
#if MULTI_READ
	/* Set Fast Read Mode. */
	IMU_FXLS8974_Write_Byte(FXLS8974_SENS_CONFIG2,FXLS8974_SENS_CONFIG2_F_READ_FAST);
	/* Set BUF Mode. */
	IMU_FXLS8974_Write_Byte(FXLS8974_BUF_CONFIG1,FXLS8974_BUF_CONFIG1_BUF_MODE_STREAM_MODE);
	/*Flush fifo*/
#else
	//IMU_FXLS8974_Write_Byte(FXLS8974_BUF_CONFIG1,FXLS8974_BUF_CONFIG1_BUF_TYPE_FILO);
#endif
	IMU_FXLS8974_Write_Byte(FXLS8974_BUF_CONFIG2,FXLS8974_BUF_CONFIG2_BUF_FLUSH_EN);
	/* Set Wake Mode ODR Rate as 6.25Hz. */
	IMU_FXLS8974_Write_Byte(FXLS8974_SENS_CONFIG3,FXLS8974_SENS_CONFIG3_WAKE_ODR_1600HZ);

	/*  end config */
	uint8_t active = (IMU_FXLS8974_Read_Byte(FXLS8974_SENS_CONFIG1)) | FXLS8974_SENS_CONFIG1_ACTIVE_ACTIVE;
	IMU_FXLS8974_Write_Byte(FXLS8974_SENS_CONFIG1,active);
#if (MULTI_READ & 0) //test
	uint8_t dataReady;
	uint8_t buf_status, sample_cnt;
	uint8_t buffer[32*6] = {0};
	uint8_t xbuffer[32*6] = {0};
	uint8_t ybuffer[32*6] = {0};
	uint8_t zbuffer[32*6] = {0};
	uint16_t x_data,y_data,z_data;
	while(1){

		buf_status = IMU_FXLS8974_Read_Byte(FXLS8974_BUF_STATUS);
		sample_cnt = FXLS8974_BUF_STATUS_BUF_CNT_MASK&buf_status;
		if(sample_cnt == 0)
			continue;
		PRINTF("Sample count=%d\r\n",sample_cnt);
		IMU_FXLS8974_Read_Len(FXLS8974_I2C_ADDR,FXLS8974_BUF_X_LSB, sample_cnt*6, buffer);

		for (int i=0;i<sample_cnt*6;i+=6)
		{
			x_data = ((int16_t)(buffer[i+0]) << 8) | buffer[i+1];
			y_data = ((int16_t)(buffer[i+2]) << 8) | buffer[i+3];
			z_data = ((int16_t)(buffer[i+4]) << 8) | buffer[i+5];
			PRINTF("x=0x%x,y=0x%x,z=0x%x\r\n",x_data,y_data,z_data);
		}
		delay_ms(400);
	}
#endif
	return 0;
}

uint16_t IMU_FXLS8974_Get_Fifo_Cnt()
{
#if MULTI_READ
	uint8_t buf_status = IMU_FXLS8974_Read_Byte(FXLS8974_BUF_STATUS);
	uint16_t sample_cnt = FXLS8974_BUF_STATUS_BUF_CNT_MASK&buf_status;

	return sample_cnt;
#else
	return 32;
#endif
}

int IMU_FXLS8974_ReadSensorData(int16_t *pBuf, uint16_t fifo_cnt, uint16_t readSize)
{
	uint8_t buffer[32*6] = {0};

	uint16_t readLen = MIN(fifo_cnt*6, readSize);
#if MULTI_READ
	IMU_FXLS8974_Read_Len(FXLS8974_I2C_ADDR,FXLS8974_BUF_X_LSB, readLen, buffer);

	for (int i=0;i<readLen;i+=6)
	{
		pBuf[0] = ((int16_t)(buffer[i+1]) << 8) | buffer[i];
		pBuf[1] = ((int16_t)(buffer[i+3]) << 8) | buffer[i+2];
		pBuf[2] = ((int16_t)(buffer[i+5]) << 8) | buffer[i+4];
		pBuf += 3;
	}
#else
	uint8_t dataReady = 0;
	int i = 0;
	while(1)
	{
		dataReady = IMU_FXLS8974_Read_Byte(FXLS8974_INT_STATUS);
		if (0 == (dataReady & FXLS8974_INT_STATUS_SRC_DRDY_MASK))
		{
			vTaskDelay(2);
			continue;
		}
		IMU_FXLS8974_Read_Len(FXLS8974_I2C_ADDR,FXLS8974_OUT_X_LSB,6,buffer);
		pBuf[0] = ((int16_t)(buffer[i+1]&0x0f) << 8) | buffer[i];
		pBuf[1] = ((int16_t)(buffer[i+3]&0x0f) << 8) | buffer[i+2];
		pBuf[2] = ((int16_t)(buffer[i+5]&0x0f) << 8) | buffer[i+4];
		pBuf += 3;
		i += 6;
		if (i >= readLen)
			break;
	}
#endif

	IMU_FXLS8974_Flush();
	return readLen;
}

