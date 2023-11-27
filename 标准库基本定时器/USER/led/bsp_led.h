#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stm32f10x.h"
void Led_Green_Config(void);
void Led_Red_Config(void);
void Led_Blue_Config(void);
void Led_OpenGreen(void);
void Led_OpenBlue(void);
void Led_OpenRed(void);
void Led_CloseGreen(void);
void Led_CloseBlue(void);
void Led_CloseRed(void);
void Led_Init(void);
#endif
