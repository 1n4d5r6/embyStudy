#include "stm32f10x.h"
#include "bsp_led.h"
void delay_ms(u16 time)		//∫¡√Îº∂—” ±∫Ø ˝
{
	u16 i=0;
	while(time--)
	{
	     i=12000;
	     while(i--);
	}
}
int main(){
	Led_Green_Config();
	Led_Red_Config();
	Led_Blue_Config();
	Led_Init();
	while(1){
		Led_OpenRed();
		delay_ms(1000);
		Led_CloseRed();
	
		Led_OpenGreen();
		delay_ms(1000);
		Led_CloseGreen();
	
		Led_OpenBlue();
		delay_ms(1000);
		Led_CloseBlue();
	
		Led_OpenRed();
		Led_OpenGreen();
		delay_ms(1000);
		Led_CloseRed();
	
		Led_OpenBlue();
		delay_ms(1000);
		Led_CloseGreen();
	
		Led_OpenRed();
		delay_ms(1000);
	
		Led_OpenGreen();
		delay_ms(1000);
	
		Led_CloseRed();
		Led_CloseGreen();
		Led_CloseBlue();
		delay_ms(1000);
	}
}