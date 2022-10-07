#include "stm32f10x.h"                  // Device header

void main(){
  RCC_APB2ENR |= (1<<3);
	GPIOB_CRL &= ~1;
	GPIOB_ODR &= ~1;
	while(1);
}