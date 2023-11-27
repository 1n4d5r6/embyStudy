#ifndef __BASETIM_H__
#define __BASETIM_H__
#include "stm32f10x.h"
void BASETIM_Config(void);
void BASETIM_Start(void);
void BASETIM_Stop(void);
void BASETIM_NVIC(void);
#endif
