#include "psd_usart.h"
#include "math.h"
static void NVIC_Configuration(){
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Init(&NVIC_InitStruct);
}
void Usart_1_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//打开串口GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//打开串口时钟
	//配置RX,TX的GPIO口
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //将TX配置为浮空输入模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; //将RX配置为复用推挽输出模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置USART1
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
	NVIC_Configuration();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
}

void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch){
	USART_SendData(pUSARTx,ch);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

void Usart_SendString(USART_TypeDef *pUSARTx, char *ch){
	unsigned int k = 0;
	do{
		Usart_SendByte(pUSARTx,*(ch+k));
		k++;
	}while(*(ch+k)!='\0');
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch){
	uint8_t temp_h, temp_l;
	
	temp_h = (ch&0xFF00)>>8;
	temp_l = ch&0xff;
	Usart_SendByte(pUSARTx, temp_h);
	Usart_SendByte(pUSARTx, temp_l);
	
}

void Usart_SendWord(USART_TypeDef *pUSARTx, uint32_t ch){
	uint8_t temp_h, temp_l;
	temp_h = (ch&0xFFFF0000)>>16;
	temp_l = ch&0xffff;
	Usart_SendHalfWord(pUSARTx, temp_h);
	Usart_SendHalfWord(pUSARTx, temp_l);
	
}

void Usart_SendNum_Uint8(USART_TypeDef *pUSARTx, uint8_t num){
	if (num == 0){
		 Usart_SendByte(pUSARTx,'0');
		 return;
	}
	char dat[4];
	dat[3] = '\0';
	uint8_t i = 0;
	while(num>0){
		uint8_t n = num%10;
		if(n==0) dat[2-i] = '0';
		else if(n==1) dat[2-i] = '1';
		else if(n==2) dat[2-i] = '2';
		else if(n==3) dat[2-i] = '3';
		else if(n==4) dat[2-i] = '4';
		else if(n==5) dat[2-i] = '5';
		else if(n==6) dat[2-i] = '6';
		else if(n==7) dat[2-i] = '7';
		else if(n==8) dat[2-i] = '8';
		else if(n==9) dat[2-i] = '9';
		i++;
		num/=10;
	}
	uint8_t a = 3-i;
	Usart_SendString(pUSARTx,&dat[a]);
}
void Usart_SendNum_Uint16(USART_TypeDef *pUSARTx, uint16_t num){
	if (num == 0){
		 Usart_SendByte(pUSARTx,'0');
		 return;
	}
	char dat[6];
	dat[5] = '\0';
	uint8_t i = 0;
	while(num>0){
		uint8_t n = num%10;
		if(n==0) dat[4-i] = '0';
		else if(n==1) dat[4-i] = '1';
		else if(n==2) dat[4-i] = '2';
		else if(n==3) dat[4-i] = '3';
		else if(n==4) dat[4-i] = '4';
		else if(n==5) dat[4-i] = '5';
		else if(n==6) dat[4-i] = '6';
		else if(n==7) dat[4-i] = '7';
		else if(n==8) dat[4-i] = '8';
		else if(n==9) dat[4-i] = '9';
		i++;
		num/=10;
	}
	uint8_t a = 5-i;
	Usart_SendString(pUSARTx,&dat[a]);
}
void Usart_SendNum_Uint32(USART_TypeDef *pUSARTx, uint32_t num){
	if (num == 0){
		 Usart_SendByte(pUSARTx,'0');
		 return;
	}
	char dat[11];
	dat[10] = '\0';
	uint8_t i = 0;
	while(num>0){
		uint8_t n = num%10;
		if(n==0) dat[9-i] = '0';
		else if(n==1) dat[9-i] = '1';
		else if(n==2) dat[9-i] = '2';
		else if(n==3) dat[9-i] = '3';
		else if(n==4) dat[9-i] = '4';
		else if(n==5) dat[9-i] = '5';
		else if(n==6) dat[9-i] = '6';
		else if(n==7) dat[9-i] = '7';
		else if(n==8) dat[9-i] = '8';
		else if(n==9) dat[9-i] = '9';
		i++;
		num/=10;
	}
	uint8_t a = 10-i;
	Usart_SendString(pUSARTx,&dat[a]);
}
void Usart_SendNum_Double(USART_TypeDef *pUSARTx, double num){
	int32_t num1 = floor(num);
	int32_t num2 = floor((num-num1)*1000);
	Usart_SendNum_Int32(pUSARTx,num1);
	Usart_SendByte(pUSARTx,'.');
	Usart_SendNum_Int32(pUSARTx,num2);
}
void Usart_SendNum_Int8(USART_TypeDef *pUSARTx, int8_t num){
		if(num>=0) Usart_SendNum_Uint8(pUSARTx,num);
		else{
			if(num == -128){
				Usart_SendString(pUSARTx,"-128");
			}
			else{
				num = 0 - num;
				Usart_SendByte(pUSARTx,'-');
				Usart_SendNum_Uint8(pUSARTx,num);
			}
		}
}
void Usart_SendNum_Int16(USART_TypeDef *pUSARTx, int16_t num){
		if(num>=0) Usart_SendNum_Uint16(pUSARTx,num);
		else{
			if(num == -32768){
				Usart_SendString(pUSARTx,"-32768");
			}
			else{
				num = 0 - num;
				Usart_SendByte(pUSARTx,'-');
				Usart_SendNum_Uint16(pUSARTx,num);
			}
		}
}
void Usart_SendNum_Int32(USART_TypeDef *pUSARTx, int32_t num){
		if(num>=0) Usart_SendNum_Uint32(pUSARTx,num);
		else{
			if(num == -2147483648){
				Usart_SendString(pUSARTx,"-2147483648");
			}
			else{
				num = 0 - num;
				Usart_SendByte(pUSARTx,'-');
				Usart_SendNum_Uint32(pUSARTx,num);
			}
		}
}
void send(uint8_t data){
	USART_SendData(USART1,data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

uint8_t USART1_RecvBuff[128] = {'\0'};
uint8_t USART1_RecvLen = 0;
void USART1_IRQHandler(){
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
		USART1_RecvBuff[USART1_RecvLen++] = USART_ReceiveData(USART1);
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET){
		USART_ReceiveData(USART1);
		if(USART1_RecvLen!=0){
			Usart_SendString(USART1,(char*)USART1_RecvBuff);
			memset(USART1_RecvBuff,'\0',128);
			USART1_RecvLen =0;
		}
	}
}