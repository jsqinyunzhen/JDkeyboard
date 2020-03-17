
#ifndef _ST_PRINTF_H_
#define _ST_PRINTF_H_

#include "stm32f10x.h"



void gpio2uart1_pin_config(void);
void gpio2uart2_pin_config(void);

void gpio2uart3_pin_config(void);
void gpio2uart4_pin_config(void);
void gpio2uart5_pin_config(void);//RS485

void gpio2uart3_pin_config_gpio(uint8_t on_off);
void gpio2uart2_txpin_config_pc4g_debug(uint8_t on_off);

void USART_Config(USART_TypeDef* USARTx);
int sendchar(char ch);
int uart1_sendchar(char ch);

void uart1_sendstring( char *pt,int len)  ;

void uart_printf( char *fmt,...); 
void Get_ChipInfo(void);	
void printf_test(void);

void DMA_Enable(DMA_Channel_TypeDef *DMA_Streamx,u16 ndtr);


void handle_uart_debug_cmd(void);



#endif

