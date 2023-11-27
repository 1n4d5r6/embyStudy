#include "nixie_tube.h"
void NIXIE_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz ;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11 ;
	GPIO_Init(GPIOB,&gpio_init);
}
static void NIXIE_Delay(void){
	for(uint16_t i = 10 ; i > 0; i--);
}
static void NIXIE_StopAll(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11);
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}

static void NIXIE_ShowEach(uint8_t index , uint8_t res){
	if (index == 0) GPIO_SetBits(GPIOB,GPIO_Pin_11);
	else if (index == 1) GPIO_SetBits(GPIOB,GPIO_Pin_10);
	else if (index == 2) GPIO_SetBits(GPIOB,GPIO_Pin_1);
	else if (index == 3) GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	if(res == 0) GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 );
	else if(res == 1) GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 );
	else if(res == 2) GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 );
	else if(res == 3) GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 );
	else if(res == 4) GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6 );
	else if(res == 5) GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 );
	else if(res == 6) GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 );
	else if(res == 7) GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 );
	else if(res == 8) GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 );
	else if(res == 9) GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 );
}

void NIXIE_Show4num(uint16_t res){
	uint16_t dat = res;
	for (uint8_t i = 0 ; i < 4 ; i++){
		if (i == 0) dat = res%10;
		else if (i == 1) dat = (res/10)%10;
		else if (i == 2) dat = (res/100)%10;
		else if (i == 3) dat =  res/1000;
		NIXIE_ShowEach(i,dat);
		NIXIE_Delay();
		NIXIE_StopAll();
	}
}
