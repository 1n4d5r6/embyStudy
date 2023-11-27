#include "stm32f10x.h"
#include "baseTIM.h"
#include "nixie_tube.h"
int main(void){
	NIXIE_Config();
  BASETIM_Config();
	BASETIM_NVIC();
  BASETIM_Start();
	extern uint16_t BASETIM_TIM2_Times;
  uint8_t hour = 0 , minute = 0 , second = 0 ;
	uint16_t	dat = 0;
	while(1){
		if(BASETIM_TIM2_Times == 1000){
			BASETIM_TIM2_Times = 0;
			second++;
			if(second == 60) {
				second = 0;
				minute++;
			}
			if(minute == 60) {
				minute = 0;
				hour++;
			}
			if(hour == 24) {
				hour = 0;
			}
		}
		dat = minute*100 + second;
		NIXIE_Show4num(dat);
	}
}
