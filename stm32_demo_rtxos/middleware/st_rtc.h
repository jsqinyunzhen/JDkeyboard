#ifndef _ST_RTC_H_
#define _ST_RTC_H_

#include "stm32f10x.h"
//#include "stm32_eval.h"
#include <stdio.h>

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_Calendar
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t TimeDisplay ;


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int st_rtc_init(void);
void st_rtc_show(void);
uint32_t time_hms2s(uint32_t Tmp_HH, uint32_t Tmp_MM , uint32_t Tmp_SS );


/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void rtc_NVIC_Configuration(void);


/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void);


/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval Current time RTC counter value
  */
uint32_t Time_Regulate(void);

/**
  * @brief  Adjusts time.
  * @param  None
  * @retval None
  */
void Time_Adjust(void);

/**
  * @brief  Displays the current time.
  * @param  TimeVar: RTC counter value.
  * @retval None
  */
void Time_Display(uint32_t TimeVar);

/**
  * @brief  Shows the current time (HH:MM:SS) on the Hyperterminal.
  * @param  None
  * @retval None
  */   
void Time_Show(void);

void rtc_set_cnt(uint32_t cnt);




/**
  * @brief  Gets numeric values from the hyperterminal.
  * @param  None
  * @retval None
  */
uint8_t USART_Scanf(uint32_t value);

#endif
