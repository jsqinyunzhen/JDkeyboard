#include "PWM.h"
#include "sys.h"
#if 0
void PWM_init(u16 arr,u16 psc)                       //PWM≈‰÷√
{
	  GPIO_InitTypeDef  GPIO_PWMinit;
	  TIM_TimeBaseInitTypeDef  TIM_PWMinit;
	  TIM_OCInitTypeDef  TIM_OCInit;
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	  //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	  GPIO_PWMinit.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;                 //≈‰÷√PWMµƒ ‰≥ˆIO
	  GPIO_PWMinit.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_PWMinit.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA,&GPIO_PWMinit);
	  
	  GPIO_PWMinit.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;                 //≈‰÷√PWMµƒ ‰≥ˆIO
	  GPIO_PWMinit.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_PWMinit.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB,&GPIO_PWMinit);
	
    TIM_PWMinit.TIM_Period=arr;
    TIM_PWMinit.TIM_Prescaler =psc;
	  TIM_PWMinit.TIM_ClockDivision=TIM_CKD_DIV1;
	  TIM_PWMinit.TIM_CounterMode=TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM3,&TIM_PWMinit);
		
	  TIM_OCInit.TIM_OCMode=TIM_OCMode_PWM2;
	  TIM_OCInit.TIM_OCPolarity=TIM_OCPolarity_High;
	  TIM_OCInit.TIM_OutputState=TIM_OutputState_Enable;
	  TIM_OC1Init(TIM3,&TIM_OCInit);
		
		TIM_OCInit.TIM_OCMode=TIM_OCMode_PWM2;
	  TIM_OCInit.TIM_OCPolarity=TIM_OCPolarity_High;
	  TIM_OCInit.TIM_OutputState=TIM_OutputState_Enable;
	  TIM_OC2Init(TIM3,&TIM_OCInit);
		
		TIM_OCInit.TIM_OCMode=TIM_OCMode_PWM2;
	  TIM_OCInit.TIM_OCPolarity=TIM_OCPolarity_High;
	  TIM_OCInit.TIM_OutputState=TIM_OutputState_Enable;
	  TIM_OC3Init(TIM3,&TIM_OCInit);
		
		TIM_OCInit.TIM_OCMode=TIM_OCMode_PWM2;
	  TIM_OCInit.TIM_OCPolarity=TIM_OCPolarity_High;
	  TIM_OCInit.TIM_OutputState=TIM_OutputState_Enable;
	  TIM_OC4Init(TIM3,&TIM_OCInit);
	  
		TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);         // πƒ‹‘§◊∞‘ÿºƒ¥Ê∆˜
		TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
		TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	  TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);        
	  
	  TIM_Cmd(TIM3,ENABLE);
}

#endif

