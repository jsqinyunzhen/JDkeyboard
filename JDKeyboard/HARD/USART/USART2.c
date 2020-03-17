#include "USART2.h"
#include "ADC.h"
#include "delay.h"
//#include "MPU6050.h"
//#include "display.h"
#include "string.h"
#include "stdlib.h"
#include "WORK.h"

u32 receive2_finish = 0;
u8 rec2_buf[16] = {0};
u8 rec2_data_len = 0;
extern u32 sys_ms_cnt;


/*读卡器通讯uart  9600 */
void USART2_init(void)           //串口初始化
{
	GPIO_InitTypeDef  Usart_IOinit;                        //配置IO口
	USART_InitTypeDef  Usart_init;                         //配置串口
	NVIC_InitTypeDef  NVIC_init;                           //配置中断

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能端口A时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能串口1时钟

	Usart_IOinit.GPIO_Pin=GPIO_Pin_2;                      //配置GPIOA9  TX
	Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	Usart_IOinit.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&Usart_IOinit);

	Usart_IOinit.GPIO_Pin=GPIO_Pin_3;                     //配置GPIOA10  RX
	Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	Usart_IOinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&Usart_IOinit);

	Usart_init.USART_BaudRate=9600;                        //配置串口
	Usart_init.USART_WordLength=USART_HardwareFlowControl_None;
	Usart_init.USART_StopBits=USART_StopBits_1;
	Usart_init.USART_Parity=USART_Parity_No;
	Usart_init.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	Usart_init.USART_HardwareFlowControl=USART_WordLength_8b;
	USART_Init(USART2,&Usart_init);

	USART_Cmd(USART2,ENABLE);                             //使能串口1

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);          //选择串口中断
#if 1
	NVIC_init.NVIC_IRQChannel=USART2_IRQn;                //配置中断函数
	NVIC_init.NVIC_IRQChannelCmd=ENABLE;
	NVIC_init.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_init.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_init);
#endif

}

//串口1发送1个字符 
//c:要发送的字符
void usart2_send_data(u8 c)
{   	
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
	USART_SendData(USART2,c);  
} 
void usart2_send(u8 * pbuf , u8 len)
{   	
    while(pbuf != 0 && len > 0)
    {
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
        USART_SendData(USART2,*pbuf++); 
        len --;
    }		 
} 

u8 USART2_FrameCheckSum(u8 *buf , u8 len)
{
    int i =0;
    u8 sum = 0;
    
    if(buf == 0 )
    {
        return 1;//帧错误
    }

    for(i = 0; i <len; i++)
    {
        sum ^= buf[i];
    }
    return sum;
}


//0x55 0xaa + 4 字节卡号（高在前）+ 2 字节余额（高字节+低字节）+1 字节校验和（前 8 字节的异或）
//
u32 card_id =0; 
u32 card_id1 =0; //第一次刷卡卡号
u32 card_id2 =0; //第二次刷卡卡号
u16 overage = 0; //刷卡前的余额
u16 overage1 = 0;//第一次刷卡的余额
u16 overage2 = 0;//第二次刷卡的余额
u8 reduce =0;
#define CARD_DATA_LEN 9 

u8 rec2_Txbuf[] = {0xAA,0x55, 0x00, 0x0A, 0xF5};//默认一次扣一块，0x0A为10角

void USART2_Receive_DataAnalysis(void)
{
	if(rec2_data_len == 0 &&  (sys_ms_cnt - receive2_finish) > 400
		&& kb_getMode() == STATE_WAITCARD1_end)
	{
		//printf("Set mode to WAITCARD2\r\n");
		kb_SetMode(STATE_WAITCARD2);
	}
	if((rec2_data_len > 0 && (sys_ms_cnt - receive2_finish) > 5))
	{
		if(kb_mode == STATE_WAITCARD1 )
		{
			//u16 cmd =((u16) <<8)|rec2_buf[1];
			if(rec2_data_len == 9 && rec2_buf[0] == 0x55 && rec2_buf[1] == 0xaa 
				&& 0 == USART2_FrameCheckSum(rec2_buf,rec2_data_len))
			{ 
				card_id = (u32)rec2_buf[2]<<24|(u32)rec2_buf[3]<<16|(u32)rec2_buf[4]<<8|rec2_buf[5];
				if(card_id1 ==0)
				{
					card_id1 = card_id;
					overage = (u16)rec2_buf[6]<<8|rec2_buf[7];
					//发送扣费帧
					if(overage > 0 && overage > reduce && reduce >0)
					{
						rec2_Txbuf[3] = reduce; 
						rec2_Txbuf[4] = USART2_FrameCheckSum(rec2_Txbuf,4);
						usart2_send(rec2_Txbuf,sizeof(rec2_Txbuf));
					}
					else
					{
						printf("no money !\r\n");
						kb_SetMode(STATE_FINISH);
					}
				}
				else if(card_id1 == card_id )
				{
					overage1 = (u16)rec2_buf[6]<<8|rec2_buf[7];
					if(overage1 < overage && overage > 0 ) //扣费成功
					{
						usart1_sendPaySuccess(rec2_buf+2,overage - overage1);
						overage = 0;
						//kb_SetMode(STATE_WAITCARD1_end); //修改到网关确认是否需要刷第二次
					}
				}
				else
				{
					kb_SetMode(STATE_FINISH);
				}
			}
		}
		
		if(kb_mode == STATE_WAITCARD2 )
		{
			//u16 cmd =((u16) <<8)|rec2_buf[1];
			if(rec2_data_len == 9 && rec2_buf[0] == 0x55 && rec2_buf[1] == 0xaa 
				&& 0 == USART2_FrameCheckSum(rec2_buf,rec2_data_len))
			{ 
				card_id = (u32)rec2_buf[2]<<24|(u32)rec2_buf[3]<<16|(u32)rec2_buf[4]<<8|rec2_buf[5];
				if(card_id2 ==0)
				{
					card_id2 = card_id;
					overage = (u16)rec2_buf[6]<<8|rec2_buf[7];
					//发送扣费帧
					if(overage > 0 && overage > reduce && reduce >0)
					{
						rec2_Txbuf[3] = reduce; 
						rec2_Txbuf[4] = USART2_FrameCheckSum(rec2_Txbuf,4);
						usart2_send(rec2_Txbuf,sizeof(rec2_Txbuf));
					}
					else
					{
						printf("no money 2!\r\n");
						kb_SetMode(STATE_FINISH);
					}
				}
				else if(card_id2== card_id )
				{
					overage2 = (u16)rec2_buf[6]<<8|rec2_buf[7];
					if(overage2 < overage && overage > 0 ) //扣费成功
					{
						usart1_sendPaySuccess(rec2_buf+2,overage - overage2);
						overage = 0;
						kb_SetMode(STATE_FINISH);
					}
				}
				else
				{
					kb_SetMode(STATE_FINISH);				
				}
			}
		}
		memset(rec2_buf,0,sizeof(rec2_buf));
		rec2_data_len = 0;
	}

	
}

void USART2_IRQHandler(void)                         //串口1中断函数
{
	#if 0
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
{
       USART_ReceiveData(USART2);
} 
#endif
    if(USART_GetITStatus(USART2,USART_IT_RXNE))
    {
    	u8 rec =  USART_ReceiveData(USART2);
        rec2_buf[rec2_data_len++] = USART_ReceiveData(USART2);
        if(rec2_data_len >= sizeof(rec2_buf))
        {
            rec2_data_len = 0;
        }
        receive2_finish = sys_ms_cnt;
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}


