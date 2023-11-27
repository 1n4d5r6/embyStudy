#include "stm32f10x.h"
#include "dma.h"
#include "psd_usart.h"
extern uint8_t send_data[10];
int main(void){
	Usart_1_Config();
	Usart_SendString(USART1,"串口配置完成\n");
	DMA_Config();
	printf("%s\n","DMA配置完成");
	for (uint16_t i = 0 ; i < 10 ; i++) send_data[i] = 65 + i ;
	printf("%s\n","数据初始化完成");
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	while(1){
//		
//		while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);
//		printf("%s\n","完成");
	}
}
