#include "stm32f10x.h"
#include "psd_usart.h"
#include "bsp_i2c_ee.h"
#include "AT24C02.h"
#include "string.h"
int main(void){
		
		Usart_1_Config();
		printf("��ʼ\n");
//		AT24C02_Test();
		AT24C02_Config();
		Soft_I2C_Stop();
		uint8_t c = 'x';
		AT24C02_WriteBytes(&c,107,1);
		printf("д����\n");
		uint32_t nCount = 0xFFFFFF;
		for(; nCount != 0; nCount--){};
		printf("��ʱ����\n");
		char rxdat[100];
		memset(rxdat,'\0',100);
		AT24C02_ReadBytes((uint8_t*)rxdat,104,sizeof("helloworld!"));
		printf("������\n");
		Usart_SendString(USART1,rxdat);
		memset(rxdat,'\0',100);
		AT24C02_ReadBytes((uint8_t*)rxdat,107,1);	
		printf("������\n");
		Usart_SendString(USART1,rxdat);
		//ee_Test();
		while(1);
//		uint8_t a = ee_CheckOk();
//		if(a == 1)printf("��⵽EEPROM�豸��");
//		else if(a == 0)printf("û�м�⵽EEPROM�豸��");
//		else printf("�쳣");
}