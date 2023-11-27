#include "Key.h"
void Key_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_IPU;
	gpio_init.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_init);
}
static void Key_Delay(void){
	uint32_t i = 0x0000ffff;
	while(i--);
}
uint8_t KeyCheck(void){
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) == 0){
		Key_Delay();
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) == 0){
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) == 0);
			return Key_1;
		} 
	}
	else	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == 0){
		Key_Delay();
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == 0){
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == 0);
			return Key_2;
		}
	}
	else	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 0){
		Key_Delay();
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 0){
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 0);
			return Key_3;
		}
	}
	else	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 0){
		Key_Delay();
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 0){
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 0);
			return Key_4;
		}
	}
	return 0;
}
