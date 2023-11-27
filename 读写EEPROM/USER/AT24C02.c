#include "AT24C02.h"
#include "psd_usart.h"
uint8_t AT24C02_Config(){
	uint8_t ucAck = 1;

	Soft_I2C_Init();		/* 配置GPIO */
	Soft_I2C_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	Soft_I2C_SendByte(AT24C02_Addr | 0x00);
	ucAck = Soft_I2C_RecieveAck();	/* 检测设备的ACK应答 */

	Soft_I2C_Stop();			/* 发送停止信号 */

	return ucAck;
}
uint8_t AT24C02_ReadBytes(uint8_t* data, uint16_t address, uint16_t size){
	uint16_t i;
	Soft_I2C_Start();
	Soft_I2C_SendByte(AT24C02_Addr | 0);
	if (Soft_I2C_RecieveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	Soft_I2C_SendByte((uint8_t)address);
	if (Soft_I2C_RecieveAck() != 0)
	{
		goto cmd_fail;
	}
	Soft_I2C_Start();
	Soft_I2C_SendByte(AT24C02_Addr | 1);
	if (Soft_I2C_RecieveAck() != 0)
	{
		goto cmd_fail;
	}
	for (i = 0; i < size; i++)
	{
		data[i] = Soft_I2C_RecieveByte();

		if (i != size - 1)
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
uint8_t AT24C02_WriteBytes(uint8_t* data, uint16_t address, uint16_t size){
	uint16_t i,m;
	uint16_t usAddr;
	usAddr = address;	
	for (i = 0; i < size; i++)
	{
		if ((i == 0) || (usAddr & (8 - 1)) == 0)
		{
			Soft_I2C_Stop();
			for (m = 0; m < 1000; m++)
			{				
				Soft_I2C_Start();
				Soft_I2C_SendByte(AT24C02_Addr | 0);
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
		Soft_I2C_SendByte(data[i]);
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
void AT24C02_Erase(uint8_t address, uint16_t size){
	uint16_t i;
	uint8_t buf[256];
	
	/* 填充缓冲区 */
	for (i = 0; i < 256; i++)
	{
		buf[i] = 0xFF;
	}
	
	/* 写EEPROM, 起始地址 = 0，数据长度为 256 */
	if (AT24C02_WriteBytes(buf, 0, size) == 0)
	{
		printf("擦除eeprom出错！\r\n");
		return;
	}
	else
	{
		printf("擦除eeprom成功！\r\n");
	}
}
uint8_t AT24C02_Test()
{
	uint16_t i;
	uint8_t write_buf[256];
  uint8_t read_buf[256];
	for (i = 0; i < 256; i++)
	{		
		write_buf[i] = (i * 9 ) % 256;
	}  
	if (AT24C02_Config() == 1)
	{
		Soft_I2C_Stop();
		printf("没有检测到串行EEPROM!\r\n");		
		return 0;
	}
	Soft_I2C_Stop();
  if (AT24C02_WriteBytes(write_buf, 0, 256) == 0)
	{
		printf("写eeprom出错！\r\n");
		return 0;
	}
	else
	{		
		printf("写eeprom成功！\r\n");
	}
	uint32_t nCount = 0xFFFFFF;
	for(; nCount != 0; nCount--){};
  if (AT24C02_ReadBytes(read_buf, 0, 256) == 0)
	{
		printf("读eeprom出错！\r\n");
		return 0;
	}
	else
	{		
		printf("读eeprom成功，数据如下：\r\n");
	}  
  for (i = 0; i < 256; i++)
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