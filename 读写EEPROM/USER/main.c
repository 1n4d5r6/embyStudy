#include "stm32f10x.h"
#include "psd_usart.h"
#include "bsp_i2c_ee.h"
#include "AT24C02.h"
#include "string.h"
int main(void){
		
		Usart_1_Config();
		printf("开始\n");
//		AT24C02_Test();
		AT24C02_Config();
		Soft_I2C_Stop();
		uint8_t c = 'x';
		AT24C02_WriteBytes(&c,107,1);
		printf("写完了\n");
		uint32_t nCount = 0xFFFFFF;
		for(; nCount != 0; nCount--){};
		printf("延时结束\n");
		char rxdat[100];
		memset(rxdat,'\0',100);
		AT24C02_ReadBytes((uint8_t*)rxdat,104,sizeof("helloworld!"));
		printf("读结束\n");
		Usart_SendString(USART1,rxdat);
		memset(rxdat,'\0',100);
		AT24C02_ReadBytes((uint8_t*)rxdat,107,1);	
		printf("读结束\n");
		Usart_SendString(USART1,rxdat);
		//ee_Test();
		while(1);
//		uint8_t a = ee_CheckOk();
//		if(a == 1)printf("检测到EEPROM设备！");
//		else if(a == 0)printf("没有检测到EEPROM设备！");
//		else printf("异常");
}