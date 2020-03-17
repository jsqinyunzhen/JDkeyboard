/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: spi_flash.c
* 内容简述:
*       本例程包含了SST25VF016B的底层驱动函数
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-07 sun68  创建该文件
*
*/

#include "stm32f10x.h"
#include "SPI_Flash.h"
#include "stm32f10x_spi.h"	
#include "stm32f10x_it.h"
#include "st_printf.h"
unsigned char SST25_buffer[4096];
void SPI_Flash_Init(void);
u8 SPI_Flash_ReadByte(void);
u8 SPI_Flash_SendByte(u8 byte);

void wip(void);
void wen(void);
void wdis(void);
void wsr(void);
unsigned char rdsr(void);
void FlashReadID(void);
void sect_clr(unsigned long a1);  
void SST25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize);
void SST25_W_BLOCK(uint32_t addr, u8 *readbuff, uint16_t BlockSize);
void SPI_Flash_Init(void);

/****************************************************************************
* 名    称：void wen(void)
* 功    能：写使能
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void wen(void){
	Select_Flash();
	SPI_Flash_SendByte(0x06);
	NotSelect_Flash();
}

/****************************************************************************
* 名    称：void wdis(void)
* 功    能：写禁止
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void wdis(void){

	Select_Flash();
	SPI_Flash_SendByte(0x04); 
	NotSelect_Flash();
	wip();
	
}	
/****************************************************************************
* 名    称：void wsr(void)
* 功    能：写状态
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void wsr(void){	
	Select_Flash();
	SPI_Flash_SendByte(0x50);
	NotSelect_Flash(); 
	Select_Flash();
	SPI_Flash_SendByte(0x01);
	SPI_Flash_SendByte(0x00); 
	NotSelect_Flash();
    wip();
}

/****************************************************************************
* 名    称：void wip(void)
* 功    能：忙检测
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void wip(void){
	unsigned char a=1;
	while((a&0x01)==1) a=rdsr();	

}
/****************************************************************************
* 名    称：unsigned char rdsr(void)
* 功    能：读状态寄存器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
unsigned char rdsr(void){
	unsigned char busy;
	Select_Flash();
	SPI_Flash_SendByte(0x05);
	busy = SPI_Flash_ReadByte();
	NotSelect_Flash();
	return(busy);
	
}

/****************************************************************************
* 名    称：void SST25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize)
* 功    能：页读
* 入口参数：unsigned long addr--页   unsigned char *readbuff--数组   unsigned int BlockSize--长度
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void SST25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize){
	unsigned int i=0; 	
	Select_Flash();
	SPI_Flash_SendByte(0x0b);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(0);
	while(i<BlockSize){	
		readbuff[i]=SPI_Flash_ReadByte();
		
		i++;
	}
	NotSelect_Flash();	 	
}
#if 0
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  sFLASH_SendByte(sFLASH_CMD_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  sFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  sFLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}
#endif
/****************************************************************************
* 名    称：void SST25_W_BLOCK(uint32_t addr, u8 *readbuff, uint16_t BlockSize)
* 功    能：页写
* 入口参数：uint32_t addr--页   u8 *readbuff--数组   uint16_t BlockSize--长度    
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void SST25_W_BLOCK(uint32_t addr, u8 *readbuff, uint16_t BlockSize){
	unsigned int i=0,a2;
	sect_clr(addr);   								  //删除页		  
	wsr();
  	wen();	
	Select_Flash();    
	SPI_Flash_SendByte(0xad);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
  	SPI_Flash_SendByte(readbuff[0]);
	SPI_Flash_SendByte(readbuff[1]);
	NotSelect_Flash();
	i=2;
	while(i<BlockSize){
		a2=120;
		while(a2>0) a2--;
		Select_Flash();
		SPI_Flash_SendByte(0xad);
		SPI_Flash_SendByte(readbuff[i++]);
		SPI_Flash_SendByte(readbuff[i++]);
		NotSelect_Flash();
	}
	
	a2=100;
	while(a2>0) a2--;
	wdis();	
	Select_Flash();	
	wip();
}
/****************************************************************************
* 名    称：void sect_clr(unsigned long a1)
* 功    能：页擦除
* 入口参数：unsigned long a1--页   
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void sect_clr(unsigned long a1){
	wsr();
	wen();     
	Select_Flash();	  
	SPI_Flash_SendByte(0x20);
	SPI_Flash_SendByte((a1&0xffffff)>>16);          //addh
	SPI_Flash_SendByte((a1&0xffff)>>8);          //addl 
	SPI_Flash_SendByte(a1&0xff);                 //wtt
	NotSelect_Flash();
	wip();
	
}
/****************************************************************************
* 名    称：void FlashReadID(void)
* 功    能：读工厂码及型号的函数
* 入口参数：  
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void FlashReadID(void)
{
	  u8 fac_id= 0;//BFH: 工程码SST
	u8 dev_id= 0;//41H: 器件型号SST25VF016B     
	Select_Flash();	
  	SPI_Flash_SendByte(0x90);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
  	fac_id= SPI_Flash_ReadByte();//BFH: 工程码SST
	dev_id= SPI_Flash_ReadByte();//41H: 器件型号SST25VF016B     
  	NotSelect_Flash();	
	printf("fac_id=0x%x,dev_id=0x%x",fac_id,dev_id);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
   
  /* 使能SPI1 时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);	
  /* 配置 SPI1 引脚: SCK, MISO 和 MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //V3---ENC28J60的SPI1 片选 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //触摸电路的SPI1 片选
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* 配置PC4为SST25VF016B的片选  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);   
 
  /* SPI1配置 */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* 使能SPI1  */
  SPI_Cmd(SPI1, ENABLE);   
  NotSelect_Flash();

  GPIO_SetBits(GPIOB, GPIO_Pin_7);			//禁止触摸电路的片选
  GPIO_SetBits(GPIOA, GPIO_Pin_4);			//禁止ENC28J60的片选
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
u8 SPI_Flash_ReadByte(void)
{
  return (SPI_Flash_SendByte(Dummy_Byte));
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_Flash_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  //NotSelect_Flash();  while(1);
  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/


//extern void SPI_Flash_Init(void);
//extern unsigned char SST25_buffer[];



/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/

static uint8_t TxBuffer1[] = "SPI SST25VF016B Example: This is SPI DEMO, ?????????,??SST25VF016B?????";


void st_spi_flash_test(void)
{
	uint16_t i=0;
	 uint8_t a=0;
	
	FlashReadID();
   /* ???????????????? */
   for(i=0; i<TxBufferSize1;i++) 
	SST25_buffer[i]=TxBuffer1[i];  
   SST25_W_BLOCK(0, SST25_buffer,4096);	        //????????SST25VF016B?0??
   Delay1ms(1);
   SST25_R_BLOCK(0, SST25_buffer,4096);	        //?SST25VF016B?0??????
   a=0;
   for(i=0; i<TxBufferSize1;i++){
  	 if(SST25_buffer[i]==TxBuffer1[i])
		 a=1;	    //??????????????, ?????????
	 else 
	{
		a=0; 
		i=TxBufferSize1;
	}
   }
   if(a == 1)
	   uart_printf("falsh rw ok");
   else
	   uart_printf("falsh rw fail");

}
