#ifndef _USART3_H
#define _USART3_H

#include "stm32f10x.h"

void USART3_init(void);
void usart3_send_data(u8 c);
void usart3_niming_send(u8 fun,u8*data,u8 len);
void usart3_send(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);
void USART3_IRQHandler(void);
void usart3_sendstring(u8 * pStr);
#endif

