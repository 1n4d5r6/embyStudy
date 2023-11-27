#include "stm32f10x.h"
#include "bsp_led.h"
#include "baseTIM.h"
int main(void){
	BASETIM_Config();
	BASETIM_NVIC();
	BASETIM_Start();
	extern uint16_t BASETIM_TIM2_Times;
	Led_Blue_Config();
	Led_OpenBlue();
	uint8_t a = 0;
	while(1){
		if(BASETIM_TIM2_Times == 1000 && a == 0){
			BASETIM_TIM2_Times = 0;
			a = 1;
			Led_OpenBlue();
		}
		else if(BASETIM_TIM2_Times == 1000 && a == 1){
			BASETIM_TIM2_Times = 0;
			a = 0;
			Led_CloseBlue();
		}
	}
}
