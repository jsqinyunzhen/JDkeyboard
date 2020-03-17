/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.h 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

unsigned int get_curtime(void);
unsigned int get_curtime2(void);

void Delay1ms(__IO uint16_t nTime);
void Delay_us(__IO uint32_t nUsTim);

extern uint8_t RxBuffer1[];
extern __IO uint8_t RxCounter1; 

extern uint8_t RxBuffer2[];
extern __IO uint8_t RxCounter2; 

extern uint8_t RxBuffer3[];
extern __IO uint16_t RxCounter3; 

extern uint8_t RxBuffer4[];
extern __IO uint16_t RxCounter4; 

//extern uint8_t RxBuffer2_proc[];
//extern __IO uint8_t RxCounter2_proc; 

extern uint8_t RxBuffer5[];
extern __IO uint8_t RxCounter5; 

extern uint8_t TxBuffer1[];
extern __IO uint16_t TxCounter1; 


extern uint8_t TxBuffer2[];
extern __IO uint16_t TxCounter2; 

extern uint8_t TxBuffer3[];
extern __IO uint16_t TxCounter3; 
extern uint8_t TxBuffer4[];
extern __IO uint16_t TxCounter4; 


extern uint8_t TxBuffer5[];
extern __IO uint8_t TxCounter5 ;


extern uint16_t CurrDataCounterEnd_dma1_4 ;
extern uint16_t CurrDataCounterEnd_dma1_7 ;
extern uint16_t CurrDataCounterEnd_dma1_2 ;//UART3
extern uint16_t CurrDataCounterEnd_dma2_5;//UART4

void handle_uart_debug_cmd(void);
void reset_uart_debug_buffer(void);
void reset_uart1_rx_buffer(void);
void reset_uart2_rx_buffer(void);
void reset_uart3_rx_buffer(void);
void reset_uart4_rx_buffer(void);
void reset_uart5_rx_buffer(void);

void reset_uart1_tx_buffer(void);
void reset_uart2_tx_buffer(void);
void reset_uart3_tx_buffer(void);
void reset_uart4_tx_buffer(void);

void reset_uart5_tx_buffer(void);

uint8_t get_smoke_happen(void);
void set_smoke_happen(void);
void clear_smoke_happen(void);

uint8_t get_dhh_happen(void);
void set_dhh_happen(void);
void clear_dhh_happen(void);
void mcu_uart5_control_init(void);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
