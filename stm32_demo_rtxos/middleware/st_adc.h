/**
  ******************************************************************************
  * @file    ADC/ADC1_DMA/main.c 
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

#ifndef _ST_ADC_H_
#define _ST_ADC_H_

#include "stm32f10x.h"

#include "project_config.h"


#include "st_printf.h"
#include "stm32f10x_it.h"


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_ADC1_DMA
  * @{
  */ 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    (ADC1_BASE + 0x4c) //((uint32_t)0x4001244C)
#define ADC2_DR_Address    (ADC2_BASE + 0x4c) //((uint32_t)0x4001244C)
#define ADC3_DR_Address    (ADC3_BASE + 0x4c) //((uint32_t)0x4001244C)


void RCC_Configuration_adc(void);

void st_adc_init(void);
void ADC_Reset(ADC_TypeDef* ADCx);
//------------------------------------------ADC运算读取数值函数-----------------------------------------------------//

int get_ntc_temp_adc1_chn9(void);

//adc1_value[0]=5 adc1_value[1]=1756 adc1_value[2]=1488
//adc1_value[2]=1488 VREF= (1488*3.3v)/4096 = 1.2V
//Temperature (in 癈) = {(V25 - VSENSE) / Avg_Slope} + 25.
//adc1_value[1]=1756 Vtmp= (1756*3.3v)/4096 = 1.414V
//V25 = 1.43 ,Temperature= (1.43-1.414)V/(4.3mv/C)+25=3.72+25=28.72


//adc1_value[0]=1831 adc1_value[1]=1727 adc1_value[2]=1488
#endif

