/*
 * imu_ops.c
 *
 *  Created on: 2024年1月22日
 *      Author: nxa04576
 */
#include <imu_ops.h>

#define IMU_6050 1
#define IMU_FXLS8974	2

static uint8_t sensor_id = 0;
uint8_t IMU_Init(void)
{
	if (MPU_Init() == 0)
	{
		sensor_id = IMU_6050;
		return 0;
	}
	else if (IMU_FXLS8974_Init() == 0)
	{
		sensor_id = IMU_FXLS8974;
		return 0;
	}

	return 1;
}

int IMU_ReadSensorData(int16_t *pBuf, uint16_t fifo_cnt, uint16_t readSize)
{
	if (sensor_id == IMU_6050)
		return MPU_ReadSensorData(pBuf, fifo_cnt, readSize);
	else if (sensor_id == IMU_FXLS8974)
		return IMU_FXLS8974_ReadSensorData(pBuf, fifo_cnt, readSize);
	else return 0;
}

uint16_t IMU_Get_Fifo_Cnt()
{
	if (sensor_id == IMU_6050)
		return MPU_Get_Fifo_Cnt();
	else if (sensor_id == IMU_FXLS8974)
		return IMU_FXLS8974_Get_Fifo_Cnt();
	else return 0;
}
