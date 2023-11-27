#include "stm32f10x.h"
#include "psd_usart.h"
#include "bkp.h"
int main(void){
	Usart_1_Config();
	printf("��ʼ\t\n");
	uint8_t res;
	uint8_t time;
	res = BKP_Init();
	if (res) {
		time = BKP_ReadBackupRegister(BKP_DR2);
		time ++ ;
		printf("��%d�θ�λ\t\n",time);
		BKP_WriteBackupRegister(BKP_DR2,time);
	}
	else {
		printf("��1�θ�λ\t\n");
		BKP_WriteBackupRegister(BKP_DR2,1);
	}
	while(1);
}
