#ifndef __PSD_SOFT_I2C_H_
#define __PSD_SOFT_I2C_H_
#include "stm32f10x.h"
#define I2C_PORT GPIOB
#define I2C_CLK  RCC_APB2Periph_GPIOB
#define SCL_PIN  GPIO_Pin_6
#define SDA_PIN  GPIO_Pin_7
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
void Soft_I2C_Delay(void);
#endif
