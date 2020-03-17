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


#include "st_printf.h"
#include "stm32f10x_it.h"
#include "app_rs485_broker.h"
#include "cjson_middleware.h"

void app_uart1_rs485_rxtx_io_init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}
void app_uart1_rs485_rxtx_io_high_is_tx(void)//TX
{
    GPIO_SetBits(GPIOA, GPIO_Pin_11);

}
void app_uart1_rs485_rxtx_io_low_is_rx(void)//RX
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_11);

}



void app_uart5_rs485_rxtx_io_init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}
void app_uart5_rs485_rxtx_io_high_is_tx(void)//TX
{
    GPIO_SetBits(GPIOD, GPIO_Pin_1);

}
void app_uart5_rs485_rxtx_io_low_is_rx(void)//RX
{
    GPIO_ResetBits(GPIOD, GPIO_Pin_1);

}


int uart5_sendchar(char ch)
{
    UART5->SR;
    USART_SendData(UART5,ch);
    while(USART_GetFlagStatus(UART5, USART_FLAG_TC)==RESET);

    return ch;
}

void uart5_cpusendstring( char *pt,int len)
{

    while((*pt) && (len>0))
    {
        uart5_sendchar(*pt++);
        len-- ;
    }
    uart5_sendchar('\r');
    uart5_sendchar('\n');

}
uint16_t uart5_cpu_send_data(unsigned char* buffer, uint16_t size)
{

    while( (size>0))
    {
        uart5_sendchar(*buffer++);
        size-- ;
    }
    return size;
}

void uart5_cpu_printf( char *fmt,...)
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
        uart5_cpusendstring((char*)string, len);
    }
    else
    {

        //uart_sendstring(string, LEN);

        //memset(string,0,LEN);
        //strcpy(string,"len error");
        //uart_sendstring(string, strlen("len error"));
        //snprintf(string,LEN-1,"len =%d error",len);

        uart5_cpusendstring((char*)string, LEN-1);
    }
    va_end(ap);

}

void uart5_rec_rs485_response(void)
{
    //int ret = 0;
//	uint8_t len = 0;
    const char*uart_buf = (const char*)RxBuffer5;
    uint8_t uart_buf_len = RxCounter5;
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
        printf("uart5 uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
//UART5 ECHO
        app_uart5_rs485_rxtx_io_high_is_tx();
        //uart5_cpu_printf("uart5 uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
        uart5_cpu_send_data((unsigned char*)uart_buf,uart_buf_len);
        app_uart5_rs485_rxtx_io_low_is_rx();
    }

out:
    reset_uart5_rx_buffer();


}

void uart1_rec_rs485_response(void)
{
    //int ret = 0;
//	uint8_t len = 0;
    const char*uart_buf = (const char*)RxBuffer1;
    uint8_t uart_buf_len = RxCounter1;
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
        printf("uart1 uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
//UART5 ECHO
//		app_uart1_rs485_rxtx_io_high_is_tx();
//		uart5_cpu_printf("uart5 uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
//		app_uart1_rs485_rxtx_io_low_is_rx();


    }

out:
    return;
}


#define BREAKER_CRC_POLY (0x01)
u8 Breaker_CRC8(u8 *ptr, u8 len)
{
    u8 i;
    u8 crc=0x00; /* 计算的初始crc值 */

    while(len--)
    {
        crc ^= *ptr++;  /* 每次先与需要计算的数据异或,计算完指向下一数据 */
        for (i=8; i>0; --i)   /* 下面这段计算过程与计算一个字节crc一样 */
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ BREAKER_CRC_POLY;
            else
                crc = (crc << 1);
        }
    }

    return (crc);
}

/*
根据命令，id数组，和数据，构造断路器发送帧
*/
void Breaker_ConstructTxCmdFrame(Breaker_Frame *frame,u8 cmd,u8 * id,u8 *data,u8 datalen)
{
//    u8 i =0;
    if(frame == 0 || 0 == id)
    {
        return;
    }


    frame->u.frame.head = BREAKER_FRAME_HEAD;
    frame->u.frame.len = BREAKER_FRAMELEN;
    frame->u.frame.cmd = cmd;

    frame->u.frame.id1 = id[0];
    frame->u.frame.id2 = id[1];
    frame->u.frame.id3 = id[2];
    frame->u.frame.id4 = id[3];

    if(data == 0 || datalen == 0)
    {
        memset(frame->u.buf+7,0,7);
    }
    else
    {
        memcpy(frame->u.buf+7,data,datalen);
    }
    frame->u.frame.crc = Breaker_CRC8(frame->u.buf, sizeof(frame->u.buf)-1);
    //frame->u.frame.crc = 0xF0;

}
rs485_broker_Info cur_rs485_broker_Info[RS485_BROKER_NUM]= {0};

void app_rs485_broker_init(void)
{
    //mcu_uart_control_init();
    memset(cur_rs485_broker_Info,0,sizeof(rs485_broker_Info)*RS485_BROKER_NUM);
	
	sFLASH_ReadBuffer((uint8_t*)cur_rs485_broker_Info, FLASH_BROKERID_ADDRESS, sizeof(rs485_broker_Info)*RS485_BROKER_NUM);


    //5A 0F A0 00 00 00 05 00 00 00 00 00 00 00 F0
    //crc test

    uint8_t cmd[BREAKER_FRAMELEN]= {0x5A,0x0F,0xA1,0x00,0x00,0x00,0x05,0x00
                                 ,0x00,0x00,0x00,0x00,0x00,0x00,0xF1
                                };
    uint8_t crc= Breaker_CRC8(cmd, BREAKER_FRAMELEN-1);
    printf("5 frame.crc=0x%02x crc=0x%02x\r\n",cmd[BREAKER_FRAMELEN-1],crc);

    uint8_t cmd1[BREAKER_FRAMELEN]= {0x5A,0x0F,0xA0,0x00,0x00,0x00,0x05,0x00
                                  ,0x00,0x00,0x00,0x00,0x00,0x00,0xF0
                                 };
    uint8_t crc1= Breaker_CRC8(cmd1, BREAKER_FRAMELEN-1);
    printf("5 ack frame.crc=0x%02x crc=0x%02x\r\n",cmd1[BREAKER_FRAMELEN-1],crc1);

    uint8_t cmd2[BREAKER_FRAMELEN]= {0x5A,0x0F,0xA1,0x00,0x00,0x00,0x04,0x00
                                  ,0x00,0x00,0x00,0x00,0x00,0x00,0xF0
                                 };
    uint8_t crc2= Breaker_CRC8(cmd2, BREAKER_FRAMELEN-1);
    printf("4 frame.crc=0x%02x crc=0x%02x\r\n",cmd2[BREAKER_FRAMELEN-1],crc2);


    uint8_t cmd3[BREAKER_FRAMELEN]= {0x5A,0x0F,0xA0,0x00,0x00,0x00,0x04,0x00
                                  ,0x00,0x00,0x00,0x00,0x00,0x00,0xF1
                                 };
    uint8_t crc3= Breaker_CRC8(cmd3, BREAKER_FRAMELEN-1);
    printf("4 ack frame.crc=0x%02x crc=0x%02x\r\n",cmd3[BREAKER_FRAMELEN-1],crc3);

    //while(1);


}
void app_rs485_broker_info_print(void)
{
    int i =0;
    for(i=0; i<RS485_BROKER_NUM; i++)
    {
        printf("i=%d used_flag=%d ",i,cur_rs485_broker_Info[i].used_flag);
        printf("channel_index=%d ",cur_rs485_broker_Info[i].channel_index);
      //  printf("broker_port_status=%d ",cur_rs485_broker_Info[i].broker_port_status);
        printf("id=0x%02x 0x%02x 0x%02x 0x%02x ",
               cur_rs485_broker_Info[i].id[0],cur_rs485_broker_Info[i].id[1],
               cur_rs485_broker_Info[i].id[2],cur_rs485_broker_Info[i].id[3]);

        printf("\r\n" );
    }


}

uint8_t app_rs485_broker_id_response_parse(uint8_t*pdata,uint8_t data_len)
{
#if 1
    int i =0;
    uint8_t* pRxBuffer = pdata;
    uint8_t crc = 0;
	 uint8_t ret = 0;
    Breaker_Frame tx_frame = {0};
    Breaker_Frame *frame = (Breaker_Frame*) pdata;
    /*
    		u8 head;
    		u8 len;
    		u8 cmd;

    		u8 id1;
    		u8 id2;
    		u8 id3;
    		u8 id4;

    		u8 voltage;
    		u8 current_i;
    		u8 current_d;
    		u8 power_i;
    		u8 power_d;
    		u8 Electricity_i;
    		u8 Electricity_d;

    		//u8 error_code;
    		u8 crc;
    */
    printf("frame.head=0x%02x\r\n",frame->u.frame.head );
    printf("frame.len=0x%02x\r\n",frame->u.frame.len );
    printf("frame.cmd=0x%02x\r\n",frame->u.frame.cmd );
    printf("frame.id=0x%02x 0x%02x 0x%02x 0x%02x\r\n",
           frame->u.frame.id1,frame->u.frame.id2,frame->u.frame.id3,frame->u.frame.id4 );
    printf("frame.crc=0x%02x\r\n",frame->u.frame.crc );

    printf("rec:\r\n");
    for(i=0; i<BREAKER_FRAMELEN; i++)
    {
        //printf("%d=0x%02x ",i,*(pRxBuffer+i) );
        printf("%02x ",*(pRxBuffer+i) );
    }
    printf("\r\n");

    if((pRxBuffer[0]== BREAKER_FRAME_HEAD) && (pRxBuffer[1]== BREAKER_FRAMELEN))
    {

        if(pRxBuffer[2]== BREAKER_CMD_ReportID)
        {
            crc= Breaker_CRC8(frame->u.buf, sizeof(frame->u.buf)-1);
            if(frame->u.frame.crc !=crc)
            {
                printf("frame.crc=0x%02x crc=0x%02x\r\n",frame->u.frame.crc,crc);
                return 3;
            }
        }
        else
        {
            printf("cmd pRxBuffer[2]=0x%x\r\n",pRxBuffer[2]);
            return 2;
        }
		for(i=0; i<RS485_BROKER_NUM; i++)
        {
  
            if(cur_rs485_broker_Info[i].used_flag == 1)
            {
				ret = memcmp(&frame->u.buf[STATE_ID1],cur_rs485_broker_Info[i].id,4);
				if(ret == 0)
				{
					printf("have in list\r\n");
                	goto out;
				}
            }
        }
        for(i=0; i<RS485_BROKER_NUM; i++)
        {
  
            if(cur_rs485_broker_Info[i].used_flag != 1)
            {
                cur_rs485_broker_Info[i].used_flag =1;
                cur_rs485_broker_Info[i].channel_index=i+1;
              //  cur_rs485_broker_Info[i].broker_port_status=PORT_FREE;//open
                cur_rs485_broker_Info[i].id[0] =frame->u.frame.id1;
                cur_rs485_broker_Info[i].id[1] =frame->u.frame.id2;
                cur_rs485_broker_Info[i].id[2] =frame->u.frame.id3;
                cur_rs485_broker_Info[i].id[3] =frame->u.frame.id4;
                break;
            }
        }
	
	    if(i == RS485_BROKER_NUM )
	    {
	        printf("ch  too max\r\n");
	        return -3;
	    }
		sFLASH_EraseSector(FLASH_BROKERID_ADDRESS);
		sFLASH_WriteBuffer((uint8_t*)cur_rs485_broker_Info, FLASH_BROKERID_ADDRESS, sizeof(rs485_broker_Info)*RS485_BROKER_NUM);

out:
	printf("cur_rs485_broker_Info.id=0x%02x 0x%02x 0x%02x 0x%02x\r\n",
              cur_rs485_broker_Info[i].id[0],cur_rs485_broker_Info[i].id[1],
              cur_rs485_broker_Info[i].id[2],cur_rs485_broker_Info[i].id[3]);
        Breaker_ConstructTxCmdFrame(&tx_frame,BREAKER_CMD_ReportIDACK,cur_rs485_broker_Info[i].id,NULL,0);

        printf("Breaker_ConstructTxCmdFrame:\r\n" );
        printf("frame.head=0x%02x\r\n",tx_frame.u.frame.head );
        printf("frame.len=0x%02x\r\n",tx_frame.u.frame.len );
        printf("frame.cmd=0x%02x\r\n",tx_frame.u.frame.cmd );
        printf("frame.id=0x%02x 0x%02x 0x%02x 0x%02x\r\n",
               tx_frame.u.frame.id1,tx_frame.u.frame.id2,tx_frame.u.frame.id3,tx_frame.u.frame.id4 );
        //crc= Breaker_CRC8(frame->u.buf, sizeof(frame->u.buf)-1);
        //tx_frame.u.frame.crc = crc;
        printf("frame.crc=0x%02x\r\n",tx_frame.u.frame.crc );

        pRxBuffer = (uint8_t *)&tx_frame;
        printf("ack:\r\n");
        for(i=0; i<BREAKER_FRAMELEN; i++)
        {
            //printf("%d=0x%02x ",i,*(pRxBuffer+i) );
            printf("%02x ",*(pRxBuffer+i) );
        }
        printf("\r\n");
        app_uart1_rs485_rxtx_io_high_is_tx();

        for(i=0; i<BREAKER_FRAMELEN; i++)
        {

            uart1_sendchar(*(pRxBuffer+i));
        }
//save to flash
		


        return 0;


    }
    else
    {
        return 1;
    }

#endif

}

void  app_rs485_broker_test(void)
{
    uint8_t id[4] = {0,0,0,4};
    Breaker_Frame tx_frame = {0};
    uint8_t* pRxBuffer = NULL;
    int j =0;
    int i =0;
    uint32_t flags =0;
    uint8_t crc =0;
    uint8_t cmd =BREAKER_CMD_ReportIDACK;
#if 1
    for(j=0; j<8; j++)
    {
        //construct
        Breaker_ConstructTxCmdFrame(&tx_frame,BREAKER_CMD_ReportIDACK+j,id,NULL,0);

        pRxBuffer = (uint8_t *)&tx_frame;
        printf("send:\r\n");
        for(i=0; i<BREAKER_FRAMELEN; i++)
        {
            //printf("%d=0x%02x ",i,*(pRxBuffer+i) );
            printf("%02x ",*(pRxBuffer+i) );
        }
        printf("\r\n");

        app_uart1_rs485_rxtx_io_high_is_tx();

        for(i=0; i<BREAKER_FRAMELEN; i++)
        {

            uart1_sendchar(*(pRxBuffer+i));
        }
    }
    return;
#endif

    //rec
    app_uart1_rs485_rxtx_io_low_is_rx();
    reset_uart1_rx_buffer();
#if (UART1_RX_DMA ==1)
    DMA_Enable(DMA1_Channel5,UART1_RX_BUFFER_LEN);//开启下一次DMA接收
#endif


    flags = EVENT_FLAGS_UART1;
    // flags = osEventFlagsWait(evt_id_uart1, flags, osFlagsWaitAny, 3000);//osWaitForever
    printf("*****rs485_broker evt_id_uart1 flags =0x%08x\r\n",flags);
    //if(flags != osFlagsErrorTimeout)
    {

        //	if((flags&EVENT_FLAGS_UART1) == EVENT_FLAGS_UART1)
        {

            //uart1_rec_rs485_response();

            pRxBuffer= ( uint8_t*)RxBuffer1;
            uint8_t uart_buf_len = RxCounter1;
            printf("rec:\r\n");
            for(i=0; i<BREAKER_FRAMELEN; i++)
            {
                //printf("%d=0x%02x ",i,*(pRxBuffer+i) );
                printf("%02x ",*(pRxBuffer+i) );
            }
            printf("\r\n");

            if((pRxBuffer[0]== BREAKER_FRAME_HEAD) && (pRxBuffer[1]== BREAKER_FRAMELEN))
            {

                if(pRxBuffer[2]== BREAKER_CMD_OpenBreakerACK)
                {
                    crc= Breaker_CRC8(pRxBuffer,BREAKER_FRAMELEN-1);
                    printf("frame.crc=0x%02x crc=0x%02x\r\n",pRxBuffer[BREAKER_FRAMELEN-1],crc);
                    if(pRxBuffer[BREAKER_FRAMELEN-1] !=crc)
                    {

                        return ;
                    }
                }
                else
                {
                    printf("cmd type error\r\n");
                    return ;
                }
            }
            else
            {
                printf("cmd header error\r\n");
                return ;
            }

        }
    }
//	else
//	{
    //	printf("cmd recv timeout\r\n");
    //	return ;
//	}

}
int app_rs485_broker_com_data(uint8_t index,uint8_t cmd,uint8_t* pdata,uint8_t data_len)
{

    uint32_t flags=0;
   // uint8_t index=RS485_BROKER_NUM;
    uint8_t* pRxBuffer = NULL;

    osEventFlagsId_t ef_id =NULL;
    Breaker_Frame tx_frame = {0};
    uint8_t crc =0;
    int i =0;
    Breaker_ConstructTxCmdFrame(&tx_frame,cmd,cur_rs485_broker_Info[index].id,NULL,0);
    printf("uart1 tx start=%d\r\n",get_curtime2());

    pRxBuffer = (uint8_t *)&tx_frame;
    printf("send:\r\n");
    for(i=0; i<BREAKER_FRAMELEN; i++)
    {
        //printf("%d=0x%02x ",i,*(pRxBuffer+i) );
        printf("%02x ",*(pRxBuffer+i) );
    }
    printf("\r\n");

	reset_uart1_rx_buffer();
#if (UART1_RX_DMA ==1)
	DMA_Enable(DMA1_Channel5,UART1_RX_BUFFER_LEN);//开启下一次DMA接收
#endif


    app_uart1_rs485_rxtx_io_high_is_tx();
    for(i=0; i<BREAKER_FRAMELEN; i++)
    {

        uart1_sendchar(*(pRxBuffer+i));
    }

    app_uart1_rs485_rxtx_io_low_is_rx();


    ef_id = evt_id_uart1;
    flags = EVENT_FLAGS_UART1;
    printf("*****dc_tx_rx_data wait flags =0x%08x %d\r\n",flags,get_curtime2());
    flags = osEventFlagsWait(ef_id, flags, osFlagsWaitAll, UART1_RX_COMMAND_TIMEOUT);

    if((int32_t)flags == osError)
    {
        printf("%s osError\r\n",__func__);
        return -4;
    }
    else if((int32_t)flags == osErrorTimeout)
    {
        printf("%s osErrorTimeout=%d\r\n",__func__,get_curtime2());
        return -5;
    }
    // else
    //a {
    printf("uart1 rx =%d from BROKER\r\n",get_curtime2());

    if((flags&EVENT_FLAGS_UART1)  == EVENT_FLAGS_UART1)
    {
        printf("uart1 rx end\r\n");
        pRxBuffer= ( uint8_t*)RxBuffer1;
        uint8_t uart_buf_len = RxCounter1;
        printf("rec:\r\n");
        for(i=0; i<BREAKER_FRAMELEN; i++)
        {
            //printf("%d=0x%02x ",i,*(pRxBuffer+i) );
            printf("%02x ",*(pRxBuffer+i) );
        }
        printf("\r\n");
		if(pdata)
		{
			memcpy(pdata,pRxBuffer,BREAKER_FRAMELEN);
		}
        if((pRxBuffer[0]== BREAKER_FRAME_HEAD) && (pRxBuffer[1]== BREAKER_FRAMELEN))
        {

            if(pRxBuffer[2]== (cmd+1))
            {
                crc= Breaker_CRC8(pRxBuffer,BREAKER_FRAMELEN-1);
                printf("frame.crc=0x%02x crc=0x%02x\r\n",pRxBuffer[BREAKER_FRAMELEN-1],crc);
                if(pRxBuffer[BREAKER_FRAMELEN-1] !=crc)
                {
                    printf("crc Error\r\n");
                    return -6;
                }
            }
            else
            {
                printf("cmd type error\r\n");
                return -7;
            }
        }
        else
        {
            printf("cmd header error\r\n");
            return -8;
        }



    }
    else
    {
        printf("invalid flags=0x%08x\r\n",flags);
        return -9;

    }
    return 0;



}

int app_rs485_broker_start_power(uint8_t ch)
{
    //uint8_t data[5]= {0};
    //uint8_t data_len =0;
    uint8_t index = 0;

    int ret =0;


    int i =0;
    if(ch > (RS485_BROKER_NUM))
    {
        printf("ch=%d not invlid\r\n",ch);
        return -1;
    }
    for(i=0; i<RS485_BROKER_NUM; i++)
    {
        printf("i=%d used_flag=%d ",i,cur_rs485_broker_Info[i].used_flag);
        printf("channel_index=%d ch=%d\r\n",cur_rs485_broker_Info[i].channel_index,ch);
    //    printf("broker_port_status=%d ",cur_rs485_broker_Info[i].broker_port_status);
        printf("id=0x%02x 0x%02x 0x%02x 0x%02x ",
               cur_rs485_broker_Info[i].id[0],cur_rs485_broker_Info[i].id[1],
               cur_rs485_broker_Info[i].id[2],cur_rs485_broker_Info[i].id[3]);

        printf("\r\n" );

        if(ch == cur_rs485_broker_Info[i].channel_index)
        {
            if(cur_rs485_broker_Info[i].used_flag == 1)
            {

                index= i;
                break;
            }
            else
            {
                printf("ch=%d not register\r\n",ch);
                return -2;
            }

        }
    }
    if(index == RS485_BROKER_NUM )
    {
        printf("ch=%d not find\r\n",ch);
        return -3;
    }

	ret = app_rs485_broker_com_data(index,BREAKER_CMD_OpenBreaker,NULL,0);
	#if KB_DEBUG
	ret =0;
	#endif
    return ret;

}
int app_rs485_broker_stop_power(uint8_t ch)
{
    //uint8_t data[5]= {0};
    //uint8_t data_len =0;
    uint8_t index = 0;

    int ret =0;


    int i =0;
    if(ch > (RS485_BROKER_NUM))
    {
        printf("ch=%d not invlid\r\n",ch);
        return -1;
    }
    for(i=0; i<RS485_BROKER_NUM; i++)
    {
        printf("i=%d used_flag=%d ",i,cur_rs485_broker_Info[i].used_flag);
        printf("channel_index=%d ",cur_rs485_broker_Info[i].channel_index);
      //  printf("broker_port_status=%d ",cur_rs485_broker_Info[i].broker_port_status);
        printf("id=0x%02x 0x%02x 0x%02x 0x%02x ",
               cur_rs485_broker_Info[i].id[0],cur_rs485_broker_Info[i].id[1],
               cur_rs485_broker_Info[i].id[2],cur_rs485_broker_Info[i].id[3]);

        printf("\r\n" );

        if(ch == cur_rs485_broker_Info[i].channel_index)
        {
            if(cur_rs485_broker_Info[i].used_flag == 1)
            {

                index= i;
                break;
            }
            else
            {
                printf("ch=%d not register\r\n",ch);
                return -2;
            }

        }
    }
    if(index == RS485_BROKER_NUM )
    {
        printf("ch=%d not find\r\n",ch);
        return -3;
    }

	ret = app_rs485_broker_com_data(index,BREAKER_CMD_CloseBreaker,NULL,0);
		
    return ret;

}
int app_rs485_broker_lookup_all_info(void)
{
    //uint8_t data[5]= {0};
    //uint8_t data_len =0;
   // uint8_t index = 0;
	Breaker_Frame rx_frame = {0};
//	uint8_t* rec = (uint8_t*)&rx_frame;

    int ret =0;


    int i =0;

    for(i=0; i<RS485_BROKER_NUM; i++)
    {
        printf("i=%d used_flag=%d ",i,cur_rs485_broker_Info[i].used_flag);
        printf("channel_index=%d ",cur_rs485_broker_Info[i].channel_index);
     //   printf("broker_port_status=%d ",cur_rs485_broker_Info[i].broker_port_status);
        printf("id=0x%02x 0x%02x 0x%02x 0x%02x ",
               cur_rs485_broker_Info[i].id[0],cur_rs485_broker_Info[i].id[1],
               cur_rs485_broker_Info[i].id[2],cur_rs485_broker_Info[i].id[3]);

        printf("\r\n" );

       // if(ch == cur_rs485_broker_Info[i].channel_index)
        {
            if(cur_rs485_broker_Info[i].used_flag == 1)
            {

               // index= i;
                ret = app_rs485_broker_com_data(i,BREAKER_CMD_ReadBreaker,(uint8_t*)&rx_frame,BREAKER_FRAMELEN);
				if(ret == 0)
				{
					//charge_Info.charge_cur_pow[i] = rx_frame.u.frame.power_i;
					uint16_t voltage= (uint16_t)(((uint16_t)rx_frame.u.frame.voltage<<8)|rx_frame.u.frame.voltage2);//v*10
					uint16_t current= (uint16_t)(((uint16_t)rx_frame.u.frame.current_i<<8)|rx_frame.u.frame.current_d);//ma
					charge_Info.charge_cur_pow[i] = voltage/10*current/1000;
					if((charge_Info.charge_cur_pow[i]) >= 5)
					{
						charge_Info.dc_port_status[i]= (Dc_Port_Status)PORT_USEING;
					}
					else
					{
						charge_Info.dc_port_status[i]= (Dc_Port_Status)PORT_FREE;
					}
				}
				else
				{
					charge_Info.dc_port_status[i]= (Dc_Port_Status)PORT_FAULT;

				}
            }
			else
			{
				charge_Info.dc_port_status[i]= (Dc_Port_Status)PORT_FREE;
			}
				


        }
    }
		
    return ret;

}



int app_rs485_broker_port_x_status(uint8_t port_num,uint16_t* pport_status)
{
	int ret =0;
	int i =0;
	Breaker_Frame rx_frame = {0};
	uint8_t* rec = (uint8_t*)&rx_frame;

    if(port_num > (RS485_BROKER_NUM))
    {
        printf("port_num=%d not invlid\r\n",port_num);
        return -1;
    }
	#if 1
	if(cur_rs485_broker_Info[port_num-1].used_flag == 1)
	{
		if(port_num != cur_rs485_broker_Info[port_num-1].channel_index)
        {
            printf("ch=%d !=%d error invalid\r\n",port_num,cur_rs485_broker_Info[port_num-1].channel_index);
            return -2;
        }
	}
	else
	{
		printf("ch=%d not use\r\n",port_num);
		return -3;
	}

	#else
    for(i=0; i<RS485_BROKER_NUM; i++)
    {
        printf("i=%d used_flag=%d ",i,cur_rs485_broker_Info[i].used_flag);
        printf("channel_index=%d ",cur_rs485_broker_Info[i].channel_index);
     //   printf("broker_port_status=%d ",cur_rs485_broker_Info[i].broker_port_status);
        printf("id=0x%02x 0x%02x 0x%02x 0x%02x ",
               cur_rs485_broker_Info[i].id[0],cur_rs485_broker_Info[i].id[1],
               cur_rs485_broker_Info[i].id[2],cur_rs485_broker_Info[i].id[3]);

        printf("\r\n" );

        if(port_num == cur_rs485_broker_Info[i].channel_index)
        {
            if(cur_rs485_broker_Info[i].used_flag == 1)
            {

        
                break;
            }
            else
            {
                printf("ch=%d not register\r\n",port_num);
                return -2;
            }

        }
    }
    if(i == RS485_BROKER_NUM )
    {
        printf("ch=%d not find\r\n",port_num);
        return -3;
    }
	#endif
	ret = app_rs485_broker_com_data(port_num-1,BREAKER_CMD_ReadBreaker,(uint8_t*)&rx_frame,BREAKER_FRAMELEN);
	if(ret ==0)
	{
		uint16_t voltage= (uint16_t)(((uint16_t)rx_frame.u.frame.voltage<<8)|rx_frame.u.frame.voltage2);//v*10
		uint16_t current= (uint16_t)(((uint16_t)rx_frame.u.frame.current_i<<8)|rx_frame.u.frame.current_d);//ma
		printf("voltage=%d,current=%d\r\n",voltage,current);
		charge_Info.charge_cur_pow[port_num-1] = voltage/10*current/1000;

		//charge_Info.charge_cur_pow[port_num-1] = rx_frame.u.frame.power_i;
		if(pport_status)
		{
			if((charge_Info.charge_cur_pow[port_num-1]) <5)
				//if(charge_Info.charge_cur_pow[port_num-1] == 0)
				*pport_status = 0;
			else
				*pport_status = 1;
		}
		return 0 ;

	}
	else
	{
		printf("ch=%d read error\r\n",port_num);
		return -4 ;
	}

}

uint8_t app_rs485_broker_set_on(uint8_t ch, uint32_t charge_tm, uint16_t max_w, uint16_t min_w, uint32_t trickle_tm)
{
	int ret =0;
	ret = app_rs485_broker_start_power(ch);


	if(ret == 0)
	{
	//	cur_rs485_broker_Info[ch].broker_port_status = PORT_USEING;
//		cur_rs485_broker_Info[ch].max_watter = max_w;
	//	cur_rs485_broker_Info[ch].min_watter = min_w;
	//	cur_rs485_broker_Info[ch].charge_tm= charge_tm;
	//	cur_rs485_broker_Info[ch].trickle_tm = trickle_tm;
	//	cur_rs485_broker_Info[ch].start_time = RTC_GetCounter();//need dw

		return 0;
	
	}
	else
	{

	//	cur_rs485_broker_Info[ch].broker_port_status = PORT_FAULT;
		//cur_rs485_broker_Info[ch].max_watter = 0;
	//	cur_rs485_broker_Info[ch].min_watter = 0;
	//	cur_rs485_broker_Info[ch].charge_tm= 0;
	//	cur_rs485_broker_Info[ch].trickle_tm = 0;
	//	cur_rs485_broker_Info[ch].start_time = 0;//need dw


		return 1;
	}
	


}
uint8_t app_rs485_broker_set_off(uint8_t ch)
{
	int ret =0;
	ret = app_rs485_broker_stop_power(ch);


	if(ret == 0)
	{
	//	cur_rs485_broker_Info[ch].broker_port_status = PORT_FREE;
//		cur_rs485_broker_Info[ch].max_watter = 0;
//		cur_rs485_broker_Info[ch].min_watter = 0;
//		cur_rs485_broker_Info[ch].charge_tm= 0;
//		cur_rs485_broker_Info[ch].trickle_tm = 0;
//		cur_rs485_broker_Info[ch].start_time = 0;//need dw

		return 0;
	
	}
	else
	{

	//	cur_rs485_broker_Info[ch].broker_port_status = PORT_FAULT;
//		cur_rs485_broker_Info[ch].max_watter = 0;
//		cur_rs485_broker_Info[ch].min_watter = 0;
//		cur_rs485_broker_Info[ch].charge_tm= 0;
//		cur_rs485_broker_Info[ch].trickle_tm = 0;
//		cur_rs485_broker_Info[ch].start_time = 0;//need dw


		return 1;
	}

	


}


