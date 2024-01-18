/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __MPU6050_H
#define __MPU6050_H

typedef unsigned char u8;
typedef unsigned short u16;

#define MPU_AD0_CTRL			PAout(15)	ַ

//#define MPU_ACCEL_OFFS_REG		0X06
//#define MPU_PROD_ID_REG			0X0C
#define MPU_SELF_TESTX_REG		0X0D
#define MPU_SELF_TESTY_REG		0X0E
#define MPU_SELF_TESTZ_REG		0X0F
#define MPU_SELF_TESTA_REG		0X10
#define MPU_SAMPLE_RATE_REG		0X19
#define MPU_CFG_REG				0X1A
#define MPU_GYRO_CFG_REG		0X1B
#define MPU_ACCEL_CFG_REG		0X1C
#define MPU_MOTION_DET_REG		0X1F
#define MPU_FIFO_EN_REG			0X23
#define MPU_I2CMST_CTRL_REG		0X24
#define MPU_I2CSLV0_ADDR_REG	0X25
#define MPU_I2CSLV0_REG			0X26
#define MPU_I2CSLV0_CTRL_REG	0X27
#define MPU_I2CSLV1_ADDR_REG	0X28
#define MPU_I2CSLV1_REG			0X29
#define MPU_I2CSLV1_CTRL_REG	0X2A
#define MPU_I2CSLV2_ADDR_REG	0X2B
#define MPU_I2CSLV2_REG			0X2C
#define MPU_I2CSLV2_CTRL_REG	0X2D
#define MPU_I2CSLV3_ADDR_REG	0X2E
#define MPU_I2CSLV3_REG			0X2F
#define MPU_I2CSLV3_CTRL_REG	0X30
#define MPU_I2CSLV4_ADDR_REG	0X31
#define MPU_I2CSLV4_REG			0X32
#define MPU_I2CSLV4_DO_REG		0X33
#define MPU_I2CSLV4_CTRL_REG	0X34
#define MPU_I2CSLV4_DI_REG		0X35

#define MPU_I2CMST_STA_REG		0X36
#define MPU_INTBP_CFG_REG		0X37
#define MPU_INT_EN_REG			0X38
#define MPU_INT_STA_REG			0X3A

#define MPU_ACCEL_XOUTH_REG		0X3B
#define MPU_ACCEL_XOUTL_REG		0X3C
#define MPU_ACCEL_YOUTH_REG		0X3D
#define MPU_ACCEL_YOUTL_REG		0X3E
#define MPU_ACCEL_ZOUTH_REG		0X3F
#define MPU_ACCEL_ZOUTL_REG		0X40

#define MPU_TEMP_OUTH_REG		0X41
#define MPU_TEMP_OUTL_REG		0X42

#define MPU_GYRO_XOUTH_REG		0X43
#define MPU_GYRO_XOUTL_REG		0X44
#define MPU_GYRO_YOUTH_REG		0X45
#define MPU_GYRO_YOUTL_REG		0X46
#define MPU_GYRO_ZOUTH_REG		0X47
#define MPU_GYRO_ZOUTL_REG		0X48

#define MPU_I2CSLV0_DO_REG		0X63
#define MPU_I2CSLV1_DO_REG		0X64
#define MPU_I2CSLV2_DO_REG		0X65
#define MPU_I2CSLV3_DO_REG		0X66

#define MPU_I2CMST_DELAY_REG	0X67
#define MPU_SIGPATH_RST_REG		0X68
#define MPU_MDETECT_CTRL_REG	0X69
#define MPU_USER_CTRL_REG		0X6A
#define MPU_PWR_MGMT1_REG		0X6B
#define MPU_PWR_MGMT2_REG		0X6C
#define MPU_FIFO_CNTH_REG		0X72
#define MPU_FIFO_CNTL_REG		0X73
#define MPU_FIFO_RW_REG			0X74
#define MPU_DEVICE_ID_REG		0X75
 
#define MPU_ADDR				0X68

#define USR_CTRL_FIFO_ENABLE     (1 << 6)
#define USR_CTRL_FIFO_RESET      (1 << 2)
#define ACCEL_FIFO_EN           (1 << 3)
#define ZG_FIFO_EN              (1 << 4)
#define YG_FIFO_EN              (1 << 5)
#define XG_FIFO_EN              (1 << 6)
#define TEMP_FIFO_EN            (1 << 7)

// accl read buffer
#define MAX_BUF_B   (60)
#define USE_RING_BUF 1

#ifdef USE_RING_BUF
#include "ringbuffer.h"
extern ringbuffer_t ringbuffer_handler;
#endif

typedef struct {
	short ax, ay, az;
} accel_data_t;

u8 MPU_Init(void);
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MPU_Write_Byte(u8 reg,u8 data);
u8 MPU_Read_Byte(u8 reg);

u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_LPF(u16 lpf);
u8 MPU_Set_Rate(u16 rate);
u8 MPU_Set_Fifo(u8 sens);
u16 MPU_Get_Fifo_Cnt();

short MPU_Get_Temperature(void);
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);
void MPU_Get_Accelerometer_From_FIFO(accel_data_t *accel, u16 cnt_B);

void MPU_ReadSensorDataToRingBuffer(u16 readSize);
int MPU_ReadSensorData(int16_t *pBuf, u16 fifo_cnt, u16 readSize);

#endif




































