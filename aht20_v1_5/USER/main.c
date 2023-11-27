#include "stdio.h"
#include "AHT20.h"
#include "stm32f10x.h"
#include "psd_usart.h"










int32_t main(void)
{
	
	uint32_t CT_data[2];
	volatile int  c1,t1;
	
	
	
	Init_I2C_Sensor_Port();//��ʼ��SDA��SCL��IO��
	

	
	Delay_1ms(500);
	Usart_1_Config();
	if((AHT20_Read_Status()&0x18)!=0x18)
	{
	AHT20_Start_Init(); //���³�ʼ���Ĵ�����һ�㲻��Ҫ�˳�ʼ����ֻ�е����ص�״̬�ֽڲ���ȷʱ�ų�ʼ��AHT20
	Delay_1ms(10);
	}

	
	while(1)
	{
	 AHT20_Read_CTdata(CT_data);  //��ȡ�¶Ⱥ�ʪ�� �� �ɼ��1.5S��һ��
	 c1 = CT_data[0]*1000/1024/1024;  //����õ�ʪ��ֵ���Ŵ���10��,���c1=523����ʾ����ʪ��Ϊ52.3%��
	 t1 = CT_data[1] *200*10/1024/1024-500;//����õ��¶�ֵ���Ŵ���10�������t1=245����ʾ�����¶�Ϊ24.5�棩
	 
		Usart_SendString(USART1,"�¶ȣ�");
		Usart_SendNum_Double(USART1,t1/10.0,1);
		Usart_SendString(USART1,"\t\n");
	 
	  Usart_SendString(USART1,"ʪ�ȣ�");
		Usart_SendNum_Double(USART1,c1/10.0,1);
		Usart_SendString(USART1,"\t\n");
		
		Usart_SendString(USART1,"\t\n");
	  Delay_1ms(1500); //��ʱ1.5S
	 
	 //Ϊ��ȡ�����ݸ��ȶ���������ʹ��ƽ��ֵ�˲����ߴ����˲�������ǰ���ȡ��ֵ������ֵ����̫��
	 }
	

 }	
























