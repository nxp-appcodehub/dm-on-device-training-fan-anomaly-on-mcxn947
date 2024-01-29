/*
 * imu_config.h
 *
 *  Created on: 2024年1月18日
 *      Author: nxa04576
 */

#ifndef IMU_OPS_H_
#define IMU_OPS_H_


#include "mpu6050.h"
#include "fxls8974.h"

uint8_t IMU_Init(void);
uint8_t IMU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t IMU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t IMU_Write_Byte(uint8_t reg,uint8_t data);
uint8_t IMU_Read_Byte(uint8_t reg);

uint8_t IMU_Set_Gyro_Fsr(uint8_t fsr);
uint8_t IMU_Set_Accel_Fsr(uint8_t fsr);
uint8_t IMU_Set_LPF(uint16_t lpf);
uint8_t IMU_Set_Rate(uint16_t rate);
uint8_t IMU_Set_Fifo(uint8_t sens);
uint16_t IMU_Get_Fifo_Cnt();



void IMU_ReadSensorDataToRingBuffer(uint16_t readSize);
int IMU_ReadSensorData(int16_t *pBuf, uint16_t fifo_cnt, uint16_t readSize);

#endif /* IMU_OPS_H_ */
