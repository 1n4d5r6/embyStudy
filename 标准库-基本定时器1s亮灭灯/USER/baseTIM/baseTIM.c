#include "baseTIM.h"
void BASETIM_Config(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseInitTypeDef tim_initstruct;
	tim_initstruct.TIM_Period = 1000 ;
	tim_initstruct.TIM_Prescaler = 71;
	TIM_TimeBaseInit(TIM2,&tim_initstruct);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);
}
void BASETIM_Start(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
}
void BASETIM_Stop(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);
}
void BASETIM_NVIC(){
	NVIC_InitTypeDef nvic_initstruct;
	nvic_initstruct.NVIC_IRQChannel = TIM2_IRQn;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 3;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_initstruct);
}
uint16_t BASETIM_TIM2_Times = 0;
void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET){
		BASETIM_TIM2_Times ++ ;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}