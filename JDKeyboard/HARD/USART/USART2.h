#ifndef _USART2_H
#define _USART2_H

#include "stm32f10x.h"

void USART2_init(void);
void usart2_send_data(u8 c);
void usart2_niming_send(u8 fun,u8*data,u8 len);
void USART2_IRQHandler(void);
void usart2_send(u8 * pStr,u8 len);
void USART2_Receive_DataAnalysis(void);

#endif

