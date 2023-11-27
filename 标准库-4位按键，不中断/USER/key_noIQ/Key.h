#ifndef __KEY_H__
#define __KEY_H__
#include "stm32f10x.h"
#define Key_1 1
#define Key_2 2
#define Key_3 3
#define Key_4 4
void Key_Config(void);
uint8_t KeyCheck(void);
#endif
