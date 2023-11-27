1、创建好HAL库项目后，将除了User之外的所有文件夹复制到项目中。
2、创建除了User之外的其他文件组
3、添加各自的文件到文件组
4、添加文件夹路径到头文件路径
5、HAL库修改启动文件的两处PendSV_Handler，SysTick_Handler为OS_CPU_PendSVHandler，OS_CPU_SysTickHandler
      app.cfg.h将APP_CFG_SERIAL_EN设置为DISABLED #define APP_TRACE BSP_Ser_Printf改为#define APP_TRACE (void)
      includes.h添加#include "gpio.h" #include "app_cfg.h" #include "app.h" ，并将#include <stm32f10x_lib.h>改为#include "stm32f1xx_hal.h"

// bsp.c
#include "includes.h"

#define  DWT_CR      *(CPU_REG32 *)0xE0001000
#define  DWT_CYCCNT  *(CPU_REG32 *)0xE0001004
#define  DEM_CR      *(CPU_REG32 *)0xE000EDFC
#define  DBGMCU_CR   *(CPU_REG32 *)0xE0042004

#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    return HAL_RCC_GetHCLKFreq();
}

void BSP_Tick_Init(void)
{
	CPU_INT32U cpu_clk_freq;
	CPU_INT32U cnts;
	cpu_clk_freq = BSP_CPU_ClkFreq();
	
	#if(OS_VERSION>=3000u)
		cnts = cpu_clk_freq/(CPU_INT32U)OSCfg_TickRate_Hz;
	#else
		cnts = cpu_clk_freq/(CPU_INT32U)OS_TICKS_PER_SEC;
	#endif
	OS_CPU_SysTickInit(cnts);
}



void BSP_Init(void)
{
	BSP_Tick_Init();
	MX_GPIO_Init();
}


#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  cpu_clk_freq_hz;


    DEM_CR         |= (CPU_INT32U)DEM_CR_TRCENA;                /* Enable Cortex-M3's DWT CYCCNT reg.                   */
    DWT_CYCCNT      = (CPU_INT32U)0u;
    DWT_CR         |= (CPU_INT32U)DWT_CR_CYCCNTENA;

    cpu_clk_freq_hz = BSP_CPU_ClkFreq();
    CPU_TS_TmrFreqSet(cpu_clk_freq_hz);
}
#endif


#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return ((CPU_TS_TMR)DWT_CYCCNT);
}
#endif


#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
	CPU_INT64U  ts_us;
  CPU_INT64U  fclk_freq;

 
  fclk_freq = BSP_CPU_ClkFreq();
  ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

  return (ts_us);
}
#endif
 
 
#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
	CPU_INT64U  ts_us;
	CPU_INT64U  fclk_freq;


  fclk_freq = BSP_CPU_ClkFreq();
  ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);
	
  return (ts_us);
}
#endif





// bsp.h
#ifndef  __BSP_H__
#define  __BSP_H__

#include "stm32f1xx_hal.h"

void BSP_Init(void);

#endif
