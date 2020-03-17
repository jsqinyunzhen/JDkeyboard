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


/*������ͨѶuart  9600 */
void USART2_init(void)           //���ڳ�ʼ��
{
	GPIO_InitTypeDef  Usart_IOinit;                        //����IO��
	USART_InitTypeDef  Usart_init;                         //���ô���
	NVIC_InitTypeDef  NVIC_init;                           //�����ж�

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ�ܶ˿�Aʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ�ܴ���1ʱ��

	Usart_IOinit.GPIO_Pin=GPIO_Pin_2;                      //����GPIOA9  TX
	Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	Usart_IOinit.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&Usart_IOinit);

	Usart_IOinit.GPIO_Pin=GPIO_Pin_3;                     //����GPIOA10  RX
	Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	Usart_IOinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&Usart_IOinit);

	Usart_init.USART_BaudRate=9600;                        //���ô���
	Usart_init.USART_WordLength=USART_HardwareFlowControl_None;
	Usart_init.USART_StopBits=USART_StopBits_1;
	Usart_init.USART_Parity=USART_Parity_No;
	Usart_init.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	Usart_init.USART_HardwareFlowControl=USART_WordLength_8b;
	USART_Init(USART2,&Usart_init);

	USART_Cmd(USART2,ENABLE);                             //ʹ�ܴ���1

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);          //ѡ�񴮿��ж�
#if 1
	NVIC_init.NVIC_IRQChannel=USART2_IRQn;                //�����жϺ���
	NVIC_init.NVIC_IRQChannelCmd=ENABLE;
	NVIC_init.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_init.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_init);
#endif

}

//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart2_send_data(u8 c)
{   	
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
	USART_SendData(USART2,c);  
} 
void usart2_send(u8 * pbuf , u8 len)
{   	
    while(pbuf != 0 && len > 0)
    {
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
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
        return 1;//֡����
    }

    for(i = 0; i <len; i++)
    {
        sum ^= buf[i];
    }
    return sum;
}


//0x55 0xaa + 4 �ֽڿ��ţ�����ǰ��+ 2 �ֽ������ֽ�+���ֽڣ�+1 �ֽ�У��ͣ�ǰ 8 �ֽڵ����
//
u32 card_id =0; 
u32 card_id1 =0; //��һ��ˢ������
u32 card_id2 =0; //�ڶ���ˢ������
u16 overage = 0; //ˢ��ǰ�����
u16 overage1 = 0;//��һ��ˢ�������
u16 overage2 = 0;//�ڶ���ˢ�������
u8 reduce =0;
#define CARD_DATA_LEN 9 

u8 rec2_Txbuf[] = {0xAA,0x55, 0x00, 0x0A, 0xF5};//Ĭ��һ�ο�һ�飬0x0AΪ10��

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
					//���Ϳ۷�֡
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
					if(overage1 < overage && overage > 0 ) //�۷ѳɹ�
					{
						usart1_sendPaySuccess(rec2_buf+2,overage - overage1);
						overage = 0;
						//kb_SetMode(STATE_WAITCARD1_end); //�޸ĵ�����ȷ���Ƿ���Ҫˢ�ڶ���
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
					//���Ϳ۷�֡
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
					if(overage2 < overage && overage > 0 ) //�۷ѳɹ�
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

void USART2_IRQHandler(void)                         //����1�жϺ���
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


