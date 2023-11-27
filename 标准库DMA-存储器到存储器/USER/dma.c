#include "dma.h"
const uint32_t DMA_Data1[32] = {
	0,1,2,3,4,5,6,7,
	15,14,13,12,11,10,9,8,
	16,17,18,19,20,21,22,23,
	31,30,29,28,27,26,25,24
};
uint32_t DMA_Data2[32];
void DMA_Config(void){
	DMA_InitTypeDef dma_init;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	dma_init.DMA_PeripheralBaseAddr = (uint32_t)DMA_Data1;
	
	dma_init.DMA_MemoryBaseAddr = (uint32_t)DMA_Data2;
	
	dma_init.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	dma_init.DMA_BufferSize = 32;
	
	dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	
	dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	
	dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	
	dma_init.DMA_Mode = DMA_Mode_Normal;
	
	dma_init.DMA_Priority = DMA_Priority_High ; 
	
	dma_init.DMA_M2M = DMA_M2M_Enable ;
	
	DMA_Init(DMA1_Channel6,&dma_init);
	
	DMA_ClearFlag(DMA1_FLAG_TC6);
	DMA_Cmd(DMA1_Channel6,ENABLE);
}
uint8_t DMA_Test(void){
	while(DMA_GetFlagStatus(DMA1_FLAG_TC6) == RESET);
	for(uint8_t i = 0 ; i < 32 ; i++){
		if(DMA_Data1[i] != DMA_Data2[i]) return 0;
	}
	return 1;
}
