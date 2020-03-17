/**
  ******************************************************************************
  * @file    DAC/OneChannel_NoiseWave/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body.
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



void  st_dac_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  DAC_InitTypeDef            DAC_InitStructure;
  /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
     connected to the DAC converter. In order to avoid parasitic consumption, 
     the GPIO pin should be configured in analog */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  //GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
  /* Enable peripheral clocks ------------------------------------------------*/
  /* GPIOA Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	
 /* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_WaveGeneration_None;//DAC_Trigger_Software;
	//DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
 

  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits8_0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_1, ENABLE);

  /* Set DAC Channel1 DHR12L register */
  //DAC_SetChannel1Data(DAC_Align_12b_L, 0x7FF0);
  DAC_SetChannel1Data(DAC_Align_12b_R, 2048);
  //DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}

void  st_dac_trig(void)
{
	uint16_t DataValue;
	#if 1
	uint16_t dac1_ataValue;
	uint16_t dac2_ataValue;

    dac1_ataValue = DAC_GetDataOutputValue(DAC_Channel_1); 
	dac2_ataValue = DAC_GetDataOutputValue(DAC_Channel_2); 
	uart_printf("dac dataValue 1=%d 2=%d",dac1_ataValue,dac1_ataValue);

	
	#else
	uint16_t DataValue;
    /* Start DAC Channel1 conversion by software */
	DAC_SetChannel1Data(DAC_Align_12b_R,4095);//
    DataValue = DAC_GetDataOutputValue(DAC_Channel_1); 
	uart_printf("DataValue=%d",DataValue);
    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
	
	#endif
}

