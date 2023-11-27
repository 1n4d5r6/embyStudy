#ifndef	__W25Q64_H__
#define	__W25Q64_H__
#define  sFLASH_ID              0XEF4017
#define W25Q64_WriteEnable		      0x06 
#define W25Q64_WriteDisable		      0x04 
#define W25Q64_ReadStatusReg		    0x05 
#define W25Q64_WriteStatusReg		    0x01 
#define W25Q64_ReadData			        0x03 
#define W25Q64_FastReadData		      0x0B 
#define W25Q64_FastReadDual		      0x3B 
#define W25Q64_PageProgram		      0x02 
#define W25Q64_BlockErase			      0xD8 
#define W25Q64_SectorErase		      0x20 
#define W25Q64_ChipErase			      0xC7 
#define W25Q64_PowerDown			      0xB9 
#define W25Q64_ReleasePowerDown	    0xAB 
#define W25Q64_DeviceID			        0xAB 
#define W25Q64_ManufactDeviceID   	0x90 
#define W25Q64_JedecDeviceID		    0x9F
#define W25Q64_WIP_Flag                  0x01
#define W25Q64_Dummy_Byte                0xFF
#include "stm32f10x.h"
void W25Q64_SPI_Init(void);
uint8_t W25Q64_SendByte(uint8_t byte);
uint8_t W25Q64_RecieveByte(void);
uint32_t W25Q64_ReadDeviceID(void);
uint32_t W25Q64_ReadID(void);
void W25Q64_WriteEn(void);
void W25Q64_WaitForWriteEnd(void);
void W25Q64_EraseSector(uint32_t SectorAddr);
void W25Q64_EraseAll(void);
void W25Q64_PageWrite(uint8_t* buffer, uint32_t w_addr, uint16_t w_size);
void W25Q64_BufferWrite(uint8_t* buffer, uint32_t w_addr, uint16_t w_size);
void W25Q64_BufferRead(uint8_t* buffer, uint32_t r_addr, uint16_t r_size);
#endif

