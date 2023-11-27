#include "stm32f10x.h"
#include "AT24C02.h"
#include "psd_usart.h"
int main(void){
	Usart_1_Config();
	printf("start\r\n");
	if (AT24C02_Config() == 0)
		printf("check\r\n");
	else
		printf("no check\r\n");
//	uint8_t a = 'K';
//	uint8_t b = '\0';
//	AT24C02_WriteBytes(&a,64,1);
//	printf("write\r\n");
//	uint32_t i = 0x0000ffff;
//	while(i--);
//	printf("end time\r\n");
//	AT24C02_ReadBytes(&b,64,1);
//	printf("read\r\n");
//	printf("%c\r\n",b);
	printf("write\r\n");
	uint8_t test[] = "Alice Goodbight...";
	uint8_t res[20] ={0};
	AT24C02_WriteBytes(test,13,sizeof(test));
	uint32_t i = 0x0000ffff;
	while(i--);
	printf("read\r\n");
	AT24C02_ReadBytes(res,13,sizeof(test));
	printf("%s\r\n",res);
	while(1);
}
