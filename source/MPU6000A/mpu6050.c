/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mpu6050.h"
#include "fsl_lpi2c.h"
#include "imu_ops.h"

#define SAMPLE_RATE (400)

#define I2C_MASTER_BASE (LPI2C7_BASE)
#define LPI2C_MASTER_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_Fro12M)
#define I2C_MASTER ((LPI2C_Type *)I2C_MASTER_BASE)
#define LPI2C_BAUDRATE               400000U

void MPU_IIC_Init()
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

extern void delay_ms(uint32_t);

u8 MPU_Init(void)
{ 
	u8 res;
	
	MPU_IIC_Init();
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);
	u8 mgmt1 = MPU_Read_Byte(MPU_PWR_MGMT1_REG);

    delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);
	MPU_Set_Gyro_Fsr(3);
	MPU_Set_Accel_Fsr(0);
	MPU_Set_Rate(SAMPLE_RATE);
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);
		MPU_Set_Rate(SAMPLE_RATE);
 	}else return 1;
	MPU_Set_Fifo(ACCEL_FIFO_EN);
	return 0;
}

u8 MPU_Set_Fifo(u8 sens)
{
	MPU_Write_Byte(MPU_FIFO_EN_REG, sens);
	u8 usr_ctrl = MPU_Read_Byte(MPU_USER_CTRL_REG);
	usr_ctrl |= (USR_CTRL_FIFO_ENABLE | USR_CTRL_FIFO_RESET);
	MPU_Write_Byte(MPU_USER_CTRL_REG, usr_ctrl);
}

static u8 MPU_Reset_Fifo()
{
	u8 usr_ctrl = MPU_Read_Byte(MPU_USER_CTRL_REG);
	usr_ctrl |= (USR_CTRL_FIFO_RESET);
	MPU_Write_Byte(MPU_USER_CTRL_REG, usr_ctrl);
}

u16 MPU_Get_Fifo_Cnt()
{
	// H first then L
	u8 FIFO_CNTH, FIFO_CNTL;
	FIFO_CNTH = MPU_Read_Byte(MPU_FIFO_CNTH_REG);
	FIFO_CNTL = MPU_Read_Byte(MPU_FIFO_CNTL_REG);

	return (FIFO_CNTH << 8) | FIFO_CNTL;
}

u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);
}

u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);
}

u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);
}

u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);
 	return MPU_Set_LPF(rate/2);
}

short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}

u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}

u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}

void MPU_ReadSensorDataToRingBuffer(u16 readSize) 
{
	// Must have all the accel data
	if(readSize < 6){
		return;
	}

	u8 buf[MAX_BUF_B + 4];
	// when have overflow, the data will be not
	u8 omit_len = (readSize == 1024 ? 4 : 0);
	u16 readLen = MIN(MAX_BUF_B, readSize) + omit_len;

	u8* pReadBuf = buf + omit_len;

	MPU_Read_Len(MPU_ADDR, MPU_FIFO_RW_REG, readLen, buf);

	u16 buf_dst[3];

	for(int i=0;i<(readLen - omit_len);i+=6){
		buf_dst[0] = (pReadBuf[i] << 8 | pReadBuf[i+1]);
		buf_dst[1] = (pReadBuf[i+2] << 8 | pReadBuf[i+3]);
		buf_dst[2] = (pReadBuf[i+4] << 8 | pReadBuf[i+5]);
		ringbuffer_in(&ringbuffer_handler, (u8*)buf_dst, 6);
	}
}

int MPU_ReadSensorData(int16_t *pBuf, u16 fifo_cnt, u16 readSize) 
{
	// Must have all the accel data
	if(readSize < 6){
		return 0;
	}

	u8 buf[MAX_BUF_B + 4];
	// when have overflow, the data will be not
	u8 omit_len = (fifo_cnt == 1024 ? 4 : 0);
	u16 readLen = MIN(MAX_BUF_B, readSize) + omit_len;

	u8* pReadBuf = buf + omit_len;

	MPU_Read_Len(MPU_ADDR, MPU_FIFO_RW_REG, readLen, buf);

	u16 buf_dst[3];
	if (readSize > readLen - omit_len)
		readSize = readLen - omit_len;
	
	int readNdx = 0;
	for(int i=0;i<(readLen - omit_len);i+=6){
		pBuf[readNdx++] = ((int16_t)(pReadBuf[i] << 8 | pReadBuf[i+1]))/4;
		pBuf[readNdx++] = ((int16_t)(int16_t)(pReadBuf[i+2] << 8 | pReadBuf[i+3]))/4 ;
		pBuf[readNdx++] = ((int16_t)(int16_t)(pReadBuf[i+4] << 8 | pReadBuf[i+5]))/4;
	}

	MPU_Reset_Fifo();
	return readNdx * 2;
}

void MPU_Get_Accelerometer_From_FIFO(accel_data_t *accel, u16 readSize)
{
	// Must have all the accel data
	if(readSize < 6){
		return;
	}

	u8 buf[MAX_BUF_B + 4];
	// when have overflow, the data will be not
	u8 omit_len = (readSize == 1024 ? 4 : 0);
	u16 readLen = MIN(MAX_BUF_B, readSize) + omit_len;

	u8* pReadBuf = buf + omit_len;

	MPU_Read_Len(MPU_ADDR, MPU_FIFO_RW_REG, readLen, buf);

	u16 *buf_dst = (u16*)accel;

	for(int i=0;i<(readLen - omit_len);i+=6){
		*buf_dst++ = (pReadBuf[i] << 8 | pReadBuf[i+1]);
		*buf_dst++ = (pReadBuf[i+2] << 8 | pReadBuf[i+3]);
		*buf_dst++ = (pReadBuf[i+4] << 8 | pReadBuf[i+5]);

#ifdef USE_RING_BUF
		ringbuffer_in(&ringbuffer_handler, (u8*)accel, 6);
#else
		volatile accel_data_t accel_data = *accel;
		DbgConsole_Printf("%d,%d,%d\r\n", accel_data.ax, accel_data.ay, accel_data.az);
#endif
		buf_dst = (u16*)accel;
	}
}

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
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

u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
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

u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
	MPU_Write_Len(MPU_ADDR, reg, 1, &data);
	return 0;
}

u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
	MPU_Read_Len(MPU_ADDR, reg, 1, &res);
	return res;		
}


