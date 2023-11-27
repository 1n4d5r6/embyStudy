#ifndef __AT24C02_H__
#define __AT24C02_H__
#include "psd_soft_i2c.h"
#include "stm32f10x.h"
#define AT24C02_Addr 0xA0
uint8_t AT24C02_Config();
uint8_t AT24C02_ReadBytes(uint8_t* data, uint16_t address, uint16_t size);
uint8_t AT24C02_WriteBytes(uint8_t* data, uint16_t address, uint16_t size);
void AT24C02_Erase(uint8_t address, uint16_t size);
uint8_t AT24C02_Test();
#endif