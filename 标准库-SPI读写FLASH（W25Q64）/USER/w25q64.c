#include "w25q64.h"
void W25Q64_SPI_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	SPI_InitTypeDef spi_init;
	spi_init.SPI_NSS = SPI_NSS_Soft;
	spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi_init.SPI_Mode = SPI_Mode_Master;
	spi_init.SPI_DataSize = SPI_DataSize_8b;
	spi_init.SPI_CPOL = SPI_CPOL_High;
	spi_init.SPI_CPHA = SPI_CPHA_2Edge;
	spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
	spi_init.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&spi_init);
	SPI_Cmd(SPI1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC,&gpio_init);
	
	gpio_init.GPIO_Pin = GPIO_Pin_5;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.GPIO_Pin = GPIO_Pin_6;
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&gpio_init);
	

	
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
}

uint8_t W25Q64_SendByte(uint8_t byte){
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1,byte);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

uint8_t W25Q64_RecieveByte(void){
	return W25Q64_SendByte(W25Q64_Dummy_Byte);
}

uint32_t W25Q64_ReadDeviceID(void){
	uint32_t t = 0;
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	W25Q64_SendByte(W25Q64_DeviceID);
	W25Q64_RecieveByte();
	W25Q64_RecieveByte();
	W25Q64_RecieveByte();
	t = W25Q64_RecieveByte();
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	return t;
}

uint32_t W25Q64_ReadID(void){
	uint32_t t1 = 0 , t2 = 0 , t3 = 0;
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	W25Q64_SendByte(W25Q64_JedecDeviceID);
	t1 = W25Q64_RecieveByte();
	t2 = W25Q64_RecieveByte();
	t3 = W25Q64_RecieveByte();
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	return (t1<<16) | (t2<<8) | t3;
}
void W25Q64_WriteEn(void){
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	W25Q64_SendByte(W25Q64_WriteEnable);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
}

void W25Q64_WaitForWriteEnd(void){
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	W25Q64_SendByte(W25Q64_ReadStatusReg);
	while((W25Q64_RecieveByte() & 0x01) == 1);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
}

void W25Q64_EraseSector(uint32_t SectorAddr){
	W25Q64_WriteEn();
	W25Q64_WaitForWriteEnd();
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	W25Q64_SendByte(W25Q64_SectorErase);
	W25Q64_SendByte((SectorAddr&0xff0000)>>16);
	W25Q64_SendByte((SectorAddr&0x00ff00)>>8);
	W25Q64_SendByte(SectorAddr&0x0000ff);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	W25Q64_WaitForWriteEnd();
}

void W25Q64_EraseAll(void){
	W25Q64_WriteEn();
	W25Q64_WaitForWriteEnd();
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	W25Q64_SendByte(W25Q64_ChipErase);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	W25Q64_WaitForWriteEnd();
}

void W25Q64_PageWrite(uint8_t* buffer, uint32_t w_addr, uint16_t w_size){
	if(w_size > 256) return;
	W25Q64_WriteEn();
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	
	W25Q64_SendByte(W25Q64_PageProgram);
	W25Q64_SendByte((w_addr&0xff0000)>>16);
	W25Q64_SendByte((w_addr&0x00ff00)>>8);
	W25Q64_SendByte(w_addr&0x0000ff);
	
	while(w_size--){
		W25Q64_SendByte(*buffer);
		buffer++;
	}
	
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	W25Q64_WaitForWriteEnd();
}

void W25Q64_BufferWrite(uint8_t* buffer, uint32_t w_addr, uint16_t w_size){
	uint8_t NumOfPage = 0 , NumOfSinge = 0 , Addr = 0 , count = 0 , temp = 0 ;
	
	Addr = w_addr % 256 ;
	count = 256 - Addr ;
	NumOfPage = w_size / 256 ;
	NumOfSinge = w_size % 256 ;
	
	if(Addr == 0){
		if(NumOfPage == 0) W25Q64_PageWrite(buffer,w_addr,w_size);
		else {
			while(NumOfPage--){
				W25Q64_PageWrite(buffer,w_addr,256);
				w_addr += 256;
				buffer += 256;
			}
			W25Q64_PageWrite(buffer,w_addr,NumOfSinge);
		}
	}
	else {
		if(NumOfPage == 0){
			if(NumOfSinge > count){
				temp = NumOfSinge - count ;
				W25Q64_PageWrite(buffer,w_addr,count);
				w_addr += count ;
				buffer += count ;
				W25Q64_PageWrite(buffer,w_addr,temp);
			}
			else {
				W25Q64_PageWrite(buffer,w_addr,w_size);
			}
		}
		else{
			w_size -= count ;
			NumOfPage = w_size / 256 ;
			NumOfSinge = w_size % 256 ;
			W25Q64_PageWrite(buffer,w_addr,count);
			w_addr += count ;
			buffer += count ;
			while(NumOfPage--){
				W25Q64_PageWrite(buffer,w_addr,256);
				w_addr += 256;
				buffer += 256;
			}
			if (NumOfSinge != 0) W25Q64_PageWrite(buffer,w_addr,NumOfSinge);
		}
	}
}

void W25Q64_BufferRead(uint8_t* buffer, uint32_t r_addr, uint16_t r_size){
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	
	W25Q64_SendByte(W25Q64_ReadData);
	W25Q64_SendByte((r_addr&0xff0000)>>16);
	W25Q64_SendByte((r_addr&0x00ff00)>>8);
	W25Q64_SendByte(r_addr&0x0000ff);
	
	while(r_size--){
		*buffer = W25Q64_SendByte(W25Q64_Dummy_Byte);
		buffer ++;
	}
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
}
