#include "stm32f10x.h"
#include "psd_usart.h"
#include "psd_soft_i2c.h"
#include "mpu6050.h"
#include "string.h"
int main(void){
		Usart_1_Config();
		printf("¿ªÊ¼\n");
		Soft_I2C_Init();
		MPU6050_Init();
		int16_t dat[6];
		uint8_t id;
		while(1){	
				MPU6050_Read(&dat[0],&dat[1],&dat[2],&dat[3],&dat[4],&dat[5]);
				printf("ax:%d,ay:%d,az:%d,vx:%d,vy:%d,vz:%d\r\n",dat[0],dat[1],dat[2],dat[3],dat[4],dat[5]);
				MPU6050_Rrd_Reg(MPU6050_WHO_AM_I,&id);
				printf("id:%x\r\n",id);
				uint32_t t = 0x00ffffff;
				while(t--);
		}

}
