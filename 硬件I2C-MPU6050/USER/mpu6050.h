#ifndef	__MPU6050_H__
#define	__MPU6050_H__
#include "stm32f10x.h"
#define MPU6050_ADDR 0xd0
#define MPU6050_SMPLRT_DIV 0x19
#define MPU6050_CONFIG 0x1a
#define MPU6050_GYRO_CONFIG 0x1b
#define MPU6050_ACCEL_CONFIG 0x1c

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H   0x41
#define MPU6050_TEMP_OUT_L   0x42
#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_GYRO_XOUT_L  0x44
#define MPU6050_GYRO_YOUT_H  0x45
#define MPU6050_GYRO_YOUT_L  0x46
#define MPU6050_GYRO_ZOUT_H  0x47
#define MPU6050_GYRO_ZOUT_L  0x48
#define MPU6050_PWR_MGMT_1   0x6b
#define MPU6050_PWR_MGMT_2   0x6C
#define MPU6050_WHO_AM_I	 	 0x75

void MPU6050_Init(void);
void MPU6050_Read(int16_t *ax,int16_t *ay,int16_t *az,int16_t *vx,int16_t *vy,int16_t *vz,uint32_t timeout);
uint8_t MPU6050_Wrt_Reg(uint8_t addr,uint8_t data,uint32_t timeout);
uint8_t MPU6050_Rrd_Reg(uint8_t addr,volatile uint8_t *data,uint32_t timeout);

#endif
