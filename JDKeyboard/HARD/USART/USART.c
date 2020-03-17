#include "USART.h"
#include "ADC.h"
#include "delay.h"
//#include "MPU6050.h"
//#include "display.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "WORK.h"

#define SETLCDID1 "SET ID:0\r\n"
#define SETLCDID2 "SET ID:10\r\n"

u32 receive_finish = 0;
//u8 rec_buf[32] = {0};
keydata uart1_rec = {0};
u8 rec_data_len = 0;
extern u32 sys_ms_cnt;
extern u8 CurDispNum ;
extern u8 LCD_ID;
extern uint8_t Version[];

#define RX_FRAME_LEN 16

void USART_TX(void ) 
{
#ifdef LCD_DEMO
    PAout(11) = 1;
    PAout(12) = 1;
#else
    PAout(8) = 1;
#endif
}
void USART_RX(void)  
{
#ifdef LCD_DEMO
       PAout(11) = 0;
       PAout(12) = 0;
#else
    PAout(8) = 0;
#endif
}

/*��������ͨѶRS485 ͨѶ4800*/
void USART_init(void)           //���ڳ�ʼ��
{
	   GPIO_InitTypeDef  Usart_IOinit;                        //����IO��
	   USART_InitTypeDef  Usart_init;                         //���ô���
	   NVIC_InitTypeDef  NVIC_init;                           //�����ж�
	
	   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ�ܶ˿�Aʱ��
	   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ�ܴ���1ʱ��
	
	   Usart_IOinit.GPIO_Pin=GPIO_Pin_9;                      //����GPIOA9  TX
	   Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	   Usart_IOinit.GPIO_Mode=GPIO_Mode_AF_PP;
	   GPIO_Init(GPIOA,&Usart_IOinit);
	
	   Usart_IOinit.GPIO_Pin=GPIO_Pin_10;                     //����GPIOA10  RX
	   Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	   Usart_IOinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	   GPIO_Init(GPIOA,&Usart_IOinit);
	
	   Usart_init.USART_BaudRate=4800;                        //���ô���
	   Usart_init.USART_WordLength=USART_HardwareFlowControl_None;
	   Usart_init.USART_StopBits=USART_StopBits_1;
	   Usart_init.USART_Parity=USART_Parity_No;
	   Usart_init.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	   Usart_init.USART_HardwareFlowControl=USART_WordLength_8b;
	   USART_Init(USART1,&Usart_init);
	   
	   USART_Cmd(USART1,ENABLE);                             //ʹ�ܴ���1
	   
	   USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);          //ѡ�񴮿��ж�
	   
	   NVIC_init.NVIC_IRQChannel=USART1_IRQn;                //�����жϺ���
	   NVIC_init.NVIC_IRQChannelCmd=ENABLE;
	   NVIC_init.NVIC_IRQChannelPreemptionPriority=1;
	   NVIC_init.NVIC_IRQChannelSubPriority=1;
	   NVIC_Init(&NVIC_init);
#ifdef LCD_DEMO
	   Usart_IOinit.GPIO_Pin=GPIO_Pin_11;                      //����485 TX
	   Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	   Usart_IOinit.GPIO_Mode=GPIO_Mode_Out_PP;
	   GPIO_Init(GPIOA,&Usart_IOinit);
	
	   Usart_IOinit.GPIO_Pin=GPIO_Pin_12;                     //����485  RX
	   Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	   Usart_IOinit.GPIO_Mode=GPIO_Mode_Out_PP;
	   GPIO_Init(GPIOA,&Usart_IOinit);
#endif
	Usart_IOinit.GPIO_Pin=GPIO_Pin_8;                      //����485 TX/RX
	Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	Usart_IOinit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&Usart_IOinit);

USART_TX();
}

//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_data(u8 c)
{   	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
	USART_SendData(USART1,c);  
} 
void usart1_send(u8 * pBuf,u8 len)
{   	
	USART_TX();
        delay_ms(5);
	while(pBuf != 0 && len > 0)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
		USART_SendData(USART1,*pBuf++); 
		len --;
	}		
	delay_ms(5);
	USART_RX();
} 
//uart1_rec.f.head == 0x7A && uart1_rec.f.head == 0x7A
//0x7A	 CMD	 LEN	 DATA	CHECKSUM	0x67
int USART1_FrameCheckSum(u8 *buf , u8 len)
{
    int i =0;
    u16 sum = 0;
    
    if(buf == 0 ||  len < 7)
    {
        return 1;//֡����
    }
    if(buf[0] != 0x7A || buf[len-1] != 0x67)
    {
        return 2;//֡����
    }
    len = buf[2]+2; //ҪУ���������
    for(i = 1; i < (len+1); i++)
    {
        sum += buf[i];
    }
    return sum - (buf[i]+(((u16)buf[i+1])<<8));

}


//0x7A	 CMD	 LEN	  DATA	CHECKSUM	0x67
//
extern u8 gKeyOn ;
extern u8 reduce ;
void USART1_Receive_DataAnalysis(void)
{
	if((rec_data_len > 0 && (sys_ms_cnt - receive_finish) > 5) )
	{
		if(USART1_FrameCheckSum (uart1_rec.buf, rec_data_len) == 0)
		{
			switch(uart1_rec.f.cmd)
			{
				case 0x02:
				{
					if(STATE_KEYON == kb_getMode() && uart1_rec.f.data[0] == gKeyOn && uart1_rec.f.data[1] == 0x01 && uart1_rec.f.data[2] > 0)
					{
						reduce =  uart1_rec.f.data[2];
						printf("can charge to WAITCARD %d \r\n",reduce);
						kb_SetMode(STATE_WAITCARD1);
					}
					else if(kb_getMode() > STATE_WAITKEY)
					{
						printf("wangguan don't charge\r\n");
						kb_SetMode(STATE_FINISH);
					}
				}
				break;
				case 0x04:
				{
					printf("\r\n%d port charge  %d minutes !\r\n",uart1_rec.f.data[0],uart1_rec.f.data[8]*60 );

					if(kb_getMode() > STATE_WAITKEY && uart1_rec.f.data[8] > 4 )//����4Сʱ�ˣ����ܳ�ֵ��
					{
						printf("\r\n timer >240  mode to finish\r\n" );
						kb_SetMode(STATE_FINISH);
					}
					else if(kb_getMode() == STATE_WAITCARD1)
					{
						kb_SetMode(STATE_WAITCARD1_end);
					}
				}
				break;
			}
		}
		memset(uart1_rec.buf,0,sizeof(uart1_rec.buf));
		rec_data_len = 0;
	}
}

void USART1_IRQHandler(void)                         //����1�жϺ���
{
	   if(USART_GetITStatus(USART1,USART_IT_RXNE))
	   {
		    //u16 data;
			  //short GX,GY,GZ;
			  //data = MPU_INIT();
			  //MPU_Get_TLY(&GX,&GY,&GZ);
		uart1_rec.buf[rec_data_len++] = USART_ReceiveData(USART1);
		if(rec_data_len >= sizeof(uart1_rec.buf))
		{
			rec_data_len = 0;
		}
		receive_finish = sys_ms_cnt;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	   }
}


