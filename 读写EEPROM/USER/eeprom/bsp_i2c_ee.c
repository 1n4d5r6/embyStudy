/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   i2c EEPROM(AT24C02)Ӧ�ú���bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"
#include "psd_usart.h" 
#include "psd_soft_i2c.h"
/*
*********************************************************************************************************
*	�� �� ��: ee_CheckOk
*	����˵��: �жϴ���EERPOM�Ƿ�����
*	��    �Σ���
*	�� �� ֵ: 1 ��ʾ������ 0 ��ʾ������
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
		/* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
		//i2c_Stop();		
		Soft_I2C_Stop();
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ee_ReadBytes
*	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
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
*	�� �� ��: ee_WriteBytes
*	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
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
				goto cmd_fail;	/* EEPROM����д��ʱ */
			}
			Soft_I2C_SendByte((uint8_t)usAddr);
			if (Soft_I2C_RecieveAck() != 0)
			{
				goto cmd_fail;	/* EEPROM������Ӧ�� */
			}
		}
		Soft_I2C_SendByte(_pWriteBuf[i]);
		if (Soft_I2C_RecieveAck() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}
		usAddr++;	/* ��ַ��1 */		
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
	
	/* ��仺���� */
	for (i = 0; i < EEPROM_SIZE; i++)
	{
		buf[i] = 0xFF;
	}
	
	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
	if (ee_WriteBytes(buf, 0, EEPROM_SIZE) == 0)
	{
		printf("����eeprom����\r\n");
		return;
	}
	else
	{
		printf("����eeprom�ɹ���\r\n");
	}
}


/*--------------------------------------------------------------------------------------------------*/
static void ee_Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


/*
 * eeprom AT24C02 ��д����
 * ��������1���쳣����0
 */
uint8_t ee_Test(void) 
{
  uint16_t i;
	uint8_t write_buf[EEPROM_SIZE];
  uint8_t read_buf[EEPROM_SIZE]; 
  if (ee_CheckOk() == 0)
	{
		printf("û�м�⵽����EEPROM!\r\n");		
		return 0;
	}
	for (i = 0; i < EEPROM_SIZE; i++)
	{		
		write_buf[i] = (i * 11 ) % EEPROM_SIZE;
	} 
  if (ee_WriteBytes(write_buf, 0, EEPROM_SIZE) == 0)
	{
		printf("дeeprom����\r\n");
		return 0;
	}
	else
	{		
		printf("дeeprom�ɹ���\r\n");
	}
  ee_Delay(0x0FFFFF);
  if (ee_ReadBytes(read_buf, 0, EEPROM_SIZE) == 0)
	{
		printf("��eeprom����\r\n");
		return 0;
	}
	else
	{		
		printf("��eeprom�ɹ����������£�\r\n");
	}
  for (i = 0; i < EEPROM_SIZE; i++)
	{
//		if(read_buf[i] != write_buf[i])
//		{
//			printf("0x%02X ", read_buf[i]);
//			printf("����:EEPROM������д������ݲ�һ��");
//			return 0;
//		}
    printf(" %02X", read_buf[i]);
		
		if ((i & 15) == 15)
		{
			printf("\r\n");	
		}		
	}
  printf("eeprom��д���Գɹ�\r\n");
  return 1;
}
/*********************************************END OF FILE**********************/

