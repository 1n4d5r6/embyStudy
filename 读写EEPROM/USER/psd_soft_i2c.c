#include "stm32f10x.h"
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
void Soft_I2C_Delay_N10us(uint32_t t)//延时函数
{
  uint32_t k;

   while(t--)
  {
    for (k = 0; k < 2; k++);//110
  }
}

void Soft_I2C_SensorDelay_us(uint32_t t)//延时函数
{
		
	for(t = t-2; t>0; t--)
	{
		Soft_I2C_Delay_N10us(1);
	}
}

void Soft_I2C_Delay_4us(void)		//延时函数
{	
	Soft_I2C_Delay_N10us(1);
	Soft_I2C_Delay_N10us(1);
	Soft_I2C_Delay_N10us(1);
	Soft_I2C_Delay_N10us(1);
}
void Soft_I2C_Delay_5us(void)		//延时函数
{	
	Soft_I2C_Delay_N10us(1);
	Soft_I2C_Delay_N10us(1);
	Soft_I2C_Delay_N10us(1);
	Soft_I2C_Delay_N10us(1);
	Soft_I2C_Delay_N10us(1);

}

void Soft_I2C_Delay_1ms(uint32_t t)		//延时函数
{
   while(t--)
  {
    Soft_I2C_SensorDelay_us(1000);//////延时1ms
  }
}
void Soft_I2C_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
}





void Soft_I2C_SDAOut(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void Soft_I2C_SDAIn(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void Soft_I2C_SCLOut(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void Soft_I2C_Start(void){
	Soft_I2C_SDAOut();
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	Soft_I2C_Delay();
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	Soft_I2C_Delay();
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	Soft_I2C_Delay(); 
}

void Soft_I2C_Stop(void){
	Soft_I2C_SDAOut();
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	Soft_I2C_Delay();
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	//Soft_I2C_Delay();
}

void Soft_I2C_SendByte(uint8_t dat){
	Soft_I2C_SDAOut();
	uint16_t i;
	for(i = 0;i < 8 ; i++){
		if(dat&0x80) GPIO_SetBits(GPIOB,GPIO_Pin_7);
		else GPIO_ResetBits(GPIOB,GPIO_Pin_7);
		Soft_I2C_Delay();
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		Soft_I2C_Delay();
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		if(i == 7) GPIO_SetBits(GPIOB,GPIO_Pin_7);
		dat <<= 1;
		Soft_I2C_Delay();
	}
}

void Soft_I2C_SendACK(void){
	Soft_I2C_SDAOut();
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	Soft_I2C_Delay();
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	Soft_I2C_Delay();
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	Soft_I2C_Delay();
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
}

void Soft_I2C_SendNotACK(void){
	Soft_I2C_SDAOut();
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	Soft_I2C_Delay();
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	Soft_I2C_Delay();
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	Soft_I2C_Delay();
}

uint8_t Soft_I2C_RecieveByte(void){
	Soft_I2C_SDAIn();
	uint8_t dat = 0,i;
	for(i = 0 ; i < 8 ; i++){
		dat <<= 1;
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		Soft_I2C_Delay();
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)) dat ++ ;
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		Soft_I2C_Delay();
	}
	return dat;
}

uint8_t Soft_I2C_RecieveAck(void){
	uint8_t re = 0;
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	Soft_I2C_Delay();
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	Soft_I2C_Delay();
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)){
		re = 1;
	} 
 	GPIO_ResetBits(GPIOB,GPIO_Pin_6);	
	Soft_I2C_Delay();
	return re;
}

