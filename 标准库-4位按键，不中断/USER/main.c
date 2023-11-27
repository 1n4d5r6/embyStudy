#include "stm32f10x.h"
#include "Key.h"
int main(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0 ;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_init);
	GPIO_SetBits(GPIOA,GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0);
	
	Key_Config();
	
	while(1){
		uint8_t i = KeyCheck();
		if(i > 0){
			GPIO_SetBits(GPIOA,GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0<<(i-1));
		}
	}
}
