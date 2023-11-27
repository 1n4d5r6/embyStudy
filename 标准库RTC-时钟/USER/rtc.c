#include "rtc.h"
//uint8_t RTC_Clock_Flag = 0;
void RTC_Clock_Init(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	
	if(BKP_ReadBackupRegister(BKP_DR1) != RTC_CLOCK_INIT){
		printf("第一次上电\t\n");
		BKP_DeInit();
		
		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);
		/* Wait till LSE is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{}

		/* Select LSE as RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Set RTC prescaler: set RTC period to 1sec */
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		RTC_SetCounter(86350);
		RTC_WaitForLastTask();

		BKP_WriteBackupRegister(BKP_DR1, RTC_CLOCK_INIT);
	}
	else{
		printf("不是第一次\t\n");
		RTC_WaitForSynchro();
		RTC_ITConfig(RTC_IT_SEC,ENABLE);
		RTC_WaitForLastTask();
	}
	RCC_ClearFlag();
	printf("RTC初始化结束\t\n");
}
void RTC_IRQHandler(void)
{
   if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
   {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);
 
    /* Enable time update */
    //RTC_Clock_Flag = 1;
 
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    
		uint32_t THH = 0, TMM = 0, TSS = 0;
		/* Reset RTC Counter when Time is 23:59:59 */
		if (RTC_GetCounter() == 0x00015180)
		{
				RTC_SetCounter(0x0);
				/* Wait until last write operation on RTC registers has finished */
				RTC_WaitForLastTask();
		}
			
		uint32_t TimeVar = RTC_GetCounter();
		
		/* Compute  hours */
		THH = TimeVar / 3600;
		/* Compute minutes */
		TMM = (TimeVar % 3600) / 60;
		/* Compute seconds */
		TSS = (TimeVar % 3600) % 60;
		
		printf("Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);		 
  }
}
