#include "USART3.h"
#include "ADC.h"
#include "delay.h"
//#include "MPU6050.h"
//#include "display.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
	USART3->DR = (u8) ch;      
	return ch;
}
#endif 

u32 receive3_finish = 0;
u8 rec3_buf[16] = {0};
u8 rec3_data_len = 0;
extern u32 sys_ms_cnt;


void USART3_init(void)           //串口初始化
{
	GPIO_InitTypeDef  Usart_IOinit;                        //配置IO口
	USART_InitTypeDef  Usart_init;                         //配置串口
	NVIC_InitTypeDef  NVIC_init;                           //配置中断

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能端口A时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能串口1时钟

	Usart_IOinit.GPIO_Pin=GPIO_Pin_10;                      //配置GPIOA9  TX
	Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	Usart_IOinit.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&Usart_IOinit);

	Usart_IOinit.GPIO_Pin=GPIO_Pin_11;                     //配置GPIOA10  RX
	Usart_IOinit.GPIO_Speed=GPIO_Speed_50MHz;
	Usart_IOinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&Usart_IOinit);

	Usart_init.USART_BaudRate=115200;                        //配置串口
	Usart_init.USART_WordLength=USART_HardwareFlowControl_None;
	Usart_init.USART_StopBits=USART_StopBits_1;
	Usart_init.USART_Parity=USART_Parity_No;
	Usart_init.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	Usart_init.USART_HardwareFlowControl=USART_WordLength_8b;
	USART_Init(USART3,&Usart_init);

	USART_Cmd(USART3,ENABLE);                             //使能串口1

	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);          //选择串口中断

	NVIC_init.NVIC_IRQChannel=USART3_IRQn;                //配置中断函数
	NVIC_init.NVIC_IRQChannelCmd=ENABLE;
	NVIC_init.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_init.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_init);
}

//串口1发送1个字符 
//c:要发送的字符
void usart3_send_data(u8 c)
{   	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
	USART_SendData(USART3,c);  
} 
void usart3_sendstring(u8 * pStr)
{   	
    while(*pStr != '\0')
    {
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
        USART_SendData(USART3,*pStr++); 
    }		
    		
} 

int USART3_FrameCheckSum(u8 *buf , u8 len)
{
    int i =0;
    u16 sum = 0;
    if(buf == 0 )
    {
        return 1;//帧错误
    }
    if(buf[0] != 0x3A || buf[2] != 0xFF)
    {
        return 2;//帧错误
    }
    len = buf[3]+3; //要校验的数据数
    for(i = 1; i < (len+1); i++)
    {
        sum += buf[i];
    }
    return sum - (buf[i]+(((u16)buf[i+1])<<8));

}


//3A+ID+FF+长度+状态+电池状态1+电池状态2+电池状态3+电池状态4+SOC+CheckSum低+CheckSum高+0D+0A
//0   1  2   3   4      5         6         7        8        9    10         11       12 13
void USART3_Receive_DataAnalysis(void)
{
	if((rec3_data_len > 0 && (sys_ms_cnt - receive3_finish) > 5))
	{
            memset(rec3_buf,0,sizeof(rec3_buf));
            rec3_data_len = 0;
	}
}

void USART3_IRQHandler(void)                         //串口1中断函数
{
    if(USART_GetITStatus(USART3,USART_IT_RXNE))
    {
        rec3_buf[rec3_data_len++] = USART_ReceiveData(USART3);
        if(rec3_data_len >= sizeof(rec3_buf))
        {
            rec3_data_len = 0;
        }
        receive3_finish = sys_ms_cnt;
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}


