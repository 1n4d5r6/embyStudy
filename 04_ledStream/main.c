#include "stm32f10x.h"

void openRed(){
	//将对应位置0，其他位不变
	GPIOB_ODR &= ~(0x01<<5);	
}
void openGreen(){
	GPIOB_ODR &= ~(0x01<<0);
}
void openBlue(){
	GPIOB_ODR &= ~(0x01<<1);
}
void closeAll(){
	//关灯，将所有引脚输出高电平
	GPIOB_ODR = 0xffff;
}
void  Delay_wxc( volatile  unsigned  int  t)
{
     unsigned  int  i;
     while(t--)
         for (i=0;i<800;i++);
}
int main(){
	while(1){
		int i = 0;
		//todo1.开启外设时钟
  	RCC_APB2ENR |= (1<<3);
		//todo2.配置相应GPIO工作模式
		GPIOB_CRL = 0;
		GPIOB_CRL |= (0x01<<4*5);
		GPIOB_CRL |= (0x01<<4*0);
		GPIOB_CRL |= (0x01<<4*1);
		//todo3.执行流水灯
		GPIOB_ODR = 0xffff;
		for (i = 0; i < 7 ; i++){
			if (i == 0) {openRed();Delay_wxc(8000);}
			else if (i == 1) {openGreen();Delay_wxc(8000);}
			else if (i == 2) {openBlue();Delay_wxc(8000);}
			else if (i == 3) {openRed();openGreen();Delay_wxc(8000);}
			else if (i == 4) {openGreen();openBlue();Delay_wxc(8000);}
			else if (i == 5) {openRed();openBlue();Delay_wxc(8000);}
			else if (i == 6) {openRed();openGreen();openBlue();Delay_wxc(8000);}
			closeAll();
		}
	}
}