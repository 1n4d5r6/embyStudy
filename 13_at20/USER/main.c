#include "stm32f10x.h" 
#include "AHT20-21_DEMO_V1_3.h" 
#define SDA_IN() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}
#define IIC_SCL PBout(6) //SCL
#define IIC_SDA PBout(7) //SDA
#define READ_SDA PBin(7)
int main(){
	while(1){
		
	}
}

