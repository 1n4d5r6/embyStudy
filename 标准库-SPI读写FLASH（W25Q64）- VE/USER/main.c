#include "stm32f10x.h"
#include "w25q64.h"
#include "psd_usart.h"
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
int main(void){
uint32_t DeviceID , FlashID ;


	/* ���ô���Ϊ��115200 8-N-1 */
	Usart_1_Config();
	printf("\r\n ����һ��8Mbyte����flash(W25Q64)ʵ�� \r\n");
	printf("\r\n ʹ��ָ���ߵװ�ʱ ���Ͻ�����λ�� ��Ҫ��PC0������ñ ��ֹӰ��PC0��SPIFLASHƬѡ�� \r\n");
	
	Delay(100000);
	uint8_t Tx_Buffer[450];
	uint8_t Rx_Buffer[450];
	for (uint16_t i = 0 ; i < 450 ; i++) Tx_Buffer[i] = ( i * 6 ) % 256;
	
	/* 8M����flash W25Q64��ʼ�� */
	W25Q64_SPI_Init();
	
	
	
	/* ��ȡ Flash Device ID */
	DeviceID = W25Q64_ReadDeviceID();	
	Delay( 200 );
	
	/* ��ȡ SPI Flash ID */
	FlashID = W25Q64_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* ���� SPI Flash ID */
	if (FlashID == sFLASH_ID)
	{	
		printf("\r\n ��⵽����flash W25Q64 !\r\n");
		
		W25Q64_EraseSector(Sector_Addr(1555));	 	 
		
		W25Q64_BufferWrite(Tx_Buffer, Sector_Addr(1555), 450);		

		W25Q64_BufferRead(Rx_Buffer, Sector_Addr(1555), 450);

		for(uint16_t i = 0 ; i <= 450-1 ; i++)
			printf("0x%02X \n",Rx_Buffer[i]);

		uint16_t i = 0;
		for (; i < 450 ; i++){
			if(Tx_Buffer[i] != Rx_Buffer[i]) break;
		}	
		if( i == 450 )
		{ 
			printf("\r\n 8M����flash(W25Q64)���Գɹ�!\n\r");
		}
		else
		{        
			printf("\r\n 8M����flash(W25Q64)����ʧ��!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else// if (FlashID == sFLASH_ID)
	{ 
		printf("\r\n ��ȡ���� W25Q64 ID!\n\r");
	}
	
	while(1);  
}
