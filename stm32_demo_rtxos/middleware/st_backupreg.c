/**
  ******************************************************************************
  * @file    BKP/Backup_Data/main.c
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
#include "st_backupreg.h"
#include "st_printf.h"
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup BKP_Backup_Data
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if !defined(STM32F10X_HD)
//defined USE_STM3210B_EVAL || defined USE_STM32100B_EVAL
#define BKP_DR_NUMBER              10
#else
#define BKP_DR_NUMBER              42
#endif /* USE_STM3210B_EVAL or USE_STM32100B_EVAL */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//#if defined USE_STM3210B_EVAL || defined USE_STM32100B_EVAL
#if !defined(STM32F10X_HD)
uint16_t BKPDataReg[BKP_DR_NUMBER] =
{
    BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4, BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8,
    BKP_DR9, BKP_DR10
};
#else
uint16_t BKPDataReg[BKP_DR_NUMBER] =
{
    BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4, BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8,
    BKP_DR9, BKP_DR10, BKP_DR11, BKP_DR12, BKP_DR13, BKP_DR14, BKP_DR15, BKP_DR16,
    BKP_DR17, BKP_DR18, BKP_DR19, BKP_DR20, BKP_DR21, BKP_DR22, BKP_DR23, BKP_DR24,
    BKP_DR25, BKP_DR26, BKP_DR27, BKP_DR28, BKP_DR29, BKP_DR30, BKP_DR31, BKP_DR32,
    BKP_DR33, BKP_DR34, BKP_DR35, BKP_DR36, BKP_DR37, BKP_DR38, BKP_DR39, BKP_DR40,
    BKP_DR41, BKP_DR42
};
#endif /* USE_STM3210B_EVAL or USE_STM32100B_EVAL */

/* Private function prototypes -----------------------------------------------*/
void WriteToBackupReg(uint16_t FirstBackupData);
uint8_t CheckBackupReg(uint16_t FirstBackupData);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Writes data Backup DRx registers.
  * @param  FirstBackupData: data to be written to Backup data registers.
  * @retval None
  */
void WriteToBackupReg(uint16_t FirstBackupData)
{
    uint32_t index = 0;

    for (index = 0; index < BKP_DR_NUMBER; index++)
    {
        BKP_WriteBackupRegister(BKPDataReg[index], FirstBackupData + (index * 0x5A));
    }
}

/**
  * @brief  Checks if the Backup DRx registers values are correct or not.
  * @param  FirstBackupData: data to be compared with Backup data registers.
  * @retval
  *          - 0: All Backup DRx registers values are correct
  *          - Value different from 0: Number of the first Backup register
  *            which value is not correct
  */
uint8_t CheckBackupReg(uint16_t FirstBackupData)
{
    uint32_t index = 0;

    for (index = 0; index < BKP_DR_NUMBER; index++)
    {
        if (BKP_ReadBackupRegister(BKPDataReg[index]) != (FirstBackupData + (index * 0x5A)))
        {
            return (index + 1);
        }
    }

    return 0;
}

int st_BackupReg_init(void)
{


    /* Enable PWR and BKP clock */
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Enable write access to Backup domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Clear Tamper pin Event(TE) pending flag */
    BKP_ClearFlag();

    /* Check if the Power On Reset flag is set */
    if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
        /* Clear reset flags */
        RCC_ClearFlag();



        /* Check if Backup data registers are programmed */
        if(CheckBackupReg(0x3210) == 0x00)
        {   /* Backup data registers values are correct */

            /* Turn on LED1 */
            //STM_EVAL_LEDOn(LED1);
            uart_printf("Backup data registers values are correct ");
        }
        else
        {   /* Backup data registers values are not correct or they are not yet
               programmed (when the first time the program is executed) */
            uart_printf("Backup data registers values are not correct ");
            /* Write data to Backup data registers */
            WriteToBackupReg(0x3210);

            /* Turn on LED2 */
            // STM_EVAL_LEDOn(LED2);
        }
    }
    else
    {
        uart_printf("no Backup data dw ");
    }
    return 0;

}
