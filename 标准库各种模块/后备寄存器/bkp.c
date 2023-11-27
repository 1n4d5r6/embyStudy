#include "bkp.h"
uint8_t BKP_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	BKP_ClearFlag();
	if(BKP_ReadBackupRegister(BKP_DR1) == BKP_INIT) return 0;
	return 1;
}
