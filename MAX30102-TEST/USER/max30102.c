/** \file max30102.cpp ******************************************************
*
* Project: MAXREFDES117#
* Filename: max30102.cpp
* Description: This module is an embedded controller driver for the MAX30102
*
* Revision History:
*\n 1-18-2016 Rev 01.00 GL Initial release.
*\n
*/

#include "max30102.h"
#include "psd_soft_i2c.h"

#define max30102_WR_address 0xAE
bool maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
/**
* \brief        Write a value to a MAX30102 register
* \par          Details
*               This function writes a value to a MAX30102 register
*
* \param[in]    uch_addr    - register address
* \param[in]    uch_data    - register data
*
* \retval       true on success
*/
{
    /* 第1步：发起I2C总线启动信号 */
    Soft_I2C_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    Soft_I2C_SendByte(max30102_WR_address | 0x00);	/* 此处是写指令 */

    /* 第3步：发送ACK */
    if (Soft_I2C_RecieveAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址 */
    Soft_I2C_SendByte(uch_addr);
    if (Soft_I2C_RecieveAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第5步：开始写入数据 */
    Soft_I2C_SendByte(uch_data);

    /* 第6步：发送ACK */
    if (Soft_I2C_RecieveAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 发送I2C总线停止信号 */
    Soft_I2C_Stop();
    return true;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    Soft_I2C_Stop();
    return false;
}

bool maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
/**
* \brief        Read a MAX30102 register
* \par          Details
*               This function reads a MAX30102 register
*
* \param[in]    uch_addr    - register address
* \param[out]   puch_data    - pointer that stores the register data
*
* \retval       true on success
*/
{
    /* 第1步：发起I2C总线启动信号 */
    Soft_I2C_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    Soft_I2C_SendByte(max30102_WR_address | 0x00);	/* 此处是写指令 */

    /* 第3步：发送ACK */
    if (Soft_I2C_RecieveAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址， */
    Soft_I2C_SendByte((uint8_t)uch_addr);
    if (Soft_I2C_RecieveAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }
    /* 第6步：重新启动I2C总线。下面开始读取数据 */
    Soft_I2C_Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    Soft_I2C_SendByte(max30102_WR_address | 0x01);	/* 此处是读指令 */

    /* 第8步：发送ACK */
    if (Soft_I2C_RecieveAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第9步：读取数据 */
    {
        *puch_data = Soft_I2C_RecieveByte();	/* 读1个字节 */

        Soft_I2C_SendNotACK();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
    }
    /* 发送I2C总线停止信号 */
    Soft_I2C_Stop();
    return true;	/* 执行成功 返回data值 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    Soft_I2C_Stop();
    return false;
}

bool maxim_max30102_init(void)
/**
* \brief        Initialize the MAX30102
* \par          Details
*               This function initializes the MAX30102
*
* \param        None
*
* \retval       true on success
*/
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PA5
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //PA5设置成浮空输入，接MAX30102的INT数据转换是否完成的信号引脚  
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA5
    if(!maxim_max30102_write_reg(REG_INTR_ENABLE_1, 0xe0)) // INTR setting
        return false;
    if(!maxim_max30102_write_reg(REG_INTR_ENABLE_2, 0x02))
        return false;
    if(!maxim_max30102_write_reg(REG_FIFO_WR_PTR, 0x00)) //FIFO_WR_PTR[4:0]
        return false;
    if(!maxim_max30102_write_reg(REG_OVF_COUNTER, 0x00)) //OVF_COUNTER[4:0]
        return false;
    if(!maxim_max30102_write_reg(REG_FIFO_RD_PTR, 0x00)) //FIFO_RD_PTR[4:0]
        return false;
    if(!maxim_max30102_write_reg(REG_FIFO_CONFIG, 0x0f)) //sample avg = 8, fifo rollover=false, fifo almost full = 17
        return false;
    if(!maxim_max30102_write_reg(REG_MODE_CONFIG, 0x03))  //0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
        return false;
    if(!maxim_max30102_write_reg(REG_SPO2_CONFIG, 0x26)) // SPO2_ADC range = 4096nA, SPO2 sample rate (400 Hz), LED pulseWidth (411uS)
        return false;

    if(!maxim_max30102_write_reg(REG_LED1_PA, 0x2f))  //Choose value for ~ 4.5mA for LED1
        return false;
    if(!maxim_max30102_write_reg(REG_LED2_PA, 0x2f))  // Choose value for ~ 4.5mA for LED2
        return false;
    if(!maxim_max30102_write_reg(REG_PILOT_PA, 0x7f))  // Choose value for ~ 25mA for Pilot LED
        return false;
    return true;
}

bool maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)

/**
* \brief        Read a set of samples from the MAX30102 FIFO register
* \par          Details
*               This function reads a set of samples from the MAX30102 FIFO register
*
* \param[out]   *pun_red_led   - pointer that stores the red LED reading data
* \param[out]   *pun_ir_led    - pointer that stores the IR LED reading data
*
* \retval       true on success
*/
{
    uint32_t un_temp;
    uint8_t uch_temp;
    *pun_ir_led = 0;
    *pun_red_led = 0;
    maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
    maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);



    /* 第1步：发起I2C总线启动信号 */
    Soft_I2C_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    Soft_I2C_SendByte(max30102_WR_address | 0x00);	/* 此处是写指令 */

    /* 第3步：发送ACK */
    if (Soft_I2C_RecieveAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址， */
    Soft_I2C_SendByte((uint8_t)REG_FIFO_DATA);
    if (Soft_I2C_RecieveAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }


    /* 第6步：重新启动I2C总线。下面开始读取数据 */
    Soft_I2C_Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    Soft_I2C_SendByte(max30102_WR_address | 0x01);	/* 此处是读指令 */

    /* 第8步：发送ACK */
    if (Soft_I2C_RecieveAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    un_temp = Soft_I2C_RecieveByte();
    Soft_I2C_SendACK();//Soft_I2C_SendACK
    un_temp <<= 16;
    *pun_red_led += un_temp;
    un_temp = Soft_I2C_RecieveByte();
    Soft_I2C_SendACK();
    un_temp <<= 8;
    *pun_red_led += un_temp;
    un_temp = Soft_I2C_RecieveByte();
    Soft_I2C_SendACK();
    *pun_red_led += un_temp;

    un_temp = Soft_I2C_RecieveByte();
    Soft_I2C_SendACK();
    un_temp <<= 16;
    *pun_ir_led += un_temp;
    un_temp = Soft_I2C_RecieveByte();
    Soft_I2C_SendACK();
    un_temp <<= 8;
    *pun_ir_led += un_temp;
    un_temp = Soft_I2C_RecieveByte();
    Soft_I2C_SendACK();
    *pun_ir_led += un_temp;
    *pun_red_led &= 0x03FFFF; //Mask MSB [23:18]
    *pun_ir_led &= 0x03FFFF; //Mask MSB [23:18]

    /* 发送I2C总线停止信号 */
    Soft_I2C_Stop();
    return true;
cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    Soft_I2C_Stop();
    return false;
}

bool maxim_max30102_reset()
/**
* \brief        Reset the MAX30102
* \par          Details
*               This function resets the MAX30102
*
* \param        None
*
* \retval       true on success
*/
{
    if(!maxim_max30102_write_reg(REG_MODE_CONFIG, 0x40))
        return false;
    else
        return true;
}


