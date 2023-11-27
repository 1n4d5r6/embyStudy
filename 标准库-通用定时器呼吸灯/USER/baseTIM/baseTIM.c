#include "baseTIM.h"
uint16_t indexWave[] = {
1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5,
6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 22,
25, 28, 32, 36, 41, 47, 53, 61, 69, 79,
89, 102, 116, 131, 149, 170, 193, 219,
250, 284, 323, 367, 417, 474, 539, 613,
697, 792, 901, 1024, 1024, 901, 792, 697,
613, 539, 474, 417, 367, 323, 284, 250,
219, 193, 170, 149, 131, 116, 102, 89,
79, 69, 61, 53, 47, 41, 36, 32, 28, 25,
22, 19, 17, 15, 13, 11, 10, 9, 8, 7, 6,
5, 5, 4, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1
};
uint16_t POINT_NUM = sizeof(indexWave)/sizeof(indexWave[0]);

void PWM_GPIO_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode =  GPIO_Mode_AF_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_1;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_init);
}

void BASETIM_Config(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitTypeDef tim_initstruct;
	tim_initstruct.TIM_Period = 1024-1 ;
	tim_initstruct.TIM_Prescaler = 200-1;
	tim_initstruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&tim_initstruct);
	
	TIM_OCInitTypeDef oc_init;
	oc_init.TIM_OCMode = TIM_OCMode_PWM1;
	oc_init.TIM_OutputState = TIM_OutputState_Enable;
	oc_init.TIM_Pulse = 0;
	oc_init.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM2,&oc_init);
	
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	
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
uint16_t num = 10;
void TIM2_IRQHandler(){
	static int index = 0;
	static int cnt = 0;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET){
		cnt ++ ;
		TIM2->CCR2 = indexWave[index];
		if (cnt > num){
				index++;
				if (index >= POINT_NUM) index = 0;
				cnt = 0;
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}