#include "stm32f10x.h"
#include "psd_soft_i2c.h"
#include "psd_usart.h"
void AHT20_SendAC(void) //向AHT20发送AC命令
{
	Soft_I2C_Start();
	Soft_I2C_SendByte(0x70);
	Soft_I2C_RecieveAck();
	Soft_I2C_SendByte(0xac);//0xAC采集命令
	Soft_I2C_RecieveAck();
	Soft_I2C_SendByte(0x33);
	Soft_I2C_RecieveAck();
	Soft_I2C_SendByte(0x00);
	Soft_I2C_RecieveAck();
	Soft_I2C_Stop();

}
void JH_Reset_REG(uint8_t addr)
{
	
	uint8_t Byte_first, Byte_second, Byte_third;
	Soft_I2C_Start();
	Soft_I2C_SendByte(0x70);//原来是0x70
	Soft_I2C_RecieveAck();
	Soft_I2C_SendByte(addr);
	Soft_I2C_RecieveAck();
	Soft_I2C_SendByte(0x00);
	Soft_I2C_RecieveAck();
	Soft_I2C_SendByte(0x00);
	Soft_I2C_RecieveAck();
	Soft_I2C_Stop();

	Soft_I2C_Delay_1ms(5);//延时5ms左右
	Soft_I2C_Start();
	Soft_I2C_SendByte(0x71);//
	Soft_I2C_RecieveAck();
	Byte_first = Soft_I2C_RecieveByte();
	Soft_I2C_SendACK();
	Byte_second = Soft_I2C_RecieveByte();
	Soft_I2C_SendNotACK();
	Byte_third = Soft_I2C_RecieveByte();
	Soft_I2C_SendNotACK();
	Soft_I2C_Stop();
	
  Soft_I2C_Delay_1ms(10);//延时10ms左右
	Soft_I2C_Start();
	Soft_I2C_SendByte(0x70);///
	Soft_I2C_RecieveAck();
	Soft_I2C_SendByte(0xB0|addr);////寄存器命令
	Soft_I2C_RecieveAck();
	Soft_I2C_SendByte(Byte_second);
	Soft_I2C_RecieveAck();
	Soft_I2C_SendByte(Byte_third);
	Soft_I2C_RecieveAck();
	Soft_I2C_Stop();
	
	Byte_second=0x00;
	Byte_third =0x00;
}

void AHT20_Start_Init(void)
{
	JH_Reset_REG(0x1b);
	JH_Reset_REG(0x1c);
	JH_Reset_REG(0x1e);
}

uint8_t AHT20_Read_Status(void)//读取AHT20的状态寄存器
{

	uint8_t Byte_first;	
	Soft_I2C_Start();
	Soft_I2C_SendByte(0x71);
	Soft_I2C_RecieveAck();
	Byte_first = Soft_I2C_RecieveByte();
	Soft_I2C_SendNotACK();
	Soft_I2C_Stop();
	return Byte_first;
}

void AHT20_Read_CTdata(uint32_t *ct) //没有CRC校验，直接读取AHT20的温度和湿度数据
{
	volatile uint8_t  Byte_1th=0;
	volatile uint8_t  Byte_2th=0;
	volatile uint8_t  Byte_3th=0;
	volatile uint8_t  Byte_4th=0;
	volatile uint8_t  Byte_5th=0;
	volatile uint8_t  Byte_6th=0;
	uint32_t RetuData = 0;
	uint16_t cnt = 0;
	AHT20_SendAC();//向AHT20发送AC命令
	Soft_I2C_Delay_1ms(80);//延时80ms左右	
    cnt = 0;
	while(((AHT20_Read_Status()&0x80)==0x80))//直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
	{
		Soft_I2C_SensorDelay_us(1508);
		if(cnt++>=100)
		{
		 break;
		 }
	}
	Soft_I2C_Start();
	Soft_I2C_SendByte(0x71);
	Soft_I2C_RecieveAck();
	Byte_1th = Soft_I2C_RecieveByte();//状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
	Soft_I2C_SendACK();
	Byte_2th = Soft_I2C_RecieveByte();//湿度
	Soft_I2C_SendACK();
	Byte_3th = Soft_I2C_RecieveByte();//湿度
	Soft_I2C_SendACK();
	Byte_4th = Soft_I2C_RecieveByte();//湿度/温度
	Soft_I2C_SendACK();
	Byte_5th = Soft_I2C_RecieveByte();//温度
	Soft_I2C_SendACK();
	Byte_6th = Soft_I2C_RecieveByte();//温度
	Soft_I2C_SendNotACK();
	Soft_I2C_Stop();

	RetuData = (RetuData|Byte_2th)<<8;
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//湿度
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0xfffff;
	ct[1] =RetuData; //温度
}
int main(){
	Usart_1_Config();
	Soft_I2C_Init();
	uint32_t CT_data[2];
	Soft_I2C_Delay_1ms(500);
	Usart_SendString(USART1,"开始检测！\n");
	if((AHT20_Read_Status()&0x18)!=0x18)
	{
	AHT20_Start_Init(); //重新初始化寄存器
	Soft_I2C_Delay_1ms(10);
	}
	while(1){
		double wd[10];
		double sd[10];
		volatile double c1,t1;
		for(uint16_t i = 0 ; i < 10 ; i++){
			AHT20_Read_CTdata(CT_data);
			sd[i] = CT_data[0]*100*10/1024/1024;  //计算得到湿度值c1（放大了10倍）
			wd[i] = CT_data[1]*200*10/1024/1024-500;//计算得到温度值t1（放大了10倍）
			Soft_I2C_Delay_1ms(1000);
		}
		c1 = (sd[0]+sd[1]+sd[2]+sd[3]+sd[4]+sd[5]+sd[6]+sd[7]+sd[8]+sd[9])/10.0;
		t1 = (wd[0]+wd[1]+wd[2]+wd[3]+wd[4]+wd[5]+wd[6]+wd[7]+wd[8]+wd[9])/10.0;
	////下一步客户处理显示数据，
		Usart_SendByte(USART1,'x');
		Usart_SendByte(USART1,' ');
		Usart_SendNum_Double(USART1,t1/10.0,1);
		Usart_SendByte(USART1,' ');
		Usart_SendNum_Double(USART1,c1/10.0,1);
		Usart_SendByte(USART1,' ');
		Usart_SendByte(USART1,'x');
		Soft_I2C_Delay_1ms(1000);
	}
}
