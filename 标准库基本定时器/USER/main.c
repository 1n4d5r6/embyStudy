#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_baseTim.h" 
int main(void){
	Led_Green_Config();
	Led_Blue_Config();
	Led_Red_Config();
	Led_Init();
	BaseTim_NVIC_Init();
	BaseTim_Tim_Init();
	BaseTim_Tim_Start();
	while(1){};
}
