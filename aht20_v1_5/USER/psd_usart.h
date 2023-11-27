#ifndef __PSD_USART_H__
#define __PSD_USART_H__
#include "stm32f10x.h"
void Usart_1_Config();
void send(uint8_t data);
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendString(USART_TypeDef *pUSARTx, char *ch);
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
void Usart_SendWord(USART_TypeDef *pUSARTx, uint32_t ch);
void Usart_SendNum_Uint8(USART_TypeDef *pUSARTx, uint8_t num);
void Usart_SendNum_Uint16(USART_TypeDef *pUSARTx, uint16_t num);
void Usart_SendNum_Uint32(USART_TypeDef *pUSARTx, uint32_t num);
void Usart_SendNum_Int8(USART_TypeDef *pUSARTx, int8_t num);
void Usart_SendNum_Int16(USART_TypeDef *pUSARTx, int16_t num);
void Usart_SendNum_Int32(USART_TypeDef *pUSARTx, int32_t num);
void Usart_SendNum_Double(USART_TypeDef *pUSARTx, double num, uint8_t num2);
#endif