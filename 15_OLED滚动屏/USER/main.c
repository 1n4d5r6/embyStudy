#include "stm32f10x.h" 
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "gui.h"
void GUI_ShowCHinese_2(int x,u8 y,u8 hsize,u8 *str,u8 mode)
{ 
	while(*str!='\0')
	{
		if(x>WIDTH-hsize)
		{
			break;
		}
		if(x<0){
			str+=2;
			x+=hsize;
		}
		else{
			if(hsize == 16)
		{
			GUI_ShowFont16(x,y,str,mode);
		}
		else if(hsize == 24)
		{
			GUI_ShowFont24(x,y,str,mode);
		}
		else if(hsize == 32)
		{
			GUI_ShowFont32(x,y,str,mode);
		}
		else
		{
			return;
		}
		x+=hsize;
		str+=2;
		}
		
	}			
}
int32_t main(void)
{
	delay_init();	    	       //��ʱ������ʼ��	  
	NVIC_Configuration(); 	   //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	
	OLED_Init();			         //��ʼ��OLED  
	OLED_Clear(0);             //������ȫ�ڣ�
	OLED_WR_Byte(0x2E,OLED_CMD);        //�رչ���
	    OLED_WR_Byte(0x26,OLED_CMD);        //ˮƽ��������ҹ��� 26/27
    OLED_WR_Byte(0x00,OLED_CMD);        //�����ֽ�
		OLED_WR_Byte(0x00,OLED_CMD);        //��ʼҳ 0
		OLED_WR_Byte(0x07,OLED_CMD);        //����ʱ����
		OLED_WR_Byte(0x07,OLED_CMD);        //��ֹҳ 7
		OLED_WR_Byte(0x00,OLED_CMD);        //�����ֽ�
		OLED_WR_Byte(0xFF,OLED_CMD);        //�����ֽ�
	GUI_ShowCHinese(0,20,16,"�Ž�����£�����Ц̸��",1);
			OLED_WR_Byte(0x2F,OLED_CMD);        //��������
	while(1)
	{
		

		
		//delay_ms(1500);		
		

		delay_ms(1500); 
//		for(int i=-176;i<128;i+=4){
//			GUI_ShowCHinese(i,0,16,"�Ž�����£�����Ц̸��",1);
//			//GUI_ShowString(i,16,"abcdefghijklmnop",16,1);
//			//GUI_ShowString(0,24,"      ",16,1);
//			delay_ms(10);
//			OLED_Clear(0);
//			delay_ms(10);
//		}
		 
		 
	 }


}