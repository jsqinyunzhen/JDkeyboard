/**
  ******************************************************************************
  * @file    ADC/ADC1_DMA/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
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
#include "stm32f10x.h"

#include "project_config.h"

#include <time.h>

#include "st_printf.h"
#include "stm32f10x_it.h"
#include "app4g.h"
#include "st_rtc.h"
#include "clock_calendar.h"
#include "mqtt_app.h"

int8_t status_4g = -1;
//0--off 1--on
void app4g_standby_io_init(void)//power
{

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;						//PB6复用为TIM4的通道1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}
void app4g_standby_io_high(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_0);

}
void app4g_standby_io_low(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_0);
}
void app4g_reset_io_init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;						//PB6复用为TIM4的通道1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}
void app4g_reset_io_high(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_1);

}
void app4g_reset_io_low(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_1);

}
uint32_t start_time_4g = 0;
uint32_t start_time2_4g = 0;
uint32_t end_time_4g = 0;

void app4g_reset_start_time(void)
{
    start_time_4g =get_curtime2();
    return ;
}
void app4g_reset_ok_time(void)
{
    //start_time2_4g =osKernelGetTickCount();
    start_time2_4g =get_curtime2();
    return ;
}

void app4g_run_ok_time(void)
{
    end_time_4g =get_curtime2();
    return ;
}
uint32_t app4g_run_ok_need_time(void)
{

    printf("4g_run_ok %d %d %d, %d\r\n",start_time_4g,start_time2_4g,end_time_4g,end_time_4g?(end_time_4g-start_time_4g)/1000:0);
    return (end_time_4g-start_time_4g)/10;
}

uint16_t uart1_dma_send_data(unsigned char* buffer, uint16_t size)
{
    uint16_t send_size = 0;
    if(!buffer)
        return 0;// 判断长度是否有效
    if(!size)
        return 0;// 判断长度是否有效
    //printf("%s buffer=%s size=%d start\r\n",__func__,buffer,size);
    // while (DMA_GetCurrDataCounter(DMA1_Channel4));// 检查DMA发送通道内是否还有数据
    if(buffer)
    {
        send_size = (size > UART1_TX_BUFFER_LEN?UART1_TX_BUFFER_LEN:size);
        reset_uart1_tx_buffer();
        memcpy(TxBuffer1, buffer,send_size);
        TxCounter1=send_size;
    }
    //printf("send_size=%d\r\n",send_size);
    //DMA发送数据-要先关 设置发送长度 开启DMA
    DMA_Cmd(DMA1_Channel4, DISABLE);
    //DMA1_Channel7->CNDTR = send_size;// 设置发送长度
    DMA_SetCurrDataCounter(DMA1_Channel4,send_size);

    CurrDataCounterEnd_dma1_4 = DMA_GetCurrDataCounter(DMA1_Channel4);
    /* Enable DMA1 Channel6 Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

    DMA_Cmd(DMA1_Channel4, ENABLE);  // 启动DMA发送
    /* Wait the end of transmission */
//	time_start = get_curtime();
    while (CurrDataCounterEnd_dma1_4 != 0 )
    {
        ;
    }


    //printf("%s buffer=%s size=%d start\r\n",__func__,buffer,size);
    return send_size;
}
uint16_t uart2_dma_send_data(unsigned char* buffer, uint16_t size)
{
    uint16_t send_size = 0;
    if(!buffer)
        return 0;// 判断长度是否有效
    if(!size)
        return 0;// 判断长度是否有效
    //printf("%s buffer=%s size=%d start\r\n",__func__,buffer,size);
    //while (DMA_GetCurrDataCounter(DMA1_Channel7));// 检查DMA发送通道内是否还有数据
    if(buffer)
    {
        send_size = (size > UART2_TX_BUFFER_LEN?UART2_TX_BUFFER_LEN:size);
        reset_uart2_tx_buffer();
        memcpy(TxBuffer2, buffer,send_size);
        TxCounter2=send_size;
    }
    //printf("send_size=%d\r\n",send_size);
    //DMA发送数据-要先关 设置发送长度 开启DMA
    DMA_Cmd(DMA1_Channel7, DISABLE);
    //DMA1_Channel7->CNDTR = send_size;// 设置发送长度
    DMA_SetCurrDataCounter(DMA1_Channel7,send_size);
#if 0//(UART2_RX_DMA ==1)
    DMA_Enable(DMA1_Channel7,UART2_RX_BUFFER_LEN);//开启下一次DMA接收
#endif
    CurrDataCounterEnd_dma1_7 = DMA_GetCurrDataCounter(DMA1_Channel7);
    /* Enable DMA1 Channel6 Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

    DMA_Cmd(DMA1_Channel7, ENABLE);	// 启动DMA发送
    /* Wait the end of transmission */
//	time_start = get_curtime();
    while (CurrDataCounterEnd_dma1_7 != 0 )
    {
        ;
    }


//	go out;
//out:
//   printf("send end\r\n");
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, DISABLE);
    DMA_Cmd(DMA1_Channel7, DISABLE);
    reset_uart2_tx_buffer();

    //reset_uart2_rx_buffer();
    //
    //DMA_Enable(DMA1_Channel6,UART2_RX_BUFFER_LEN);//开启下一次DMA接收
    //USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//开启DMA接收


    //printf("%s buffer=%s size=%d start\r\n",__func__,buffer,size);
    return send_size;
}
uint16_t uart3_dma_send_data(unsigned char* buffer, uint16_t size)
{
    uint16_t send_size = 0;
    if(!buffer)
        return 0;// 判断长度是否有效
    if(!size)
        return 0;// 判断长度是否有效
    //printf("%s buffer=%s size=%d start\r\n",__func__,buffer,size);
    //send_size = DMA_GetCurrDataCounter(DMA1_Channel2);
    //printf("uart3 tx de send_size=%d\r\n",send_size);
    //while (DMA_GetCurrDataCounter(DMA1_Channel2));// 检查DMA发送通道内是否还有数据
    if(buffer)
    {
        send_size = (size > UART3_TX_BUFFER_LEN?UART3_TX_BUFFER_LEN:size);
        reset_uart3_tx_buffer();
        memcpy(TxBuffer3, buffer,send_size);
        TxCounter3=send_size;
    }
    //printf("send_size=%d\r\n",send_size);
    //DMA发送数据-要先关 设置发送长度 开启DMA
    DMA_Cmd(DMA1_Channel2, DISABLE);
    //DMA1_Channel7->CNDTR = send_size;// 设置发送长度
    DMA_SetCurrDataCounter(DMA1_Channel2,send_size);
#if 0//(UART3_RX_DMA ==1)
    DMA_Enable(DMA1_Channel3,UART4_RX_BUFFER_LEN);//开启下一次DMA接收
#endif
    CurrDataCounterEnd_dma1_2 = DMA_GetCurrDataCounter(DMA1_Channel2);
    /* Enable DMA1 Channel6 Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

    DMA_Cmd(DMA1_Channel2, ENABLE);	// 启动DMA发送
    /* Wait the end of transmission */
//	time_start = get_curtime();
    while (CurrDataCounterEnd_dma1_2 != 0 )
    {
        ;
    }


//	go out;
//out:
//   printf("send end\r\n");
    //  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);
    //  DMA_Cmd(DMA1_Channel2, DISABLE);
    //  reset_uart3_tx_buffer();

    //reset_uart2_rx_buffer();
    //
    //DMA_Enable(DMA1_Channel6,UART2_RX_BUFFER_LEN);//开启下一次DMA接收
    //USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//开启DMA接收


    //printf("%s buffer=%s size=%d start\r\n",__func__,buffer,size);
    return send_size;
}
uint16_t uart4_dma_send_data(unsigned char* buffer, uint16_t size)
{
    uint16_t send_size = 0;
    if(!buffer)
        return 0;// 判断长度是否有效
    if(!size)
        return 0;// 判断长度是否有效
    //printf("%s buffer=%s size=%d start\r\n",__func__,buffer,size);
    //while (DMA_GetCurrDataCounter(DMA2_Channel5));// 检查DMA发送通道内是否还有数据
    if(buffer)
    {
        send_size = (size > UART4_TX_BUFFER_LEN?UART4_TX_BUFFER_LEN:size);
        reset_uart4_tx_buffer();
        memcpy(TxBuffer4, buffer,send_size);
        TxCounter4=send_size;
    }
    //printf("send_size=%d\r\n",send_size);
    //DMA发送数据-要先关 设置发送长度 开启DMA
    DMA_Cmd(DMA2_Channel5, DISABLE);
    //DMA1_Channel7->CNDTR = send_size;// 设置发送长度
    DMA_SetCurrDataCounter(DMA2_Channel5,send_size);
#if 0//(UART2_RX_DMA ==1)
    DMA_Enable(DMA1_Channel7,UART2_RX_BUFFER_LEN);//开启下一次DMA接收
#endif
    CurrDataCounterEnd_dma2_5 = DMA_GetCurrDataCounter(DMA2_Channel5);
    /* Enable DMA1 Channel6 Transfer Complete interrupt */
    DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, ENABLE);

    DMA_Cmd(DMA2_Channel5, ENABLE);	// 启动DMA发送
    /* Wait the end of transmission */
//	time_start = get_curtime();
    while (CurrDataCounterEnd_dma2_5 != 0 )
    {
        ;
    }


//	go out;
//out:
//   printf("send end\r\n");
    //DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, DISABLE);
    //DMA_Cmd(DMA2_Channel5, DISABLE);
    //reset_uart4_tx_buffer();

    return send_size;
}
int uart3_sendchar(char ch)
{
    USART3->SR;
    USART_SendData(USART3,ch);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);

    return ch;
}

void uart3_cpusendstring( unsigned char *pt,int len)
{

#if 0

    while(*pt)
        sendchar(*pt++);

    //  sendchar('\n');
    UART0_PutStr("\n");


    {

        if(*pt == '\n')
        {

            sendchar('\r');
            pt++ ;
        }
        else
            sendchar(*pt++);
    }
#else

    while((*pt) && (len>0))
    {
        uart3_sendchar(*pt++);
        len-- ;
    }
    // UART0_PutStr("\n");
    uart3_sendchar('\r');
    uart3_sendchar('\n');
#endif
}
void uart3_dma_printf( char *fmt,...)
{
#if(UART3_TX_DMA == 0)
    return;
#else
    /*
    	if(low_power_req_flag ==1)
    	{
    		memset(string,0,100);
    		strcpy(string,"LOWPOWER 1");
    		uart_sendstring(string, strlen("LOWPOWER 1"));
    		return;
    	}
    */
    va_list ap;
    int len=0;
    memset(string,0,LEN);
    va_start(ap,fmt);

    len = vsprintf((char*)string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    if((len >0)&&(len <=LEN))
    {
        //	UART0_PutStr("len succ ");
        //"len =%d",len);
        uart3_dma_send_data(string, len);
    }
    else
    {

        //uart_sendstring(string, LEN);

        //memset(string,0,LEN);
        //strcpy(string,"len error");
        //uart_sendstring(string, strlen("len error"));
        //snprintf(string,LEN-1,"len =%d error",len);

        uart3_dma_send_data(string, LEN-1);
    }
    va_end(ap);
#endif
}
void uart3_cpu_printf( char *fmt,...)
{
    //#if(UART2_TX_DMA == 1)
    //return;
    //#endif

    va_list ap;
    int len=0;
    memset(string,0,LEN);
    va_start(ap,fmt);

    len = vsprintf((char*)string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    if((len >0)&&(len <=LEN))
    {
        //	UART0_PutStr("len succ ");
        //"len =%d",len);
        uart3_cpusendstring(string, len);
    }
    else
    {

        //uart_sendstring(string, LEN);

        //memset(string,0,LEN);
        //strcpy(string,"len error");
        //uart_sendstring(string, strlen("len error"));
        //snprintf(string,LEN-1,"len =%d error",len);

        uart3_cpusendstring(string, LEN-1);
    }
    va_end(ap);

}
int uart4_sendchar(char ch)
{
    UART4->SR;
    USART_SendData(UART4,ch);
    while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);

    return ch;
}

void uart4_cpusendstring( unsigned char *pt,int len)
{

#if 0

    while(*pt)
        sendchar(*pt++);

    //  sendchar('\n');
    UART0_PutStr("\n");


    {

        if(*pt == '\n')
        {

            sendchar('\r');
            pt++ ;
        }
        else
            sendchar(*pt++);
    }
#else

    while((*pt) && (len>0))
    {
        uart4_sendchar(*pt++);
        len-- ;
    }
    // UART0_PutStr("\n");
    uart4_sendchar('\r');
    uart4_sendchar('\n');
#endif
}
void uart4_dma_printf( char *fmt,...)
{
#if(UART4_TX_DMA == 0)
    return;
#else
    /*
    	if(low_power_req_flag ==1)
    	{
    		memset(string,0,100);
    		strcpy(string,"LOWPOWER 1");
    		uart_sendstring(string, strlen("LOWPOWER 1"));
    		return;
    	}
    */
    va_list ap;
    int len=0;
    memset(string,0,LEN);
    va_start(ap,fmt);

    len = vsprintf((char*)string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    if((len >0)&&(len <=LEN))
    {
        //	UART0_PutStr("len succ ");
        //"len =%d",len);
        uart4_dma_send_data(string, len);
    }
    else
    {

        //uart_sendstring(string, LEN);

        //memset(string,0,LEN);
        //strcpy(string,"len error");
        //uart_sendstring(string, strlen("len error"));
        //snprintf(string,LEN-1,"len =%d error",len);

        uart4_dma_send_data(string, LEN-1);
    }
    va_end(ap);
#endif
}
void uart4_cpu_printf( char *fmt,...)
{
    //#if(UART2_TX_DMA == 1)
    //return;
    //#endif

    va_list ap;
    int len=0;
    memset(string,0,LEN);
    va_start(ap,fmt);

    len = vsprintf((char*)string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    if((len >0)&&(len <=LEN))
    {
        //	UART0_PutStr("len succ ");
        //"len =%d",len);
        uart4_cpusendstring(string, len);
    }
    else
    {

        //uart_sendstring(string, LEN);

        //memset(string,0,LEN);
        //strcpy(string,"len error");
        //uart_sendstring(string, strlen("len error"));
        //snprintf(string,LEN-1,"len =%d error",len);

        uart4_cpusendstring(string, LEN-1);
    }
    va_end(ap);

}

void uart1_dma_printf( char *fmt,...)
{
#if(UART1_TX_DMA == 0)
    return;
#else
    /*
    	if(low_power_req_flag ==1)
    	{
    		memset(string,0,100);
    		strcpy(string,"LOWPOWER 1");
    		uart_sendstring(string, strlen("LOWPOWER 1"));
    		return;
    	}
    */
    va_list ap;
    int len=0;
    memset(string,0,LEN);
    va_start(ap,fmt);

    len = vsprintf((char*)string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    if((len >0)&&(len <=LEN))
    {
        //	UART0_PutStr("len succ ");
        //"len =%d",len);
        uart1_dma_send_data(string, len);
    }
    else
    {

        //uart_sendstring(string, LEN);

        //memset(string,0,LEN);
        //strcpy(string,"len error");
        //uart_sendstring(string, strlen("len error"));
        //snprintf(string,LEN-1,"len =%d error",len);

        uart1_dma_send_data(string, LEN-1);
    }
    va_end(ap);
#endif
}
void uart1_cpu_printf( char *fmt,...)  
{ 
	//#if(UART2_TX_DMA == 1)
	//return;
	//#endif

	va_list ap;  
	int len=0;
	 memset(string,0,LEN);
	va_start(ap,fmt);  

	len = vsprintf((char*)string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency  
	if((len >0)&&(len <=LEN))
	{
	//	UART0_PutStr("len succ ");
		//"len =%d",len);
		uart1_sendstring((char*)string, len);  
	}
	else
	{

		//uart_sendstring(string, LEN);  
		
		//memset(string,0,LEN);
		//strcpy(string,"len error");
		//uart_sendstring(string, strlen("len error"));  
		//snprintf(string,LEN-1,"len =%d error",len);

		uart1_sendstring((char*)string, LEN-1);  
	}
	va_end(ap);  

}  

void uart2_dma_printf( char *fmt,...)
{
#if(UART2_TX_DMA == 0)
    return;
#endif
    /*
    	if(low_power_req_flag ==1)
    	{
    		memset(string,0,100);
    		strcpy(string,"LOWPOWER 1");
    		uart_sendstring(string, strlen("LOWPOWER 1"));
    		return;
    	}
    */
    va_list ap;
    int len=0;
    memset(string,0,LEN);
    va_start(ap,fmt);

    len = vsprintf((char*)string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    if((len >0)&&(len <=LEN))
    {
        //	UART0_PutStr("len succ ");
        //"len =%d",len);
        uart2_dma_send_data(string, len);
    }
    else
    {

        //uart_sendstring(string, LEN);

        //memset(string,0,LEN);
        //strcpy(string,"len error");
        //uart_sendstring(string, strlen("len error"));
        //snprintf(string,LEN-1,"len =%d error",len);

        uart2_dma_send_data(string, LEN-1);
    }
    va_end(ap);

}
int uart2_sendchar(char ch)
{
    USART2->SR;
    USART_SendData(USART2,ch);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);

    return ch;
}

void uart2_cpusendstring( unsigned char *pt,int len)
{

#if 0

    while(*pt)
        sendchar(*pt++);

    //  sendchar('\n');
    UART0_PutStr("\n");


    {

        if(*pt == '\n')
        {

            sendchar('\r');
            pt++ ;
        }
        else
            sendchar(*pt++);
    }
#else

    while((*pt) && (len>0))
    {
        uart2_sendchar(*pt++);
        len-- ;
    }
    // UART0_PutStr("\n");
    uart2_sendchar('\r');
    uart2_sendchar('\n');
#endif
}

void uart2_cpu_printf( char *fmt,...)
{
    //#if(UART2_TX_DMA == 1)
    //return;
    //#endif

    va_list ap;
    int len=0;
    memset(string,0,LEN);
    va_start(ap,fmt);

    len = vsprintf((char*)string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency
    if((len >0)&&(len <=LEN))
    {
        //	UART0_PutStr("len succ ");
        //"len =%d",len);
        uart2_cpusendstring(string, len);
    }
    else
    {

        //uart_sendstring(string, LEN);

        //memset(string,0,LEN);
        //strcpy(string,"len error");
        //uart_sendstring(string, strlen("len error"));
        //snprintf(string,LEN-1,"len =%d error",len);

        uart2_cpusendstring(string, LEN-1);
    }
    va_end(ap);

}

void uart2_rec_at_cmd_response(uint8_t *rcv_buf, uint8_t* rcv_len)
{
//    int ret = 0;
//	uint8_t len = 0;
#if 0
    const char*uart_buf = (const char*)RxBuffer2_proc;
    uint8_t uart_buf_len = RxCounter2_proc;

#else
    const char*uart_buf = (const char*)RxBuffer2;
    uint8_t uart_buf_len = RxCounter2;
#endif
    if(uart_buf ==NULL)
    {
        printf("uart_buf ==NULL\r\n");
        goto out;
    }
    if(uart_buf_len ==0)
    {
        printf("uart_buf_len ==0\r\n");
        goto out;
    }
    else
    {
        // printf("uart2 uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
//UART 2 ECHO
        //uart2_cpu_printf("uart2 uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
        if(rcv_buf)
        {
            //if(RxCounter2 > (*rcv_len))
            //	RxCounter2 = *rcv_len;
            memcpy(rcv_buf,uart_buf,uart_buf_len);
            *rcv_len  = uart_buf_len;
        }
        else
            printf("uart2 uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
        /*
        ret=strncmp(uart_buf,MODULE_4G_POWER_OK,strlen(MODULE_4G_POWER_OK));
        if(ret==0)
        {
            status_4g =1;
            printf("MODULE_4G_POWER_OK\r\n");
            goto out;
        }
        */


    }

out:
    reset_uart2_rx_buffer();
#if (UART2_RX_DMA ==1)
    DMA_Enable(DMA1_Channel6,UART2_RX_BUFFER_LEN);//开启下一次DMA接收
#endif

}
void uart2_rec_at_cmd_response_check_ack(uint8_t *rcv_buf, uint8_t* rcv_len, uint8_t* ack)
{
//   int ret = -1;
//	uint8_t len = 0;
#if 0
    const char*uart_buf = (const char*)RxBuffer2_proc;
    uint8_t uart_buf_len = RxCounter2_proc;

#else
    const char*uart_buf = (const char*)RxBuffer2;
    uint8_t uart_buf_len = RxCounter2;
#endif

    if(uart_buf ==NULL)
    {
        printf("uart_buf ==NULL\r\n");
        goto out;
    }
    if(uart_buf_len ==0)
    {
        printf("uart_buf_len ==0\r\n");
        goto out;
    }
    else
    {
        // printf("uart2 uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
//UART 2 ECHO
        //uart2_cpu_printf("uart2 uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
        if(rcv_buf)
        {
            //if(RxCounter2 > (*rcv_len))
            //	RxCounter2 = *rcv_len;
            memcpy(rcv_buf,uart_buf,uart_buf_len);
            *rcv_len  = uart_buf_len;
        }
        if (strlen((const char*)rcv_buf) > 0 && strstr((const char*)rcv_buf,(const char*) ack))
        {
            // ret = 0;
            // break;
        }
        /*
        ret=strncmp(uart_buf,MODULE_4G_POWER_OK,strlen(MODULE_4G_POWER_OK));
        if(ret==0)
        {
            status_4g =1;
            printf("MODULE_4G_POWER_OK\r\n");
            goto out;
        }
        */


    }

out:
    // if(ret ==0)
    {
        reset_uart2_rx_buffer();
#if (UART2_RX_DMA ==1)
        DMA_Enable(DMA1_Channel6,UART2_RX_BUFFER_LEN);//开启下一次DMA接收
#endif
    }

}

/********************************************************************
* 名称：                  AT_cmd
* 功能：                发送数据at命令并获取结果
* 入口参数： cmd:at command    cmd_len:len
*            ret_buf:result    buf_len:result len
*
* 出口参数：        正确返回为0，错误返回为-1
*******************************************************************/
#define LTE_MAX_TRY_TIME    (2)  //读取串口可能失败，最多尝试3次
#define UART2_AT_COMMAND_TIMEOUT (5*1000)
uint8_t cur_at_rec_len = 0;

int AT_cmd(uint8_t *cmd, uint8_t cmd_len, uint8_t *rcv_buf, uint8_t* rcv_len)
{
    //  int fd = -1;                            //文件描述符
    //  char rcv_buf[512] = {0};
    //char send_buf[UART2_TX_BUFFER_LEN]= {0};
    // int rcv_len = 0;
    int count = 0;
    int ret = -1;
    uint16_t send_len = 0;
    cur_at_rec_len = *rcv_len;
    uint32_t flags = 0;
    if(NULL == cmd || cmd_len == 0)
    {
        printf("at err:cmd is NULL\n");
        return -1;
    }

    if(NULL == rcv_buf || rcv_len == NULL)
    {
        printf("at err:cmd is NULL\n");
        return -1;
    }

    if (cmd_len >= UART2_TX_BUFFER_LEN)
    {
        printf("at err:cmd is too long =%d\n",cmd_len);
        return -1;
    }
//	printf("**cmd=%s,cmd_len=%d ,strlen(cmd)=%d**\r\n",cmd,cmd_len,strlen(cmd));
//   snprintf(send_buf, sizeof(send_buf), "%s\r", cmd);
//	memcpy(send_buf,cmd,strlen(cmd));

//    while (count < LTE_MAX_TRY_TIME)
    {
        //tcflush(fd,TCIOFLUSH);
        //printf("send at=%s len=%d\r\n",send_buf,strlen(send_buf));
        //uart2_dma_send_data(RxBuffer1,RxCounter1-len);
        if(cur_at_rec_len != 0)
        {
            printf("cur_at_rec_len=%d\r\n",cur_at_rec_len);
            USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
            USART_DMACmd(USART2, USART_DMAReq_Rx, DISABLE);//关闭DMA接收
            DMA_Cmd(DMA1_Channel6, DISABLE);
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        }
        send_len = uart2_dma_send_data(cmd,cmd_len);

        printf("send start(%s) and rx start,a=%d n=%d\r\n",cmd,send_len,cmd_len);
        flags = EVENT_FLAGS_UART2_TX_COMPLETE|EVENT_FLAGS_UART2;
        printf("*****osEventFlagsWait s flags =0x%08x %d\r\n",flags,get_curtime());
        flags =osEventFlagsWait(evt_id_uart, flags, osFlagsWaitAll, UART2_AT_COMMAND_TIMEOUT);
        //flags =osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2, osFlagsWaitAny, osWaitForever);
        //rx at command response from 4g module
        printf("*****osEventFlagsWait e flags =0x%08x %d\r\n",flags,get_curtime());
        //if(flags == EVENT_FLAGS_UART2_TX_COMPLETE)
        //{
        //printf("send at end,rx...\r\n");
        //}
        if( (flags &EVENT_FLAGS_UART2_TX_COMPLETE) == EVENT_FLAGS_UART2_TX_COMPLETE)
            printf("send  end\r\n");
        else
            printf("send  fail send_len =%d, cmd_len =%d\r\n",send_len, cmd_len);
        //if( (flags &EVENT_FLAGS_UART2) == EVENT_FLAGS_UART2)
        //	printf("rx  end\r\n");
        // if(send_len == cmd_len)
        if( (flags &EVENT_FLAGS_UART2) == EVENT_FLAGS_UART2)
        {
            //printf("rx...\r\n");
            //uint32_t flags =osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2, osFlagsWaitAll, UART2_AT_COMMAND_TIMEOUT);
            //UART2_AT_COMMAND_TIMEOUT);
            //rx at command response from 4g module
            //printf("*****osEventFlagsWait flags =0x%08x\r\n",flags);
            printf("rx 11 end\r\n");
            if(cur_at_rec_len != 0)
            {
                printf("cur_at_rec_len=%d\r\n",cur_at_rec_len);
                USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
                USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//开启DMA接收
                USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
            }
            //if(flags == EVENT_FLAGS_UART2)
            {
                //printf("uart2 rx at command response from 4g moduler\r\n");
                uart2_rec_at_cmd_response(rcv_buf,rcv_len);
                // printf("at recv  data is %s\r\n", rcv_buf);
                /* 有OK表明接收的数据是对的 */
                if (strlen((const char*)rcv_buf) > 0 && strstr((const char*)rcv_buf, "OK"))
                {
                    printf("rx 11 ok\r\n");
                    ret = 0;
                    // break;
                }
                else
                {
                    printf("at err:recv invalid data is %s\r\n", rcv_buf);
                }
            }
            //else
            // {
            //    printf("send at command,but 4g moduler no response\r\n");
            //   printf("at err:recv failed =0x%08x\r\n",flags);
            //}

        }
        else
        {
            printf("at err:rx failed\n");
        }

        count++;
    }

    //sleep(1);
    return ret;
}
//bool sim800c_send_cmd(char *cmd_data, char *ack, uint8_t retry, uint32_t time_out)
int at_cmd_ack(uint8_t *cmd, uint16_t cmd_len, uint8_t *rcv_buf,uint8_t* rcv_len,uint8_t *ack, uint32_t time_out)
{

//    int count = 0;
    int ret = -1;
    uint16_t send_len = 0;
    uint32_t flags = 0;
    if(NULL == cmd || cmd_len == 0)
    {
        printf("at err:cmd is NULL\n");
        return -1;
    }

    if(NULL == rcv_buf || rcv_len == NULL)
    {
        printf("at err:cmd is NULL\n");
        return -1;
    }

    if (cmd_len >= UART2_TX_BUFFER_LEN)
    {
        printf("at err:cmd is too long cmd_len=%d\n",cmd_len);
        return -1;
    }
    cur_at_rec_len = *rcv_len;
    if(ack == NULL)
    {
        ret = AT_cmd(cmd, cmd_len, rcv_buf, rcv_len);
        return ret;
    }
    else
    {

        send_len = uart2_dma_send_data(cmd,cmd_len);

        MQTT_PRINT_DEBUG("send start(%s) and rx start,a=%d n=%d\r\n",cmd,send_len,cmd_len);
        osDelay(100);
        flags = EVENT_FLAGS_UART2_TX_COMPLETE|EVENT_FLAGS_UART2;
        flags =osEventFlagsWait(evt_id_uart, flags, osFlagsWaitAll, UART2_AT_COMMAND_TIMEOUT);
        MQTT_PRINT_DEBUG("*****osEventFlagsWait flags =0x%08x t=%d\r\n",flags,get_curtime());
        if( (flags &EVENT_FLAGS_UART2_TX_COMPLETE) == EVENT_FLAGS_UART2_TX_COMPLETE)
            printf("send  end\r\n");
        else
        {
            printf("send  fail send_len =%d, cmd_len =%d\r\n",send_len, cmd_len);
            goto out;
        }

        if( (flags &EVENT_FLAGS_UART2) == EVENT_FLAGS_UART2)
        {

            uart2_rec_at_cmd_response_check_ack(rcv_buf,rcv_len,ack);
            MQTT_PRINT_DEBUG("rx 1  end,ack=%s,rcv_len=%d\r\n",ack,*rcv_len);
            //uart2_rec_at_cmd_response(rcv_buf,rcv_len);
            /* 有OK表明接收的数据是对的 */
			//strstr
            //if (strlen((const char*)rcv_buf) > 0 && strstr((const char*)rcv_buf, (const char*)ack))
			if (strlen((const char*)rcv_buf) > 0 && dx_memmem((char*)rcv_buf, *rcv_len,(char*)ack,strlen((const char*)ack))>=0)
            {
                ret = 0;
                printf("rx 1  ok\r\n");
                goto out;
            }
			//else if (strlen((const char*)rcv_buf) > 0 && dx_memmem((const char*)rcv_buf, rcv_len,"+QIURC: \"recv\",0",strlen(ack))
            else if (strlen((const char*)rcv_buf) > 0 && strstr((const char*)rcv_buf, "+QIURC: \"recv\",0"))
            {
                ret = 1;
                printf("rx 2 ok,need to get\r\n");
                goto out;
            }
            else if (strlen((const char*)rcv_buf) > 0 && strstr((const char*)rcv_buf, "+QIURC: \"pdpdeact\","))
            {
                ret = 2;
                printf("rx 3 ok,but error\r\n");
                goto out;
            }
            else
            {
                ret = -2;
                //printf("at err:recv no ack data is %s ,times=%d\n", rcv_buf,times);
                
                printf("at err:recv len=%d,no ack data is %s\r\n", *rcv_len,rcv_buf);
				printf("strlen((const char*)rcv_buf)=%d\r\n",strlen((const char*)rcv_buf));
#if 1
				int i =0;
				for(i=0; i<*rcv_len; i++)
				{
					printf("0x%02x,",rcv_buf[i]);
					if((i+1)%10 == 0)
						printf("\r\n");
				}
				printf("\r\n");
#endif


            }

        }

        //{
        printf("rec_wait..\r\n");
        unsigned int times = time_out/1000;
rec_wait:
//			reset_uart2_rx_buffer();
#if (UART2_RX_DMA ==1)
//			DMA_Enable(DMA1_Channel6,UART2_RX_BUFFER_LEN);//开启下一次DMA接收
#endif

        while(times)
        {
            times --;
            uint32_t flags =osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2, osFlagsWaitAll, 1000);
            //UART2_AT_COMMAND_TIMEOUT);
            //rx at command response from 4g module
            MQTT_PRINT_DEBUG("*****osEventFlagsWait flags =0x%08x t=%d\r\n",flags,get_curtime());

            if( flags  == EVENT_FLAGS_UART2)
            {
                //printf("uart2 rx at command response from 4g moduler\r\n");

                uart2_rec_at_cmd_response_check_ack(rcv_buf,rcv_len,ack);
                //uart2_rec_at_cmd_response(rcv_buf,rcv_len);
                /* 有OK表明接收的数据是对的 */
                if (strlen((const char*)rcv_buf) > 0 && strstr((const char*)rcv_buf, (const char*)ack))
                {
                    ret = 0;
                    printf("rx 4  ok\r\n");
                    break;
                }
                else if (strlen((const char*)rcv_buf) > 0 && strstr((const char*)rcv_buf, "+QIURC: \"recv\",0"))
                {
                    ret = 1;
                    printf("rx 5 ok,need to get\r\n");
                    goto out;
                }
                else if (strlen((const char*)rcv_buf) > 0 && strstr((const char*)rcv_buf, "+QIURC: \"pdpdeact\","))
                {
                    ret = 2;
                    printf("rx 6 ok,but error\r\n");
                    goto out;
                }

                else
                {
                    ret = -2;
                    printf("at err:recv no ack data is %s ,times=%d\n", rcv_buf,times);

                    goto rec_wait;
                }
            }
            else
            {
                ret = -3;
                printf("send at command,but 4g moduler no response\r\n");
                printf("at err:recv failed =0x%08x ,times=%d\r\n",flags,times);
            }
        }


        //}

        //count++;
    }

out:
    reset_uart2_rx_buffer();
#if (UART2_RX_DMA ==1)
    DMA_Enable(DMA1_Channel6,UART2_RX_BUFFER_LEN);//开启下一次DMA接收
#endif

    //sleep(1);
    return ret;
}

char g_sim_ccid[SIM_CCID_LEN] ;
char g_imei[16] = {0};
char g_imsi[16] = {0};

#if 0
AT+GSN
865860040521701

OK

#endif
int dx_check_imei(char *imei)
{
    int i = 0;
    int imei_len = strlen(imei);
    if (imei_len < 15)
    {
        printf("imei_len= %d\r\n", imei_len);
        return 0;
    }

    for(i = 0; i < imei_len; i++)
    {
        if (imei[i] < '0' || imei[i] > '9')
        {
            printf("data0-9 is %d\r\n", i);
            return 0;
        }
    }

    return 1;
}
uint8_t at_tx_buffer[UART2_TX_BUFFER_LEN] = {0};
uint8_t at_rx_buffer[UART2_RX_BUFFER_LEN] = {0};

void dx_get_lte_imei(void)
{
    // char cmd[64] = {0};
    //char result[512] = {0};
    int ret = -1;
    char *pcTmp = NULL;
    char *pcOut = g_imei;
    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;
    printf("g_imei:%s\n", g_imei);
    if (dx_check_imei(g_imei))
    {
        // snprintf(imei, imei_len, "%s", g_imei);
        printf("imei haved get ,is %s\r\n",g_imei);
        return;
    }
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset((char*)at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, "%s",AT_GSN);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result,&result_len);
    /*
    	AT+GSN
    	865860040521701

    	OK

    */
    printf("ret=%d \r\n", ret);
    printf("strstr(result, OK=0x%08x \r\n", (unsigned int)strstr((const char*)result, "OK"));
    printf("strstr(result, GSN=0x%08x \r\n", (unsigned int)strstr((const char*)result, "GSN"));
    /* 过滤掉错误的数据 */
#if 0
    //  if ((ret ==0) && strstr((const char*)result, "OK") && !strstr((const char*)result, "GSN"))
    if ((ret ==0) && strstr((const char*)result, "OK") )
    {
        // memset(result, 0, sizeof(result));
        // ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
        printf("GSN read ok data %s----\n", result);
        printf("AT+GSN:\n%s\n", result);
        // goto out ;
    }
#endif
    if (ret ==0)
    {

        if(strstr((const char*)result, "OK") ==NULL )
        {
            // memset(result, 0, sizeof(result));
            // ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
            printf("GSN read error data1 %s----\n", result);
            printf("AT+GSN:\n%s\n", result);
            goto out ;
        }
#if 0
        else if(!strstr((const char*)result, "GSN"))
        {
            // memset(result, 0, sizeof(result));
            // ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
            printf("GSN read error data2 %s----\n", result);
            printf("AT+GSN:\n%s\n", result);
            goto out ;
        }
#endif
    }
    else
    {
        printf("AT+GSN fail\n");
        goto out ;
    }
    /*
    if (ret || !strlen(result) || strstr(result, "OK"))
    {
        snprintf(g_imei, sizeof(g_imei), "000000000000000");
        return;
    }
    */
    printf("\n result =%s\n", result);
    pcTmp = strchr((const char*)result, '\n');
    if (pcTmp)
    {
        pcTmp++;
        while(*pcTmp != '\r' && *pcTmp != '\n' && *pcTmp != '\0' && (*pcTmp >= '0' && *pcTmp <= '9'))
        {
            //printf("*pcTmp=%c\r\n",*pcTmp);
            *pcOut++ = *pcTmp++;
        }
    }

    if (strlen(g_imei) < 15)
    {
        snprintf(g_imei, sizeof(g_imei), "000000000000000");
        goto out ;
    }
    else
    {
        //memset(g_imei, 0, sizeof(g_imei));
        // snprintf(g_imei, sizeof(g_imei), "%s", imei);
        printf("g_imei=%s\r\n",g_imei);
    }
out:
//    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
//    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);

    return;
}
/*
 0	-113dBm or less
 1	-111dBm
 2~30  -109~-53dBm
 31 -51dBm or greater
 99 Not known or not detectable

 rssi= -113 + (X * 2)

 >-80dbm   4格信号
 -90~-80   3格信号
 -105~-90	2格信号
 <-105		1格信号
 other	   0
*/

int dx_get_lte_signalQuality(void)
{
    // char cmd[64] = {0};
    //char result[512] = {0};
    int ret = -1;
    char *pcTmp = NULL;
    char sigal[32] = {0};
    char *pcSigal = sigal;
    int rssi = 0;
    int index = 0;
    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;

    // snprintf(cmd, sizeof(cmd), "AT+CSQ");
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_CSQ);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    /*
        AT+CSQ
        +CSQ: 19,99

        OK
    */
    //  printf("AT+CSQ:\n%s\n", result);
    printf("\n result =%s\n", result);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        ret = 0;
        goto out ;
    }

    pcTmp = strchr((const char*)result, ' ');
    if (pcTmp)
    {
        pcTmp++;
        while(*pcTmp != ',' && *pcTmp >= '0' && *pcTmp <= '9' &&  *pcTmp != '\0')
        {
            *pcSigal++ = *pcTmp++;
        }
    }
    else
    {
        ret = 0;
        goto out ;
    }

    *pcSigal = '\0';
    index = atoi(sigal);
    if (index < 0 || index >= 32)
    {
        ret = 0;
        return 0;
    }

    rssi = -113 + (index * 2);
    printf("index:%d rssi:%d\n", index, rssi);

    if (rssi >= -80)
    {
        // return 4;
        ret =4;
        return ret;
    }
    else if (-90 <= rssi && -80 > rssi)
    {
        // return 3;
        ret =3;
        return ret;
    }
    else if (-105 <= rssi && -90 > rssi)
    {
        //  return 2;
        ret =2;
        return ret;
    }
    else if (-105 > rssi)
    {
        ret =1;
        return ret;
    }
out:
    //	memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    //	memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    return ret;

}
int dx_get_lte_pinCount(void)
{
    //  char cmd[64] = {0};
    // char result[512] = {0};
    int ret = -1;
    char *pcTmp = NULL;
    char countStr[32] = {0};
    char *pcCount = countStr;
    int count = 0;

    //snprintf(cmd, sizeof(cmd), "AT+QPINC?");
    //ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;


    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QPINC_READ);
    ret = AT_cmd(cmd, strlen((const char *)cmd), result, &result_len);

    /*
        AT+QPINC?
        +QPINC: "SC",3,10
        +QPINC: "P2",3,10

        OK
    */
    printf("\n%s\n", result);
    if (ret || !strlen((const char *)result) || !strstr((const char *)result, "OK"))
    {
        return 3;
    }

    pcTmp = strchr((const char *)result, ',');
    if (pcTmp)
    {
        pcTmp++;
        while(*pcTmp != ',' && *pcTmp >= '0' && *pcTmp <= '9' &&  *pcTmp != '\0')
        {
            *pcCount++ = *pcTmp++;
        }

        count = atoi(countStr);
        printf("pin left count:%d\n", count);
    }
    else
    {
        return 3;
    }

    if (count < 0)
    {
        count = 0;
    }

    return count;
}

/*
    0:donot need pin
    1:need pin to unlock
    2:other error
*/
int dx_get_lte_pinState(void)
{
    //  char cmd[64] = {0};
    //  char result[512] = {0};
    int ret = 0;

    //  snprintf(cmd, sizeof(cmd), "AT+CPIN?");
    //  ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
    unsigned char  *cmd = at_tx_buffer;
    unsigned  char *result = at_rx_buffer;
    unsigned char result_len = 0;


    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_CPIN_READ);
    ret = AT_cmd(cmd, strlen((const char *)cmd), result, &result_len);

    /*
        AT+CPIN?
        +CPIN: READY

        OK
    */
    printf("\n%s\n", result);
    if (ret || !strlen((const char *)result) || !strstr((const char *)result, "OK"))
    {
        return 2;
    }

    if (strstr((const char *)result, "READY"))
    {
        return 0;
    }
    else if((strstr((const char *)result, "SIM PIN") || strstr((const char *)result, "SIM PUK")) && !strstr((const char *)result, "SIM PIN2"))
    {
        return 1;
    }

    return 2;
}


/*
    0:pin is disable
    1:pin is enable
    2:other error
*/

int dx_get_lte_pinSwitch(void)
{
//   char cmd[64] = {0};
//   char result[512] = {0};
    int ret = -1;
//    char *pcTmp = NULL;
//    char countStr[32] = {0};
//    char *pcCount = countStr;
//    int count = 0;

    //  snprintf(cmd, sizeof(cmd), "AT+CLCK=\"SC\",2");
    //  ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;


    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    //snprintf(at_tx_buffer,UART2_TX_BUFFER_LEN, AT_CLCK_READ);
    snprintf((char  *)cmd, sizeof(cmd), "AT+CLCK=\"SC\",2");
    ret = AT_cmd(cmd, strlen((const char  *)cmd), result, &result_len);

    /*
        AT+CLCK="SC",2
        +CLCK: 1

        OK
    */
    printf("\n%s\n", result);
    if (ret || !strlen((const char  *)result) || !strstr((const char  *)result, "OK"))
    {
        return 2;
    }

    if (strstr((const char  *)result, "+CLCK: 1"))
    {
        return 1;
    }
    else if(strstr((const char  *)result, "+CLCK: 0"))
    {
        return 0;
    }

    return 2;
}



int dx_get_lte_network_resiger_status(void)
{
    //char cmd[64] = {0};
    // char result[512] = {0};
    int ret = -1;

    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_CGREG_READ);

    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    /*
        AT+CGREG?
        +CGREG: 0,5

        OK
    */
    printf("cmd:%s result:%s\r\n",cmd, result);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        return 0;
    }
    if (strstr((const char*)result, ",1"))
    {
        printf("network_resiger_status ok 1\r\n");
        return 1;
    }

    else if (strstr((const char*)result, ",5"))
    {
        printf("network_resiger_status ok 5\r\n");
        return 1;
    }
    else
    {
        printf("network_resiger_fail(1or5)\r\n");
    }
    return 0;


}

int dx_interface_get_imsi(char *imsi, int len)
{
    //  char cmd[64] = {0};
    //  char result[512] = {0};
    int ret = -1;
    char *pcTmp = NULL;
    char *pcOut = imsi;

    // snprintf(cmd, sizeof(cmd), "AT+CIMI");
    // ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_CIMI);
    ret = AT_cmd(cmd, strlen((const char *)cmd), result, &result_len);

    /*
        AT+CIMI
        460110192279581

        OK
    */
    //printf("\n%s\n", result);
    if (ret || !strlen((const char *)result) || !strstr((const char *)result, "OK"))
    {
        return -1;
    }

    pcTmp = strchr((const char *)result, '\n');
    if (pcTmp)
    {
        pcTmp++;
        while(*pcTmp != '\r' && *pcTmp != '\n' && *pcTmp != '\0' )
        {
            *pcOut++ = *pcTmp++;
        }
    }

    printf("imsi:%s\n", imsi);
    return 0;
}

/*
    0:ok
    1:sim is not insert
    2:other error
*/
int dx_lte_traffic_GetLteStatus(void)
{
//   char cmd[64] = {0};
//   char result[512] = {0};
    int ret = -1;

    /* 1.检查是否有插卡 */
    // snprintf(cmd, sizeof(cmd), "AT+QSIMSTAT?");
    // ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QSIMSTAT_READ);
    ret = AT_cmd(cmd, strlen((const char *)cmd), result, &result_len);

    /*
        AT+QSIMSTAT?
        +QSIMSTAT: 0,1

        OK
    */
    printf("\n%s\n", result);
    if (ret != 0 || !strlen((const char *)result) || !strstr((const char *)result, "OK"))
    {
        return 2;
    }

    if (!strstr((const char *)result, ",1"))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int dx_lte_traffic_GetCCID(char *ccid, int ccid_len)
{
    //  char cmd[64] = {0};
    // char result[512] = {0};
    int ret = -1;
    char *pcTmp = NULL;
    char *pcOut = ccid;

    if (ccid_len < SIM_CCID_LEN)
    {
        return -1;
    }


    /* 1.检查是否有插卡 */
    // snprintf(cmd, sizeof(cmd), "AT+QCCID");
    //  ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned  char result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QCCID);
    ret = AT_cmd(cmd, strlen((const char *)cmd), result, &result_len);
    /*
        AT+QCCID
        +QCCID: 89860318045712762387

        OK
    */
    //printf("\n%s\n", result);
    if (!ret && strlen((const char *)result) && strstr((const char *)result, "OK"))
    {
        pcTmp = strchr((const char *)result, ' ');
        pcTmp++;
        while(*pcTmp != '\r' && *pcTmp != '\n' && *pcTmp != '\0' )
        {
            *pcOut++ = *pcTmp++;
        }

        *pcOut = '\0';
        if (strlen(ccid) != SIM_CCID_LEN)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }

    return -1;
}
//
void dx_get_lte_network(char *network, int network_len)
{
    // char cmd[64] = {0};
    //char result[512] = {0};
    int ret = -1;
    char *pcTmp = NULL;
    char *pcnetwork = network;

    //  snprintf(cmd, sizeof(cmd), "AT+COPS?");
    // ret = AT_cmd(cmd, strlen(cmd), result, sizeof(result));
    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_COPS_READ);
    ret = AT_cmd(cmd, strlen((const char *)cmd), result, &result_len);

    /*
    	+COPS: 0,0,"CHINA MOBILE",7

    	OK

    	AT+COPS
    	AT+COPS?
    	+COPS: 0,0,"CHINA MOBILE",7

    	OK

    	dx_get_lte_network=0 CHINA MOBILE


    +COPS: 0,0,"CHINA MOBILE",7

    */

    if (ret || !strlen((const char *)result) || !strstr((const char *)result, "OK"))
    {
        return ;
    }
    printf("AT+COPS\n%s\n", result);
    pcTmp = strchr((const char *)result, '"');
    if (pcTmp)
    {
        pcTmp++;
        while(*pcTmp != '"' && *pcTmp != '\0')
        {
            *pcnetwork++ = *pcTmp++;
        }
    }

    *pcnetwork = '\0';

    return;
}


/*

NONE

3G:
CDMA1X
CDMA1X AND HDR
CDMA1X AND EHRPD
HDR
HDR-EHRPD

2G:
GSM
GPRS
EDGE

3G:
WCDMA
HSDPA
HSUPA
HSPA+
TDSCDMA

4G:
TDD LTE
FDD LTE

*/

void dx_get_lte_datacap(char *datacap, int datacap_len)
{
    // char cmd[64] = {0};
    // char result[512] = {0};
    int ret = -1;
    char *pcTmp = NULL;
    char cap[64] = {0};
    char *pcdatacap = cap;

    // snprintf(cmd, sizeof(cmd), "AT+QNWINFO");
    /* AT+QNWINFO命令经常长时间获取不到结果会导致页面卡住，因此改为只获取一次，获取不到的话直接返回4G */
    //ret = AT_cmd_once(cmd, strlen(cmd), result, sizeof(result));

    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QNWINFO);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);

    /*
        AT+QNWINFO
        +QNWINFO: "TDD LTE","46000","LTE BAND 40",38950

        OK
    */

    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        snprintf(datacap, datacap_len, "unknow");
        return ;
    }
    printf("AT+QNWINFO:\n%s\n", result);
    pcTmp = strchr((const char*)result, '"');
    if (pcTmp)
    {
        pcTmp++;
        while(*pcTmp != '"' && *pcTmp != '\0')
        {
            *pcdatacap++ = *pcTmp++;
        }
    }

    *pcdatacap = '\0';

    if (strlen(cap))
    {
        if (strstr(cap, "NONE"))
        {
            snprintf(datacap, datacap_len, "unknow");
        }
        else if (strstr(cap, "LTE"))
        {
            snprintf(datacap, datacap_len, "4G");
        }
        else if (strstr(cap, "GSM") || strstr(cap, "GPRS") || strstr(cap, "EDGE") )
        {
            snprintf(datacap, datacap_len, "2G");
        }
        else
        {
            snprintf(datacap, datacap_len, "3G");
        }
    }
    else
    {
        snprintf(datacap, datacap_len, "unknow");
    }

    return;
}


int myantoi(char *p,int len)
{
    int temp = 0;
    while(len)
    {
        // printf("*p=%c\r\n",*p);
        temp *= 10;
        temp += (*p++ - '0');
        //printf("temp=%d\r\n",temp);

        len--;
    }
    //printf("temp=%d\r\n",temp);
    return temp;
}
int mytransfor(char *src,myst *pt)
{
    char *p;
    p = src;

    pt->year = myantoi(p,2);
    p += 3;
    pt->mon = myantoi(p,2);
    p += 3;
    pt->day = myantoi(p,2);
    p += 3;
    pt->hour = myantoi(p,2);
    p += 3;
    pt->min = myantoi(p,2);
    p += 3;
    pt->sec = myantoi(p,2);
    p += 2;
    if(*p == '+')
        pt->tzone = myantoi(p+1,2)/4;
    else if(*p == '-')
        pt->tzone =  (myantoi(p+1,2)/4) * (-1);

    //  printf("year:%d mon:%d day:%d hour:%d min:%d sec:%d tzone:%d\r\n", \
    pt->year,pt->mon,pt->day,pt->hour,pt->min,pt->sec,pt->tzone);
    return 0;
}
void time_trans_test(void)
{
    char *p = "19/09/03,10:40:33+32";
    myst  pt;

    mytransfor(p,&pt);
    printf("year:%d mon:%d day:%d hour:%d min:%d sec:%d tzone:%d\n",pt.year,pt.mon,pt.day,pt.hour,pt.min,pt.sec,pt.tzone);
}
int dx_set_lte_ntp_server(void)
{
    // char cmd[64] = {0};
    // char result[512] = {0};
    int ret = -1;
    int contextid = QNTP_UDP_CONTEXT_ID;
    //char *pcTmp = NULL;
    //char cap[64] = {0};
    //char *pcdatacap = cap;

    // snprintf(cmd, sizeof(cmd), "AT+QNWINFO");
    /* AT+QNWINFO命令经常长时间获取不到结果会导致页面卡住，因此改为只获取一次，获取不到的话直接返回4G */
    //ret = AT_cmd_once(cmd, strlen(cmd), result, sizeof(result));

    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;

    //QISTATE
    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    //ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    ret =at_cmd_ack(cmd, strlen((const char*)cmd),result,&result_len,"OK", 3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }
#if 0
    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QIACT=1\r\n");
    //ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    ret =at_cmd_ack(cmd, strlen((const char*)cmd),result,&result_len,"OK", 3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }
#endif
    //QISTATE
    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    //ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    ret =at_cmd_ack(cmd, strlen((const char*)cmd),result,&result_len,"OK", 3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }

    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    // snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QNTP_WRITE);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QNTP=%d,\"182.92.12.11\",123\r\n",contextid);
    // ret = AT_cmd(cmd, strlen((const char *)cmd), result, &result_len);
    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,"+QNTP: 0,",5000);
    /*
        AT+QNWINFO
        +QNWINFO: "TDD LTE","46000","LTE BAND 40",38950

        OK
    */

    //  if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "+QNTP: 0,"))
    {
        // snprintf(datacap, datacap_len, "4G");
        printf("QNTP ret=%d\r\n",ret);
        return ret;
    }
    printf("AT+QNTP:%s\n", result);

    return 0;
}
int dx_set_lte_ate0(void)
{
    // char cmd[64] = {0};
    // char result[512] = {0};
    int ret = -1;
    //char *pcTmp = NULL;
    //char cap[64] = {0};
    //char *pcdatacap = cap;

    // snprintf(cmd, sizeof(cmd), "AT+QNWINFO");
    /* AT+QNWINFO命令经常长时间获取不到结果会导致页面卡住，因此改为只获取一次，获取不到的话直接返回4G */
    //ret = AT_cmd_once(cmd, strlen(cmd), result, sizeof(result));

    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
    unsigned char result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, ATE0);
    //snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, ATE1);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);

    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");

        return ret;
    }
    printf("ATE0:\n%s\n",result);

    return 0;
}

int dx_get_lte_ntp_time(char *datacap, int datacap_len)
{
    // char cmd[64] = {0};
    // char result[512] = {0};
    int ret = -1;
    char *pcTmp = NULL;
    //char cap[64] = {0};
    // char *pcdatacap = cap;
//    char ch =0;
    // snprintf(cmd, sizeof(cmd), "AT+QNWINFO");
    /* AT+QNWINFO命令经常长时间获取不到结果会导致页面卡住，因此改为只获取一次，获取不到的话直接返回4G */
    //ret = AT_cmd_once(cmd, strlen(cmd), result, sizeof(result));

    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
    //char result_len = 48;
    uint8_t result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_CCLK_READ);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);

    /*
        AT+QNWINFO
        +QNWINFO: "TDD LTE","46000","LTE BAND 40",38950

        OK
    */
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }
    //printf("AT+CCLK:\n%s\n", result);
#if 1
    pcTmp = strstr((const char*)result, "+CCLK: ");
#else
    ch = '"';
    pcTmp = strchr((const char*)result, ch);
#endif
    //printf("|%c| 之后的字符串是 - |%s|\n", ch, pcTmp);
    //|"| 之后的字符串是 - |"19/09/04,05:57:24+32"
    if (pcTmp)
    {
        pcTmp = pcTmp +strlen("+CCLK: \"");
        printf("pcTmp=%s\r\n", pcTmp);
        if(datacap)
        {
            memcpy(datacap,pcTmp,21);
            printf("ntp time is:%s\r\n", datacap);
        }
        return 0;
    }
    else
        return 1;
}

int dx_tcp_test(void)
{
    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
    int ret = -1;
    uint8_t result_len = 0;
    int connectid= 0;
    int error= 0;

    //QISTATE
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }

    printf("AT_QIOPEN start\r\n");
    //QIOPEN
    result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QIOPEN);
#if 1
    ret =at_cmd_ack(cmd, strlen((const char*)cmd),result,&result_len,"QIOPEN", 2000);
    //+QIOPEN: 0,0
    if (ret ==0 )
    {
        char i = 0;
        printf("result=%s result_len=%d\r\n",result,result_len);
        char* pstr= strstr((const char*)result, ":");
        printf("pstr=%s strlen(pstr)=%d\r\n",pstr,strlen(pstr));
        for(i=0; i<strlen(pstr); i++)
            printf("*%d:%c=0x%x,",i,*(pstr+i),*(pstr+i));


        connectid= *(pstr+2)-'0';
        error= *(pstr+4)-'0';
        printf("connectid=%d error=%d\r\n",connectid,error);
        //return -1;
    }

#else
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }
#endif
    printf("QISTATE start\r\n");
    //QISTATE
    result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }

    result_len = 0;

    //AT_QICLOSE
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QICLOSE=%d\r\n",connectid);
    printf("at_tx_buffer=%s\r\n",at_tx_buffer);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }

    result_len = 0;
    //QISTATE
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }
    return 0;
}

int dx_lte_ping_demo(void)
{

    int ret = -1;

    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;
    const char  *ack = "QPING: 0,4,4";
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QPING);

    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        // snprintf(datacap, datacap_len, "4G");

        return ret;
    }
    // printf("AT_QPING:%s\n", result);

    return 0;
}

int dx_get_lte_qiact(void)
{

    int ret = -1;


    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
    unsigned char result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QIACT);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);

    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");

        return ret;
    }
    // printf("ATE0:\n%s\n",result);

    return 0;
}
//Configure Parameters of a TCP/IP Context
int dx_set_lte_pdp(void)
{

    int ret = -1;


    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
    unsigned char result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    //snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QICSGP=1,1,\"UNINET\",\"\",\"\",1\r\n");
    snprintf((char*)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n");
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);

    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");

        return ret;
    }
    // printf("ATE0:\n%s\n",result);

    return 0;
}

int dx_lte_init_check(void)
{
    //rx at command response from 4g module
    //printf("uart2 rx at command *ext* response from 4g moduler\r\n");
    //uart2_dma_send_data("uart2_dma_send test\r\n",strlen("uart2_dma_send test\r\n"));
    uint32_t flags = 0;
    flags = EVENT_FLAGS_UART2|EVENT_FLAGS_UART2_TX_COMPLETE;
	printf("*****dx_lte_init_check\r\n");
    flags = osEventFlagsWait(evt_id_uart, flags, osFlagsWaitAny, osWaitForever);
    printf("*****dx_lte_init_check cb osEventFlagsWait flags =0x%08x\r\n",flags);
    if(status_4g == -1)
    {
        uint8_t rcv_buf[30]= {0};
        uint8_t rcv_len = 0;
        int ret = 0;
        uart2_rec_at_cmd_response(rcv_buf,&rcv_len);
        printf("rcv_buf=%s rcv_len=%d\r\n",rcv_buf,rcv_len);

        ret=strncmp(( const char *)rcv_buf,MODULE_4G_PDPDEACT,strlen(MODULE_4G_PDPDEACT));
        if(ret==0)
        {
            printf("***MODULE_4G_PDPDEACT****\r\n");
        }

        ret=strncmp(( const char *)rcv_buf,MODULE_4G_POWER_OK,strlen(MODULE_4G_POWER_OK));
        //+QIURC: "pdpdeact",1
        if(ret==0)
        {

            app4g_reset_ok_time();
            char buffer[22]= {0};
#if (TIME_VALID_JUDGE == 1)
            myst  *pt;
            myst  src_pt;
            myst  dst_pt;
#endif
            char times =3;

            status_4g =1;
            printf("MODULE_4G_POWER_OK\r\n");
            printf("dx_set_lte_ate0\r\n");
            dx_set_lte_ate0();
            while(1)
            {
                ret = dx_get_lte_pinState();
                if(ret ==0)
                    break;
                else
                {
                    printf("please insert sim card %d\r\n",ret);
                    osDelay(2000);
                }
            }
            dx_get_lte_network(buffer,sizeof(buffer));
            printf("dx_get_lte_network=%d %s\r\n",ret,buffer);
            times = 0;
            while(1)
            {
                times ++;
                osDelay(2000);
                ret = dx_get_lte_network_resiger_status();
                printf("dx_get_lte_network_resiger_status times=%d =%d\r\n",times,ret);
                if(ret ==1)
                    break;
            }
            ret = dx_get_lte_signalQuality();
            printf("dx_get_lte_signalQuality=%d\r\n",ret);
            app4g_run_ok_time();
            dx_get_lte_imei();

            dx_get_lte_qiact();
            //dx_set_lte_pdp();

            dx_set_lte_ntp_server();
            times =3;
            while(times--)
            {
                osDelay(200);
                ret = dx_get_lte_ntp_time(buffer,22);
                if(ret == 0)
                {

                    printf("dx_get_lte_ntp_time %d=%d %s\r\n",times,ret,buffer);
                    break;
                }
            }
            if(ret != 0)
            {
                printf("ntp server synv fail\r\n");
                // continue;
            }

            pt= &src_pt;
            mytransfor(buffer,pt);
            printf("src year:%d mon:%d day:%d hour:%d min:%d sec:%d tzone:%d\r\n", \
                   pt->year,pt->mon,pt->day,pt->hour,pt->min,pt->sec,pt->tzone);
#if (STM_RTC_ENABLE == 1)
            rtc_set_cnt(time_hms2s(pt->hour+pt->tzone,pt->min,pt->sec));

            SetDate(pt->day,pt->mon,pt->year);
#endif

            time_t seconds = 0;
            time_t src_seconds = 0;
            struct tm dst_tm= {0};
            struct tm *tm_now;

            dst_tm.tm_sec =pt->sec;
            dst_tm.tm_min =pt->min;
            dst_tm.tm_hour =pt->hour;
            dst_tm.tm_mday =pt->day;
            dst_tm.tm_mon =pt->mon;
            dst_tm.tm_year = pt->year+2000-1900;

            src_seconds=mktime(&dst_tm);
            printf("unix cur timestamp:%08x \r\n",src_seconds);
            tm_now = localtime(&src_seconds);
            printf("cur tm_now =%d-%d-%d %d:%d:%d \r\n",	   \
                   tm_now->tm_year+1900, tm_now->tm_mon, tm_now->tm_mday,  \
                   tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);

            pt= &dst_pt;
            mytransfor(DEAD_TIME,pt);
            printf("dst year:%d mon:%d day:%d hour:%d min:%d sec:%d tzone:%d\r\n", \
                   pt->year,pt->mon,pt->day,pt->hour,pt->min,pt->sec,pt->tzone);



            //seconds = time(NULL);
            tm_now = localtime(&seconds);
            printf("tm_now =%d-%d-%d %d:%d:%d \r\n",	   \
                   tm_now->tm_year, tm_now->tm_mon, tm_now->tm_mday,  \
                   tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
            //tm_now =70-0-1 0:0:0
            //从1970年1月1日0时0分0秒到此时的秒数。
#if 0
            struct tm {

                int tm_sec; 	 /* 秒 – 取值区间为[0,59] */

                int tm_min; 	 /* 分 - 取值区间为[0,59] */

                int tm_hour;	 /* 时 - 取值区间为[0,23] */

                int tm_mday;	/* 一个月中的日期 - 取值区间为[1,31] */

                int tm_mon;    /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */

                int tm_year;	/* 年份，其值等于实际年份减去1900 */

                int tm_wday;	/* 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一 */

                int tm_yday;	/* 从每年1月1日开始的天数– 取值区间[0,365]，其中0代表1月1日 */

                int tm_isdst;	/* 夏令时标识符，夏令时tm_isdst为正；不实行夏令时tm_isdst为0 */

            };

#endif
            dst_tm.tm_sec =pt->sec;
            dst_tm.tm_min =pt->min;
            dst_tm.tm_hour =pt->hour;
            dst_tm.tm_mday =pt->day;
            dst_tm.tm_mon =pt->mon;
            dst_tm.tm_year = pt->year+2000-1900;
            seconds=mktime(&dst_tm);
            printf("unix timestamp:%08x \r\n",seconds);
            tm_now = localtime(&seconds);
            printf("tm_now =%d-%d-%d %d:%d:%d \r\n",	   \
                   tm_now->tm_year+1900, tm_now->tm_mon, tm_now->tm_mday,  \
                   tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
#if (TIME_VALID_JUDGE == 1)

            if(src_seconds > seconds)
            {
                printf("mcu_sys_soft_reset,please 缴费\r\n");
                osDelay(20000);
                mcu_sys_soft_reset();
                while(1);
            }

#endif

            module4g_init =1;
            //dx_get_lte_imei();
            //mqtt_set(g_imei);
            //app_mqtt_init();
        }

    }
    //else
    //    uart2_rec_at_cmd_response(NULL,NULL);

    return 0;
}
//app4g_AT+CGDCONT=1,"IP","cmnet"
void dc_lte_runtime_valid_judge(void)
{

#if (TIME_VALID_JUDGE == 1)
    int ret = 0;
    char buffer[22]= {0};
    myst  *pt;
    myst  src_pt;
    myst  dst_pt;

    ret = dx_get_lte_ntp_time(buffer,22);

    if(ret != 0)
    {
        printf("ntp server synv fail\r\n");
        return;
    }

    pt= &src_pt;
    mytransfor(buffer,pt);
    printf("src year:%d mon:%d day:%d hour:%d min:%d sec:%d tzone:%d\r\n", \
           pt->year,pt->mon,pt->day,pt->hour,pt->min,pt->sec,pt->tzone);

    time_t seconds = 0;
    time_t src_seconds = 0;
    struct tm dst_tm= {0};
    struct tm *tm_now;

    dst_tm.tm_sec =pt->sec;
    dst_tm.tm_min =pt->min;
    dst_tm.tm_hour =pt->hour;
    dst_tm.tm_mday =pt->day;
    dst_tm.tm_mon =pt->mon;
    dst_tm.tm_year = pt->year+2000-1900;

    src_seconds=mktime(&dst_tm);
    printf("unix cur timestamp:%08x \r\n",src_seconds);
    tm_now = localtime(&src_seconds);
    printf("cur tm_now =%d-%d-%d %d:%d:%d \r\n",	   \
           tm_now->tm_year+1900, tm_now->tm_mon, tm_now->tm_mday,  \
           tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);

    pt= &dst_pt;
    mytransfor(DEAD_TIME,pt);
    printf("dst year:%d mon:%d day:%d hour:%d min:%d sec:%d tzone:%d\r\n", \
           pt->year,pt->mon,pt->day,pt->hour,pt->min,pt->sec,pt->tzone);


    dst_tm.tm_sec =pt->sec;
    dst_tm.tm_min =pt->min;
    dst_tm.tm_hour =pt->hour;
    dst_tm.tm_mday =pt->day;
    dst_tm.tm_mon =pt->mon;
    dst_tm.tm_year = pt->year+2000-1900;
    seconds=mktime(&dst_tm);
    printf("unix timestamp:%08x \r\n",seconds);
    tm_now = localtime(&seconds);
    printf("tm_now =%d-%d-%d %d:%d:%d \r\n",	   \
           tm_now->tm_year+1900, tm_now->tm_mon, tm_now->tm_mday,  \
           tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);

    if(src_seconds > seconds)
    {
        printf("mcu_sys_soft_reset,please 缴费\r\n");
        osDelay(20000);
        mcu_sys_soft_reset();
        while(1);
    }

#endif

}

int dx_lte_http_contextid_config(void)
{

    int ret = -1;

    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;
    const char  *ack = "OK";
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QHTTPCFG,HTTP_TCP_CONTEXT_ID);

    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("dx_lte_http_contextid_config fail\r\n");

        return ret;
    }

    return 0;
}
int dx_lte_http_url_config(char* purl)
{

    int ret = -1;

    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;
    const char  *ack = "CONNECT";
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    //AT+QHTTPURL=40,80
    printf("strlen(purl)=%d\r\n",strlen(purl));
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QHTTPURL=%d,80\r\n",strlen(purl));

    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("dx_lte_http_url_config len fail ret=%d\r\n",ret);

        return -1;
    }
    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    //send  data  with changeable length
    sprintf((char*)at_tx_buffer,"%s",purl);
    //sprintf((char*)at_tx_buffer,"AT+QISEND=%d\r\n",sock);
    ret =at_cmd_ack(cmd, strlen((const char*)cmd),result,&result_len,"OK", 2000);
    //+QIOPEN: 0,0
    if (ret ==0 )
    {
        printf("ready send data...\r\n");
    }
    else
    {
        printf("%s fail,ret=%d\r\n",cmd,ret);
        return -2;
    }
#if 1

    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QIACT);
    //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    ret = at_cmd_ack(cmd, strlen((const char*)cmd), result, &result_len,"OK", 2000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        printf("%s fail,ret=%d\r\n",cmd,ret);
        return -1;
    }
#endif

    return 0;
}
#define HTTP_TIMEOUT  1000

int dx_lte_http_file_len(int* plen)
{

    int ret = -1;

    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;
    const char  *ack = "+QHTTPGET:";
    char*p =NULL;
    int data_len = -1;
    int bit_num;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);

    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QHTTPGET,HTTP_TIMEOUT);

    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("dx_lte_http_url_config len fail ret=%d\r\n",ret);

        return -1;
    }
    //p = strstr((const char*)result,"+QHTTPGET: ");
    //if(p != NULL)
    {

        p = (char*)strstr((const char*)result,"+QHTTPGET: 0,200,");
        if(p != NULL)
        {
            printf("p =%s\r\n",p );


            p += strlen("+QHTTPGET: 0,200,");
            *plen = 0;
            data_len = 0;
            bit_num = 0;
            while(*p != 0x0d)
            {
                data_len *= 10;
                data_len += *p - '0';

                p++;
                bit_num++;
            }
            printf("QHTTPGET :data_len=%d bit_num=%d\r\n",data_len,bit_num);
            *plen = data_len;
        }
        else
            return -2;
    }
    //else
     //   return -2;

    return 0;
}


//ufs
#define FLASH_PAGE_WRITE 1

#define HTTP_GET_FILE_IN_RAM 0


int dx_lte_http_getfile(char* filename,int file_len)
{

    int ret = -1;
    unsigned char  *cmd = at_tx_buffer;
    unsigned char *result = at_rx_buffer;
    unsigned char result_len = 0;
    const char  *ack = "OK";
    char*p =NULL;
	char*pread =NULL;
    int data_len = -1;
    int bit_num;
	int free_size = 0;
	int total_size = 0;
	int file_handle = 0;
	int once_read_len = 0;
	int i =0;
	int j =0;
	int read_times =0;
	int left_data =0;
	int skip_len = 0;
	int start_time = get_curtime();
	int end_time = get_curtime();
	int start_time2 = get_curtime2();
	int end_time2 = get_curtime2();

	#if (FLASH_PAGE_WRITE ==1)
	char flash_page_temp[sFLASH_SPI_PAGESIZE];
	static int flash_page_index=0;
	int page_pos =0;
	#endif


	result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
	#if(HTTP_GET_FILE_IN_RAM == 1)
	snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFDEL_RAM);
	#else
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFDEL_UFS);
	#endif
    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("AT_QFDEL_RAM fail ret=%d\r\n",ret);

        return -13;
    }

	
	result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
	#if(HTTP_GET_FILE_IN_RAM == 1)
	snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFLST_RAM);
	#else
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFLST);
	#endif
    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

        return -3;
    }
	
	result_len = 0;
	memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
	memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
	#if (HTTP_GET_FILE_IN_RAM ==1)
	snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFLDS_RAM);
	#else
	snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFLDS);
	#endif
	ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
	if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
	{
		printf("dx_lte_http_getfile fail ret=%d\r\n",ret);
	
		return -1;
	}

	p = (char*)strstr((const char*)result,"+QFLDS: ");

	if(p != NULL)
	{
		printf("p =%s\r\n",p );
	
	
		p += strlen("+QFLDS: ");
		data_len = 0;
		bit_num = 0;
		while(*p != ',')
		{
			data_len *= 10;
			data_len += *p - '0';
	
			p++;
			bit_num++;
		}
		printf("QFLDS :data_len=%d bit_num=%d\r\n",data_len,bit_num);
		free_size = data_len;

		p = (char*)strstr((const char*)result,",");
		if(p != NULL)
		{
			printf("p =%s\r\n",p );
		
			p += strlen(",");
			data_len = 0;
			bit_num = 0;
			while(*p != 0x0d)
			{
				data_len *= 10;
				data_len += *p - '0';
		
				p++;
				bit_num++;
			}
			
			total_size = data_len;
			printf("QFLDS :total_size=%d bit_num=%d\r\n",total_size,bit_num);
		}
	}
	else
		return -2;
	if((file_len == ApplicationSize)||(file_len == ApplicationSize_other))
	{
//jd app bin get mcu header

	}
	else if(file_len > (ApplicationSize))
	{
		printf("file_len > (512*1024) file invalid\r\n");
		return -3;
	}

	if(free_size < file_len)
	{
		#if (HTTP_GET_FILE_IN_RAM ==1)
		printf("module 4g ,ram space not engough\r\n");
		#else
		printf("module 4g ,ufs space not engough\r\n");
		#endif
		return -3;
	}

	result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
	#if 0
	AT+QHTTPREADFILE="2.gif",80
	
	OK
	
	+QHTTPREADFILE: 0

		#endif
		ack = "+QHTTPREADFILE: 0";
	#if (HTTP_GET_FILE_IN_RAM ==1)
	 snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QHTTPREADFILE=\"RAM:%s\",%d\r\n",filename,HTTP_TIMEOUT);
	#else
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QHTTPREADFILE=\"%s\",%d\r\n",filename,HTTP_TIMEOUT);
	#endif
    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,4000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

        return -4;
    }
	else
	{
		printf("QHTTPREADFILE ack=%s result=%s\r\n",ack,result );
	}
	ack = "OK";
	result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
	#if (HTTP_GET_FILE_IN_RAM ==1)
	 snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFLST_RAM);
	#else
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFLST);
	#endif
    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

        return -6;
    }
//read file and write ext spi flash
	//open
	result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
	//file mode read-only
	#if (HTTP_GET_FILE_IN_RAM ==1)
	 snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFOPEN=\"RAM:%s\",2\r\n",filename);
	#else
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFOPEN=\"%s\",2\r\n",filename);
	#endif
    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

        return -7;
    }
	p = (char*)strstr((const char*)result,"+QFOPEN: ");
	if(p != NULL)
	{
		printf("p =%s\r\n",p );
	
	
		p += strlen("+QFOPEN: ");
		data_len = 0;
		bit_num = 0;
		while(*p != 0x0d)
		{
			data_len *= 10;
			data_len += *p - '0';
	
			p++;
			bit_num++;
		}
		file_handle = data_len;
		printf("QFOPEN :file_handle=%d bit_num=%d\r\n",file_handle,bit_num);
		if(file_handle <0)
		{
			printf("file_handle invalid\r\n");
		
			return -8;
		}

	}
	//read
	//AT+QFREAD=3000,100 
	//file_len
	if((file_len == ApplicationSize)||(file_len == ApplicationSize_other))
	{
//jd app bin get mcu header
		result_len = 0;
		memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		//file mode read-only
		//snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFSEEK=\"%s\",2\r\n",file_handle);
		//snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFSEEK=%d,%d,0\r\n",file_handle,ApplicationSize-MCU_HEADER);
		snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFSEEK=%d,0,2\r\n",file_handle);
		printf("at_tx_buffer=%s =%d\r\n",at_tx_buffer,strlen((const char *)cmd));
		ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
		if (ret < 0 )
		{
			printf("QFSEEK fail ret=%d\r\n",ret);

			return -10;
		}
#if 1
		result_len = 0;
		memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		//file mode read-only
		snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFPOSITION=%d\r\n",file_handle);

		ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
		if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
		{
			printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

			return -9;
		}
		else
		{
			printf("QFPOSITION result=%s\r\n",result);
		
			p = (char*)strstr((const char*)result,"+QFPOSITION: ");
			if(p != NULL)
			{
				printf("p =%s\r\n",p );
			
			
				p += strlen("+QFPOSITION: ");
				data_len = 0;
				bit_num = 0;
				while(*p != 0x0d)
				{
					data_len *= 10;
					data_len += *p - '0';
			
					p++;
					bit_num++;
				}
		
				printf("QFPOSITION :data_len=%d bit_num=%d\r\n",data_len,bit_num);
				if(file_len != data_len)
				{
					printf("http get len fail,%d ,%d \r\n",file_len,data_len);
				
					return -8;
				}
			
			}
		}

#endif		
		
		//file len  == ?file_len
		//seek
		result_len = 0;
		memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		//file mode read-only
		//snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFSEEK=\"%s\",2\r\n",file_handle);
		//snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFSEEK=%d,%d,0\r\n",file_handle,ApplicationSize-MCU_HEADER);
		snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFSEEK=%d,%d,2\r\n",file_handle,MCU_HEADER);
		printf("at_tx_buffer=%s =%d\r\n",at_tx_buffer,strlen((const char *)cmd));
		ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
		if (ret < 0 )
		{
			printf("QFSEEK fail ret=%d\r\n",ret);

			return -10;
		}
		//file seek
		#if 1
		result_len = 0;
		memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		//file mode read-only
		snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFPOSITION=%d\r\n",file_handle);

		ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
		if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
		{
			printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

			return -9;
		}
		else
			printf("QFPOSITION result=%s\r\n",result);
		#endif		
		//read
		result_len = 0;
		memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFREAD=%d,%d\r\n",file_handle,MCU_HEADER);
		ack = "OK";
		ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
		//if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
		if (ret < 0 )
		{
			printf("QFREAD  fail ret=%d\r\n",ret);

			return -10;
		}
		else
		{
			printf(" mcu  heade read...\r\n");
			#ifdef MCU_USING_MODULE_BACKTRACE
			dump_memeory((uint32_t)result,result_len);
			#endif
			once_read_len = MCU_HEADER;
			skip_len = 14;
			if(skip_len == 14)
			{
				char ref_data[14]={0x0d,0x0a,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x20,
								0xff,0xff,0x0d,0x0a,};
				ref_data[11]=  (once_read_len%10) +'0';
				ref_data[10]=  (once_read_len/10) +'0';
				printf("ref_data[10]= 0x%x [11]= 0x%x\r\n",ref_data[10],ref_data[11]);
				bit_num = dx_memmem((char*)result, result_len,(char*)ref_data,sizeof(ref_data));
				printf(" 14 bit_num=%d\r\n",bit_num);
				if(bit_num >=0)
				{
					p = (char*)result +bit_num+14;//offset
				}
				else
				{
					printf(" mcu  data error\r\n");
					return -99;
				}
			}
			T_McuFirmwareHeader mcu_header = {0};
			memcpy(&mcu_header,p,MCU_HEADER);
	
				
			printf("sizeof(mcu_header)=%d\r\n", sizeof(mcu_header));
			printf("mcu_header.u32Magic is 0x%08x\r\n", mcu_header.u32Magic);
			printf("mcu_header.u32RawDataLen is 0x%02x\r\n", mcu_header.u32RawDataLen);
			printf("mcu_header.u16Crc16 is 0x%02x\r\n", mcu_header.u16Crc16);
			printf("mcu_header.u16McuVersion is 0x%02x\r\n", mcu_header.u16McuVersion);
			printf("mcu_header.u8ArrayBuildTime is %s\r\n", mcu_header.u8ArrayBuildTime);
			if(mcu_header.u32Magic == MCU_HEADER_MAIGC)
			{
				printf("HWSERIAL_SOFTVERSIONis 0x%02x\r\n", HWSERIAL_SOFTVERSION);
				if(mcu_header.u16McuVersion > HWSERIAL_SOFTVERSION)
				{
					file_len =mcu_header.u32RawDataLen;
					printf("update mcu header\r\n");
					sFLASH_EraseSector(FLASH_MCU_HEADER_ADDRESS);
					sFLASH_WriteBuffer((uint8_t*)&mcu_header, FLASH_MCU_HEADER_ADDRESS, MCU_HEADER);
				}
				else if(mcu_header.u16McuVersion == HWSERIAL_SOFTVERSION)
				{
					file_len =mcu_header.u32RawDataLen;
					printf(" mcu ver =,not need upgrade\r\n");
					return -98;
		
				}
				else 
				{
					file_len =mcu_header.u32RawDataLen;
					
								
				}

			}
			else
			{
				printf(" mcu header data error\r\n");
				return -99;
			}
		}
		//seek reset
		result_len = 0;
		memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		//file mode read-only
		//snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFSEEK=\"%s\",2\r\n",file_handle);
		snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFSEEK=%d,0,0\r\n",file_handle);
		ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
		if (ret < 0 )
		{
			printf("QFSEEK reset fail ret=%d\r\n",ret);

			return -10;
		}
		else
			printf("QFSEEK result=%s\r\n",result);
			

	}
	
	//once_read_len = sFLASH_SPI_PAGESIZE;
	#if (FLASH_PAGE_WRITE ==1)
	flash_page_index = 0;
	#endif
	once_read_len = sFLASH_SPI_PAGESIZE/2;//16*16
	//once_read_len = sFLASH_SPI_PAGESIZE/16;//16*16
	if(once_read_len>0 && once_read_len<10)
		skip_len =13;
	if(once_read_len>=10 && once_read_len<100)
		skip_len =14;
	else if(once_read_len >=100 && once_read_len<1000)
		skip_len =15;
	else
	{
		printf("once_read_len=%d\r\n",once_read_len);
	}
	if(file_len%once_read_len == 0)
	{
		read_times = file_len/once_read_len;
		left_data = 0;
	}
	else
	{
		read_times = file_len/once_read_len;
		left_data = file_len%once_read_len;
	}
	//erase 512KB
	for(i=0;i<8;i++)
	{
		sFLASH_EraseSector(FLASH_APP_BAK_BIN_ADDRESS+sFLASH_SPI_SECTORSIZE*i);
	}
	for(i=0;i<read_times;i++)
	{
		//file seek
		#if 0
		result_len = 0;
		memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		//file mode read-only
		snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFPOSITION=%d\r\n",file_handle);

		ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
		if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
		{
			printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

			return -9;
		}
		#endif
		//read
		result_len = 0;
		memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		//file mode read-only
		snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFREAD=%d,%d\r\n",file_handle,once_read_len);
		ack = "OK";
		//ack = "CONNECT";
		ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
		//if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
		if (ret < 0 )
		{
			printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

			return -10;
		}
		else
		{
			//write
			//skip CONNECT
			//result[14]=0x0a,11
			printf("read data =%d,result_len=%d skip_len=%d once_read_len=%d...\r\n",i,result_len,skip_len,once_read_len);
			//printf("result[x]=0x%02x,%d\r\n",result[strlen("CONNECT")+1+2],strlen("CONNECT 16"));
			//printf("result[14]=0x%02x,%d\r\n",result[strlen("CONNECT")+1+2],strlen("CONNECT 256"));
			//p = (char*)strstr((const char*)result,"CONNECT 256");
			//GIF89a
			//p = (char*)strstr((const char*)result,"GIF89a");
			//offset=15
			if(result_len != (skip_len + once_read_len +6))
			{
				printf("result_len error, result is %s\r\n",result);
				#ifdef MCU_USING_MODULE_BACKTRACE
				dump_memeory((uint32_t)result,result_len);
				#endif
				//return -11;
				if(skip_len == 13)
				{
					char ref_data[13]={0x0d,0x0a,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x20,
									0xff,0x0d,0x0a,};
					ref_data[10]= once_read_len +'0';
					printf("ref_data[10]= 0x%x\r\n",ref_data[10]);
					bit_num = dx_memmem((char*)result, result_len,(char*)ref_data,sizeof(ref_data));
					printf(" 13 bit_num=%d\r\n",bit_num);
					if(bit_num >=0)
					{
						p = (char*)result +bit_num+13;//offset
					}
					else
					{
						
						return -99;
					}

				}

				else if(skip_len == 14)
				{
					char ref_data[14]={0x0d,0x0a,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x20,
									0xff,0xff,0x0d,0x0a,};
					ref_data[11]=  (once_read_len%10) +'0';
					ref_data[10]=  (once_read_len/10) +'0';
					printf("ref_data[10]= 0x%x [11]= 0x%x\r\n",ref_data[10],ref_data[11]);
					bit_num = dx_memmem((char*)result, result_len,(char*)ref_data,sizeof(ref_data));
					printf(" 14 bit_num=%d\r\n",bit_num);
					if(bit_num >=0)
					{
						p = (char*)result +bit_num+14;//offset
					}
					else
					{
						
						return -99;
					}

				}
				else if(skip_len == 15)
				{
					char ref_data[15]={0x0d,0x0a,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x20,
									0xff,0xff,0xff,0x0d,0x0a,};
					ref_data[12]=  (once_read_len%10) +'0';
					ref_data[11]=  (once_read_len/10%10) +'0';

					ref_data[10]=  (once_read_len/100) +'0';
					printf("ref_data[10]= 0x%x [11]= 0x%x [12]= 0x%x\r\n",
							ref_data[10],ref_data[11],ref_data[12]);

					bit_num = dx_memmem((char*)result, result_len,(char*)ref_data,sizeof(ref_data));
					printf("15 bit_num=%d\r\n",bit_num);
					if(bit_num >=0)
					{
						p = (char*)result +bit_num+15;//offset
					}
					else
					{
						
						return -99;
					}
				}
				
				//0x0d,0x0a,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x20,
				//0x31,0x36,0x0d,0x0a,
				
			}
			else
				p = (char*)result +skip_len;//offset
			//if(p)
			//	printf("offset=%d\r\n",p-result);
			#ifdef MCU_USING_MODULE_BACKTRACE
			dump_memeory((uint32_t)p,16);
			#endif
			#if 0
			====Disp Memory  0x2000c1c0,len=24=================
			0x2000c1c0:  --  --  --  --  --  --  --  --  f2  95  b0  8c  a5  2c  67  49  |
			0x2000c1d0:  cb  5a  da  b2  09  11  00  00  |

			#endif
			//sFLASH_WritePage((uint8_t *)p,FLASH_APP_BAK_BIN_ADDRESS+once_read_len*i,once_read_len);
			#if (FLASH_PAGE_WRITE ==1)
			
			memcpy(&flash_page_temp[flash_page_index],p,once_read_len);
			flash_page_index += once_read_len;
			if(flash_page_index == sFLASH_SPI_PAGESIZE)
			{

				//int 
				page_pos = (i+1)/(sFLASH_SPI_PAGESIZE/once_read_len);
				printf("cur loop is i=%d page_pos=%d\r\n",i,page_pos);
				sFLASH_WritePage((uint8_t *)flash_page_temp,
				FLASH_APP_BAK_BIN_ADDRESS+(page_pos-1)*sFLASH_SPI_PAGESIZE,sFLASH_SPI_PAGESIZE);
				//FLASH_APP_BAK_BIN_ADDRESS+once_read_len*(i+1),sFLASH_SPI_PAGESIZE);
				//FLASH_APP_BAK_BIN_ADDRESS+page_pos*sFLASH_SPI_PAGESIZE,sFLASH_SPI_PAGESIZE);
				
				//read check
				pread = mymalloc(sFLASH_SPI_PAGESIZE);
				memset(pread,0,sFLASH_SPI_PAGESIZE);

				sFLASH_ReadBuffer((uint8_t *)pread, 
				FLASH_APP_BAK_BIN_ADDRESS+(page_pos-1)*sFLASH_SPI_PAGESIZE, sFLASH_SPI_PAGESIZE);

				ret = memcmp(pread,flash_page_temp,sFLASH_SPI_PAGESIZE);
				if(ret == 0)
				{
					//reset
					flash_page_index = 0;
					memset(&flash_page_temp[0],0xff,sFLASH_SPI_PAGESIZE);

					printf("*****write check ok =%d,process=%% %d\r\n",page_pos,
							(page_pos*sFLASH_SPI_PAGESIZE)*100/file_len);
				}
				else
				{
					printf(" write check fail =%d\r\n",page_pos);
					for(j=0;j<sFLASH_SPI_PAGESIZE;j++)
					{
						if(flash_page_temp[j] != *(pread+j))
						{
							printf("****j=%d 0x%02x 0x%02x\r\n",j,flash_page_temp[j],*(pread+j));
							break;
						}
					}

				}		

				myfree(pread);

			}
			#else
			
			pread = mymalloc(once_read_len);
			memset(pread,0,once_read_len);
			//check erase is oxff?
			sFLASH_ReadBuffer((uint8_t *)pread, FLASH_APP_BAK_BIN_ADDRESS+once_read_len*i, once_read_len);
			dump_memeory((uint32_t)pread,once_read_len);

			//write
			sFLASH_WritePage((uint8_t *)p,FLASH_APP_BAK_BIN_ADDRESS+once_read_len*i,once_read_len);

			//read check
			memset(pread,0,once_read_len);
			sFLASH_ReadBuffer((uint8_t *)pread, FLASH_APP_BAK_BIN_ADDRESS+once_read_len*i, once_read_len);

			ret = memcmp(pread,p,once_read_len);
			if(ret == 0)
			{
				printf("*****write check ok last\r\n");
			}

			else
			{

				for(j=0;j<once_read_len;j++)
				{
					if(*(p+j) != *(pread+j))
					{
						printf("****j=%d 0x%02x 0x%02x\r\n",j,*(p+j),*(pread+j));
						break;
					}
				}
			}
			myfree(pread);
			#endif
		}
		
		
	}
	#if (FLASH_PAGE_WRITE ==1)
	if((left_data == 0) && (flash_page_index>0))
	{
		printf("if((left_data == 0) && (flash_page_index>0)),%d %d\r\n",read_times,page_pos);
		sFLASH_WritePage((uint8_t *)flash_page_temp,
		FLASH_APP_BAK_BIN_ADDRESS+page_pos*sFLASH_SPI_PAGESIZE,sFLASH_SPI_PAGESIZE);


		//read check
		pread = mymalloc(sFLASH_SPI_PAGESIZE);
		memset(pread,0,sFLASH_SPI_PAGESIZE);
		
		sFLASH_ReadBuffer((uint8_t *)pread, 
		FLASH_APP_BAK_BIN_ADDRESS+page_pos*sFLASH_SPI_PAGESIZE, sFLASH_SPI_PAGESIZE);
		
		ret = memcmp(pread,flash_page_temp,sFLASH_SPI_PAGESIZE);
		if(ret == 0)
		{
			printf("*****write check ok,last1 \r\n");
		}
		else
		{
			printf(" write check fail =%d\r\n",page_pos);
			for(j=0;j<sFLASH_SPI_PAGESIZE;j++)
			{
				if(flash_page_temp[j] != *(pread+j))
				{
					printf("****j=%d 0x%02x 0x%02x\r\n",j,flash_page_temp[j],*(pread+j));
					break;
				}
			}
		
		}		
		
		myfree(pread);

	}
	#endif
	if(left_data > 0)
	{
		result_len = 0;
		memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		//file mode read-only

		
		snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFREAD=%d,%d\r\n",file_handle,left_data);

		ack = "OK";
		//ack = "CONNECT";
		ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
		//if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
		if (ret < 0 )
		{
			printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

			return -11;
		}
		else
		{
			//write
			//skip CONNECT 
			
			//p = (char*)strstr((const char*)result,"CONNECT");
			if(left_data>0 && left_data<10)
				skip_len =13;
			if(left_data>=10 && left_data<100)
				skip_len =14;
			else if(left_data >=100 && left_data<1000)
				skip_len =15;
			else
			{
				printf("left_data=%d\r\n",left_data);
			}
			printf("read data =%d,result_len=%d skip_len=%d left_data=%d...\r\n",i,result_len,skip_len,left_data);
			if(result_len != (skip_len + left_data +6))
			{
				printf("result is %s\r\n",result);
			}

			p = (char*)result +skip_len;//offset
			#if 0

			
			read data =93,result_len=20 skip_len=14 left_data=1...
			left_data=1
			
			====Disp Memory  0x2000c1b0,len=30=================
			0x2000c1b0:  --  --  --  --  --  --  --  --  --  --  0d  0a  43  4f  4e  4e  |
			0x2000c1c0:  45  43  54  20  31  0d  0a  3b  0d  0a  4f  4b  0d  0a  |
			
			
			====Disp Memory  0x2000c1c0,len=8=================
			0x2000c1c0:  --  --  --  --  --  --  --  3b  |

			#endif
#ifdef MCU_USING_MODULE_BACKTRACE
			dump_memeory((uint32_t)result,result_len);

			dump_memeory((uint32_t)p,left_data);

#endif

			#if (FLASH_PAGE_WRITE ==1)
			memcpy(&flash_page_temp[flash_page_index],p,left_data);
			//flash_page_index += once_read_len;
			//if(flash_page_index == sFLASH_SPI_PAGESIZE)
			{
				//write
				//flash_page_index = 0;
				//int j =0;
				//int page_pos = (i+1)/(sFLASH_SPI_PAGESIZE/once_read_len);
				printf("cur loop is  page_pos=%d flash_page_index=%d\r\n",page_pos,flash_page_index);
				sFLASH_WritePage((uint8_t *)flash_page_temp,
				FLASH_APP_BAK_BIN_ADDRESS+page_pos*sFLASH_SPI_PAGESIZE,sFLASH_SPI_PAGESIZE);
				//FLASH_APP_BAK_BIN_ADDRESS+once_read_len*(i+1),sFLASH_SPI_PAGESIZE);
				//FLASH_APP_BAK_BIN_ADDRESS+page_pos*sFLASH_SPI_PAGESIZE,sFLASH_SPI_PAGESIZE);
				
				//read check
				pread = mymalloc(sFLASH_SPI_PAGESIZE);
				memset(pread,0xff,sFLASH_SPI_PAGESIZE);

				sFLASH_ReadBuffer((uint8_t *)pread, 
				FLASH_APP_BAK_BIN_ADDRESS+page_pos*sFLASH_SPI_PAGESIZE, sFLASH_SPI_PAGESIZE);
				
				for(j=0;j<sFLASH_SPI_PAGESIZE;j++)
				{
					if(flash_page_temp[j] != *(pread+j))
					{
						printf("****j=%d 0x%02x 0x%02x\r\n",j,flash_page_temp[j],*(pread+j));
						break;
					}
				}
				myfree(pread);

			}

			#else
			printf("read_times=%d ,i=%d\r\n",read_times,i);
			pread = mymalloc(left_data);
			memset(pread,0,left_data);
			//check erase is oxff?
			sFLASH_ReadBuffer((uint8_t *)pread, FLASH_APP_BAK_BIN_ADDRESS+once_read_len*read_times, left_data);
			dump_memeory((uint32_t)pread,left_data);	
			//write
			sFLASH_WritePage((uint8_t *)p,FLASH_APP_BAK_BIN_ADDRESS+once_read_len*read_times,left_data);

			//read check
			memset(pread,0,left_data);
			sFLASH_ReadBuffer((uint8_t *)pread, FLASH_APP_BAK_BIN_ADDRESS+once_read_len*read_times, left_data);
			printf("read check\r\n");
			dump_memeory((uint32_t)pread,left_data);
			for(j=0;j<left_data;j++)
			{
		
				if(*(p+j) != *(pread+j))
				{
					printf("####i=%d 0x%02x 0x%02x\r\n",j,*(p+j),*(pread+j));
					break;
				}
			}
			myfree(pread);
			#endif
		}

	}
	//close
	result_len = 0;
	memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
	memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
	//file mode read-only
	snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QFCLOSE=%d\r\n",file_handle);

	ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
	if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
	{
		printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

		return -12;
	}

//delete ckeck
	
	result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFDEL,filename);

    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

        return -13;
    }

	result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf((char *)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QFLST);

    ret =at_cmd_ack(cmd, strlen((const char *)cmd), result, &result_len,( unsigned char *)ack,3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, ack))
    {
        printf("dx_lte_http_getfile fail ret=%d\r\n",ret);

        return -14;
    }

	end_time = get_curtime();
	end_time2 = get_curtime2();
	printf("upgrade time %d %d %dms\r\n",end_time,start_time,end_time-start_time);
	printf("upgrade time %d %d %dms\r\n",end_time2,start_time2,(end_time2-start_time2)*10);

	
    return 0;
}
volatile char mqtt_thread_status = 0;
void dx_set_mqtt_thread_stop(void)
{
	mqtt_thread_status = 2;
}

void dx_set_mqtt_thread_pause(void)
{
	mqtt_thread_status = 1;
}
void dx_clear_mqtt_thread_pause(void)
{
	mqtt_thread_status = 0;
}
char dx_get_mqtt_thread_status(void)
{
	return mqtt_thread_status;
}

