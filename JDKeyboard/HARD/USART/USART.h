#ifndef _USART_H
#define _USART_H

#include "stm32f10x.h"
#include "string.h"
#include "WORK.h"

#define LCD_DEMO

void USART_init(void);
void usart1_send_data(u8 c);
void usart1_niming_send(u8 fun,u8*data,u8 len);
void USART1_IRQHandler(void);
void usart1_send(u8 * pBuf,u8 len);
void USART1_Receive_DataAnalysis(void);

typedef struct keyframe_t{
	u8 head;
	u8 cmd;
	u8 len;
	u8 data[10];
	u8 crc1;
	u8 crc2;
	u8 end;
}keyframe;
typedef union keydata_t{
	keyframe f;
	u8 buf[16];
} keydata;

#endif

