
/****************************************
 * 文件名  ：main.c
 * 描述    ：通过串口调试软件，向板子发送数据，板子接收到数据后，立即回传给电脑。         
 * 实验平台：MINI STM32开发板 基于STM32F103VET6
 * 库版本  ：ST3.0.0  																										  
*********************************************************/

#include "stm32f10x.h"
#include "USART.h"
#include "USART2.h"
#include "USART3.h"
#include "SYS.h"
#include "delay.h"
#include "TM1628.h"
#include "stdio.h"

u32 sys_ms_cnt = 0;
uint8_t gDevIsSlave = 0;
extern u8 testkey;
extern void USART3_init(void)  ;

void Led_OnOff(void)
{
	static u32 led_delay = 0;
	
	if(sys_ms_cnt - led_delay > 2000)
	{
		led_delay = sys_ms_cnt;
		PAout(6)=!PAout(6);
		PAout(7)=!PAout(7);
#ifdef LCD_DEMO
		PAout(8)=!PAout(8);
#endif		
		if(1||gDevIsSlave)//test
		{
			//u8 b_io = PAout(8);
			//b_io?printf("Led7\r\n"):printf("Led6\r\n");
			//usart2_sendstring("Led_onff");
			//usart1_send("runin\r\n",strlen("runin\r\n"));
		}
		
		//testkey = !testkey;
	}
}

void Device_IsSlave(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;						//????
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	gDevIsSlave = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13);
}
#define IAP_ADDR 0X08000000
#define ApplicationAddress    0x08008000 //32k
#define APP_OFFSET (ApplicationAddress-IAP_ADDR)

int main(void)
{  
	SysTick_Config(72000);
#if 1 //is iap
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,APP_OFFSET);
#endif
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Device_IsSlave();
	delay_init();
	LED_IO_INIT();
	TM1628_init(); 

	USART_init();//
	USART2_init();
	USART3_init();

	while (1)
	{	 
		Led_OnOff();
		KEY_Scan();
		USART1_Receive_DataAnalysis();//解析网关通讯
		USART2_Receive_DataAnalysis();//解析刷卡数据帧
		kb_mode_process();
		if(gDevIsSlave)
		{
			
		}
		else
		{
			
		}
	}
}




