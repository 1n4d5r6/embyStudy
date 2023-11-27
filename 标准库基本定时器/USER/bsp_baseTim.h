#ifndef __BASP_BASETIM_H__
#define __BASP_BASETIM_H__
#include "stm32f10x.h" 
#include "bsp_led.h"
void BaseTim_NVIC_Init(void);
void BaseTim_Tim_Init(void);
void BaseTim_Tim_Start(void);
#endif
