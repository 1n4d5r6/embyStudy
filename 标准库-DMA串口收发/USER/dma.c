#include "dma.h"
uint8_t send_data[10];
void DMA_Config(void){
	DMA_InitTypeDef dma_init;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	dma_init.DMA_PeripheralBaseAddr = (USART1_BASE+0x04);
	dma_init.DMA_MemoryBaseAddr = (uint32_t)send_data;
	dma_init.DMA_DIR = DMA_DIR_PeripheralDST;
	dma_init.DMA_BufferSize = 10;
	dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma_init.DMA_Mode = DMA_Mode_Circular;
	dma_init.DMA_Priority = DMA_Priority_High ; 
	dma_init.DMA_M2M = DMA_M2M_Disable ;
	DMA_Init(DMA1_Channel4,&dma_init);
	DMA_Cmd(DMA1_Channel4,ENABLE);
	printf("%s\n","DMAÆô¶¯");
//	
//	dma_init.DMA_DIR = DMA_DIR_PeripheralSRC;
//	DMA_Init(DMA1_Channel5,&dma_init);
//	DMA_Cmd(DMA1_Channel5,ENABLE);
}
