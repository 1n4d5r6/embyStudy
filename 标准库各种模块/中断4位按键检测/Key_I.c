#include "Key_I.h"
static void KEY_I_NVIC_Config(void){
	NVIC_InitTypeDef nvic_init;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvic_init.NVIC_IRQChannel = EXTI3_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_init.NVIC_IRQChannelSubPriority = 1;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init);
	
	nvic_init.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_Init(&nvic_init);
	
	nvic_init.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Init(&nvic_init);

}
static void KEY_I_EXIT_Config(void){
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
	EXTI_InitTypeDef exti_init;
	exti_init.EXTI_Line = EXTI_Line3 | EXTI_Line4 | EXTI_Line5 | EXTI_Line6;
//	exti_init.EXTI_Line = EXTI_Line3 | EXTI_Line4 ;
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti_init);
}
static void KEY_I_GPIO_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_IPU;
	gpio_init.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_init);
}
void KEY_I_Config(void){
	KEY_I_GPIO_Config();
	KEY_I_NVIC_Config();
	KEY_I_EXIT_Config();
}
void EXTI3_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) {
		uint32_t i = 0x0000ffff;
		while(i--);
		if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) == 0){
			GPIO_SetBits(GPIOA,GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		}
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}
void EXTI4_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) {
		uint32_t i = 0x0000ffff;
		while(i--);
		if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == 0){
			GPIO_SetBits(GPIOA,GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		}
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
void EXTI9_5_IRQHandler(){
	
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) {
		uint32_t i = 0x0000ffff;
		while(i--);
		if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 0){
			GPIO_SetBits(GPIOA,GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
	else if(EXTI_GetITStatus(EXTI_Line6) != RESET) {
		uint32_t i = 0x0000ffff;
		while(i--);
		if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 0){
			GPIO_SetBits(GPIOA,GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}

