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
#include "stm32f10x.h"

#include "project_config.h"


#include "st_printf.h"
#include "stm32f10x_it.h"
#include "st_adc.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_ADC1_DMA
  * @{
  */ 




/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//static ADC_InitTypeDef ADC_InitStructure;
//static DMA_InitTypeDef DMA_InitStructure;
//__IO uint16_t ADCConvertedValue;
__IO uint16_t ADC1ConvertedValue[3];
//__IO uint16_t ADC2ConvertedValue[3];
//__IO uint16_t ADC3ConvertedValue[3];
/* Private function prototypes -----------------------------------------------*/


void RCC_Configuration_adc(void)
{
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  /* ADCCLK = PCLK2/2 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
#else
  /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
#endif
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 and GPIOC clock */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
}



void st_adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration_adc();

  /* GPIO configuration ------------------------------------------------------*/
  //GPIO_Configuration_adc_pc4();


  /* Configure PC.04 (ADC Channel14) as analog input -------------------------*/
	 /* Configure PA.00 (ADC Channel 0) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
 /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC1ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 3;//1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
 // ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_InitStructure.ADC_NbrOfChannel = 3;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5);
  //add bu xushxiong
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_TempSensor,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_Vrefint,3,ADC_SampleTime_55Cycles5);
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
/*reset  adc start*/
  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
 /*reset  adc end*/    
  /* Start ADC1 Software Conversion */ 
//  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


void ADC_Reset(ADC_TypeDef* ADCx)
{
		
	ADC_ResetCalibration(ADCx);                                                           
	//复位校准寄存器             寄存器置1

	while(ADC_GetResetCalibrationStatus(ADCx));                                           
	//等待校准寄存器复位完成      寄存器置0

	ADC_StartCalibration(ADCx);                                                           
	//ADC校准                    寄存器置1

	while(ADC_GetCalibrationStatus(ADCx));                                                
	//等待校准完成                寄存器置0
	
}
 
 
//------------------------------------------ADC运算读取数值函数-----------------------------------------------------//
 
#if 1
#define NTC_NUM_TOTAL 101
const int bat_ntc[NTC_NUM_TOTAL][2] = {	
						-30, 17737,
						-29, 16666,
						-28, 15667,
						-27, 14734,
						-26, 13863,

						-25, 13049,
						-24, 12288,
						-23, 11576,
						-22, 10910,
						-21, 10286,
						-20, 97026,
						-19, 91554,
						-18, 86425,
						-17, 81616,
						-16, 77104,
						-15, 72869,
						-14, 68893,
						-13, 65159,
						-12, 61650,
						-11, 58351,
						-10, 55249,
						-9, 52330,
						-8, 49584,
						-7, 46998,
						-6, 44562,
						-5, 42268,
						-4, 40105,
						-3, 38065,
						-2, 36142,
						-1, 34327,
						0, 32614,
						
						1, 30996,
						2, 29468,
						3, 28025,
						4, 26660,
						5, 25370,
						6, 24150,
						7, 22995,
						8, 21902,
						9, 20868,
						10, 19888,
						11, 18960,
						12, 18080,
						13, 17246,
						14, 16456,
						15, 15706,
						16, 14994,
						17, 14319,
						18, 13677,
						19, 13069,
						20, 12490,
						21, 11940,
						22, 11418,
						23, 10921,
						24, 10449,
						25, 10000,
						26, 9572,
						27, 9165,
						28, 8777,
						29, 8408,
						30, 8057,
						31, 7722,
						32, 7403,
						33, 7099,
						34, 6808,
						35, 6532,
						36, 6268,
						37, 6016,
						38, 5775,
						39, 5545,
						40, 5326,
						41, 5117,
						42, 4916,
						43, 4725,
						44, 4542,
						45, 4368,
						46, 4200,
						47, 4040,
						48, 3887,
						49, 3741,
						50, 3601,
						51, 3467,
						52, 3338,
						53, 3215,
						54, 3097,
						55, 2984,
						56, 2876,
						57, 2772,
						58, 2672,
						59, 2577,
						60, 2485,
						61, 2397,
						62, 2313,
						63, 2232,
						64, 2154,
						65, 2080,
						66, 2008,
						67, 1939,
						68, 1873,
						69, 1809,
						70, 1748};

#endif
static int ntc_to_temperature(int ntc)
{
	int i;

	if(ntc >= bat_ntc[0][1])
	{
		return bat_ntc[0][0];
	}
	else if(ntc < bat_ntc[NTC_NUM_TOTAL-1][1])
	{
		return bat_ntc[NTC_NUM_TOTAL-1][0];
	}

	for(i=1; i<NTC_NUM_TOTAL; i++)
	{
		if(ntc < bat_ntc[i-1][1] && ntc >= bat_ntc[i][1])
			break;
	}

	return bat_ntc[i][0];
} 
int get_ntc_temp_adc1_chn9(void)
{
	float kk=0;                                                                                                                                                                                                                                                                                                                                                               
	uint16_t adc_value = 0;
	uint32_t start_time ;
	uint32_t end_time ;
//	FlagStatus flag_status = RESET;
	ADC_TypeDef* ADCx =ADC1;
	
	start_time =get_curtime();
	//uart_printf("Read_ADC_data start =%d\r\n",get_curtime());
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);                                                
	//开关_ADC软件触发-开关    状态寄存器为0                                                                  
	//flag_status =  ADC_GetFlagStatus(ADCx,ADC_FLAG_STRT);	
	//uart_printf("flag_status =%d\r\n",flag_status);
	while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC))
	{
		
	};                                         
	//等待转换结束             寄存器置1                                                                            
	adc_value = ADC_GetConversionValue(ADCx);
	end_time =get_curtime();
	uart_printf("Read_ADC_data s=%d end =%d\r\n",start_time,end_time);
	kk=(3.3*(((float)adc_value/4096)));    
	//kk=(3.3*(((float)ADC_GetConversionValue(ADCx)/4096)));                                
	//百分比值转化成电压值，    因为读取了数据寄存器，状态寄存器自动清0     
	uart_printf("adc1_value[0]=%d adc1_value[1]=%d adc1_value[2]=%d\r\n",
	ADC1ConvertedValue[0],ADC1ConvertedValue[1],ADC1ConvertedValue[2]);
	//uart_printf("Read_ADC_data adc_value=%d kk=%f\r\n",adc_value,kk);

	
	float Rrtc = (float)(10*1000*ADC1ConvertedValue[0]) /( 4096-ADC1ConvertedValue[0] );
	uart_printf("adc1 chn1: ntc=%d Rrtc=%f oumu \r\n",ADC1ConvertedValue[0],Rrtc);
	int Rrtc_int = (int)Rrtc;
	int ntc_temp  =0;
	
	ntc_temp = ntc_to_temperature(Rrtc_int);
	uart_printf("ntc_to_temperature  Rrtc_int=%d oumu ,ntc_temp=%d C\r\n",Rrtc_int,ntc_temp);
	//	4096  adc
	//	3.3   xx
//adc1_value[1]=1756 Vtmp= (1756*3.3v)/4096 = 1.414V
//V25 = 1.43 ,Temperature= (1.43-1.414)V/(4.3mv/C)+25=3.72+25=28.72
	float Vtmp= ((float)ADC1ConvertedValue[1]*3.3)/4096 ;
	float Temperature= (1.43-Vtmp)*1000/4.3+25;
	uart_printf("adc1 chn2: TempSensor ,adc=%d Vtmp=%f , Temperature=%f\r\n",ADC1ConvertedValue[1],Vtmp,Temperature);
	kk=(3.3*(((float)ADC1ConvertedValue[2]/4096)));  
	uart_printf("adc1 chn3: Vrefint 1.2v, kk=%f\r\n",adc_value,kk);

	
	return ntc_temp;                                                                            
	 
	
}
//adc1_value[0]=5 adc1_value[1]=1756 adc1_value[2]=1488
//adc1_value[2]=1488 VREF= (1488*3.3v)/4096 = 1.2V
//Temperature (in 癈) = {(V25 - VSENSE) / Avg_Slope} + 25.
//adc1_value[1]=1756 Vtmp= (1756*3.3v)/4096 = 1.414V
//V25 = 1.43 ,Temperature= (1.43-1.414)V/(4.3mv/C)+25=3.72+25=28.72


//adc1_value[0]=1831 adc1_value[1]=1727 adc1_value[2]=1488
