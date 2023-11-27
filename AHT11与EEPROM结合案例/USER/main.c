#include "stm32f10x.h"
#include "AT24C02.h"
#include "AHT20.h"
#include "psd_usart.h"
int main(void){
	Usart_1_Config();
	Soft_I2C_Init();
	uint32_t CT_data[2];
	uint8_t tx_data[6];
	uint8_t rx_data[6];
	Soft_I2C_Delay_1ms(500);
	extern uint8_t USART1_RecvBuff[128];
	extern uint8_t USART1_RecvLen;
	if((AHT20_Read_Status()&0x18)!=0x18)
	{
		AHT20_Start_Init(); //重新初始化寄存器
		Delay_1ms(10);
	}
	while(1){
		//printf("%s\n",USART1_RecvBuff);
		if(USART1_RecvBuff[0] == '0'){
			volatile int  c1,t1;
			AHT20_Read_CTdata(CT_data);       
			c1 = CT_data[0]*100*10/1024/1024;  
			t1 = CT_data[1]*200*10/1024/1024-500;	
			for(uint16_t i = 0 ; i < 6 ; i ++){
				if(i == 0){
					if(t1 >= 0) tx_data[0] = '+';
					else {
						tx_data[0] = '-';
						t1 = 0 - t1;
					}
				}
				else if(i == 1) tx_data[1] = t1 / 100 + '0';
				else if(i == 2) tx_data[2] = t1 / 10 % 10 + '0';
				else if(i == 3) tx_data[3] = '.';
				else if(i == 4) tx_data[4] = t1 % 10 + '0';
				else tx_data[5] = '\0';
		}
		AT24C02_WriteBytes(tx_data,0,6);
		for(uint16_t i = 0 ; i < 6 ; i ++){
			if(i == 0){
				if(c1 >= 0) tx_data[0] = '+';
				else {
					tx_data[0] = '-';
					c1 = 0 - c1;
				}
			}
			else if(i == 1) tx_data[1] = c1 / 100 + '0';
			else if(i == 2) tx_data[2] = c1 / 10 % 10 + '0';
			else if(i == 3) tx_data[3] = '.';
			else if(i == 4) tx_data[4] = c1 % 10 + '0';
			else tx_data[5] = '\0';
		}
		AT24C02_WriteBytes(tx_data,6,6);
		USART1_RecvBuff[0] = '\0';
		USART1_RecvLen =0;
		}
		else if(USART1_RecvBuff[0] == '1'){

			AT24C02_ReadBytes(rx_data,0,6);
			printf("%s\n",rx_data);
			USART1_RecvBuff[0] = '\0';
			USART1_RecvLen =0;
		}
		else if(USART1_RecvBuff[0] == '2'){
			
			AT24C02_ReadBytes(rx_data,6,6);
			printf("%s\n",rx_data);
			USART1_RecvBuff[0] = '\0';
			USART1_RecvLen =0;
		}
		//Delay_1ms(10000);
	}
	
}