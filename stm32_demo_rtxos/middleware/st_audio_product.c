



#include "stm32f10x_it.h"
#include "st_audio_product.h"
#include "st_printf.h"
#include "pack.h"
#if (USE_SOFTTIMER_DIY ==1)
#include "mod_date_time.h"
#include "mod_time_list.h"
#endif

void audio_pa_io_init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;						//PB6复用为TIM4的通道1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}
 void audio_pa_io_disable(void)
 {
	 GPIO_SetBits(GPIOB, GPIO_Pin_7);
 
 }
 void audio_pa_io_enable(void)
 {
	 GPIO_ResetBits(GPIOB, GPIO_Pin_7);
 
 }
 //--1khz
 #if (USE_SOFTTIMER_DIY ==1)
 void init_tim1_for_softtimer(uint32_t fre)
 {
 //TIM2
//	 NVIC_InitTypeDef NVIC_InitStructure;
	 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	 TIM_OCInitTypeDef	TIM1_OCInitStructure;
//	 GPIO_InitTypeDef GPIO_InitStructure;
//	 uint16_t TIM4_Period;
//	 uint16_t TIM3_Period;
//	 uint32_t cur_time;
	 

	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//初始化与6的时钟
		 
	 TIM_DeInit(TIM1);	 /*复位TIM1定时器*/

	 TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	 TIM_TimeBaseStructure.TIM_Prescaler = 720-1;	 /* 分频720*/
	 if(fre ==1000)
		 TIM_TimeBaseStructure.TIM_Period = 100;	 /*时钟滴答的次数，够数中断这里是1ms中断一次*/	   
	 else  if(fre ==100)
		 TIM_TimeBaseStructure.TIM_Period = 1000;
	 //TIM4_Period = (uint16_t)(SystemCoreClock /TIM_Prescaler/ fre) - 1;
	 //TIM_Prescaler=SystemCoreClock/((TIM4_Period+1)*fre)=72000000/((255+1)*132000)=
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//0x0;//时钟不分频
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//增计数
	 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	 TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
 #if 0
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;						 //PB6复用为TIM4的通道1
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	 TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			 //PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
										 //一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
	 TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB5
	 TIM1_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period/2;						 //确定占空比，这个值决定了有效电平的时间。
	 TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		 //输出极性  低电平有效 TIM3_CCER[5]=1;
	 TIM_OC1Init(TIM1, &TIM1_OCInitStructure);
	 TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
#endif	 

	 TIM_Cmd(TIM1, ENABLE);//打开TIM4
	 


	 /* Clear TIM1 update pending flag	清除TIM1溢出中断标志]  */
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	 //TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);//打开TIM3
	 //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#if 0
	 NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
#endif
	// cur_time = get_curtime();
	 //No need to configure RTC....tim2 init=6111 ms TIM4_Period=2,TIM3_Period=5
	// printf("tim2 init=%d ms TIM4_Period=%d,TIM3_Period=%d\r\n",cur_time,TIM4_Period,TIM3_Period);
 }
#endif
 //音乐播放测试2
 void init_pwm_para_tim34(uint32_t pwm_fre,uint32_t fs_fre,uint8_t bit_depth)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM3_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t TIM4_Period;
	uint16_t TIM3_Period;
	uint32_t cur_time;

	//audio_pa_io_init();
	//audio_pa_io_enable();
	//TIM4_Period = (uint16_t)(SystemCoreClock /720/ fre) - 1;
	if(bit_depth ==8)
		TIM4_Period = 255;
	else if(bit_depth ==16)
		TIM4_Period = 32767;
	else
		TIM4_Period = 255;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);//初始化与6的时钟
		
	TIM_DeInit(TIM3);	/*复位TIM1定时器*/
	TIM_DeInit(TIM4);	/*复位TIM1定时器*/
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = TIM4_Period;//
	//TIM_TimeBaseStructure.TIM_Prescaler = 72;//没有预分频
	//TIM_TimeBaseStructure.TIM_Period = 100;	/*时钟滴答的次数，够数中断这里是1ms中断一次*/	
	if(TIM4_Period ==255)
	{
		if(pwm_fre == 32000)
			TIM_TimeBaseStructure.TIM_Prescaler = (9-1);//720-1;	/* 分频720*/	 
		else if(pwm_fre == 132000)
			TIM_TimeBaseStructure.TIM_Prescaler = 4;//720-1;	/* 分频720*/   
		else 
			TIM_TimeBaseStructure.TIM_Prescaler = (9-1);
	}
	else if(TIM4_Period ==32767)
	{
		if(pwm_fre == 32000)
			TIM_TimeBaseStructure.TIM_Prescaler = (9-1);//720-1;	/* 分频720*/	 
		else if(pwm_fre == 132000)
			TIM_TimeBaseStructure.TIM_Prescaler = 4;//720-1;	/* 分频720*/   
		else 
			TIM_TimeBaseStructure.TIM_Prescaler = (9-1);
	}
	//TIM4_Period = (uint16_t)(SystemCoreClock /TIM_Prescaler/ fre) - 1;
	//TIM_Prescaler=SystemCoreClock/((TIM4_Period+1)*fre)=72000000/((255+1)*132000)=
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;//时钟不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//增计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;						//PB6复用为TIM4的通道1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
										//一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	  //输入/捕获2输出允许	OC2信号输出到对应的输出引脚PB5
	TIM3_OCInitStructure.TIM_Pulse = TIM4_Period/2; 						//确定占空比，这个值决定了有效电平的时间。
	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性	低电平有效 TIM3_CCER[5]=1;
	TIM_OC1Init(TIM4, &TIM3_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
//	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM4, ENABLE);//打开TIM4
	
	//TEST 16KHZ
#if 1
	if(fs_fre != 16000)
	{
		printf("fs_fre=%d not support\r\n",fs_fre);
	}
	TIM3_Period = (uint16_t)(SystemCoreClock /36/ (16*1000)) - 1;
	TIM_TimeBaseStructure.TIM_Period = TIM3_Period;//
	//TIM_TimeBaseStructure.TIM_Prescaler = 72;//没有预分频
	//TIM_TimeBaseStructure.TIM_Period = 100;	/*时钟滴答的次数，够数中断这里是1ms中断一次*/	  
	TIM_TimeBaseStructure.TIM_Prescaler = 36-1; /* 分频720*/	 
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;//时钟不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//增计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//PB0复用为TIM3的通道3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
										//一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	  //输入/捕获2输出允许	OC2信号输出到对应的输出引脚PB5
	TIM3_OCInitStructure.TIM_Pulse = TIM3_Period/2; 						//确定占空比，这个值决定了有效电平的时间。
	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		//输出极性	低电平有效 TIM3_CCER[5]=1;
	TIM_OC3Init(TIM3, &TIM3_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	
	//TIM_Cmd(TIM3, ENABLE);//打开TIM3
#endif
#if 1
	/* Clear TIM1 update pending flag  清除TIM1溢出中断标志]  */
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	//TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);//打开TIM3
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	cur_time = get_curtime2();
	//No need to configure RTC....tim2 init=6111 ms TIM4_Period=2,TIM3_Period=5
	printf("tim34 init=%d ms TIM4_Period=%d,TIM3_Period=%d\r\n",cur_time,TIM4_Period,TIM3_Period);
}

//借用TIM4 在GPIOB 6 输出占空比XXXX，频率为32K波形(100K基础上计算)
//借用TIM3 （16KZ）的中断 调整TIM4 输出的PWM 占空比
void set_pwm_period_init_tim34(uint32_t fre)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM3_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t TIM4_Period;
	uint16_t TIM3_Period;
	uint32_t cur_time;

	//audio_pa_io_init();
	//audio_pa_io_enable();
	//TIM4_Period = (uint16_t)(SystemCoreClock /720/ fre) - 1;
	TIM4_Period = 255;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);//初始化与6的时钟
		
	TIM_DeInit(TIM3);	/*复位TIM1定时器*/
	TIM_DeInit(TIM4);	/*复位TIM1定时器*/
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = TIM4_Period;//
	//TIM_TimeBaseStructure.TIM_Prescaler = 72;//没有预分频
	//TIM_TimeBaseStructure.TIM_Period = 100;	/*时钟滴答的次数，够数中断这里是1ms中断一次*/     
	if(fre == 32000)
		TIM_TimeBaseStructure.TIM_Prescaler = (9-1);//720-1;	/* 分频720*/     
	else if(fre == 132000)
		TIM_TimeBaseStructure.TIM_Prescaler = 4;//720-1;	/* 分频720*/   
	else 
		TIM_TimeBaseStructure.TIM_Prescaler = (9-1);
	//TIM4_Period = (uint16_t)(SystemCoreClock /TIM_Prescaler/ fre) - 1;
	//TIM_Prescaler=SystemCoreClock/((TIM4_Period+1)*fre)=72000000/((255+1)*132000)=
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;//时钟不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//增计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;						//PB6复用为TIM4的通道1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			    //PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
										//一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB5
	TIM3_OCInitStructure.TIM_Pulse = TIM4_Period/2; 					    //确定占空比，这个值决定了有效电平的时间。
	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	    //输出极性  低电平有效 TIM3_CCER[5]=1;
	TIM_OC1Init(TIM4, &TIM3_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
//	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM4, ENABLE);//打开TIM4
	
	//TEST 16KHZ
#if 1
	TIM3_Period = (uint16_t)(SystemCoreClock /36/ (16*1000)) - 1;
	TIM_TimeBaseStructure.TIM_Period = TIM3_Period;//
	//TIM_TimeBaseStructure.TIM_Prescaler = 72;//没有预分频
	//TIM_TimeBaseStructure.TIM_Period = 100;	/*时钟滴答的次数，够数中断这里是1ms中断一次*/     
	TIM_TimeBaseStructure.TIM_Prescaler = 36-1;	/* 分频720*/     
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;//时钟不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//增计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//PB0复用为TIM3的通道3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			    //PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
										//一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB5
	TIM3_OCInitStructure.TIM_Pulse = TIM3_Period/2; 					    //确定占空比，这个值决定了有效电平的时间。
	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 	    //输出极性  低电平有效 TIM3_CCER[5]=1;
	TIM_OC3Init(TIM3, &TIM3_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	
	//TIM_Cmd(TIM3, ENABLE);//打开TIM3
#endif
#if 1
	/* Clear TIM1 update pending flag  清除TIM1溢出中断标志]  */
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	//TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);//打开TIM3
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	cur_time = get_curtime();
	//No need to configure RTC....tim2 init=6111 ms TIM4_Period=2,TIM3_Period=5
	printf("tim2 init=%d ms TIM4_Period=%d,TIM3_Period=%d\r\n",cur_time,TIM4_Period,TIM3_Period);
}
void pcm_data_tim3_reable(void)
{
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);//打开TIM3
	TIM_Cmd(TIM3, ENABLE);//打开TIM3
}
void set_tim4_pwm_duty(uint8_t num)
{
	uint16_t ChannelxPulse;
	
	ChannelxPulse  = num;
	//printf("f=%s l=%d\r\n",__FILE__,__LINE__);

	//TIM_ARRPreloadConfig(TIMx,DISABLE);
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Disable);
	
	//TIMx->ARR=TIM2_Period;                //更新预装载值 
	TIM4->CCR1 =ChannelxPulse;//(ChannelxPulse<<4);
	//
	//TIM_ARRPreloadConfig(TIMx,ENABLE);
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);

 
}
//unsigned char pcm_buff[64*1024];
#define PCM_BUF_LEN (16*1024)
unsigned char pcm_buff_index = 0;//0--invalid ,1--pcm_buff 2 pcm_buff_other
unsigned char pcm_buff_flag=0;//0 no data--pcm_buff,1-full data
unsigned char pcm_buff_other_flag=0;//0 no data--pcm_buff,1-full data
unsigned char pcm_buff[PCM_BUF_LEN];
unsigned char pcm_buff_other[PCM_BUF_LEN];

unsigned char* rom_address;
unsigned int total_pcm_len;
unsigned int left_pcm_len;

unsigned int get_pcm_leftlen(void)
{	

#if(PCM_IS == PCM_IS_CHAEGE_FULL_PART)
	return left_pcm_len;
#else
	return 0;
#endif
}
unsigned int get_pcm_len(void)
{	
#if(PCM_IS == PCM_IS_CONFIGUREOK)
	return configure_ok_pcm_len;
#elif(PCM_IS == PCM_IS_CHAEGE_PUB)	
	
	return charge_part1_pcm_len;
#elif(PCM_IS == PCM_IS_CHAEGE_ALL)
	return charge_pcm_len;
#elif(PCM_IS == PCM_IS_CHAEGE_FULL_PART)
	return total_pcm_len;
#elif(PCM_IS == PCM_IS_NUM6)
	return audio_num6_pcm_len;

#elif(PCM_IS == PCM_IS_IN_FLASH)
	return total_pcm_len;

#else
	#error "fail get_pcm_len"
#endif
}
unsigned int  pwm_buffer1_pos = 0;
unsigned int  pwm_buffer2_pos = 0;
unsigned char buffer_pos = 1;
void reset_pwm_out_index(void)
{
	pwm_buffer1_pos = 0;
	pwm_buffer2_pos = 0;
	buffer_pos = 1;
}


unsigned char get_audio_pcm_ram_address_len(AUDIO_OUT_TYPE audio_type,unsigned char**pram_address,unsigned int*plen )
{
	unsigned char ret =0;
	#if(PCM_IS == PCM_IS_IN_FLASH)
	unsigned char* pcm_flash_address = NULL;
	unsigned int pcm_flash_len =0;
	char* filename = (char*)paudio_pcm[audio_type];
	printf("%d --%s\r\n",audio_type,filename);
	ret = get_pcm_data_by_name(filename,&pcm_flash_address,&pcm_flash_len);
	if(ret == 0)
	{
		*pram_address = pcm_flash_address;
		*plen = pcm_flash_len;
	}
	return ret;
	#endif
	switch(audio_type)
	{
		case AUDIO_DEVICE_OPEN:
			#if(PCM_IS == PCM_IS_IN_FLASH)
			ret = get_pcm_data_by_name(FILE_AUDIO_DEVICE_OPEN,&pcm_flash_address,&pcm_flash_len);
			if(ret == 0)
			{
				*pram_address = pcm_flash_address;
				*plen = pcm_flash_len;
			}

			#elif(PCM_IS == PCM_IS_CHAEGE_FULL_PART)
			*pram_address = (unsigned char*)audio_device_open_pcm;
			*plen = audio_device_open_pcm_len;
			#endif
			break;
		case AUDIO_INNER_TEMP_TOO_HIGH:
			#if(PCM_IS == PCM_IS_IN_FLASH)
			ret = get_pcm_data_by_name(FILE_AUDIO_INNER_TEMP_TOO_HIGH,&pcm_flash_address,&pcm_flash_len);
			if(ret == 0)
			{
				*pram_address = pcm_flash_address;
				*plen = pcm_flash_len;
			}
			#elif(PCM_IS == PCM_IS_CHAEGE_FULL_PART)
			*pram_address = (unsigned char*)audio_temp_high_pcm;
			*plen = audio_temp_high_pcm_len;
			#endif
			break;
		
		case AUDIO_CHARGE_START:
			#if(PCM_IS == PCM_IS_IN_FLASH)
			ret = get_pcm_data_by_name(FILE_AUDIO_CHARGE_START,&pcm_flash_address,&pcm_flash_len);
			if(ret == 0)
			{
				*pram_address = pcm_flash_address;
				*plen = pcm_flash_len;
			}

			#elif(PCM_IS == PCM_IS_CHAEGE_FULL_PART)
			*pram_address = (unsigned char*)audio_charge_start_pcm;
			*plen = audio_charge_start_pcm_len;
			#endif
			break;
		case AUDIO_NUM_1:
			#if(PCM_IS == PCM_IS_IN_FLASH)
			ret = get_pcm_data_by_name(FILE_AUDIO_NUM_1,&pcm_flash_address,&pcm_flash_len);
			if(ret == 0)
			{
				*pram_address = pcm_flash_address;
				*plen = pcm_flash_len;
			}

			#elif(PCM_IS == PCM_IS_CHAEGE_ALL)
			*pram_address = (unsigned char*)audio_num1_pcm;
			*plen = audio_num1_pcm_len;
			#endif
			break;
			case AUDIO_NUM_2:
	#if(PCM_IS == PCM_IS_IN_FLASH)
				ret = get_pcm_data_by_name(FILE_AUDIO_NUM_2,&pcm_flash_address,&pcm_flash_len);
				if(ret == 0)
				{
					*pram_address = pcm_flash_address;
					*plen = pcm_flash_len;
				}
	#endif
				break;
			case AUDIO_NUM_3:
#if(PCM_IS == PCM_IS_IN_FLASH)
				ret = get_pcm_data_by_name(FILE_AUDIO_NUM_3,&pcm_flash_address,&pcm_flash_len);
				if(ret == 0)
				{
					*pram_address = pcm_flash_address;
					*plen = pcm_flash_len;
				}
#endif
				break;
		case AUDIO_NUM_4:
#if(PCM_IS == PCM_IS_IN_FLASH)
			ret = get_pcm_data_by_name(FILE_AUDIO_NUM_4,&pcm_flash_address,&pcm_flash_len);
			if(ret == 0)
			{
				*pram_address = pcm_flash_address;
				*plen = pcm_flash_len;
			}
#endif
			break;
		case AUDIO_NUM_5:
#if(PCM_IS == PCM_IS_IN_FLASH)
			ret = get_pcm_data_by_name(FILE_AUDIO_NUM_5,&pcm_flash_address,&pcm_flash_len);
			if(ret == 0)
			{
				*pram_address = pcm_flash_address;
				*plen = pcm_flash_len;
			}
#endif
			break;

		case AUDIO_NUM_6:
#if(PCM_IS == PCM_IS_IN_FLASH)
				ret = get_pcm_data_by_name(FILE_AUDIO_NUM_6,&pcm_flash_address,&pcm_flash_len);
				if(ret == 0)
				{
					*pram_address = pcm_flash_address;
					*plen = pcm_flash_len;
				}

#elif(PCM_IS == PCM_IS_NUM6)

				*pram_address = (unsigned char*)audio_num6_pcm;
				*plen = audio_num6_pcm_len;
#endif
				break;

		default:
			ret = 1;
			printf("audio_type error=%d\r\n",audio_type);
			break;
	
	}
	return ret;
}
//dma_copy_data_from_rom to ram,以字节拷贝
static DMA_InitTypeDef  DMA_InitStructure;
__IO uint32_t CurrDataCounterBegin =0;
__IO uint32_t CurrDataCounterEnd = 0x01; /* This variable should not be initialized to 0 */


#if(PCM_ROM2RAM_DMA_SUPPORT == 1)

void NVIC_Configuration_pcmrom2ram_dma(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable DMA1 channel6 IRQ Channel */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
#endif
const unsigned char rom_char[2] = {0};
 unsigned char ram_char[2] = {0};
void dma_copy_data_init(void)
{	
	static char init_flag =0;
	  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clock */
	if(init_flag == 0)
	{
		init_flag = 1;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		NVIC_Configuration_pcmrom2ram_dma();
	#if 0
	  DMA_DeInit(DMA1_Channel6);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)rom_char;
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ram_char;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	  DMA_InitStructure.DMA_BufferSize = 2;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	  DMA_Init(DMA1_Channel6, &DMA_InitStructure);

	  /* Get Current Data Counter value before transfer begins */
	  CurrDataCounterBegin = DMA_GetCurrDataCounter(DMA1_Channel6);
	  
	  printf("CurrDataCounterBegin=%d\r\n",CurrDataCounterBegin);
	  
	/* Enable DMA1 Channel6 Transfer Complete interrupt */
	  DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, DISABLE);
	  DMA_Cmd(DMA1_Channel6, DISABLE);
	  #endif
  }
}
uint32_t dma_copy_data_start(uint8_t* SRC_Const_Buffer,uint8_t* DST_Buffer,uint32_t BufferSize)
{
	//DMA_InitTypeDef  DMA_InitStructure;
	uint32_t time_start = 0;
	uint32_t time_end = 0;
	uint32_t time_out = 200;
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
  /* System Clocks Configuration */
  //RCC_Configuration();
       
	/* NVIC configuration */
	//NVIC_Configuration();
	dma_copy_data_init();
	/* DMA1 channel6 configuration */
	printf("dma_copy_data_start BufferSize =%d\r\n",BufferSize);
	 //DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, DISABLE);
	// DMA_Cmd(DMA1_Channel6, DISABLE);

	  DMA_DeInit(PCM_ROM2RAM_DMA_CHANNEL);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;//
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer;//
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	  DMA_InitStructure.DMA_BufferSize = BufferSize;//
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	  DMA_Init(PCM_ROM2RAM_DMA_CHANNEL, &DMA_InitStructure);


	
  /* Enable DMA1 Channel6 Transfer Complete interrupt */
  DMA_ITConfig(PCM_ROM2RAM_DMA_CHANNEL, DMA_IT_TC, ENABLE);

  /* Get Current Data Counter value before transfer begins */
  CurrDataCounterBegin = DMA_GetCurrDataCounter(PCM_ROM2RAM_DMA_CHANNEL);
  CurrDataCounterEnd = 0x1;
  printf("CurrDataCounterBegin s=0x%08x d=0x%08x b=%d\r\n",SRC_Const_Buffer,DST_Buffer,CurrDataCounterBegin);
  /* Enable DMA1 Channel6 transfer */
  DMA_Cmd(PCM_ROM2RAM_DMA_CHANNEL, ENABLE);
  /* Wait the end of transmission */
  time_start = get_curtime();
  while (CurrDataCounterEnd != 0 )
  {
	  
	  time_end = get_curtime();
	  if((time_end-time_start)>=time_out)
	  {
		printf("dma pcm data time_e=%d s=%d out=%d\r\n",time_end,time_start,CurrDataCounterEnd);
		return CurrDataCounterEnd;
	  }
  }
  time_end = get_curtime();
  printf("e=%d s=%d l=%d\r\n",time_end,time_start,(time_end-time_start));
  return 0;
}
#if(PCM_IS == PCM_IS_IN_FLASH)
unsigned char* paudio_pcm[AUDIO_END+1]; 

void flash_paudio_pcm_init(void)
{

	
	paudio_pcm[AUDIO_NUM_1] = FILE_AUDIO_NUM_1;
	paudio_pcm[AUDIO_NUM_2] = FILE_AUDIO_NUM_2;
	paudio_pcm[AUDIO_NUM_3] = FILE_AUDIO_NUM_3;
	paudio_pcm[AUDIO_NUM_4] = FILE_AUDIO_NUM_4;
	paudio_pcm[AUDIO_NUM_5] = FILE_AUDIO_NUM_5;
	
	paudio_pcm[AUDIO_NUM_6] = FILE_AUDIO_NUM_6;
	paudio_pcm[AUDIO_NUM_7] = FILE_AUDIO_NUM_7;
	paudio_pcm[AUDIO_NUM_8] = FILE_AUDIO_NUM_8;
	paudio_pcm[AUDIO_NUM_9] = FILE_AUDIO_NUM_9;
	paudio_pcm[AUDIO_NUM_10] = FILE_AUDIO_NUM_10;

	paudio_pcm[AUDIO_NUM_11] = FILE_AUDIO_NUM_11;
	paudio_pcm[AUDIO_NUM_12] = FILE_AUDIO_NUM_12;
	paudio_pcm[AUDIO_NUM_13] = FILE_AUDIO_NUM_13;
	paudio_pcm[AUDIO_NUM_14] = FILE_AUDIO_NUM_14;
	paudio_pcm[AUDIO_NUM_15] = FILE_AUDIO_NUM_15;

	paudio_pcm[AUDIO_NUM_16] = FILE_AUDIO_NUM_16;
	paudio_pcm[AUDIO_NUM_17] = FILE_AUDIO_NUM_17;
	paudio_pcm[AUDIO_NUM_18] = FILE_AUDIO_NUM_18;
	paudio_pcm[AUDIO_NUM_19] = FILE_AUDIO_NUM_19;
	paudio_pcm[AUDIO_NUM_20] = FILE_AUDIO_NUM_20;

	paudio_pcm[AUDIO_DEVICE_OPEN] = FILE_AUDIO_DEVICE_OPEN;
	paudio_pcm[AUDIO_SMOKE_TOO_HIGH] = FILE_AUDIO_SMOKE_TOO_HIGH;
	paudio_pcm[AUDIO_INNER_TEMP_TOO_HIGH] = FILE_AUDIO_INNER_TEMP_TOO_HIGH;
	paudio_pcm[AUDIO_SAFE] = FILE_AUDIO_SAFE;
	paudio_pcm[AUDIO_CHARGE_METHOD] = FILE_AUDIO_CHARGE_METHOD;
	paudio_pcm[AUDIO_CHARGE_START] = FILE_AUDIO_CHARGE_START;
	paudio_pcm[AUDIO_CHARGE_END] = FILE_AUDIO_CHARGE_END;
	paudio_pcm[AUDIO_CHARGE_AO] = FILE_AUDIO_CHARGE_AO;
	paudio_pcm[AUDIO_CHARGE_BUG] = FILE_AUDIO_CHARGE_BUG;

}
#endif
void merger_audio_pcm(unsigned char index,AUDIO_OUT_TYPE audio_type)
{
	unsigned char ret = 0;
	uint32_t len_trans = 0;
	uint32_t len_autul = 0;
	uint8_t* SRC_Const_Buffer;
	unsigned int len;

	#if( PCM_IS == PCM_IS_IN_FLASH )
	unsigned int len1;
	unsigned int len2;

	unsigned char* flash_address1;
	unsigned char* flash_address2;
	#endif
	switch(index)
	{
	//one seg
		case 1:
			ret = get_audio_pcm_ram_address_len(audio_type,&rom_address,&len);
			printf("get_audio_pcm_ram_address_len len=%d\r\n",len);
			if(ret == 0)
			{
				total_pcm_len = len;
				left_pcm_len = len;

				reset_pwm_out_index();
				pcm_buff_index = 0;
				pcm_buff_flag = 0;
				pcm_buff_other_flag = 0;
				//first buffer
				if(pcm_buff_index ==0)
				{
					pcm_buff_index = 1;
					
					if(left_pcm_len > PCM_BUF_LEN)
						len_trans = PCM_BUF_LEN;
					else
						len_trans = left_pcm_len;
					
					SRC_Const_Buffer = (uint8_t*)(rom_address+total_pcm_len-left_pcm_len);
					#if( PCM_IS == PCM_IS_IN_FLASH )
					//start copy from spi flash to ram by dma
					sFLASH_ReadBuffer(pcm_buff,(uint32_t)SRC_Const_Buffer,len_trans);
					len_autul =0;
					#else
					//start copy from rom to ram by dma
					len_autul = dma_copy_data_start(SRC_Const_Buffer/*rom_address*/,pcm_buff,len_trans);
					#endif
					if(len_autul ==0)
					{
						pcm_buff_flag =1;
						left_pcm_len -= len_trans;
						 printf("dma_copy_data_start 1 success=%d,left=%d\r\n",len_trans,left_pcm_len);
						
					}
					else
					{	
						printf("dma_copy_data_start 1 fail=%d,n=%d\r\n",len_autul,len_trans);
						return ;
					
					}
				}
				//second buffer
				if((pcm_buff_index ==1) && (left_pcm_len>0))
				{
					pcm_buff_index = 2;
					
					if(left_pcm_len > PCM_BUF_LEN)
						len_trans = PCM_BUF_LEN;
					else
						len_trans = left_pcm_len;
					
					
					//start copy from rom to ram by dma
					SRC_Const_Buffer = (uint8_t*)(rom_address+total_pcm_len-left_pcm_len);
					#if( PCM_IS == PCM_IS_IN_FLASH )

					//start copy from spi flash to ram by dma
					sFLASH_ReadBuffer(pcm_buff_other,(uint32_t)SRC_Const_Buffer,len_trans);
					len_autul =0;
					#else

					len_autul = dma_copy_data_start(SRC_Const_Buffer,pcm_buff_other,len_trans);
					#endif
					if(len_autul ==0)
					{
						pcm_buff_other_flag =1;
						left_pcm_len -= len_trans;
						printf("dma_copy_data_start 2 success=%d,left=%d\r\n",len_trans,left_pcm_len);
					}
					else
					{	
						printf("dma_copy_data_start 2 fail=%d,n=%d\r\n",len_autul,len_trans);
						return ;
					
					}
				}
			}
			pcm_buffer_status();
			break;
			//two seg
		case 2:
			#if( PCM_IS == PCM_IS_IN_FLASH )
			ret = get_audio_pcm_ram_address_len(AUDIO_NUM_1,&flash_address1,&len1);
			if(ret == 0)
				total_pcm_len = len1;
			ret = get_audio_pcm_ram_address_len(AUDIO_CHARGE_START,&flash_address2,&len2);
			if(ret == 0)
				total_pcm_len += len2;
			if(ret == 0)
			{
				if((len1+len2) < sizeof(pcm_buff))
				{
					total_pcm_len = (len1+len2);
		#if 1
					//start copy from spi flash to ram by dma
					sFLASH_ReadBuffer(pcm_buff,(uint32_t)flash_address1,len1);
					sFLASH_ReadBuffer(pcm_buff+len1,(uint32_t)flash_address2,len2);
		#else
					memcpy(pcm_buff,rom_address,len);
		#endif
				}
				else
					printf("get_audio_pcm_ram_address_len error\r\n");
			}

			#else
			ret = get_audio_pcm_ram_address_len(AUDIO_NUM_1,&rom_address,&len);
			if(ret == 0)
				total_pcm_len = len;
			ret = get_audio_pcm_ram_address_len(AUDIO_CHARGE_START,&rom_address,&len);
			if(ret == 0)
				total_pcm_len += len;
			if(ret == 0)
			{
				if(len < sizeof(pcm_buff))
				{
					total_pcm_len = len;

					memcpy(pcm_buff,rom_address,len);
				
				}
				else
					printf("get_audio_pcm_ram_address_len error\r\n");
			}
			#endif
			break;
		default:
			printf("merger_audio_pcm error\r\n");
			total_pcm_len = 0;
			left_pcm_len = 0;
			break;
	}
}
unsigned char get_pcm_value(unsigned int  pos)
{	
	unsigned int cur_pos = 0;
//	uint32_t len_trans = 0;
//	uint32_t len_autul = 0;
//	uint8_t* SRC_Const_Buffer  = NULL;
	
#if(PCM_IS == PCM_IS_CONFIGUREOK)
	return configure_ok_pcm[pos];
#elif(PCM_IS == PCM_IS_CHAEGE_PUB)	
	return charge_part1_pcm[pos];
#elif(PCM_IS == PCM_IS_CHAEGE_ALL)
	return charge_pcm[pos];


#elif((PCM_IS == PCM_IS_CHAEGE_FULL_PART)||(PCM_IS == PCM_IS_IN_FLASH)||(PCM_IS == PCM_IS_NUM6))
	//if(pcm_buff_index ==1)
	{
		if((pcm_buff_flag == 1) && (buffer_pos ==1 ))
		{
			cur_pos = pwm_buffer1_pos;
			pwm_buffer1_pos ++;
			if((left_pcm_len > 0) && (pcm_buff_other_flag == 0))
			{
				//notify other task to start dma copy to pcm_buff_other
				//printf("notify other task to start dma copy to pcm_buff_other\r\n");
				#if (USE_SOFTTIMER_DIY ==1)
				if(time_status_get(timer_dma_pcm) == 0)
					time_restart(timer_dma_pcm,1);
				#else
				if(get_status_timer_trig_dma_pcm() == 0)
					restart_timer_trig_dma_pcm();
				#endif
				#if 0
				if(left_pcm_len > PCM_BUF_LEN)
					len_trans = PCM_BUF_LEN;
				else
					len_trans = left_pcm_len;
				
				SRC_Const_Buffer = (uint8_t*)(rom_address+total_pcm_len-left_pcm_len);
				//start copy from rom to ram by dma
				len_autul = dma_copy_data_start(SRC_Const_Buffer,pcm_buff_other,len_trans);
				if(len_autul ==0)
				{
					pcm_buff_other_flag =1;
					left_pcm_len -= len_trans;
					printf("dma_copy_data_start 3 success=%d,left=%d\r\n",len_trans,left_pcm_len);
				}
				else
				{	
					printf("dma_copy_data_start 3 fail=%d,n=%d\r\n",len_autul,len_trans);
					//return ;
				
				}
				#endif
			}
			if(pwm_buffer1_pos == PCM_BUF_LEN)
			{
				pwm_buffer1_pos = 0;
				pcm_buff_flag = 0;
				buffer_pos = 2;
			}
			return pcm_buff[cur_pos];
		}
		else if((pcm_buff_other_flag == 1) && (buffer_pos ==2 ))
		{
			cur_pos = pwm_buffer2_pos;
			pwm_buffer2_pos ++;
			if((left_pcm_len > 0) && (pcm_buff_flag == 0))
			{
			//	printf("notify other task to start dma copy to pcm_buff\r\n");
#if (USE_SOFTTIMER_DIY ==1)
				if(time_status_get(timer_dma_pcm) == 0)
					time_restart(timer_dma_pcm,1);
#else
				if(get_status_timer_trig_dma_pcm() == 0)
					restart_timer_trig_dma_pcm();
#endif

				#if 0
				if(left_pcm_len > PCM_BUF_LEN)
					len_trans = PCM_BUF_LEN;
				else
					len_trans = left_pcm_len;
				
				SRC_Const_Buffer = (uint8_t*)(rom_address+total_pcm_len-left_pcm_len);
				//start copy from rom to ram by dma
				len_autul = dma_copy_data_start(SRC_Const_Buffer,pcm_buff,len_trans);
				if(len_autul ==0)
				{
					pcm_buff_flag =1;
					left_pcm_len -= len_trans;
					printf("dma_copy_data_start 4 success=%d,left=%d\r\n",len_trans,left_pcm_len);
				}
				else
				{	
					printf("dma_copy_data_start 4 fail=%d,n=%d\r\n",len_autul,len_trans);
					//return ;
				
				}
				#endif
			}
			if(pwm_buffer2_pos == PCM_BUF_LEN)
			{
				pwm_buffer2_pos = 0;
				pcm_buff_other_flag = 0;
				buffer_pos = 1;
				
			}
			return pcm_buff_other[cur_pos];
		}
		else
			return 0;
	}
	/*
	else if(pcm_buff_flag == 1)
	{
		return pcm_buff_other[pos];
	}
	else
	{
		printf("pcm_buff_flag error=%d\r\n",pcm_buff_flag);
		return 128;
	}
	*/
#else
	#error "fail get_pcm_value"
#endif
}

#if (USE_SOFTTIMER_DIY ==1)
void timer_dma_pcm_data( void)
#else
void timer_dma_pcm_data( void	*pram)
#endif
{	
	//unsigned int cur_pos = 0;
	uint32_t len_trans = 0;
	uint32_t len_autul = 0;
	uint8_t* SRC_Const_Buffer  = NULL;

	printf("notify other task to start dma copy to pcm_buff_other\r\n");
	//if(pcm_buff_flag == 1)
	//if((pcm_buff_flag == 1) && (buffer_pos ==1 ))
	//reset_timer_trig_dma_pcm();
	 if((pcm_buff_flag == 1)  && (buffer_pos ==1 ))
	{
		if((left_pcm_len > 0) && (pcm_buff_other_flag == 0))
		{
			//notify other task to start dma copy to pcm_buff_other
			#if 1
			if(left_pcm_len > PCM_BUF_LEN)
				len_trans = PCM_BUF_LEN;
			else
				len_trans = left_pcm_len;
			
			SRC_Const_Buffer = (uint8_t*)(rom_address+total_pcm_len-left_pcm_len);
#if( PCM_IS == PCM_IS_IN_FLASH )
			
			//start copy from spi flash to ram by dma
			sFLASH_ReadBuffer(pcm_buff_other,(uint32_t)SRC_Const_Buffer,len_trans);
			len_autul =0;
#else

			//start copy from rom to ram by dma
			len_autul = dma_copy_data_start(SRC_Const_Buffer,pcm_buff_other,len_trans);
#endif
			if(len_autul ==0)
			{
				pcm_buff_other_flag =1;
				left_pcm_len -= len_trans;
				printf("dma_copy_data_start 4 success=%d,left=%d\r\n",len_trans,left_pcm_len);
			}
			else
			{	
				printf("dma_copy_data_start 4 fail=%d,n=%d\r\n",len_autul,len_trans);
				//return ;
			
			}
			if(left_pcm_len == 0)
				printf("******dma_copy_data 4 ok********\r\n");
			#endif
		}
		else
			printf("******dma_copy_data 6 ok********\r\n");

		//reset_timer_trig_dma_pcm();
		return ;
	
	}
	else if((pcm_buff_other_flag == 1)  && (buffer_pos ==2 ))
	{
		if((left_pcm_len > 0) && (pcm_buff_flag == 0))
		{
			//notify other task to start dma copy to pcm_buff
			#if 1
			if(left_pcm_len > PCM_BUF_LEN)
				len_trans = PCM_BUF_LEN;
			else
				len_trans = left_pcm_len;
			
			SRC_Const_Buffer = (uint8_t*)(rom_address+total_pcm_len-left_pcm_len);
			#if( PCM_IS == PCM_IS_IN_FLASH )
			
			//start copy from spi flash to ram by dma
			sFLASH_ReadBuffer(pcm_buff,(uint32_t)SRC_Const_Buffer,len_trans);
			len_autul =0;
			#else
			//start copy from rom to ram by dma
			len_autul = dma_copy_data_start(SRC_Const_Buffer,pcm_buff,len_trans);
			#endif
			if(len_autul ==0)
			{
				pcm_buff_flag =1;
				left_pcm_len -= len_trans;
				printf("dma_copy_data_start 3 success=%d,left=%d\r\n",len_trans,left_pcm_len);
			}
			else
			{	
				printf("dma_copy_data_start 3 fail=%d,n=%d\r\n",len_autul,len_trans);
				//return ;
			
			}
			if(left_pcm_len == 0)
				printf("******dma_copy_data 3 ok********\r\n");
			#endif
		}
		else
			printf("******dma_copy_data 5 ok********\r\n");

		//reset_timer_trig_dma_pcm();
		return ;
	}
	else
	{
		printf("******error=********\r\n");
		return ;
	}
}
void pcm_buffer_status(void)
{
	printf("******total_pcm_len=%d********\r\n",total_pcm_len);
	printf("******left_pcm_len=%d********\r\n",left_pcm_len);
	printf("******pcm_buff_flag=%d********\r\n",pcm_buff_flag);
	printf("******pcm_buff_other_flag=%d********\r\n",pcm_buff_other_flag);
	printf("******buffer_pos=%d %d %d********\r\n",buffer_pos,	pwm_buffer1_pos,pwm_buffer2_pos );
}
