#include "stm32f10x.h"
#include "rtc.h"
#include "psd_usart.h"
extern uint8_t RTC_Clock_Flag;
int main(void){
	Usart_1_Config();
	RTC_Clock_Init();
	while(1){

	}
}
