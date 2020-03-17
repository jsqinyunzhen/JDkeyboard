/**
  ******************************************************************************
  * @file    GPIO/JTAG_Remap/main.c
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
#include "st_gpio.h"
#if (USE_SOFTTIMER_DIY ==1)
#include "mod_date_time.h"
#include "mod_time_list.h"
#endif

void gpio_led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure= {0};
    uint8_t gpio_value = 0;

#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1 )

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;				     //LED1  V6	   //将V6,V7,V8 配置为通用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    gpio_value = GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_12);
    if(gpio_value == Bit_SET)
    {
        uart_printf("GPIO_Pin_12 is h");
    }
    else
    {
        uart_printf("GPIO_Pin_12 is l");
    }

    gpio_value = GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_14);
    if(gpio_value == Bit_SET)
    {
        uart_printf("GPIO_Pin_14 is h");
    }
    else
    {
        uart_printf("GPIO_Pin_14 is l");
    }
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2 )
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					 //LED1  V6    //将V6,V7,V8 配置为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	gpio_value = GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_12);
	if(gpio_value == Bit_SET)
	{
		uart_printf("GPIO_Pin_12 is h");
	}
	else
	{
		uart_printf("GPIO_Pin_12 is l");
	}

#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				   //LED1  V6	 //将V6,V7,V8 配置为通用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //口线翻转速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;	   //LED2, LED3    V7 V8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


    gpio_value = GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5);
    if(gpio_value == Bit_SET)
    {
        uart_printf("GPIO_Pin_5 is h");
    }
    else
    {
        uart_printf("GPIO_Pin_5 is l");
    }

    gpio_value = GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_6);
    if(gpio_value == Bit_SET)
    {
        uart_printf("GPIO_Pin_6 is h");
    }
    else
    {
        uart_printf("GPIO_Pin_6 is l");
    }

    gpio_value = GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_3);
    if(gpio_value == Bit_SET)
    {
        uart_printf("GPIO_Pin_3 is h");
    }
    else
    {
        uart_printf("GPIO_Pin_3 is l");
    }

#endif

}
void gpio_led_on(void)
{
#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
    GPIO_WriteBit(GPIOD, GPIO_Pin_6, Bit_SET);
    //GPIO_WriteBit(GPIOD, GPIO_Pin_3, 1);
    GPIO_SetBits(GPIOD, GPIO_Pin_3);
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1 )
    GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_RESET);//yellow
    GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_RESET);
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2 )
	//yellow
	GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_RESET);
#endif
}

void gpio_led_off(void)
{
#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
    GPIO_WriteBit(GPIOD, GPIO_Pin_6, Bit_RESET);
//	GPIO_WriteBit(GPIOD, GPIO_Pin_3, 0);
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1 )
    GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_SET);
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2 )
	GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
#endif
}
void gpio_led_tog(void)
{
#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5)));
    GPIO_WriteBit(GPIOD, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_6)));
    GPIO_WriteBit(GPIOD, GPIO_Pin_3, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_3)));
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1 )
	GPIO_WriteBit(GPIOE, GPIO_Pin_12, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_12)));
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14)));
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2 )
	
	GPIO_WriteBit(GPIOE, GPIO_Pin_12, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_12)));

#endif
}

void EXTI5_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

    /* Enable GPIOA clock */
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure PC.05 pin as input floating */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    /* Connect EXTI0 Line to PC.05 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);

    /* Configure EXTI0 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;//EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI0 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


}
void EXTI2_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    /* Configure PG.08 pin as input floating */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Enable AFIO clock */
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /* Connect EXTI8 Line to PG.08 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);//|GPIO_PinSource3);

    /* Configure EXTI8 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI9_5 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}
void EXTI3_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    /* Configure PG.08 pin as input floating */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Enable AFIO clock */
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /* Connect EXTI8 Line to PG.08 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);//|GPIO_PinSource3);

    /* Configure EXTI8 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI9_5 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}
void gpio_key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t gpio_value = 0;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)
    /* K1 ???????PC5 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //????
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* K2 ???????PC2 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //????
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* K3 ???????PC3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //????
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* K4 ???????PE6 */
    //reset key
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //????
    GPIO_Init(GPIOE, &GPIO_InitStructure);
#elif (HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				     //LED1  V6	   //将V6,V7,V8 配置为通用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
    GPIO_Init(GPIOE, &GPIO_InitStructure);

#elif (HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				     //LED1  V6	   //将V6,V7,V8 配置为通用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    gpio_value = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10);
    if(gpio_value == Bit_SET)
    {
        uart_printf("GPIO_Pin_10 is h");
    }
    else
    {
        uart_printf("GPIO_Pin_10 is l");
    }

#endif


}

/****************************************************************************
* ?    ?:void NVIC_Configuration(void)
* ?    ?:?????
* ????:?
* ????:?
* ?    ?:
* ????:?
****************************************************************************/
#if 0
void gpio_key_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    /* Configure one bit for preemption priority */
    /* ???? ?????????????,???????????   ????1, 3
     0?:  ??????0?, ??????4?
     1?:  ??????1?, ??????3?
     2?:  ??????2?, ??????2?
     3?:  ??????3?, ??????1?
     4?:  ??????4?, ??????0?  */

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the EXTI9-5 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;				 //????9-5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //????? 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			 //????1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //??
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;				 //????2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //????? 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			 //????2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //??
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				 //????3
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //????? 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //????0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //??
    NVIC_Init(&NVIC_InitStructure);

    //????AFIO?????????AFIO_EXTICR1,????EXTI5?????????PC5?
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);     //??????AFIO--ETXI9-5
    //????AFIO?????????AFIO_EXTICR1,????EXTI2?????????PC2?
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);     //??????AFIO--ETXI2
    //????AFIO?????????AFIO_EXTICR1,????EXTI3?????????PC3?
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);     //??????AFIO--ETXI3

    EXTI_InitStructure.EXTI_Line = EXTI_Line5;						//PC5 ????K1 ????
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //????
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //?????
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line2;						//PC2 ????K2 ????
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //????
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //?????
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line3;						//PC3 ????K3 ????
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //????
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //?????
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
#endif
void gpio_key_exti_config(void)
{
//	GPIO_InitTypeDef  GPIO_InitStruct;                          //初始化GPIO结构体
    EXTI_InitTypeDef  EXTI_InitStruct;                         //初始化EXTI结构体
    /*
    	// 初始化GPIO
    	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    	//选脚GPIO_Pin_0
    	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    	//浮空输入
    	GPIO_Init(GPIOC, &GPIO_InitStruct);
    	//初始化GPIOA端口
    	*/
#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
    //选择PA端口0脚 PA0
    EXTI_InitStruct.EXTI_Line = EXTI_Line2;
    //选择Line0
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    //上升沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    //使能中断屏蔽事件寄存器
    EXTI_Init(&EXTI_InitStruct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
    //选择PA端口0脚 PA0
    EXTI_InitStruct.EXTI_Line = EXTI_Line3;
    //选择Line0
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    //上升沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    //使能中断屏蔽事件寄存器
    EXTI_Init(&EXTI_InitStruct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
    //选择PA端口0脚 PA0
    EXTI_InitStruct.EXTI_Line = EXTI_Line5;
    //选择Line0
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    //上升沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    //使能中断屏蔽事件寄存器
    EXTI_Init(&EXTI_InitStruct);
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1)
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource10);
    //选择PA端口0脚 PA0
    EXTI_InitStruct.EXTI_Line = EXTI_Line10;
    //选择Line0
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    //上升沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    //使能中断屏蔽事件寄存器
    EXTI_Init(&EXTI_InitStruct);
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2)
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource10);
		//选择PA端口0脚 PA0
		EXTI_InitStruct.EXTI_Line = EXTI_Line10;
		//选择Line0
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		//中断模式
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
		//上升沿触发
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		//使能中断屏蔽事件寄存器
		EXTI_Init(&EXTI_InitStruct);

#endif
}

void gpio_key_value(void)
{
    uint8_t gpio_value = 0;
#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)
    gpio_value = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);
    if(gpio_value == Bit_SET)
    {
        uart_printf("GPIO_Pin_5 is h");
    }
    else
    {
        uart_printf("GPIO_Pin_5 is l");
    }

    gpio_value = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2);
    if(gpio_value == Bit_SET)
    {
        uart_printf("GPIO_Pin_2 is h");
    }
    else
    {
        uart_printf("GPIO_Pin_2 is l");
    }

    gpio_value = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3);
    if(gpio_value == Bit_SET)
    {
        uart_printf("GPIO_Pin_3 is h");
    }
    else
    {
        uart_printf("GPIO_Pin_3 is l");
    }
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1)
    gpio_value = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10);
    if(gpio_value == Bit_SET)
    {
        //uart_printf("GPIO_Pin_10 is h");
    }
    else
    {
        //uart_printf("GPIO_Pin_10 is l");
    }
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2)
		gpio_value = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10);
		if(gpio_value == Bit_SET)
		{
			//uart_printf("GPIO_Pin_10 is h");
		}
		else
		{
			//uart_printf("GPIO_Pin_10 is l");
		}

#endif
}
//#define    KEY_PRESS(GPIOx,GPIO_Pin)      GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)
#define KEY_INPUT     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)    
//KEY_PRESS(GPIOE,GPIO_PinSource10)    //读取按键状态
#define KEY_STATE_0         0       // 按键状态位
#define KEY_STATE_1         1
#define KEY_STATE_2         2
#define KEY_STATE_3         3 
#define LONG_KEY_TIME       300     //长按的3秒时间
#define SINGLE_KEY_TIME     3       // 短按的消抖时间
#define N_KEY    0                  // 无状态
#define S_KEY    1                  // 单击
#define L_KEY    10                 // 长按

#define KEY_ON	Bit_RESET
#define KEY_OFF	Bit_SET



void timer_key_detect( void)
{         
	static unsigned char key_state = KEY_STATE_0;         // 按键状态变量   
	static unsigned int key_time = 0;           // 按键计时变量    
	unsigned char key_press, key_return ;      
	//GPIO_TypeDef* GPIOx = GPIOE;
	//uint16_t GPIO_Pin= GPIO_PinSource10;
	
	key_return = N_KEY;                         // 清除 返回按键值     
	key_press = KEY_INPUT;                      // 读取当前键值     
	switch (key_state)         
	{               
		case KEY_STATE_0:                       // 按键状态0：判断有无按键按下          
			if (key_press == KEY_ON)                     // 有按键按下           
			{                
				key_time = 0;                   // 清零时间间隔计数                
				key_state = KEY_STATE_1;        // 然后进入 按键状态1          
			}                    
			break;         
		case KEY_STATE_1:                       // 按键状态1：软件消抖（确定按键是否有效，而不是误触）。按键有效的定义：按键持续按下超过设定的消抖时间。          
			if (key_press == KEY_ON)                                 
			{   
				key_time++;                     // 一次10ms              
				if(key_time>=SINGLE_KEY_TIME)   // 消抖时间为：SINGLE_KEY_TIME*10ms = 30ms;               
				{                    
					key_state = KEY_STATE_2;    // 如果按键时间超过 消抖时间，即判定为按下的按键有效。按键有效包括两种：单击或者长按，进入 按键状态2， 继续判定到底是那种有效按键             
				}           
			}                     
			else 
				key_state = KEY_STATE_0;       // 如果按键时间没有超过，判定为误触，按键无效，返回 按键状态0，继续等待按键           
			break;          
		case KEY_STATE_2:                       // 按键状态2：判定按键有效的种类：是单击，还是长按            
			if(key_press == KEY_OFF)                       // 如果按键在 设定的长按时间 内释放，则判定为单击          
			{                  
				key_return = S_KEY;            // 返回 有效按键值：单击                
				key_state = KEY_STATE_0;       // 返回 按键状态0，继续等待按键           
			}            
			else            
			{                
				key_time++;                                      
				if(key_time >= LONG_KEY_TIME)   // 如果按键时间超过 设定的长按时间（LONG_KEY_TIME*10ms=200*10ms=2000ms）, 则判定为 长按                
				{                    
					key_return = L_KEY;         // 返回 有效键值值：长按                 
					key_state = KEY_STATE_3;    // 去状态3，等待按键释放              
				}           
			}            
			break;       
		case KEY_STATE_3:                         // 等待按键释放         
			if (key_press == KEY_OFF)          
			{              
				key_state = KEY_STATE_0;          // 按键释放后，进入 按键状态0 ，进行下一次按键的判定        
			}                   
			break;          
		default:                                // 特殊情况：key_state是其他值得情况，清零key_state。这种情况一般出现在 没有初始化key_state，第一次执行这个函数的时候            
			key_state = KEY_STATE_0;            
			break;    
	}     
	//return key_return;                          // 返回 按键值
	if(key_return == S_KEY)
	{
		if(evt_id_app)
			osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_KEY_ENTER_SHORT);
		time_stop(timer_detect);
	}
	else if(key_return == L_KEY)
	{
		if(evt_id_app)
			osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_KEY_ENTER_LONG);
		time_stop(timer_detect);
	}

} 
void led_poweron(void)
{

	led_mode =1;
	times=0;


}

void rs485_set_start(void)
{
	time_stop( timer_y_ctrl);
	led_mode =2;
	times=0;
	time_restart( timer_y_ctrl ,100);

}

void rs485_ok_led_fast(void)
{
	time_stop( timer_y_ctrl);
	led_mode =3;
	times=0;
	time_restart( timer_y_ctrl ,20);

}
void rs485_fail_led_fast(void)
{
	time_stop( timer_y_ctrl );
	led_mode =4;
	times=0;
	//__enable_irq();
	time_restart( timer_y_ctrl ,40);

}
void fw_upgeade_start(void)
{
	time_stop( timer_y_ctrl );
	led_mode =5;
	times=0;
	//__enable_irq();
	time_restart( timer_y_ctrl ,40);

}

void mcu_led_y_fast_ctrl( void )
{
	//mcu_led_toggle(LED_R_PIN);
//	static u8 times =0;
//power on

	if(led_mode ==1)
	{
		times++;
		if(times >=10)
		{
			times =0;
			gpio_led_off();
			time_stop( timer_y_ctrl );
		
			//timer_y_fast = TIME_UNIT_MAX;
		}
	}
	else if(led_mode ==2)
	{
//start sync,slew
		//mcu_led_onoff(LED_R_PIN,GPIO_LOW);
		gpio_led_tog();
		times++;
		if(times >=120)
		{
			times =0;
			gpio_led_off();
			printf("sync,mcu fail");
			time_stop( timer_y_ctrl );
		}
	}
	else if(led_mode ==3)
	{
// sync success,fast
		gpio_led_tog();
	
		times++;
		if(times >=25)
		{
			times =0;
			gpio_led_off();
			time_stop( timer_y_ctrl );
		}
		uart_printf("y led=%d",times);
	}
	else if(led_mode ==4)
	{
// sync fail,fast
		//mcu_led_onoff(LED_R_PIN,GPIO_LOW);
		//..\User\main.c(238): warning:  #186-D: pointless comparison of unsigned integer with zero

		uart_printf("r led=%d",times);
		gpio_led_tog();
		times++;
		if(times >=25)
		{
			gpio_led_off();
			time_stop( timer_y_ctrl );
			times =0;
			//led_mode = 0xff;
		}
	}
	else if(led_mode ==5)
	{
//fw upgrade,slew
		times =0;
		gpio_led_tog();
	}

}


void gpio_smoke_detect_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
//    uint8_t gpio_value = 0;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //????
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void gpio_smoke_detect_exti_config(void)
{
    EXTI_InitTypeDef  EXTI_InitStruct;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource9);
    //选择PA端口0脚 PA0
    EXTI_InitStruct.EXTI_Line = EXTI_Line9;
    //选择Line0
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    //上升沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    //使能中断屏蔽事件寄存器
    EXTI_Init(&EXTI_InitStruct);

}
int gpio_smoke_detect_value(void)
{
    uint8_t gpio_value = 0;

    gpio_value = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9);
    if(gpio_value == Bit_SET)
    {
        printf("GPIO_Pin_9 is h\r\n");
		return 1;
		//gpio_led_on();
    }
    else
    {
        printf("GPIO_Pin_9 is l\r\n");
		return 0;
		//gpio_led_off();
    }

}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
