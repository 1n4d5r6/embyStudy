#include "stm32f10x.h"
#include "psd_soft_i2c.h"
void Soft_I2C_Delay(void)
{
	uint8_t i;
	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 40; i++);
}

void Soft_I2C_Init(void){
	RCC_APB2PeriphClockCmd(I2C_CLK,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SCL_PIN;
	
	GPIO_Init(I2C_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	GPIO_Init(I2C_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(I2C_PORT,SCL_PIN);
	GPIO_SetBits(I2C_PORT,SDA_PIN);
}

void Soft_I2C_SCL(uint8_t t){
		if (t)  GPIO_SetBits(I2C_PORT,SCL_PIN);
		else 		GPIO_ResetBits(I2C_PORT,SCL_PIN);
		Soft_I2C_Delay();
}

void Soft_I2C_SDA(uint8_t t){
		if (t)  GPIO_SetBits(I2C_PORT,SDA_PIN);
		else 		GPIO_ResetBits(I2C_PORT,SDA_PIN);
		Soft_I2C_Delay();
}

void Soft_I2C_Start(void){
	  Soft_I2C_SDA(1);
		Soft_I2C_SCL(1);
		Soft_I2C_SDA(0);
		Soft_I2C_SCL(0);
}

void Soft_I2C_Stop(void){
	  Soft_I2C_SDA(0);
		Soft_I2C_SCL(1);
		Soft_I2C_SDA(1);
}

void Soft_I2C_SendByte(uint8_t dat){
	for(uint8_t i = 0;i < 8 ; i++){
		Soft_I2C_SDA(dat&0x80);
		Soft_I2C_SCL(1);
		Soft_I2C_SCL(0);
		dat <<= 1;
	}
	Soft_I2C_SDA(1);
}

void Soft_I2C_SendACK(void){
	Soft_I2C_SDA(0);
	Soft_I2C_SCL(1);
	Soft_I2C_SCL(0);
	Soft_I2C_SDA(1);
}

void Soft_I2C_SendNotACK(void){
	Soft_I2C_SDA(1);
	Soft_I2C_SCL(1);
	Soft_I2C_SCL(0);
}

uint8_t Soft_I2C_RecieveByte(void){
	uint8_t dat = 0,i;
	for(i = 0 ; i < 8 ; i++){
		dat <<= 1;
		Soft_I2C_SCL(1);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)) dat ++ ;
		Soft_I2C_SCL(0);
	}
	return dat;
}

uint8_t Soft_I2C_RecieveAck(void){
	uint8_t re = 0;
	Soft_I2C_SCL(1);
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)){
		re = 1;
	} 
 	Soft_I2C_SCL(0);
	return re;
}

