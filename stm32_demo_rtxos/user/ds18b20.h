#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f10x.h"
//#include "bsp_SysTick.h" //精确延时函数头文件----参考http://blog.csdn.net/xuxuechen/article/details/40783209这个看一下
#include "stm32f10x_it.h"

#define HIGH 1
#define LOW 0
#if 0
#define DS18B20_CLK    RCC_APB2Periph_GPIOA
#define DS18B20_PIN    GPIO_Pin_8
#define DS18B20_PORT   GPIOA
//总体代表DS18B20的GPIO口为PB9

#else
#define DS18B20_CLK    RCC_APB2Periph_GPIOB
#define DS18B20_PIN    GPIO_Pin_9
#define DS18B20_PORT   GPIOB
//总体代表DS18B20的GPIO口为PB9
#endif

//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define DS18B20_DATA_OUT(a)if (a) \
		GPIO_SetBits(DS18B20_PORT,DS18B20_PIN);\
		else \
		GPIO_ResetBits(DS18B20_PORT,DS18B20_PIN)
//读取引脚的电平
#define DS18B20_DATA_IN() GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN)

uint8_t DS18B20_Init(void);
float DS18B20_Get_Temp(uint8_t *a,uint8_t b);
//float DS18B20_Get_Temp(uint8_t *a,uint16_t go_temp,uint8_t b)
extern uint8_t serial_4[8];
void read_serial(uint8_t *serial);
#endif /* __DS18B20_H */

