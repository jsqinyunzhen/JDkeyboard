#include "stm32f10x.h"
#include <math.h> 
//注意需添加此头文件，包含了求正弦值函数sin(弧度值)；

/*
在代码中，我设置两路正弦波输出，一路输出频率为800Hz的阶梯波，另一路输出频率为1600Hz的阶梯波，
他们分别对应的DAC通道1的PA4引脚，与DAC通道2的PA5引脚。所以代码中首先初始化这两个引脚：
*/

/*************************************************************

Function : Escalatar_GPIO_Init

Deion: 阶梯波相关引脚配置

Input : none

return : none

*************************************************************/

static void Escalatar_GPIO_Init(void)

{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //初始化引脚时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; //DAC CH1与CH2对应的引脚

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*接下去要配置定时器了，定时器的作用就是设置阶梯波的频率，由于要输出两路频率不相同的定时器，
所以这里需要配置两路的定时器，一路设置频率为800Hz，另一路设置频率为1600Hz，代码如下：
*/

#define _16000Hz 72000000/32/16000 //频率值为16khz,1个正弦波是32个点，定时器周期是16K*32
#define _800Hz 72000000/6/800 //频率值为800Hz --0x3a98  
//_800Hz 0x3a98
// 480000    0x19
#define _1600Hz 72000000/6/1600 //频率值为1600Hz

/*************************************************************

Function : Escalatar_TIM_Init

Deion: 阶梯波定时器初始化

Input : none

return : none

*************************************************************/

static void Escalatar_TIM_Init(void)

{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM6, ENABLE);//初始化与6的时钟
		

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period = _800Hz;//正弦波1频率设置

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;//没有预分频

	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;//时钟不分频

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//增计数

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		

	TIM_TimeBaseStructure.TIM_Period = _1600Hz;//正弦波2频率设置

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//更新TIM2输出触发

	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);//更新TIM6输出触发

	TIM_Cmd(TIM2, ENABLE);//打开TIM2

	TIM_Cmd(TIM6, ENABLE);//打开TIM6

}
/*
这段代码中分别设置TIM2输出800Hz频率与TIM6输出1600Hz频率，设置TIM2与TIM6为更新触发。

接下去配置DAC，DAC总共2路通道，由于要输出正弦波，所以这两个通道都需要配置。代码如下：
*/

/*************************************************************

Function : Escalatar_DAC_Init

Deion: 阶梯波DAC初始化

Input : none

return : none

*************************************************************/

static void Escalatar_DAC_Init(void)

{

DAC_InitTypeDef DAC_InitStructure;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//初始化DAC的时钟


DAC_StructInit(&DAC_InitStructure);

DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;//指定DAC1的触发定时器TIM2

DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//无波形产生

DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//不是能DAC输出缓冲

DAC_Init(DAC_Channel_1, &DAC_InitStructure);//初始化DAC channel1


DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;//指定DAC2的触发定时器TIM6

DAC_Init(DAC_Channel_2, &DAC_InitStructure);//初始化DAC channel2

DAC_Cmd(DAC_Channel_1, ENABLE);//使能DAC channel1

DAC_Cmd(DAC_Channel_2, ENABLE);//使能DAC channel2

DAC_DMACmd(DAC_Channel_1, ENABLE);//使能DAC Channel1的DMA

DAC_DMACmd(DAC_Channel_2, ENABLE);//使能DAC Channel2的DMA

}

/*

配置了两个DAC通道CH1与CH2，分别设置为T2触发与T6触发，除此之外还要打开DAC的DMA功能，以便被DMA控制输出想要的波形。

讲到DMA，就要讲讲DMA的配置，它的代码如下：
*/

#define DAC_DHR8R1_Address 0x40007410//DAC通道1的8位右对齐数据保持寄存器地址

#define DAC_DHR8R2_Address 0x4000741C//DAC童道2的8位有对齐数据保持寄存器地址

const uint8_t Escalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};//阶梯形描点

/*************************************************************

Function : Escalatar_DMA_Init

Deion: 阶梯波DAM初始化

Input : none

return : none

*************************************************************/

static void Escalatar_DMA_Init(void)

{

DMA_InitTypeDef DMA_InitStructure;

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//初始化DMA2的时钟


DMA_DeInit(DMA2_Channel3);//将DMA配置成默认值

DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;//指定DMA2通道3的目标地址为DAC1_DHR12R1

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Escalator8bit;//指定DMA的源地址为数组Escalator8bit

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//外设作为数据传输的目的地

DMA_InitStructure.DMA_BufferSize = 6;//DMA缓冲区大小

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设机地址存器不变

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器递增

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据宽度为半字

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //内存数据宽度为半字

DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//工作在循环缓存模式，数据传输数为0时，自动恢复配置初值

DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//非常高优先级

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//通道未被设置成内存到内存模式，与循环模式相对

DMA_Init(DMA2_Channel3, &DMA_InitStructure);//初始化DMA


DMA_DeInit(DMA2_Channel4);

DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R2_Address;//指定DMA2通道3的目标地址为DAC2_DHR12R2

DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

DMA_Init(DMA2_Channel4, &DMA_InitStructure);


DMA_Cmd(DMA2_Channel3, ENABLE);//使能DMA的channel3

DMA_Cmd(DMA2_Channel4, ENABLE);//使能DMA的channel4

}
#define DAC_DHR12RD_Address      0x40007420

/* Init Structure definition */

uint32_t Idx = 0;  
  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//const 
uint16_t Sine12bit[32] = {
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};


//获取不同点数的正弦波数据
//point: 一周期内的取样点数
//maxnum: 一周期内对应DA输出最大值
//uint16_t sine_data_12bit[16] = {0};
 #if 0
i=1 ,2447
i=2 ,2831
i=3 ,3185
i=4 ,3495
i=5 ,3749
i=6 ,3939
i=7 ,4056
i=8 ,4095
i=9 ,4057
i=10 ,3941
i=11 ,3752
i=12 ,3498
i=13 ,3189
i=14 ,2835
i=15 ,2452
i=16 ,2052
i=17 ,1653
i=18 ,1269
i=19 ,915
i=20 ,604
i=21 ,348
i=22 ,158
i=23 ,40
i=24 ,0
i=25 ,37
i=26 ,152
i=27 ,340
i=28 ,593
i=29 ,902
i=30 ,1255
i=31 ,1639
#endif
void getSinTab(uint16_t point,uint16_t maxnum)
{
	uint16_t i=0;
	float x;//弧度
	float jiao;//角度 分度角
	jiao=360.000/point;

	printf("sizeof(Sine12bit)=%d\r\n",sizeof(Sine12bit));
	for(i=0;i<point;i++)
	{
		x=jiao*i;//得到角度值
		x=x*0.01744; //角度转弧度  弧度=角度*（π/180）
		Sine12bit[i]=(maxnum/2)*sin(x)+(maxnum/2);
		printf("i=%d ,%d\r\n",i,Sine12bit[i]);
	}
	

}
//#define count(x) sizeof(x)/sizeof(uint16_t)
#define HZ(x) (uint16_t)(72000000/(sizeof(Sine12bit)/sizeof(uint16_t))/x)     //计算Hz
//#define HZ(x) (uint16_t)(72000000/32/x)     //计算Hz
void  Set_Period(uint16_t value)
{
     TIM_ARRPreloadConfig(TIM2,DISABLE);
     TIM2->ARR=HZ(value);                //更新预装载值 
     TIM_ARRPreloadConfig(TIM2,ENABLE);

}

static void sine_TIM_Init(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM6, ENABLE);//初始化与6的时钟
		

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period = _16000Hz;//正弦波1频率设置

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;//没有预分频

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//0x0;//时钟不分频

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//增计数

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		


	TIM_Cmd(TIM2, ENABLE);//打开TIM2



}
static void sine_DMA_Init(void)

{

DMA_InitTypeDef DMA_InitStructure;

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//初始化DMA2的时钟


DMA_DeInit(DMA2_Channel3);//将DMA配置成默认值

DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_Address;//指定DMA2通道3的目标地址为DAC1_DHR12R1

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;//指定DMA的源地址为数组Escalator8bit

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//外设作为数据传输的目的地

DMA_InitStructure.DMA_BufferSize = 32;//DMA缓冲区大小

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设机地址存器不变

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器递增

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//DMA_PeripheralDataSize_Byte;//外设数据宽度为半字

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//DMA_MemoryDataSize_Byte; //内存数据宽度为半字

DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//工作在循环缓存模式，数据传输数为0时，自动恢复配置初值

DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//非常高优先级

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//通道未被设置成内存到内存模式，与循环模式相对

DMA_Init(DMA2_Channel3, &DMA_InitStructure);//初始化DMA

DMA_Cmd(DMA2_Channel3, ENABLE);//使能DMA的channel3
}

/*************************************************************

Function : Escalatar_Init

Deion: 阶梯波初始化

Input : none

return : none

*************************************************************/

void Escalatar_Init(void)
{

	Escalatar_GPIO_Init();

	Escalatar_TIM_Init();

	Escalatar_DAC_Init();

	Escalatar_DMA_Init();

	getSinTab(32,4096);

	sine_TIM_Init();
	Set_Period(32*1000);
	sine_DMA_Init();
}
/*
频率：
//Fpwm = 72M / ((arr+1)*(psc+1))(单位：Hz)
占空比：

//duty circle = TIM3->CCR1 / arr(单位：%)

     - TIM1_Period = (SystemCoreClock / 17570) - 1
   The channel 1 and channel 1N duty cycle is set to 50%
   The channel 2 and channel 2N duty cycle is set to 37.5%
   The channel 3 and channel 3N duty cycle is set to 25%
   The channel 4 duty cycle is set to 12.5%
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
*/
void set_pwm_period_duty(TIM_TypeDef* TIMx,uint32_t fre,uint8_t duty)
{
	uint16_t TIM1_Period,ChannelxPulse;
	
	TIM1_Period = (uint16_t)(SystemCoreClock / fre) - 1;

	ChannelxPulse=duty* (TIM1_Period - 1) / 100;

	//PWM_Config_step((u16)T_ARR, (u16)DPwm_CCR2, 1);
	//PWM_Period = (u16)T_ARR;
	
	TIM_ARRPreloadConfig(TIM1,DISABLE);
	TIM_OC1PreloadConfig(TIM1,DISABLE);
	
	TIMx->ARR=TIM1_Period;                //更新预装载值 
	TIMx->CCR1 =ChannelxPulse;
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	TIM_OC1PreloadConfig(TIMx,ENABLE);

 
}



void pwm_tim_test(void)
{
	GPIO_InitTypeDef     GPIO_InitStructure;


	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t TimerPeriod = 0;
	uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;
	
	/* TIM1, GPIOA, GPIOB, GPIOE and AFIO clocks enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE|
							RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
	
/* TIM1 Configuration ---------------------------------------------------
   Generate 7 PWM signals with 4 different duty cycles:
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
   SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
   and Connectivity line devices and to 24 MHz for Low-Density Value line and
   Medium-Density Value line devices
   
   The objective is to generate 7 PWM signal at 17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
   The channel 1 and channel 1N duty cycle is set to 50%
   The channel 2 and channel 2N duty cycle is set to 37.5%
   The channel 3 and channel 3N duty cycle is set to 25%
   The channel 4 duty cycle is set to 12.5%
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
  ----------------------------------------------------------------------- */
  /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / 17570 ) - 1;
  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
  Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
  /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
  Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
  /* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
  Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
  /* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
  Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod- 1)) / 1000);

  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
  set_pwm_period_duty(TIM1,5000,50);	
}
void NVIC_Configuration_TIM2(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;


  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 }
//static void TIM2_int_Init(void)
 //https://blog.csdn.net/fanxp66/article/details/80264700
 
 
 //音乐播放测试1.fail
 //借用TIM3 在GPIOA 6 输出占空比50%，频率为100K/FRE波形，频率为声音音调频率
//借用TIM3 的中断 调整当前音调（频率）以及其持续时间
void set_pwm_period_init(TIM_TypeDef* TIMx,uint32_t fre,uint8_t num)
{
//TIM2
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM3_OCInitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t TIM2_Period;
	
	uint32_t cur_time;
	TIM2_Period = (uint16_t)(SystemCoreClock /720/ fre) - 1;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//初始化与6的时钟
		
	TIM_DeInit(TIM3);	/*复位TIM1定时器*/
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = TIM2_Period;//
	//TIM_TimeBaseStructure.TIM_Prescaler = 72;//没有预分频
	//TIM_TimeBaseStructure.TIM_Period = 100;	/*时钟滴答的次数，够数中断这里是1ms中断一次*/     
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1;	/* 分频720*/     
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;//时钟不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//增计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);



 

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;						//PB5复用为TIM3的通道2
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			    //PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
										//一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB5
	TIM3_OCInitStructure.TIM_Pulse = TIM2_Period/2; 					    //确定占空比，这个值决定了有效电平的时间。
	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 	    //输出极性  低电平有效 TIM3_CCER[5]=1;

	TIM_OC1Init(TIM3, &TIM3_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
//	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM3, ENABLE);//打开TIM2
/* Clear TIM1 update pending flag  清除TIM1溢出中断标志]  */
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);//打开TIM2
	  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
  
	cur_time = get_curtime();
	printf("tim2 init=%d ms\r\n",cur_time);
}

void set_pwm_fre_num(TIM_TypeDef* TIMx,uint32_t fre,uint8_t num)
{
	uint16_t TIM2_Period,ChannelxPulse;
	TIM2_Period = (uint16_t)(SystemCoreClock /720/ fre) - 1;
	ChannelxPulse  =TIM2_Period/2;
	TIM_ARRPreloadConfig(TIMx,DISABLE);
	TIM_OC1PreloadConfig(TIMx,DISABLE);
	
	TIMx->ARR=TIM2_Period;                //更新预装载值 
	TIMx->CCR1 =ChannelxPulse;
	//
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	TIM_OC1PreloadConfig(TIMx,ENABLE);

 
}
//         低Si Do Re  Mi  Fa So  La  Si ¸高Do¸高Re¸高Mi¸高Fa¸高So 无

uint16_t tone[] ={247,262,294,330,349,392,440, 294,523,587, 659, 698,784,1000};

//红尘情歌
 uint8_t music[]={ 5,5,6,8,7,6,5,6,13,13,//音调
        5,5,6,8,7,6,5,3,13,13,
         2,2,3,5,3,5,6,3,2,1,
        6,6,5,6,5,3,6,5,13,13,
 
        5,5,6,8,7,6,5,6,13,13,
        5,5,6,8,7,6,5,3,13,13,
         2,2,3,5,3,5,6,3,2,1,
        6,6,5,6,5,3,6,1, 
 
        13,8,9,10,10,9,8,10,9,8,6,
        13,6,8,9,9,8,6,9,8,6,5,
        13,2,3,5,5,3,5,5,6,8,7,6,
        6,10,9,9,8,6,5,6,8
 }; 
 uint8_t time[] = { 2,4,2,2,2,2,2,8,4, 4, //时间
        2,4,2,2,2,2,2,8,4, 4, 
        2,4,2,4,2,2,4,2,2,8,
        2,4,2,2,2,2,2,8,4 ,4, 
 
        2,4,2,2,2,2,2,8,4, 4, 
        2,4,2,2,2,2,2,8,4, 4, 
        2,4,2,4,2,2,4,2,2,8,
        2,4,2,2,2,2,2,8,
 
        4, 2,2,2, 4, 2,2,2, 2,2,8,
        4, 2,2,2,4,2,2,2,2,2,8,
        4, 2,2,2,4,2,2,5,2,6,2,4,
        2,2 ,2,4,2,4,2,2,12
 }; 
 uint8_t total_times = 0;
 uint8_t play_cur_tone(void)	 
{
	static int i = 0;
	
	uint16_t fre_cur = 0;
	//for(i=0;i<sizeof(music)/sizeof(music[0]);i++)
	if(i < sizeof(music)/sizeof(music[0]))
	{
		fre_cur = tone[music[i]];
		total_times = time[i];
		set_pwm_fre_num(TIM3,fre_cur,time[i]);
		i++;
		return 0;
	}
	else
	{
		i = 0;
		return 1;
	}
}
uint8_t get_cur_play_times(void)	
{
	return total_times*2;
}
 