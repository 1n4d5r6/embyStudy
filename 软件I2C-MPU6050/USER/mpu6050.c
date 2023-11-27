#include "mpu6050.h"
#include "psd_soft_i2c.h"
#include "stdio.h"
void MPU6050_Init(void){
		MPU6050_Wrt_Reg(MPU6050_PWR_MGMT_1,0x01);
		MPU6050_Wrt_Reg(MPU6050_PWR_MGMT_2,0x00);
	
		MPU6050_Wrt_Reg(MPU6050_SMPLRT_DIV,0x09);
		MPU6050_Wrt_Reg(MPU6050_CONFIG,0x06);
		MPU6050_Wrt_Reg(MPU6050_GYRO_CONFIG,0x18);
		MPU6050_Wrt_Reg(MPU6050_ACCEL_CONFIG,0x18);

}

void MPU6050_Wrt_Reg(uint8_t addr,uint8_t data){
		Soft_I2C_Start();
		Soft_I2C_SendByte(MPU6050_W_ADDR);
		Soft_I2C_RecieveAck();
		Soft_I2C_SendByte(addr);
		Soft_I2C_RecieveAck();
		Soft_I2C_SendByte(data);
		Soft_I2C_RecieveAck();
		Soft_I2C_Stop();
}

void MPU6050_Rrd_Reg(uint8_t addr,volatile uint8_t *data){
		Soft_I2C_Start();
		Soft_I2C_SendByte(MPU6050_W_ADDR);
		Soft_I2C_RecieveAck();
		Soft_I2C_SendByte(addr);
		Soft_I2C_RecieveAck();
		Soft_I2C_Start();
		Soft_I2C_SendByte(MPU6050_R_ADDR);
		Soft_I2C_RecieveAck();
		*data = Soft_I2C_RecieveByte();
		Soft_I2C_SendNotACK();
		Soft_I2C_Stop();
}
void MPU6050_Read(int16_t *ax,int16_t *ay,int16_t *az,int16_t *vx,int16_t *vy,int16_t *vz){
		volatile uint8_t high = 0, low = 0;
		MPU6050_Rrd_Reg(MPU6050_ACCEL_XOUT_H,&high);
		MPU6050_Rrd_Reg(MPU6050_ACCEL_XOUT_L,&low);
		*ax = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_ACCEL_YOUT_H,&high);
		MPU6050_Rrd_Reg(MPU6050_ACCEL_YOUT_L,&low);
		*ay = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_ACCEL_ZOUT_H,&high);
		MPU6050_Rrd_Reg(MPU6050_ACCEL_ZOUT_L,&low);
		*az = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_GYRO_XOUT_H,&high);
		MPU6050_Rrd_Reg(MPU6050_GYRO_XOUT_L,&low);
		*vx = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_GYRO_YOUT_H,&high);
		MPU6050_Rrd_Reg(MPU6050_GYRO_YOUT_L,&low);
		*vy = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_GYRO_ZOUT_H,&high);
		MPU6050_Rrd_Reg(MPU6050_GYRO_ZOUT_L,&low);
		*vz = ( high << 8 ) | low ;
}
