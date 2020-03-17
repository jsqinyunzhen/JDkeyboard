/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "project_config.h"
//#include "st_gpio.h"
#include "st_printf.h"
#include "st_rtc.h"
//#include "st_audio_product.h"
//#include "mod_time_list.h"
#include "clock_calendar.h"

#include "rtx_os.h"
#include "st_audio_product.h"
#if (USE_SOFTTIMER_DIY ==1)
#include "mod_date_time.h"
#include "mod_time_list.h"
#endif
#include "app4g.h"
#include "app_rs485_broker.h"
#include "ds18b20.h"
#include "ta6932.h"
#include "dianchuan.h"
#include "st_gpio.h"
#include "cjson_middleware.h"
#include "mqtt_app.h"


volatile unsigned int time_base = 0;
__IO uint16_t TimingDelay;

extern uint32_t checkontime;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
#if 1
__weak void SVC_Handler(void)
{
}
#endif
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#if 1
__weak void PendSV_Handler(void)
{
}
#endif
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
#if 0
volatile unsigned int time_base = 0;
__IO uint16_t TimingDelay;
void SysTick_Handler(void)
{
    time_base++ ;

    if(TimingDelay != 0x0)
    {
        TimingDelay--;
    }
    time_isr_query();

}
unsigned int get_curtime(void)
{
    return time_base;
}
void Delay1ms(__IO uint16_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}
#endif
//extern void Delay_us(__IO uint32_t t);

//https://blog.csdn.net/yunzhifeiti/article/details/60467483

unsigned int get_curtime(void)
{
    return osKernelGetTickCount();
}


void Delay_us(__IO uint32_t nUsTime)
{
#if 0
    while(nUsTime--)
    {
        __NOP();
    }
#else
    volatile uint32_t i = 0;
    //volatile uint32_t j = 0;
    for(i=0; i<(nUsTime); i++)
        //for(i=0;i<(nUsTime*10);i++)
    {
        //for(j=0;j<72;j++)
        //for(j=0;j<72;j++)
        //{
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
#if 0
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();

        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();

        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
#endif
        //}
    }
#endif
}
#if 0
extern uint8_t TxBuffer1[];
extern uint8_t TxBuffer2[];
extern uint8_t RxBuffer1[];
extern uint8_t RxBuffer2[];
extern __IO uint8_t TxCounter1;
extern __IO uint8_t TxCounter2;
extern __IO uint8_t RxCounter1;
extern __IO uint8_t RxCounter2;

extern uint8_t rec_f,tx_flag;
#endif
extern uint8_t Onkey_index;

//#define UART1_RX_BUFFER_LEN 32
uint8_t RxBuffer1[UART1_RX_BUFFER_LEN]= {0};
__IO uint8_t RxCounter1 = 0x00;
uint8_t RxBuffer2[UART2_RX_BUFFER_LEN]= {0};
__IO uint8_t RxCounter2 = 0x00;

uint8_t RxBuffer3[UART3_RX_BUFFER_LEN]= {0};
__IO uint16_t RxCounter3 = 0x00;
uint8_t RxBuffer4[UART4_RX_BUFFER_LEN]= {0};
__IO uint16_t RxCounter4 = 0x00;

#if 0
uint8_t RxBuffer2_proc[UART2_RX_BUFFER_LEN]= {0};
__IO uint8_t RxCounter2_proc = 0x00;
#endif

uint8_t RxBuffer5[UART5_RX_BUFFER_LEN]= {0};
__IO uint8_t RxCounter5 = 0x00;

uint8_t TxBuffer1[UART1_TX_BUFFER_LEN]= {0};
__IO uint16_t TxCounter1 = 0x00;

uint8_t TxBuffer2[UART2_TX_BUFFER_LEN]= {0};
__IO uint16_t TxCounter2 = 0x00;

uint8_t TxBuffer3[UART3_TX_BUFFER_LEN]= {0};
__IO uint16_t TxCounter3 = 0x00;
uint8_t TxBuffer4[UART4_TX_BUFFER_LEN]= {0};
__IO uint16_t TxCounter4 = 0x00;


uint8_t TxBuffer5[UART5_TX_BUFFER_LEN]= {0};
__IO uint8_t TxCounter5 = 0x00;

void reset_uart_debug_buffer(void)
{
    memset(RxBuffer1,0,UART1_RX_BUFFER_LEN);
    RxCounter1 = 0 ;
}
void reset_uart1_rx_buffer(void)
{
    memset(RxBuffer1,0,UART1_RX_BUFFER_LEN);
    RxCounter1 = 0 ;
}

void reset_uart2_rx_buffer(void)
{
    memset(RxBuffer2,0,UART2_RX_BUFFER_LEN);
    RxCounter2 = 0 ;
}
void reset_uart3_rx_buffer(void)
{
    memset(RxBuffer3,0,UART3_RX_BUFFER_LEN);
    RxCounter3 = 0 ;
}
void reset_uart4_rx_buffer(void)
{
    memset(RxBuffer4,0,UART4_RX_BUFFER_LEN);
    RxCounter4 = 0 ;
}

void reset_uart5_rx_buffer(void)
{
    memset(RxBuffer5,0,UART5_RX_BUFFER_LEN);
    RxCounter5 = 0 ;
}

void reset_uart1_tx_buffer(void)
{
    memset(TxBuffer1,0,UART1_TX_BUFFER_LEN);
    TxCounter1 = 0 ;
}

void reset_uart2_tx_buffer(void)
{
    memset(TxBuffer2,0,UART2_TX_BUFFER_LEN);
    TxCounter2 = 0 ;
}
void reset_uart3_tx_buffer(void)
{
    memset(TxBuffer3,0,UART3_TX_BUFFER_LEN);
    TxCounter3 = 0 ;
}
void reset_uart4_tx_buffer(void)
{
    memset(TxBuffer4,0,UART4_TX_BUFFER_LEN);
    TxCounter4 = 0 ;
}


void reset_uart5_tx_buffer(void)
{
    memset(TxBuffer5,0,UART5_TX_BUFFER_LEN);
    TxCounter5 = 0 ;
}






#if 1
//Breaker_Frame_t gMcuMsg_Recv = {0};
JC_Frame_t gUart5_Msg_Recv = {0};
KB_Frame_t gUart5_Msgkb_Recv = {0};

volatile uint8_t gstate = 0;
volatile uint8_t iDataCnt = 0;
//uint8_t iCalcCheckSum = 0;
void mcu_uart5_control_init(void)
{

	memset(&gUart5_Msg_Recv,0,sizeof(gUart5_Msg_Recv));
	gstate = 0;
	iDataCnt = 0;

	//rx_rssi = 6;
}
volatile uint8_t dhh_happen =0;
uint8_t get_dhh_happen(void)
{
	 return dhh_happen ;

}

void set_dhh_happen(void)
{
	dhh_happen = 1;

}
void clear_dhh_happen(void)
{
	dhh_happen = 0;
}

void UART5_IRQHandler(void)
{
        JC_Frame_t *pMsg = &gUart5_Msg_Recv;
        KB_Frame_t *pMsgkb = &gUart5_Msgkb_Recv;

	uint8_t ucVal = 0;
	uint16_t cur_crc = 0;
//	uint32_t flags =0;
	//

	// uint8_t ch = 0;
	/*--接收缓存器是否有数据--*/
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)    //判断读寄存器是否非空
    {

        ucVal= USART_ReceiveData(UART5);   //将读寄存器的数据缓存到接收缓冲区里
#if KB_DEBUG
		uart_printf("UART5-> %x ",ucVal); 
#endif
		switch(gstate)
		{
			//ff fe 01 01 08 00 a0 56
			//FF FE 05 01 08 0F 01 01
			case STATE_JC_MGAIC_START:
			//{
				//SBUF = '1';
				if(ucVal == MAGIC_START)
				{
			                pMsg->start = ucVal;
                                        pMsgkb->start = ucVal;
					gstate++;
				} 
				else
				{
					gstate = 0;
					iDataCnt = 0;
					//uart_printf("MAGIC1 ERR=0X%x\n",ucVal); 
				}
				break;
			case STATE_JC_CMD:

				if(ucVal == MSG_CMD)
				{
					pMsg->cmd= ucVal;
					gstate++;
				} 
				else if(ucVal >  MSG_CMD &&  ucVal <= 0x03)
				{
					pMsgkb->cmd= ucVal;
					gstate++;
				} 				
				else
				{
					gstate = 0;
					iDataCnt = 0;
				
				}
				break;

			case STATE_JC_LEN:
			//{
				//SBUF = '2';
				if(ucVal == MSG_LEN)
				{
					pMsg->len = ucVal;
					gstate++;
				}
				else if(pMsgkb->cmd >  MSG_CMD &&  pMsgkb->cmd <= 0x03)
				{
					pMsgkb->len = ucVal;
					gstate++;
                                        iDataCnt = 0;
				}
				else
				{
					gstate = 0;
					iDataCnt = 0;
					
				}
				break;
				
			case STATE_JC_DATA0:
			        if(pMsgkb->cmd >  MSG_CMD &&  pMsgkb->cmd <= 0x03)
			        {
			            pMsgkb->data[iDataCnt%10]= ucVal;
			            iDataCnt++;
			            if(iDataCnt == pMsgkb->len)
			            {
                                        gstate = STATE_JC_CRC0;
			            }
			        }
			        else
			        {
        				pMsg->data0= ucVal;
        				gstate++;
				}
				break;
			case STATE_JC_DATA1:
				pMsg->data1= ucVal;
				gstate++;
				break;
			case STATE_JC_DATA2:
				pMsg->data2= ucVal;
				gstate++;
				break;
			case STATE_JC_DATA3:
				pMsg->data3= ucVal;
				gstate++;
				break;
			case STATE_JC_DATA4:
				pMsg->data4= ucVal;
				gstate++;
				break;
			case STATE_JC_CRC0:
				pMsg->crc0= ucVal;
	                        pMsgkb->crc0= ucVal;
				gstate++;
				break;

			case STATE_JC_CRC1:
				pMsg->crc1= ucVal;
				pMsgkb->crc1= ucVal;
				cur_crc = pMsg->cmd+pMsg->len+pMsg->data0+pMsg->data1+  \
							pMsg->data2+pMsg->data3+pMsg->data4;//紫外烟使用

                                if(pMsgkb->cmd >  MSG_CMD &&  pMsgkb->cmd <= 0x03)//键盘使用
                                {
                                    u8 i = 0;
                                    
                                    RxCounter5 = 0x00;
                                    RxBuffer5[RxCounter5++]= 0x7A;
                                    RxBuffer5[RxCounter5++]= pMsgkb->cmd+1;
                                    RxBuffer5[RxCounter5++] = pMsgkb->len;
                                    
                                    cur_crc = pMsgkb->cmd+pMsgkb->len;
                                    for(i = 0; i < pMsgkb->len; i++)
                                    {
                                        cur_crc += pMsgkb->data[i];
                                        RxBuffer5[RxCounter5++] =pMsgkb->data[i];
                                    }

                                                                   
                                    if(cur_crc == (((uint16_t)pMsgkb->crc1<<8)|pMsgkb->crc0) && mqtt_sta == MQTT_OK)
                                    {
                                        uint32_t cur_time_sec = 0;
                                        uint32_t charge_left_tm = 0;
                                        
                                        u8 index = pMsgkb->data[0] -1;
                                        
                                        if(charge_Info.dc_port_status[index] == PORT_USEING)
                                        {
                                            cur_time_sec = RTC_GetCounter();//need dw							
				            charge_left_tm =  (charge_Info.charge_tm[index]-(cur_time_sec-charge_Info.start_time[index])/60);
				        }      
				        else
				        {
				        	charge_left_tm = charge_Info.charge_tm[index];
				        }

#if KB_DEBUG
				        printf("dc_port_status[%d]=%d, left time %d\r\n",index,charge_Info.dc_port_status[index],charge_left_tm);  
				        if(charge_Info.dc_port_status[index] == PORT_FAULT)
				        {
				        	charge_Info.dc_port_status[index] = PORT_FREE;
				        }
#endif
				        
					if(pMsgkb->cmd == 0x01)    //按键命令
                                        {
                                            if(pMsgkb->data[0] > 0 && pMsgkb->data[0] <= TA6932_LEDCOUNT)
                                            {
					        if(pMsgkb->data[1] == 0)//申请刷卡
					        {
                                                    if(charge_left_tm <= 240 &&  charge_Info.dc_port_status[index] != PORT_FAULT)//充电时间大于240不能加了
                                                    {
                                                        TA6932_DisplayPortNumber(index+1,0xffff);//显示---
                                                        RxBuffer5[2]++;
                                                        RxBuffer5[4] =0x01;
                                                        RxBuffer5[RxCounter5++]= 0x01;          
                                                    }
                                                    else
                                                    {
                                                        RxBuffer5[2]++;
                                                        RxBuffer5[4] =0x00;
                                                        RxBuffer5[RxCounter5++]= 0x00;                                                    
                                                    }
                                                }
                                                else
                                                {
                                                    if(charge_Info.dc_port_status[index] == PORT_USEING )//充电时间大于240不能加了
                                                    {
                                                        TA6932_DisplayPortNumber(index+1,charge_left_tm);//显示剩余时间
                                                    }
                                                    else
                                                    {
                                                        TA6932_DisplayPortNumber(index+1,index+1);//显示端口号
                                                    }
	                                           RxBuffer5[2]++;
	                                           RxBuffer5[4] =0x00;
                                                   RxBuffer5[RxCounter5++]= 0x00;           
                                                }
                                            }
                                            else
                                            {
                                                RxBuffer5[2]++;
                                                RxBuffer5[4] =0x00;
                                                RxBuffer5[RxCounter5++]= 0x01;                                            
                                            }
                                        }
                                        else if (pMsgkb->cmd == 0x03)//刷卡命令
                                        {
                                            //暂定更改变量后，发送时间处理事件，断路器更改变量，电川要调用函数。
                                            //上报
                                            uint8_t onflag = 0;
                                            Onkey_index = index;

                                            if(charge_Info.dc_port_status[index] == PORT_USEING)//
                                            {
                                               /* if(charge_mode == CHARGE_MODE_BROKER )
                                                    ret= app_rs485_broker_start_power(ch+1);
                                                else
                                                    ret = dc_start_power(ch+1,charge_tm);
                                                    */
                                                    if(charge_left_tm <= 240)
                                                    {
                                                         charge_Info.charge_tm[Onkey_index] += 240;
                                                        onflag = 1;
                                                    }
                                                    charge_left_tm =  (charge_Info.charge_tm[index]-(cur_time_sec-charge_Info.start_time[index])/60);

                                                     RxBuffer5[2]++;
                                                    RxBuffer5[RxCounter5++] =charge_left_tm%60 ==0 ? charge_left_tm/60:charge_left_tm/60+1;
                                            }
                                            else//如果是空的端口需要打开
                                            {
                                                if(charge_Info.charge_tm[Onkey_index] <= 240)
                                                {
                                                    charge_Info.charge_tm[Onkey_index] += 240;//默认刷一次卡，加240分钟
                                                    onflag =1;
                                                }
                                                //剩余时间传给按键板
                                                RxBuffer5[2]++;
                                                RxBuffer5[RxCounter5++] =charge_Info.charge_tm[Onkey_index]%60 ==0 ? charge_Info.charge_tm[Onkey_index]/60:charge_Info.charge_tm[Onkey_index]/60 +1;
                                            }
                                            
                                           if(onflag)
                                           {
                                           	if(charge_Info.charge_tm[Onkey_index]  <=240)
                                           	{
                                           		checkontime = 0;
                                           		time_restart(timer_kb_openport,300);
                                           	}
                                           	else
                                           	{
                                           		if(time_status_get(timer_kb_openport)  || charge_Info.dc_port_status[index] == PORT_USEING)
                                           		{
                                           			time_stop(timer_kb_openport);
                                           			osEventFlagsSet(evt_id_app,EVENT_FLAGS_APP_KEYBOARD_ON);
                                           		}
                                           		else  //charge_Info.dc_port_status[index] == PORT_FREE && 已经发出EVENT_FLAGS_APP_KEYBOARD_ON
                                           		{
                                           			checkontime = charge_Info.charge_tm[Onkey_index];
                                           			time_restart(timer_kb_openport,600);
                                           		}                                          
                                           	}
  
                                           }

                                        }
                                        else
                                        {
                                                RxBuffer5[1]=0x02;
                                                RxBuffer5[2]=0x03;
                                                RxBuffer5[3]= 0;
                                                RxBuffer5[4] =0x00;
                                                RxBuffer5[5] =0x00;
                                                RxCounter5 = 6;
                                        }
                                        
                                    }
                                    else
                                    {
                                        if(pMsgkb->cmd == 0x01) 
                                        {
                                            RxBuffer5[2]++;
                                            RxBuffer5[4] =0x00;
                                            RxBuffer5[RxCounter5++] =0x01;
                                        }
                                        else if(pMsgkb->cmd == 0x03)
                                        {
                                            RxBuffer5[2]++;
                                            RxBuffer5[RxCounter5++] = 0xff;
                                        }
#if KB_DEBUG
                                        printf("uart5  pMsgkb crc error\r\n");  
#endif
                                    }
                                    
                                    cur_crc = 0;
                                    for(i = 1; i < RxCounter5; i++)
                                    {
                                        cur_crc += RxBuffer5[i];
                                    }
                                    RxBuffer5[RxCounter5++] = cur_crc&0xff;
                                    RxBuffer5[RxCounter5++] = (cur_crc>>8)&0xff;
                                    RxBuffer5[RxCounter5++] =0x67;
                                    if(debug_uart_id ==1)
                                    {
                                   	osEventFlagsSet(evt_id_uart1, EVENT_FLAGS_UART5);
                                    }
                                    else
                                    {
                                    	osEventFlagsSet(evt_id_uart4, EVENT_FLAGS_UART5);
                                    }
                                    gstate = 0;
                                    iDataCnt = 0;
#if KB_DEBUG
                                    printf("uart5  pMsgkb reply len= %d\r\n",RxCounter5);       
#endif
                                }
				else if(cur_crc == ((pMsg->crc1<<8)|pMsg->crc0))
				{
					//notify 
					gstate = 0;
					iDataCnt = 0;
					if(pMsg->data0 == STATE_JC_SAFE)
						clear_dhh_happen();
					else if(pMsg->data0 == STATE_JC_ALARM)
						set_dhh_happen();
					#if 0
					if(evt_id_uart5)
					{
	                    flags=osEventFlagsSet(evt_id_uart5, EVENT_FLAGS_UART5);
	                    switch(flags)
	                    {
	                    case osFlagsErrorUnknown:
	                        sendchar('!');
	                        break;
	                    case osFlagsErrorParameter:
	                        sendchar('@');
	                        break;
	                    case osFlagsErrorResource:
	                        sendchar('#');

	                        break;
	                    default:
	                        //sendchar('&');
	                        break;

	                    }
					}
					#endif

				} 
				else
				{
					gstate = 0;
					iDataCnt = 0;
#if KB_DEBUG
					printf("uart5  crc error\r\n");
#endif
				
				}
				break;
			default:
				mcu_uart5_control_init();
				break;
		}
  	}
	
}
#else
void UART5_IRQHandler(void)
{

    uint8_t ch = 0;
    uint32_t flags = 0;



    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)    //判断读寄存器是否非空
    {

        ch = USART_ReceiveData(UART5);   //将读寄存器的数据缓存到接收缓冲区里
        //debug_uart_id = PRINTF_UART_ID;
        RxBuffer5[RxCounter5] =ch;
        RxCounter5++;
        if( RxCounter5 >= UART5_RX_BUFFER_LEN)
        {
            reset_uart5_rx_buffer();
            return;
        }
#if 1
        //printf("RxBuffer1[RxCounter1]=0x%02x\r\n",RxBuffer1[RxCounter1]);
        if(ch != '\r' && ch != '\n')
        {
            //echo char
            sendchar(ch);
        }
        else
        {
            /*
            if(ch=='\r')
            {
            	sendchar('!');
            }
            else if(ch=='\n')
            {
            	sendchar('@');
            }
            */
            if(RxCounter5 >= 1)
            {
                if (RxBuffer5[RxCounter5 -1] == '\r' || RxBuffer2[RxCounter5 -1] == '\n')
                {
                    RxBuffer5[RxCounter5] = '\0';
                    //sendchar('#');
                    //set uart rx complete event
                    flags=osEventFlagsSet(evt_id_uart, EVENT_FLAGS_UART5);
                    switch(flags)
                    {
                    case osFlagsErrorUnknown:
                        sendchar('!');
                        break;
                    case osFlagsErrorParameter:
                        sendchar('@');
                        break;
                    case osFlagsErrorResource:
                        sendchar('#');

                        break;
                    default:
                        //sendchar('&');
                        break;

                    }

                }
            }
        }
#endif
        //debug_uart_id = 0xff;
    }

}
#endif
void UART4_IRQHandler(void)	  //串口1 中断服务程序
{
#if 1
	uint8_t ch = 0;
	//uint8_t index = 0;

	uint32_t flags = 0;
	//TxCounter2 = 0;;
	//unsigned int i;
	if(USART_GetITStatus(UART4,USART_IT_IDLE)!= RESET)
	{
		//uart_sendstring("rx idle",strlen("rx idle"));
		//printf("uart2 rx2 int\n");
		ch=UART4->SR;//先读SR，然后读DR才能清除
		ch=UART4->DR;// //软件序列清除IDLE标志位
		ch=ch; // 防止编译器警告

#if (UART4_RX_DMA ==1)
		DMA_Cmd(DMA2_Channel3, DISABLE);//关闭DMA，准备重新配置
		RxCounter4 = UART4_RX_BUFFER_LEN - DMA_GetCurrDataCounter(DMA2_Channel3);
		//DMA_Cmd(DMA2_Channel3, ENABLE);
		//计算接收数据长度
		//debug_uart_id = PRINTF_UART_ID;
		//sendchar('*');
		//sendchar(ch);
		//sendchar('*');

		//set uart rx complete event
		//printf("set uart2 rx complete event =%s %d\r\n",RxBuffer2,RxCounter2);


		if(evt_id_uart4)
		{
#if 0
			//printf("set uart2 rx complete event:%d=%s\r\n",RxCounter2,RxBuffer2);
			printf("\r\nset uart4 rx complete:%d\r\n",RxCounter4);
#else
			//41 54 2B 51 49 52 44 3D 30 2C 34 30 0D 0D 0A 2B 51 49 52 44 3A 20 34 0D 0A 20 02 00 00 0D 0A 0D 0A 4F 4B 0D 0A

			printf("\r\nset uart4 rx complete:%d\r\n",RxCounter4);
			if(debug_uart_id ==1)
			{
				int i =0;
				for(i=0; i<RxCounter4; i++)
				{
					printf("0x%02x,",RxBuffer4[i]);
					if((i+1)%8 == 0)
						printf("\r\n");
				}
				printf("\r\n");
			}
			else if(debug_uart_id ==4)
			{
				printf("uart4:%s\r\n",RxBuffer4);
			}
#endif

			flags=osEventFlagsSet(evt_id_uart4, EVENT_FLAGS_UART4);
			switch(flags)
			{
			case osFlagsErrorUnknown:
				sendchar('!');
				break;
			case osFlagsErrorParameter:
				sendchar('@');
				break;
			case osFlagsErrorResource:
				sendchar('#');

				break;
			default:
				//sendchar('&');
				break;

			}
		}
		else
		{
			printf("\r\nset uart4 rx comp:%d\r\n",RxCounter4);
			//uart4_dma_send_data(RxBuffer4,RxCounter4);
			//reset_uart4_rx_buffer();
			//DMA_Enable(UART4_RX_DMA_CHANNEL,UART4_RX_BUFFER_LEN);//开启下一次DMA接收
#if 0
			uart1_dma_printf("\r\nset uart2 rx complete:%d=%s\r\n",RxCounter2,RxBuffer2);
//#else
			//41 54 2B 51 49 52 44 3D 30 2C 34 30 0D 0D 0A 2B 51 49 52 44 3A 20 34 0D 0A 20 02 00 00 0D 0A 0D 0A 4F 4B 0D 0A
			uart1_dma_printf("\r\nset uart4 rx comp:%d\r\n",RxCounter4);
			int i =0;
			for(i=0; i<RxCounter4; i++)
			{
				uart1_dma_printf("0x%02x,",RxBuffer4[i]);
				if((i+1)%8 == 0)
					uart1_dma_printf("\r\n");
			}
			uart1_dma_printf("\r\n");
						reset_uart4_rx_buffer();
#if (UART4_RX_DMA ==1)
			DMA_Enable(DMA2_Channel3,UART4_RX_BUFFER_LEN);//开启下一次DMA接收
#endif
#endif


		}
		//debug_uart_id = 0xff;
#endif

		// Uart_Set_Event(E_uart_idle);// 设置接收完成(空闲)事件
		//USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
#if 1
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)	 //判断读寄存器是否非空
	{

		ch = USART_ReceiveData(UART4);   //将读寄存器的数据缓存到接收缓冲区里
		//debug_uart_id = PRINTF_UART_ID;
		RxBuffer4[RxCounter4] =ch;
		RxCounter4++;
		if( RxCounter4 >= UART4_RX_BUFFER_LEN)
		{
			reset_uart4_rx_buffer();
			return;
		}

		//printf("RxBuffer1[RxCounter1]=0x%02x\r\n",RxBuffer1[RxCounter1]);
		if(ch != '\r' && ch != '\n')
		{
			//echo char
			sendchar(ch);
		}
		else
		{
			/*
			if(ch=='\r')
			{
				sendchar('!');
			}
			else if(ch=='\n')
			{
				sendchar('@');
			}
			*/

			if(RxCounter4 >= 1)
			{
				//if (RxBuffer2[RxCounter2 -1] == '\r' || RxBuffer2[RxCounter2 -1] == '\n')
				{
					RxBuffer4[RxCounter4] = '\0';
					//sendchar('#');
					//set uart rx complete event
					flags=osEventFlagsSet(evt_id_uart4, EVENT_FLAGS_UART4);
					switch(flags)
					{
					case osFlagsErrorUnknown:
						sendchar('!');
						break;
					case osFlagsErrorParameter:
						sendchar('@');
						break;
					case osFlagsErrorResource:
						sendchar('#');

						break;
					default:
						//sendchar('&');
						break;

					}

				}
			}
		}

		//debug_uart_id = 0xff;
	}
#endif
#endif

}


void USART3_IRQHandler(void)      //串口1 中断服务程序
{
#if 1
    uint8_t ch = 0;
    //uint8_t index = 0;
	uint8_t ret =0;
    uint32_t flags = 0;
    //TxCounter2 = 0;;
    //unsigned int i;
    if(USART_GetITStatus(USART3,USART_IT_IDLE)!= RESET)
    {
        //uart_sendstring("rx idle",strlen("rx idle"));
        //printf("uart2 rx2 int\n");
        ch=USART3->SR;//先读SR，然后读DR才能清除
        ch=USART3->DR;// //软件序列清除IDLE标志位
        ch=ch; // 防止编译器警告

#if (UART3_RX_DMA ==1)
        DMA_Cmd(UART3_RX_DMA_CHANNEL, DISABLE);//关闭DMA，准备重新配置
        RxCounter3 = UART3_RX_BUFFER_LEN - DMA_GetCurrDataCounter(UART3_RX_DMA_CHANNEL);
       // DMA_Cmd(DMA1_Channel3, ENABLE);//关闭DMA，准备重新配置
        //计算接收数据长度
        //debug_uart_id = PRINTF_UART_ID;
        //sendchar('*');
        //sendchar(ch);
        //sendchar('*');

        //set uart rx complete event
        //printf("set uart2 rx complete event =%s %d\r\n",RxBuffer2,RxCounter2);


        if(evt_id_uart3)
        {
#if 1
            //printf("set uart2 rx complete event:%d=%s\r\n",RxCounter2,RxBuffer2);
            printf("\r\nset uart3 rx complete:%d\r\n",RxCounter3);
#else
            //41 54 2B 51 49 52 44 3D 30 2C 34 30 0D 0D 0A 2B 51 49 52 44 3A 20 34 0D 0A 20 02 00 00 0D 0A 0D 0A 4F 4B 0D 0A
            uart1_dma_printf("\r\nset uart2 rx complete:%d\r\n",RxCounter2);
            int i =0;
            for(i=0; i<RxCounter2; i++)
            {
#if 1
                if(RxBuffer2[i] == 0x00)
                    uart1_dma_printf("&&&&\r\n");
#else
                uart1_dma_printf("0x%02x,",RxBuffer2[i]);
                if((i+1)%8 == 0)
                    uart1_dma_printf("\r\n");
#endif
            }
            uart1_dma_printf("\r\n");
#endif
			ret = dc_active_send_mcu_response(1,RxBuffer3,RxCounter3);
			if(ret == 1)
			{
				reset_uart3_rx_buffer();
				#if (UART3_RX_DMA ==1)
				DMA_Enable(UART3_RX_DMA_CHANNEL,UART3_RX_BUFFER_LEN);//开启下一次DMA接收
				#endif
				return;
			}
            flags=osEventFlagsSet(evt_id_uart3, EVENT_FLAGS_UART3);
            switch(flags)
            {
            case osFlagsErrorUnknown:
                sendchar('!');
                break;
            case osFlagsErrorParameter:
                sendchar('@');
                break;
            case osFlagsErrorResource:
                sendchar('#');

                break;
            default:
                //sendchar('&');
                break;

            }
        }
        else
        {
        	//echo test
#if 1
            printf("\r\nset uart3 rx comp:%d,%d\r\n",RxCounter3,get_curtime2());
			//uart3_dma_send_data(RxBuffer3,RxCounter3);
			//reset_uart3_rx_buffer();
			//DMA_Enable(UART3_RX_DMA_CHANNEL,UART3_RX_BUFFER_LEN);//开启下一次DMA接收


#else
            //41 54 2B 51 49 52 44 3D 30 2C 34 30 0D 0D 0A 2B 51 49 52 44 3A 20 34 0D 0A 20 02 00 00 0D 0A 0D 0A 4F 4B 0D 0A
            uart1_dma_printf("\r\nset uart3 rx comp:%d\r\n",RxCounter3);
            int i =0;
            for(i=0; i<RxCounter3; i++)
            {
                uart1_dma_printf("0x%02x,",RxBuffer3[i]);
                if((i+1)%8 == 0)
                    uart1_dma_printf("\r\n");
            }
            uart1_dma_printf("\r\n");
#endif


        }
        //debug_uart_id = 0xff;
#endif

        // Uart_Set_Event(E_uart_idle);// 设置接收完成(空闲)事件
        //USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
#if 1
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //判断读寄存器是否非空
    {

        ch = USART_ReceiveData(USART3);   //将读寄存器的数据缓存到接收缓冲区里
        //debug_uart_id = PRINTF_UART_ID;
        RxBuffer3[RxCounter3] =ch;
        RxCounter3++;
        if( RxCounter3 >= UART3_RX_BUFFER_LEN)
        {
            reset_uart3_rx_buffer();
            return;
        }

        //printf("RxBuffer1[RxCounter1]=0x%02x\r\n",RxBuffer1[RxCounter1]);
        if(ch != '\r' && ch != '\n')
        {
            //echo char
            sendchar(ch);
        }
        else
        {
            /*
            if(ch=='\r')
            {
            	sendchar('!');
            }
            else if(ch=='\n')
            {
            	sendchar('@');
            }
            */

            if(RxCounter3 >= 1)
            {
                //if (RxBuffer2[RxCounter2 -1] == '\r' || RxBuffer2[RxCounter2 -1] == '\n')
                {
                    RxBuffer3[RxCounter3] = '\0';
                    //sendchar('#');
                    //set uart rx complete event
                    flags=osEventFlagsSet(evt_id_uart3, EVENT_FLAGS_UART3);
                    switch(flags)
                    {
                    case osFlagsErrorUnknown:
                        sendchar('!');
                        break;
                    case osFlagsErrorParameter:
                        sendchar('@');
                        break;
                    case osFlagsErrorResource:
                        sendchar('#');

                        break;
                    default:
                        //sendchar('&');
                        break;

                    }

                }
            }
        }

        //debug_uart_id = 0xff;
    }
#endif
#endif

}


void USART1_IRQHandler(void)      //串口1 中断服务程序
{
#if 1
    uint8_t ch = 0;
    //uint8_t index = 0;
	uint8_t ret =0;
    uint32_t flags = 0;
//TxCounter1 = 0;;
    //unsigned int i;
    if(USART_GetITStatus(USART1,USART_IT_IDLE)!= RESET)
    {
        //uart_sendstring("rx idle",strlen("rx idle"));

        ch=USART1->SR;//先读SR，然后读DR才能清除
        ch=USART1->DR;// //软件序列清除IDLE标志位
        ch=ch; // 防止编译器警告

#if (UART1_RX_DMA ==1)
        DMA_Cmd(DMA1_Channel5, DISABLE);//关闭DMA，准备重新配置
        RxCounter1 = UART1_RX_BUFFER_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);
        //计算接收数据长度
#endif

        //debug_uart_id = PRINTF_UART_ID;
        //sendchar('*');
        //sendchar(ch);
        //sendchar('*');
#if 1
        //set uart rx complete event
        if(evt_id_uart1 )
        {

        	if(debug_uart_id ==4)
        	{
        	    //hex_dump(RxBuffer1,RxCounter1);
				//dump_memeory((uint32_t)RxBuffer1,RxCounter1);
				//printf("uart1**=%s %d\r\n",RxBuffer1,RxCounter1);
				ret = app_rs485_broker_id_response_parse(RxBuffer1,RxCounter1);	
				if((ret ==0)  ||(ret==1))
				{
					
					reset_uart1_rx_buffer();
#if (UART1_RX_DMA ==1)
					DMA_Enable(DMA1_Channel5,UART1_RX_BUFFER_LEN);//开启下一次DMA接收
#endif
					app_uart1_rs485_rxtx_io_low_is_rx();
					return;
				}
			
        	}
            flags=osEventFlagsSet(evt_id_uart1, EVENT_FLAGS_UART1);
            switch(flags)
            {
            case osFlagsErrorUnknown:
                sendchar('!');
                break;
            case osFlagsErrorParameter:
                sendchar('@');
                break;
            case osFlagsErrorResource:
                sendchar('#');

                break;
            default:
               // sendchar('&');
               if(debug_uart_id ==4)
               	{
               		//printf("send EVENT_FLAGS_UART1\r\n");
               	}
                break;

            }
        }
		else
		{
			printf("uart1 =%s %d\r\n",RxBuffer1,RxCounter1);
			reset_uart1_rx_buffer();
#if (UART1_RX_DMA ==1)
			DMA_Enable(DMA1_Channel5,UART1_RX_BUFFER_LEN);//开启下一次DMA接收
#endif
			
		}

        //debug_uart_id = 0xff;
#endif

        // Uart_Set_Event(E_uart_idle);// 设置接收完成(空闲)事件
        //USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	   //判断读寄存器是否非空
    {

        ch = USART_ReceiveData(USART1);   //将读寄存器的数据缓存到接收缓冲区里
        //debug_uart_id = PRINTF_UART_ID;
        RxBuffer1[RxCounter1] =ch;
        RxCounter1++;
        if( RxCounter1 >= UART1_RX_BUFFER_LEN)
        {
            reset_uart_debug_buffer();
            return;
        }
#if 1
        //printf("RxBuffer1[RxCounter1]=0x%02x\r\n",RxBuffer1[RxCounter1]);
        if(ch != '\r' && ch != '\n')
        {
            //echo char
            sendchar(ch);
        }
        else
        {
            /*
            if(ch=='\r')
            {
            	sendchar('!');
            }
            else if(ch=='\n')
            {
            	sendchar('@');
            }
            */
            if(RxCounter1 >= 1)
            {
                if (RxBuffer1[RxCounter1 -1] == '\r' || RxBuffer1[RxCounter1 -1] == '\n')
                {
                    RxBuffer1[RxCounter1] = '\0';
                    //sendchar('#');
                    //set uart rx complete event
                    flags=osEventFlagsSet(evt_id_uart1, EVENT_FLAGS_UART1);
                    switch(flags)
                    {
                    case osFlagsErrorUnknown:
                        sendchar('!');
                        break;
                    case osFlagsErrorParameter:
                        sendchar('@');
                        break;
                    case osFlagsErrorResource:
                        sendchar('#');

                        break;
                    default:
                        //sendchar('&');
                        break;

                    }

                }
            }
        }
#endif
        //debug_uart_id = 0xff;
    }
#endif
#if 0
    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG
    {
        TxCounter1 = 0xff;
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断，
    }
    if(USART_GetITStatus(USART1, USART_IT_TC) == SET  )                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG
    {
        USART_ClearFlag(USART1, USART_FLAG_TC);
        TxCounter1 = 0xff-1;
        //USART_ITConfig(USART1, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断，
    }
#endif
}
void USART2_IRQHandler(void)      //串口1 中断服务程序
{
#if 1
    uint8_t ch = 0;
    //uint8_t index = 0;

    uint32_t flags = 0;
    //TxCounter2 = 0;;
    //unsigned int i;
    if(USART_GetITStatus(USART2,USART_IT_IDLE)!= RESET)
    {
        //uart_sendstring("rx idle",strlen("rx idle"));
        //printf("uart2 rx2 int\n");
        ch=USART2->SR;//先读SR，然后读DR才能清除
        ch=USART2->DR;// //软件序列清除IDLE标志位
        ch=ch; // 防止编译器警告

#if (UART2_RX_DMA ==1)
        DMA_Cmd(DMA1_Channel6, DISABLE);//关闭DMA，准备重新配置
        RxCounter2 = UART2_RX_BUFFER_LEN - DMA_GetCurrDataCounter(DMA1_Channel6);
#if 0
        RxCounter2_proc =  RxCounter2;
        memcpy(RxBuffer2_proc,RxBuffer2,RxCounter2_proc);
#endif
        DMA_Cmd(DMA1_Channel6, ENABLE);//打开DMA，准备重新配置
        //计算接收数据长度
        //debug_uart_id = PRINTF_UART_ID;
        //sendchar('*');
        //sendchar(ch);
        //sendchar('*');

        //set uart rx complete event
        //printf("set uart2 rx complete event =%s %d\r\n",RxBuffer2,RxCounter2);


        if(evt_id_uart)
        {
#if 1
            //printf("set uart2 rx complete event:%d=%s\r\n",RxCounter2,RxBuffer2);
            //printf("\r\nset uart2 rx complete:%d=%s\r\n",RxCounter2,RxBuffer2);
            //printf("\r\nset uart2 rx complete:%d\r\n",RxCounter2);
#else
            //41 54 2B 51 49 52 44 3D 30 2C 34 30 0D 0D 0A 2B 51 49 52 44 3A 20 34 0D 0A 20 02 00 00 0D 0A 0D 0A 4F 4B 0D 0A
            uart1_dma_printf("\r\nset uart2 rx complete:%d\r\n",RxCounter2);
            int i =0;
            for(i=0; i<RxCounter2; i++)
            {
#if 1
                if(RxBuffer2[i] == 0x00)
                    uart1_dma_printf("&&&&\r\n");
#else
                uart1_dma_printf("0x%02x,",RxBuffer2[i]);
                if((i+1)%8 == 0)
                    uart1_dma_printf("\r\n");
#endif
            }
            uart1_dma_printf("\r\n");
#endif

            flags=osEventFlagsSet(evt_id_uart, EVENT_FLAGS_UART2);
            switch(flags)
            {
            case osFlagsErrorUnknown:
                sendchar('!');
                break;
            case osFlagsErrorParameter:
                sendchar('@');
                break;
            case osFlagsErrorResource:
                sendchar('#');

                break;
            default:
                //sendchar('&');
                break;

            }
        }
        else
        {
#if 0
            uart1_dma_printf("\r\nset uart2 rx complete:%d=%s\r\n",RxCounter2,RxBuffer2);
//#else
            //41 54 2B 51 49 52 44 3D 30 2C 34 30 0D 0D 0A 2B 51 49 52 44 3A 20 34 0D 0A 20 02 00 00 0D 0A 0D 0A 4F 4B 0D 0A
  
				
            uart1_dma_printf("\r\nset uart2 rx comp:%d.%s\r\n",RxCounter2,RxBuffer2);
            int i =0;
            for(i=0; i<RxCounter2; i++)
            {
                uart1_dma_printf("0x%02x,",RxBuffer2[i]);
                if((i+1)%8 == 0)
                    uart1_dma_printf("\r\n");
            }
            uart1_dma_printf("\r\n");
#endif
			printf("POWER ON =%s %d\r\n",RxBuffer2,RxCounter2);
            reset_uart2_rx_buffer();
#if (UART2_RX_DMA ==1)
            DMA_Enable(DMA1_Channel6,UART2_RX_BUFFER_LEN);//开启下一次DMA接收
#endif

        }
        //debug_uart_id = 0xff;
#endif

        // Uart_Set_Event(E_uart_idle);// 设置接收完成(空闲)事件
        //USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
#if 1
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //判断读寄存器是否非空
    {

        ch = USART_ReceiveData(USART2);   //将读寄存器的数据缓存到接收缓冲区里
        //debug_uart_id = PRINTF_UART_ID;
        RxBuffer2[RxCounter2] =ch;
        RxCounter2++;
        if( RxCounter2 >= UART2_RX_BUFFER_LEN)
        {
            reset_uart2_rx_buffer();
            return;
        }

        //printf("RxBuffer1[RxCounter1]=0x%02x\r\n",RxBuffer1[RxCounter1]);
        if(ch != '\r' && ch != '\n')
        {
            //echo char
            sendchar(ch);
        }
        else
        {
            /*
            if(ch=='\r')
            {
            	sendchar('!');
            }
            else if(ch=='\n')
            {
            	sendchar('@');
            }
            */
            //	if(cur_at_rec_len == RxCounter2 )
            if((RxCounter2 >= 1) && (cur_at_rec_len == RxCounter2 ) )
            {
                //if (RxBuffer2[RxCounter2 -1] == '\r' || RxBuffer2[RxCounter2 -1] == '\n')
                {
                    RxBuffer2[RxCounter2] = '\0';
                    //sendchar('#');
                    //set uart rx complete event
                    flags=osEventFlagsSet(evt_id_uart, EVENT_FLAGS_UART2);
                    switch(flags)
                    {
                    case osFlagsErrorUnknown:
                        sendchar('!');
                        break;
                    case osFlagsErrorParameter:
                        sendchar('@');
                        break;
                    case osFlagsErrorResource:
                        sendchar('#');

                        break;
                    default:
                        //sendchar('&');
                        break;

                    }

                }
            }
        }

        //debug_uart_id = 0xff;
    }
#endif
#endif
#if 0
    if(USART_GetITStatus(USART2,USART_IT_TC)!= RESET) // 全部数据发送完成，产生该标记**
    {
        USART_ClearITPendingBit(USART2, USART_IT_TC);   // 清除完成标记
        DMA_Cmd(DMA1_Channel7, DISABLE); // 关闭DMA
        //DMA1_Channel7->CNDTR=0;          // 清除数据长度
        DMA_SetCurrDataCounter(DMA1_Channel7,0);
        //Uart_Set_Event(E_uart_tc);     //设置发送完成事件
    }
#endif

}

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        /* Toggle LED1 */
        // STM_EVAL_LEDToggle(LED1);
        uint8_t gpio_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
        uart_printf("int GPIO_Pin_5=%d",gpio_value);
        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}



/****************************************************************************
* ?    ?:void EXTI2_IRQHandler(void)
* ?    ?:EXTI2??????
* ????:?
* ????:?
* ?    ?:
* ????:?
****************************************************************************/
void EXTI15_10_IRQHandler(void)
{
    uint8_t gpio_value = 0;
    if(EXTI_GetITStatus(EXTI_Line10) != RESET)				  //????????
    {

        //debug_uart_id = PRINTF_UART_ID;
        EXTI_ClearITPendingBit(EXTI_Line10);										  //??????

        gpio_value = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10);
        printf("int GPIOE_Pin_10=%d\r\n",gpio_value);
        if(gpio_value == Bit_RESET)
        {
        	#if 0
        	if(evt_id_app)
     			osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_KEY_ENTER_SHORT);
			#else
			time_restart(timer_detect,1);
			#endif
        }


        //debug_uart_id = 0xff;
    }
	#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2)
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)				  //????????
    {
        //debug_uart_id = PRINTF_UART_ID;
        EXTI_ClearITPendingBit(EXTI_Line13);										  //??????
        gpio_value = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13);
        printf("int GPIOE_Pin_13=%d\r\n",gpio_value);
        if(gpio_value == Bit_RESET)
        {
            if(dc1_exist == 1)
            {
                dc1_exist = 0;
                printf("DC1_INSERT\r\n");
				if(evt_id_app)
               	 osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_DC1_INSERT);
            }
            else
            {


            }
        }
        else if(gpio_value == Bit_SET )
        {
            if(dc1_exist == 0)
            {
                dc1_exist = 1;
                printf("DC1_REMOVE\r\n");
				if(evt_id_app)
                osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_DC1_REMOVE);
            }
            else
            {

            }
        }
	}
	#endif
    if(EXTI_GetITStatus(EXTI_Line15) != RESET)				  //????????
    {
		EXTI_ClearITPendingBit(EXTI_Line15);	
		#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1)
        //debug_uart_id = PRINTF_UART_ID;
 									  //??????
        gpio_value = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15);
        printf("int GPIOE_Pin_15=%d\r\n",gpio_value);
        if(gpio_value == Bit_RESET)
        {
            if(dc1_exist == 1)
            {
                dc1_exist = 0;
                printf("DC1_INSERT\r\n");
				if(evt_id_app)
                osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_DC1_INSERT);
            }
            else
            {


            }
        }
        else if(gpio_value == Bit_SET )
        {
            if(dc1_exist == 0)
            {
                dc1_exist = 1;
                printf("DC1_REMOVE\r\n");
				if(evt_id_app)
                osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_DC1_REMOVE);
            }
            else
            {

            }
        }
		#endif
        gpio_value = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);
        printf("int GPIOA_Pin_15=%d\r\n",gpio_value);
        if(gpio_value == Bit_RESET)
        {
            if(dc2_exist == 1)
            {
                dc2_exist = 0;
                printf("DC2_INSERT\r\n");
				if(evt_id_app)
                osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_DC2_INSERT);
            }
            else
            {


            }
        }
        else if(gpio_value == Bit_SET )
        {
            if(dc2_exist == 0)
            {
                dc2_exist = 1;
                printf("DC2_REMOVE\r\n");
				if(evt_id_app)
                osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_DC2_REMOVE);
            }
            else
            {

            }
        }
    }
}

/****************************************************************************
* ?    ?:void EXTI3_IRQHandler(void)
* ?    ?:EXTI3??????
* ????:?
* ????:?
* ?    ?:
* ????:?
****************************************************************************/
void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)				  //????????
    {
        //debug_uart_id = PRINTF_UART_ID;
#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)
        uint8_t gpio_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
        uart_printf("int GPIO_Pin_5=%d",gpio_value);
        gpio_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
        uart_printf("int GPIO_Pin_3=%d",gpio_value);
        //_it0=1;	    			  //??????
        gpio_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
        uart_printf("int GPIO_Pin_2=%d",gpio_value);										  //??????
        EXTI_ClearITPendingBit(EXTI_Line3);					   //????????
#endif
        //debug_uart_id = 0xff;
    }
}
/****************************************************************************
* ?    ?:void EXTI9_5_IRQHandler(void)
* ?    ?:EXTI9-5??????
* ????:?
* ????:?
* ?    ?:
* ????:?
****************************************************************************/
volatile uint8_t smoke_happen =0;
uint8_t get_smoke_happen(void)
{
	 return smoke_happen ;

}

void set_smoke_happen(void)
{
	smoke_happen = 1;

}
void clear_smoke_happen(void)
{
	smoke_happen = 0;
}

void EXTI9_5_IRQHandler(void)
{
#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)

    if(EXTI_GetITStatus(EXTI_Line5) != RESET)				  //????????
    {
        //debug_uart_id = PRINTF_UART_ID;

        uint8_t gpio_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
        uart_printf("int GPIO_Pin_5=%d",gpio_value);
        gpio_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
        uart_printf("int GPIO_Pin_3=%d",gpio_value);
        //_it0=1;	    			  //??????
        gpio_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
        uart_printf("int GPIO_Pin_2=%d",gpio_value);
        EXTI_ClearITPendingBit(EXTI_Line5);					  //????????


        //debug_uart_id = 0xff;
    }
#elif ((HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1)||  \
		(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2))

    if(EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        //debug_uart_id = PRINTF_UART_ID;

        uint8_t gpio_value = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);
        uart_printf("int GPIO_Pin_9=%d",gpio_value);

        EXTI_ClearITPendingBit(EXTI_Line9);
		//gpio_led_off();
        // debug_uart_id = 0xff;
		//printf("DC2_REMOVE\r\n");
		if(evt_id_app)
		{
			if(gpio_value == 1)
			{
				//set_smoke_happen();
				osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_SMOKE_DETECT);
				
			}
			else
				osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_SMOKE_DETECT_NO);
		}

    }
#endif

}
/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
/**
  * @brief  This function handles RTCAlarm_IRQHandler .
  * @param  None
  * @retval : None
  */
void RTCAlarm_IRQHandler(void)
{
    /* Clear the Alarm Pending Bit */
    if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);  //?????
        /* Clear the EXTI Line 17 */
        EXTI_ClearITPendingBit(EXTI_Line17);
        AlarmStatus = 2;

        printf("RTCAlarm_IRQHandler get_curtime=%d\r\n",get_curtime());
        return ;
    }
    else
    {
        printf("RTCAlarm_IRQHandler **error****get_curtime=%d\r\n",get_curtime());
        return ;

    }

}
#if 1
/**
  * @brief  This function handles RTC_IRQHandler .
  * @param  None
  * @retval : None
  */
void RTC_IRQHandler(void)
{
    uint8_t Month,Day;
    uint16_t Year;


    if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);  //?????
        /* Clear the EXTI Line 17 */
        EXTI_ClearITPendingBit(EXTI_Line17);
        AlarmStatus = 1;
        printf("\n\r RTC Alarm \n\r");
        //	printf("\n\r Now is %d? %d? %d? %d? %d? %d? ,??%d\n\r",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec,calendar.week);
        //	RTC_SetAlarm(RTC_GetCounter() + 80);
        //	RTC_WaitForLastTask();
        return;
    }

    if(RTC_GetITStatus(RTC_IT_SEC) != RESET)//
    {
        /* Clear the RTC Second interrupt */
        RTC_ClearITPendingBit(RTC_IT_SEC);
        //	printf("\n\r RTC sec \n\r");
        //	RTC_Get();
    }
    Month = BKP_ReadBackupRegister(BKP_DR2);
    Day = BKP_ReadBackupRegister(BKP_DR3);
    Year = BKP_ReadBackupRegister(BKP_DR4);
// NVIC_ClearPendingIRQ(RTC_IRQn);
    //RTC_ClearITPendingBit(RTC_IT_SEC);

    /* If counter is equal to 86399: one day was elapsed */
    /* This takes care of date change and resetting of counter in case of
    power on - Run mode/ Main supply switched on condition*/
    if(RTC_GetCounter() == 86399)
    {
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
        /* Reset counter value */
        RTC_SetCounter(0x0);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        /* Increment the date */
        DateUpdate();
    }

    if((RTC_GetCounter()/3600 == 1)&&(((RTC_GetCounter()%3600)/60) == 59)&&
            (((RTC_GetCounter()%3600)%60) == 59))
    {
        /* March Correction */
        if((Month == 3) && (Day >24))
        {
            if(WeekDay(Year,Month,Day)==0)
            {
                if((SummerTimeCorrect & 0x8000) == 0x8000)
                {
                    RTC_SetCounter(RTC_GetCounter() + 3601);

                    /* Reset March correction flag */
                    SummerTimeCorrect &= 0x7FFF;

                    /* Set October correction flag  */
                    SummerTimeCorrect |= 0x4000;
                    SummerTimeCorrect |= Year;
                    BKP_WriteBackupRegister(BKP_DR7,SummerTimeCorrect);
                }
            }
        }
        /* October Correction */
        if((Month == 10) && (Day >24))
        {
            if(WeekDay(Year,Month,Day)==0)
            {
                if((SummerTimeCorrect & 0x4000) == 0x4000)
                {
                    RTC_SetCounter(RTC_GetCounter() - 3599);

                    /* Reset October correction flag */
                    SummerTimeCorrect &= 0xBFFF;

                    /* Set March correction flag  */
                    SummerTimeCorrect |= 0x8000;
                    SummerTimeCorrect |= Year;
                    BKP_WriteBackupRegister(BKP_DR7,SummerTimeCorrect);
                }
            }
        }
    }
}
#else
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        RTC_ClearITPendingBit(RTC_IT_SEC);

        /* Toggle LED1 */
        // STM_EVAL_LEDToggle(LED1);

        /* Enable time update */
        TimeDisplay = 1;

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

    }
}
#endif
/**
  * @brief  This function handles ADC1 and ADC2 global interrupts requests.
  * @param  None
  * @retval None
  */
void ADC1_2_IRQHandler(void)
{

    //not use
//	uint16_t ADC2ConvertedValue = 0;
    /* Get injected channel13 converted value */
    //ADC2ConvertedValue = ADC_GetConversionValue(ADC2);
}
/**
  * @brief  This function handles DMA1 Channel 6 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel6_IRQHandler(void)
{
    /* Test on DMA1 Channel6 Transfer Complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC6))
    {
        printf("dma1_6 int=%d ms\r\n",get_curtime());
        /* Get Current Data Counter value after complete transfer */
        CurrDataCounterEnd = DMA_GetCurrDataCounter(DMA1_Channel6);
        /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
        DMA_ClearITPendingBit(DMA1_IT_GL6);
    }
}

uint16_t CurrDataCounterEnd_dma1_4 = 0x01;//UART1
uint16_t CurrDataCounterEnd_dma1_7 = 0x01;//UART2
uint16_t CurrDataCounterEnd_dma1_2 = 0x01;//UART3
uint16_t CurrDataCounterEnd_dma2_5 = 0x01;//UART4

//uint16_t CurrDataCounterEnd_dma1_2 = 0x01;//PCM ROM2RAM

#if (UART3_TX_DMA == 1)
void DMA1_Channel2_IRQHandler(void)
{
    uint32_t flags =0;
	//            DMA_ClearFlag(DMA1_FLAG_GL2);        
    //        DMA_Cmd(DMA1_Channel2, DISABLE);  
    /* Test on DMA1 Channel6 Transfer Complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC2))
    {
    	DMA_ClearITPendingBit(DMA1_FLAG_GL2);
		DMA_Cmd(DMA1_Channel2, DISABLE);  
		reset_uart3_tx_buffer();
         printf("dma1_2 int=%d ms\r\n",get_curtime2());
        /* Get Current Data Counter value after complete transfer */
        CurrDataCounterEnd_dma1_2= DMA_GetCurrDataCounter(DMA1_Channel2);
        /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
        
        if(evt_id_uart3)
        {
        	printf("set uart3 tx complete event\r\n");
            flags=osEventFlagsSet(evt_id_uart3, EVENT_FLAGS_UART3_TX_COMPLETE);
            switch(flags)
            {
            case osFlagsErrorUnknown:
                sendchar('!');
                break;
            case osFlagsErrorParameter:
                sendchar('@');
                break;
            case osFlagsErrorResource:
                sendchar('#');

                break;
            default:
                //sendchar('&');
                break;

            }
        }
    }
	else
	{
		printf("***dma1_2 int other flag\r\n");
	}
}
#endif
#if(UART4_TX_DMA == 1)

void DMA2_Channel4_5_IRQHandler(void)
{
    uint32_t flags =0;
    /* Test on DMA1 Channel6 Transfer Complete interrupt */
    if(DMA_GetITStatus(DMA2_IT_TC5))
    {
    	DMA_ClearITPendingBit(DMA2_IT_TC5);
        printf("dma2_45 int=%d ms\r\n",get_curtime2());
        /* Get Current Data Counter value after complete transfer */
        CurrDataCounterEnd_dma2_5= DMA_GetCurrDataCounter(DMA2_Channel5);
        /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
        
        if(evt_id_uart4)
        {
        	printf("set uart4 tx complete event\r\n");
            flags=osEventFlagsSet(evt_id_uart4, EVENT_FLAGS_UART4_TX_COMPLETE);
            switch(flags)
            {
            case osFlagsErrorUnknown:
                sendchar('!');
                break;
            case osFlagsErrorParameter:
                sendchar('@');
                break;
            case osFlagsErrorResource:
                sendchar('#');

                break;
            default:
                //sendchar('&');
                break;

            }
        }
    }
	else
	{
		printf("***dma2_45 int other flag\r\n");
	}
}
#endif
#if(UART1_TX_DMA == 1)

void DMA1_Channel4_IRQHandler(void)
{
    uint32_t flags =0;
    /* Test on DMA1 Channel6 Transfer Complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC4))
    {
    	DMA_ClearITPendingBit(DMA1_IT_TC4);
       // printf("dma1_4 int=%d ms\r\n",get_curtime());
        /* Get Current Data Counter value after complete transfer */
        CurrDataCounterEnd_dma1_4 = DMA_GetCurrDataCounter(DMA1_Channel4);
        /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
        //DMA_ClearITPendingBit(DMA1_IT_GL4);
		//printf("set uart1 tx complete event\r\n");
        if(evt_id_uart1)
        {
            flags=osEventFlagsSet(evt_id_uart1, EVENT_FLAGS_UART1_TX_COMPLETE);
            switch(flags)
            {
            case osFlagsErrorUnknown:
                sendchar('!');
                break;
            case osFlagsErrorParameter:
                sendchar('@');
                break;
            case osFlagsErrorResource:
                sendchar('#');

                break;
            default:
                //sendchar('&');
                break;

            }
        }
    }
}
#endif
#if(UART2_TX_DMA == 1)
void DMA1_Channel7_IRQHandler(void)
{
    uint32_t flags =0;
    /* Test on DMA1 Channel6 Transfer Complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC7))
    {
         printf("dma1_7 int=%d ms\r\n",get_curtime2());
        /* Get Current Data Counter value after complete transfer */
        CurrDataCounterEnd_dma1_7 = DMA_GetCurrDataCounter(DMA1_Channel7);
        /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
        DMA_ClearITPendingBit(DMA1_IT_TC7);
        //printf("set uart2 tx complete event\r\n");
        flags=osEventFlagsSet(evt_id_uart, EVENT_FLAGS_UART2_TX_COMPLETE);
        switch(flags)
        {
        case osFlagsErrorUnknown:
            sendchar('!');
            break;
        case osFlagsErrorParameter:
            sendchar('@');
            break;
        case osFlagsErrorResource:
            sendchar('#');

            break;
        default:
            //sendchar('&');
            break;

        }
    }
}
#endif

#if(PCM_ROM2RAM_DMA_SUPPORT   ==  1)
#if(PCM_ROM2RAM_DMA_CHANNEL_INDEX   == 2) 
/* Test on DMA1 Channel6 Transfer Complete interrupt */
void DMA1_Channel2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC2))
	{
		printf("dma1_2 int=%d ms\r\n",get_curtime());
		/* Get Current Data Counter value after complete transfer */
		CurrDataCounterEnd = DMA_GetCurrDataCounter(PCM_ROM2RAM_DMA_CHANNEL);
		/* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
		DMA_ClearITPendingBit(DMA1_IT_GL2);
	}
}
#endif
#endif

#if(SPI_FLASH_OP_MODE   ==  SPI_FLASH_DMA)
void DMA1_Channel2_IRQHandler(void)
{
    if (DMA_GetFlagStatus(DMA1_IT_TC2) != RESET)
    {

        /* 1.清中断标志 */
        DMA_ClearFlag(DMA1_IT_TC2);
        /* 2.禁能 DMA   传输完成中断 */
        DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);
        /* 3.禁能 SPI TX DMA   请求 */
        SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, DISABLE);
        /* 4.禁能 DMA Stream */
        DMA_Cmd(DMA1_Channel2, DISABLE);
        /* 6.通知应用程序 RX 传输完成 */
        printf("dma1_2 rx=%d ms\r\n",get_curtime());
        spi_post_rx();
    }
}


void DMA1_Channel3_IRQHandler(void)
{
    if (DMA_GetFlagStatus( DMA1_IT_TC3) != RESET)
    {

        /* 1.清中断标志 */
        DMA_ClearFlag(DMA1_IT_TC3);
        /* 2.禁能 DMA   传输完成中断 */
        DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, DISABLE);
        /* 3.禁能 SPI TX DMA   请求 */
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
        /* 4.禁能 DMA Stream */
        DMA_Cmd(DMA1_Channel3, DISABLE);
        /* 5.通知应用程序 TX 传输完成 */
        printf("dma1_3 tx=%d ms\r\n",get_curtime());
        spi_post_tx();
        /* Get Current Data Counter value after complete transfer */
        //CurrDataCounterEnd = DMA_GetCurrDataCounter(DMA1_Channel3);
        /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
        //DMA_ClearITPendingBit(DMA1_IT_GL3);
    }
}
#endif
/*File stm32f10x_it.c *//*添加中断处理函数*/

unsigned int get_curtime2(void)
{
    return time_base;
}


void Delay1ms(__IO uint16_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

void TIM1_UP_IRQHandler(void)
{
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);

    time_base++ ;

    if(TimingDelay != 0x0)
    {
        TimingDelay--;
    }
#if( USE_SOFTTIMER_DIY ==1)
    time_isr_query();
#endif

}


static uint32_t pcm_read_index =0;
void 	TIM3_IRQHandler(void)
{
#if 1
#if 0
    static uint8_t cur_times =0;
    uint8_t total_times =0;
    uint8_t ret =0;
#endif
    //static uint32_t index =0;
    if (TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
    {
        /* Clear the RTC Second interrupt */
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
#if 1
        //printf("tim3 int=%d ms\r\n",get_curtime());
        if(pcm_read_index < get_pcm_len())
        {
            set_tim4_pwm_duty(get_pcm_value(pcm_read_index));
            pcm_read_index ++;
        }
        else
        {
            printf("play end,pcm_read_index=%d\r\n",pcm_read_index);
            pcm_buffer_status();
            pcm_read_index = 0;
            play_audio_end();
            TIM_Cmd(TIM3, DISABLE);
            TIM_ITConfig(TIM3,TIM_IT_Update, DISABLE);

        }
#else
        total_times = get_cur_play_times();
        //printf("tim3 int=%d ms,%d:%d\r\n",get_curtime(),cur_times,total_times);
        cur_times ++;
        if(cur_times >= total_times)
        {
            cur_times =0;
            ret = play_cur_tone();
            if(ret == 1)
            {
                TIM_Cmd(TIM3, DISABLE);//??TIM2
                TIM_ITConfig(TIM3,TIM_IT_Update, DISABLE);//??TIM2

            }
        }
#endif


    }
    else
        printf("*****tim3 int=%d ms\r\n",get_curtime());
#endif
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
