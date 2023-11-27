#include "mpu6050.h"
#include "psd_soft_i2c.h"
#include "stdio.h"
void MPU6050_Init(void){
		MPU6050_Wrt_Reg(MPU6050_PWR_MGMT_1,0x01,1000);
		MPU6050_Wrt_Reg(MPU6050_PWR_MGMT_2,0x00,1000);
	
		MPU6050_Wrt_Reg(MPU6050_SMPLRT_DIV,0x09,1000);
		MPU6050_Wrt_Reg(MPU6050_CONFIG,0x06,1000);
		MPU6050_Wrt_Reg(MPU6050_GYRO_CONFIG,0x18,1000);
		MPU6050_Wrt_Reg(MPU6050_ACCEL_CONFIG,0x18,1000);

}

uint8_t MPU6050_Wrt_Reg(uint8_t addr,uint8_t data,uint32_t timeout){
		
		uint32_t t = timeout;
		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)){
				if ((t--) == 0) return 1;
		}
		
		I2C_GenerateSTART(I2C1,ENABLE);
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
				if ((t--) == 0) return 1;
		}
		
		I2C_Send7bitAddress(I2C1, MPU6050_ADDR,I2C_Direction_Transmitter);
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
				if ((t--) == 0) return 1;
		}
		
		I2C_SendData(I2C1, addr);
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)){
				if ((t--) == 0) return 1;
		}
		
		I2C_SendData(I2C1,data);
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
				if ((t--) == 0) return 1;
		}
		
		I2C_GenerateSTOP(I2C1, ENABLE);
		
		return 0;
}

uint8_t MPU6050_Rrd_Reg(uint8_t addr,volatile uint8_t *data,uint32_t timeout){
		uint32_t t = timeout;
		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)){
				if ((t--) == 0) return 1;
		}

		I2C_GenerateSTART(I2C1,ENABLE);		
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
				if ((t--) == 0) return 1;
		}
		
		I2C_Send7bitAddress(I2C1, MPU6050_ADDR,I2C_Direction_Transmitter);
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
				if ((t--) == 0) return 1;
		}
		
		I2C_Cmd(I2C1, ENABLE);
		I2C_SendData(I2C1,addr);
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
				if ((t--) == 0) return 1;
		}
		
		I2C_GenerateSTART(I2C1,ENABLE);
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
				if ((t--) == 0) return 1;
		}
		
		I2C_Send7bitAddress(I2C1, MPU6050_ADDR,I2C_Direction_Receiver);
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
				if ((t--) == 0) return 1;
		}
		
		t = timeout;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)){
				if ((t--) == 0) return 1;
		}
		
		*data = I2C_ReceiveData(I2C1);
		
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		I2C_GenerateSTOP(I2C1, ENABLE);
		return 0;
}
void MPU6050_Read(int16_t *ax,int16_t *ay,int16_t *az,int16_t *vx,int16_t *vy,int16_t *vz,uint32_t timeout){
		volatile uint8_t high = 0, low = 0;
		MPU6050_Rrd_Reg(MPU6050_ACCEL_XOUT_H,&high, timeout);
		MPU6050_Rrd_Reg(MPU6050_ACCEL_XOUT_L,&low, timeout);
		*ax = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_ACCEL_YOUT_H,&high, timeout);
		MPU6050_Rrd_Reg(MPU6050_ACCEL_YOUT_L,&low, timeout);
		*ay = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_ACCEL_ZOUT_H,&high, timeout);
		MPU6050_Rrd_Reg(MPU6050_ACCEL_ZOUT_L,&low, timeout);
		*az = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_GYRO_XOUT_H,&high, timeout);
		MPU6050_Rrd_Reg(MPU6050_GYRO_XOUT_L,&low, timeout);
		*vx = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_GYRO_YOUT_H,&high, timeout);
		MPU6050_Rrd_Reg(MPU6050_GYRO_YOUT_L,&low, timeout);
		*vy = ( high << 8 ) | low ;
		MPU6050_Rrd_Reg(MPU6050_GYRO_ZOUT_H,&high, timeout);
		MPU6050_Rrd_Reg(MPU6050_GYRO_ZOUT_L,&low, timeout);
		*vz = ( high << 8 ) | low ;
}
