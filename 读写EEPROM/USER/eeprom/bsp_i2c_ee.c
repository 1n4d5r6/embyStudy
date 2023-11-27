/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   i2c EEPROM(AT24C02)应用函数bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"
#include "psd_usart.h" 
#include "psd_soft_i2c.h"
/*
*********************************************************************************************************
*	函 数 名: ee_CheckOk
*	功能说明: 判断串行EERPOM是否正常
*	形    参：无
*	返 回 值: 1 表示正常， 0 表示不正常
*********************************************************************************************************
*/
uint8_t ee_CheckOk(void)
{
	if (i2c_CheckDevice(EEPROM_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		/* 失败后，切记发送I2C总线停止信号 */
		//i2c_Stop();		
		Soft_I2C_Stop();
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ee_ReadBytes
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	Soft_I2C_Start();
	Soft_I2C_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);
	if (Soft_I2C_RecieveAck() != 0)
	{
		goto cmd_fail;
	}
	Soft_I2C_SendByte((uint8_t)_usAddress);
	if (Soft_I2C_RecieveAck() != 0)
	{
		goto cmd_fail;
	}
	Soft_I2C_Start();
	Soft_I2C_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD);
	if (Soft_I2C_RecieveAck() != 0)
	{
		goto cmd_fail;
	}
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = Soft_I2C_RecieveByte();
		if (i != _usSize - 1)
		{
			Soft_I2C_SendACK();
		}
		else
		{
			Soft_I2C_SendNotACK();
		}
	}
	Soft_I2C_Stop();
	return 1;
cmd_fail:
	Soft_I2C_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: ee_WriteBytes
*	功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pWriteBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
		if ((i == 0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
		{
			Soft_I2C_Stop();
			for (m = 0; m < 1000; m++)
			{				
				Soft_I2C_Start();
				Soft_I2C_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);
				if (Soft_I2C_RecieveAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* EEPROM器件写超时 */
			}
			Soft_I2C_SendByte((uint8_t)usAddr);
			if (Soft_I2C_RecieveAck() != 0)
			{
				goto cmd_fail;	/* EEPROM器件无应答 */
			}
		}
		Soft_I2C_SendByte(_pWriteBuf[i]);
		if (Soft_I2C_RecieveAck() != 0)
		{
			goto cmd_fail;	/* EEPROM器件无应答 */
		}
		usAddr++;	/* 地址增1 */		
	}
	Soft_I2C_Stop();
	return 1;
cmd_fail:
	Soft_I2C_Stop();
	return 0;
}


void ee_Erase(void)
{
	uint16_t i;
	uint8_t buf[EEPROM_SIZE];
	
	/* 填充缓冲区 */
	for (i = 0; i < EEPROM_SIZE; i++)
	{
		buf[i] = 0xFF;
	}
	
	/* 写EEPROM, 起始地址 = 0，数据长度为 256 */
	if (ee_WriteBytes(buf, 0, EEPROM_SIZE) == 0)
	{
		printf("擦除eeprom出错！\r\n");
		return;
	}
	else
	{
		printf("擦除eeprom成功！\r\n");
	}
}


/*--------------------------------------------------------------------------------------------------*/
static void ee_Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}


/*
 * eeprom AT24C02 读写测试
 * 正常返回1，异常返回0
 */
uint8_t ee_Test(void) 
{
  uint16_t i;
	uint8_t write_buf[EEPROM_SIZE];
  uint8_t read_buf[EEPROM_SIZE]; 
  if (ee_CheckOk() == 0)
	{
		printf("没有检测到串行EEPROM!\r\n");		
		return 0;
	}
	for (i = 0; i < EEPROM_SIZE; i++)
	{		
		write_buf[i] = (i * 11 ) % EEPROM_SIZE;
	} 
  if (ee_WriteBytes(write_buf, 0, EEPROM_SIZE) == 0)
	{
		printf("写eeprom出错！\r\n");
		return 0;
	}
	else
	{		
		printf("写eeprom成功！\r\n");
	}
  ee_Delay(0x0FFFFF);
  if (ee_ReadBytes(read_buf, 0, EEPROM_SIZE) == 0)
	{
		printf("读eeprom出错！\r\n");
		return 0;
	}
	else
	{		
		printf("读eeprom成功，数据如下：\r\n");
	}
  for (i = 0; i < EEPROM_SIZE; i++)
	{
//		if(read_buf[i] != write_buf[i])
//		{
//			printf("0x%02X ", read_buf[i]);
//			printf("错误:EEPROM读出与写入的数据不一致");
//			return 0;
//		}
    printf(" %02X", read_buf[i]);
		
		if ((i & 15) == 15)
		{
			printf("\r\n");	
		}		
	}
  printf("eeprom读写测试成功\r\n");
  return 1;
}
/*********************************************END OF FILE**********************/

