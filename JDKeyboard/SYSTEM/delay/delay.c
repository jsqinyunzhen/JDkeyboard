#include "delay.h"

//static u8  fac_us=0;							//us延时倍乘数			   
//static u16 fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数
extern uint32_t sys_ms_cnt;
//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void sys_us_timer_init(void);

void delay_init()
{
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	//fac_us=SystemCoreClock/8000000;				//为系统时钟的1/8  
	//fac_ms=(u16)fac_us*1000;					//每个ms需要的systick时钟数   
	sys_us_timer_init();
}								    
/*不能delay超过65535us,超过建议用delay_ms*/
void delay_us(u32 us)
{
    uint16_t start_tm = TIM4->CNT;
    uint16_t now_tm = TIM4->CNT;
    uint16_t dis_tm = now_tm - start_tm;
    
    while(dis_tm < us)
    {
        now_tm = TIM4->CNT;
        dis_tm = now_tm - start_tm;
       // IWDG_ReloadCounter();
    }
    
    return;
}

void sys_us_timer_init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = 0xffff;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM4, ENABLE);
}
extern int iii;
void delay_ms(u16 ms_cnt)
{
    uint16_t start_tm = sys_ms_cnt;
    uint16_t now_tm = sys_ms_cnt;
    uint16_t dis_tm = now_tm - start_tm;

    while(dis_tm < ms_cnt)
    {
        now_tm = sys_ms_cnt;
        dis_tm = now_tm - start_tm;
        IWDG_ReloadCounter();
    }

    return;
}

#if 0
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;       					//清空计数器	  	    
} 

#endif








































