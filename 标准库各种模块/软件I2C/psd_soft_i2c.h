#ifndef __PSD_SOFT_I2C_H_
#define __PSD_SOFT_I2C_H_
#include "stm32f10x.h"
void Soft_I2C_Init(void);
void Soft_I2C_Start(void);
void Soft_I2C_Stop(void);
void Soft_I2C_SDAOut(void);
void Soft_I2C_SDAIn(void);
void Soft_I2C_SCLOut(void);
void Soft_I2C_SendByte(uint8_t dat);
void Soft_I2C_SendACK(void);
void Soft_I2C_SendNotACK(void);
uint8_t Soft_I2C_RecieveByte(void);
uint8_t Soft_I2C_RecieveAck(void);
void Soft_I2C_Delay_N10us(uint32_t t);
void Soft_I2C_SensorDelay_us(uint32_t t);
void Soft_I2C_Delay_4us(void);
void Soft_I2C_Delay_5us(void);
void Soft_I2C_Delay_1ms(uint32_t t);
void Soft_I2C_Delay();
#endif
