#include "stm32f10x.h"
#include "bsp_led.h"
#include "baseTIM.h"
int main(void){
	PWM_GPIO_Init();
	BASETIM_Config();
	BASETIM_NVIC();
	BASETIM_Start();
	while(1){

	}
}
