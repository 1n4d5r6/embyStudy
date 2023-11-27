#include "bsp_led.h"

void Led_Init(){
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
}
void Led_Green_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_INITSTRUCT;
	GPIO_INITSTRUCT.GPIO_Pin = GPIO_Pin_0;
	GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_INITSTRUCT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_INITSTRUCT);
}

void Led_Blue_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_INITSTRUCT;
	GPIO_INITSTRUCT.GPIO_Pin = GPIO_Pin_1;
	GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_INITSTRUCT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_INITSTRUCT);
}

void Led_Red_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_INITSTRUCT;
	GPIO_INITSTRUCT.GPIO_Pin = GPIO_Pin_5;
	GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_INITSTRUCT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_INITSTRUCT);
}

void Led_OpenGreen(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}

void Led_OpenBlue(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}

void Led_OpenRed(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
}

void Led_CloseGreen(){
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

void Led_CloseBlue(){
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
}

void Led_CloseRed(){
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
}
