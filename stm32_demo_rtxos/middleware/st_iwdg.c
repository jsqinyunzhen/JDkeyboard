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
#include "stm32f10x_it.h"
#include "st_printf.h"
#include "st_iwdg.h"

/*
独立看门狗用通俗一点的话来解释就是一个 12 位的递减计数器，当计数器的值从某个值一直减到 0 的时候，
系统就会产生一个复位信号，即 IWDG_RESET。如果在计数没减到 0 之前，刷新了计数器的值的话，那么就不
会产生复位信号，这个动作就是我们经常说的喂狗。看门狗功能由 VDD 电压域供电，在停止模式和待机模式下仍能工作。　
*/
/*

 * 设置 IWDG 的超时时间

 * Tout = prv/40 * rlv (s)

 *      prv可以是[4,8,16,32,64,128,256]

 * prv:预分频器值，取值如下：

 *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4

 *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8

 *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16

 *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32

 *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64

 *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128

 *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256

 *

 *        独立看门狗使用LSI作为时钟。

 *        LSI 的频率一般在 30~60KHZ 之间，根据温度和工作场合会有一定的漂移，我

 *        们一般取 40KHZ，所以独立看门狗的定时时间并一定非常精确，只适用于对时间精度

 *        要求比较低的场合。

 *

 * rlv:重装载寄存器的值，取值范围为：0-0XFFF

 * 函数调用举例：

 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s 超时溢出 

 *                        （64/40）*625 = 1s

 */



void IWDG_Config(uint8_t prv ,uint16_t rlv)

{    

    // 使能 预分频寄存器PR和重装载寄存器RLR可写

    IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );

    

    // 设置预分频器值

    IWDG_SetPrescaler( prv );

    

    // 设置重装载寄存器值

    IWDG_SetReload( rlv );

    

    // 把重装载寄存器的值放到计数器中

    IWDG_ReloadCounter();

    

    // 使能 IWDG

    IWDG_Enable();    

}




// 喂狗

void IWDG_Feed(void)
{
	//uart_printf("IWDG_Feed get_curtime=%d",get_curtime());
    // 把重装载寄存器的值放到计数器中，喂狗，防止IWDG复位

    // 当计数器的值减到0的时候会产生系统复位

    IWDG_ReloadCounter();

}

 // IWDG 1s 超时溢出
void IWDG_Config_Timeout(uint8_t time)
{
	//IWDG_Config(IWDG_Prescaler_64 ,625); 
	if((625*time) >4096)
		time = 6;
	uart_printf("IWDG_Config_Timeout =%d s",time);
	IWDG_Config(IWDG_Prescaler_64 ,625*time); 
}
