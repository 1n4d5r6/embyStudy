#include "stm32f10x.h"
#include "psd_usart.h"
#include "string.h"
void delay_ms(u32 time);
extern uint8_t USART1_RecvBuff[256];
extern uint8_t USART1_RecvLen;
int main(){
	uint16_t i = 50000;
	Usart_1_Config();
	while(1){
		//Usart_SendNum_Int16(USART1,32767);
		//Usart_SendNum_Double(USART1, 12546.98562);
		//Usart_SendByte(USART1,'\n');
		//uint16_t a = 125;
		//Usart_SendHalfWord(USART1,a);
		//Usart_SendString(USART1,"你好中国\t\n");

		if(USART1_RecvLen!=0){
			Usart_SendString(USART1,(char*)USART1_RecvBuff);
			memset(USART1_RecvBuff,'\0',256);
			USART1_RecvLen =0;
		}
		delay_ms(1000);
	}
}
void delay_ms(u32 time)		//毫秒级延时函数
{
	u32 i=0;
	while(time--)
	{
	     i=12000;
	     while(i--);
	}
}