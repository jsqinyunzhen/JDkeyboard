#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include <stdio.h>
#define Dummy_Byte 0

/* Select SPI FLASH: ChipSelect pin low  */
#define Select_Flash()     GPIO_ResetBits(GPIOC, GPIO_Pin_4)
/* Deselect SPI FLASH: ChipSelect pin high */
#define NotSelect_Flash()    GPIO_SetBits(GPIOC, GPIO_Pin_4)



void SPI_Flash_Init(void);	        //SPI初始化
uint8_t SPI_Flash_ReadByte(void);		//flash操作基本函数，读一个字节
uint8_t SPI_Flash_SendByte(uint8_t byte);		//	FLASH操作基本函数，发送一个字节


void FlashWaitBusy(void);			//Flash忙检测
void FlashReadID(void);		        //读取flashID四个字节


void st_spi_flash_test(void);

#endif
