/**
  ******************************************************************************
  * @file    FLASH/Program/main.c 
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
#include "st_flash.h"
#include "st_printf.h"
#include "project_config.h"
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup FLASH_Program
  * @{
  */ 



/* Private define ------------------------------------------------------------*/
/* Define the STM32F10x FLASH Page Size depending on the used STM32 device */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

#define FLASH_DEBUG 0




volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

//Flash_Data KL_Flash_Data={0};
uint32_t Flash_Read_LongWord(uint32_t FlashAdd)
{
	return *(__IO uint32_t*)FlashAdd;    
}


u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
uint16_t IAP_ReadFlag(void)
{
	return STMFLASH_ReadHalfWord(IAP_FLAG_ADDR);  
}



void IAP_WriteFlag(uint16_t flag)
{

	FLASH_Unlock();
	FLASH_ErasePage(IAP_FLAG_ADDR);
	FLASHStatus = FLASH_ProgramHalfWord(IAP_FLAG_ADDR, flag);
	if(FLASHStatus!=FLASH_COMPLETE)
	{
		printf("flash program error");
		while(1);
	}
	FLASH_Lock();
}



/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
