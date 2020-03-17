/**
  ******************************************************************************
  * @file    SPI/SPI_FLASH/main.c
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
#ifndef __ST_EXT_SPI_FLASH__
#define __ST_EXT_SPI_FLASH__

#include "stm32f10x.h"
#include "project_config.h"

#include "stm32_eval.h"
#include "stm32_eval_spi_flash.h"
#include "st_printf.h"


/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/




/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))





extern __IO uint32_t FlashID ;



void ext_flash_os_lock_init(void);

#if 1
void ext_flash_erase(uint32_t SectorAddr);

void ext_flash_read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

void ext_flash_write(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

#endif
int main_ext_spi_flash(void);


/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp_b8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);


#endif

