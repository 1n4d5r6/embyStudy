#include "bsp_baseTim.h" 
#include "bsp_led.h"
void BaseTim_NVIC_Init(void){
	NVIC_InitTypeDef nvic_inittypeddef;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	nvic_inittypeddef.NVIC_IRQChannel = TIM6_IRQn;
	nvic_inittypeddef.NVIC_IRQChannelCmd = ENABLE;
	nvic_inittypeddef.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_inittypeddef.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&nvic_inittypeddef);
}

void BaseTim_Tim_Init(void){
	TIM_TimeBaseInitTypeDef tbit;
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6EN,ENABLE);
	tbit.TIM_Prescaler = 71;
	tbit.TIM_Period = 1000;
	TIM_TimeBaseInit(TIM6,&tbit);
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6EN,DISABLE);
}

void BaseTim_Tim_Start(void){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6EN,ENABLE);
}

void TIM6_IRQHandler(void){
	static uint16_t t = 1;
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET){
		TIM_ClearFlag(TIM6,TIM_FLAG_Update);
		t++;
		if(t%1000 == 0){
			if(t==1000){
				Led_CloseRed();
				Led_OpenGreen();
			}
			else if(t==2000){
				Led_CloseGreen();
				Led_OpenBlue();
			}
			else if(t==3000){
				Led_CloseBlue();
				Led_OpenRed();
			}
		}
		if(t == 3001) t = 1;	
	}
}
