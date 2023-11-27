#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stm32f10x.h"
void Led_Green_Config();
void Led_Red_Config();
void Led_Blue_Config();
void Led_OpenGreen();
void Led_OpenBlue();
void Led_OpenRed();
void Led_CloseGreen();
void Led_CloseBlue();
void Led_CloseRed();
void Led_Init();
#endif