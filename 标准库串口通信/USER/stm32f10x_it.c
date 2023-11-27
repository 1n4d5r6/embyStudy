//#include "stm32f10x.h"
//void USART1_IRQHandler(){
//	uint8_t ucTemp;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
//		ucTemp = USART_ReceiveData(USART1);
//		USART_SendData(USART1,ucTemp);
//	}
//}