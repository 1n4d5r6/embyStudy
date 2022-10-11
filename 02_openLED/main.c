#include "stm32f10x.h"

int main(){
	//todo1.开启外设时钟
  	RCC_APB2ENR |= (1<<3);
	//todo2.将B端口的低配置寄存器最低位置1，其他位全0
	//这表示B组端口0普通推挽输出，以及输出速度10MHz
	GPIOB_CRL = (0x01<<4*5);
	//todo3.B组输出寄存器，0号输出低电平
	GPIOB_ODR &= ~(0x01<<5);
	while(1);
}