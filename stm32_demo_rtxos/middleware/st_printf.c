
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "misc.h"

#include <stdarg.h>  
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>  

#include "project_config.h"
#include "st_printf.h"
#include "stm32f10x_it.h"


#include "st_audio_product.h"
#include "app4g.h"
#include "app_rs485_broker.h"
#include "ds18b20.h"
#include "ta6932.h"
#include "st_rtc.h"
#include "clock_calendar.h"
#include "transport.h"
#include "mqtt_app.h"
#include "st_adc.h"
#include "dianchuan.h"
#include "st_gpio.h"


/****************************************************************************
* Ãû    ³Æ£ºvoid GPIO_Configuration(void)
* ¹¦    ÄÜ£ºÍ¨ÓÃIO¿ÚÅäÖÃ
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£º
****************************************************************************/  
void gpio2uart4_pin_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};
  /* Ä¬ÈÏ¸´ÓÃ¹¦ÄÜ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         		 //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 
  /* ¸´ÓÃ¹¦ÄÜµÄÊäÈëÒý½Å±ØÐëÅäÖÃÎªÊäÈëÄ£Ê½£¨¸¡¿Õ/ÉÏÀ­/ÏÂÀ­µÄÒ»ÖÖ£©*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 //¸´ÓÃ¸¡¿ÕÊäÈë
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);		         	 //A¶Ë¿Ú 

//GPIO_PinRemapConfig
}
void gpio2uart3_pin_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};
  /* Ä¬ÈÏ¸´ÓÃ¹¦ÄÜ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         		 //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 
  /* ¸´ÓÃ¹¦ÄÜµÄÊäÈëÒý½Å±ØÐëÅäÖÃÎªÊäÈëÄ£Ê½£¨¸¡¿Õ/ÉÏÀ­/ÏÂÀ­µÄÒ»ÖÖ£©*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 //¸´ÓÃ¸¡¿ÕÊäÈë
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);		         	 //A¶Ë¿Ú 

//GPIO_PinRemapConfig
}
void gpio2uart3_pin_config_gpio(uint8_t on_off)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	         		 //USART1 TX
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    		 //¸´ÓÃÍÆÍìÊä³ö

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 

	if(on_off ==1)//on==1
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
	}
	
	else if(on_off ==0)//on==1
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	}
}
void gpio2uart4_pin_config_gpio(uint8_t on_off)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	         		 //USART1 TX
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    		 //¸´ÓÃÍÆÍìÊä³ö

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 

	if(on_off ==1)//on==1
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_10);
		GPIO_SetBits(GPIOC,GPIO_Pin_11);
	}
	
	else if(on_off ==0)//on==1
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_10);
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	}
}

void gpio2uart1_pin_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};
  /* Ä¬ÈÏ¸´ÓÃ¹¦ÄÜ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 
  /* ¸´ÓÃ¹¦ÄÜµÄÊäÈëÒý½Å±ØÐëÅäÖÃÎªÊäÈëÄ£Ê½£¨¸¡¿Õ/ÉÏÀ­/ÏÂÀ­µÄÒ»ÖÖ£©*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 //¸´ÓÃ¸¡¿ÕÊäÈë
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 //A¶Ë¿Ú 

//GPIO_PinRemapConfig
}
volatile uint8_t pc_4g_debug_on =0;
void gpio2uart2_txpin_config_pc4g_debug(uint8_t on_off)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};
	pc_4g_debug_on = on_off;
  /* Ä¬ÈÏ¸´ÓÃ¹¦ÄÜ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         		 //USART1 TX
	if(on_off ==0)//on==1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö
	else
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    		 //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 
  /* ¸´ÓÃ¹¦ÄÜµÄÊäÈëÒý½Å±ØÐëÅäÖÃÎªÊäÈëÄ£Ê½£¨¸¡¿Õ/ÉÏÀ­/ÏÂÀ­µÄÒ»ÖÖ£©*/
}
#if 0
void gpio2uart2_rxpin_config_pc4g_debug(uint8_t on_off)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};
	pc_4g_debug_on = on_off;
  /* Ä¬ÈÏ¸´ÓÃ¹¦ÄÜ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         		 //USART1 TX
	if(on_off ==0)//on==1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö
	else
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    		 //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 
  /* ¸´ÓÃ¹¦ÄÜµÄÊäÈëÒý½Å±ØÐëÅäÖÃÎªÊäÈëÄ£Ê½£¨¸¡¿Õ/ÉÏÀ­/ÏÂÀ­µÄÒ»ÖÖ£©*/
}
#endif
void gpio2uart2_pin_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};
  /* Ä¬ÈÏ¸´ÓÃ¹¦ÄÜ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         		 //USART1 TX
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö

  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    		 //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 
  /* ¸´ÓÃ¹¦ÄÜµÄÊäÈëÒý½Å±ØÐëÅäÖÃÎªÊäÈëÄ£Ê½£¨¸¡¿Õ/ÉÏÀ­/ÏÂÀ­µÄÒ»ÖÖ£©*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         	 //USART1 RX
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   	 //¸´ÓÃ¸¡¿ÕÊäÈë

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 //¸´ÓÃ¸¡¿ÕÊäÈë
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 //A¶Ë¿Ú 

//GPIO_PinRemapConfig
}
void gpio2uart5_pin_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};

  
  //USART5_TX	PC12	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12	 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //¸´ÓÃÍÆÍìÊä³ö	
  GPIO_Init(GPIOC, &GPIO_InitStructure); //³õÊ¼»¯PC12 
  //USART5_RX		PD2    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//ÉÏÀ­ÊäÈë    
  GPIO_Init(GPIOD, &GPIO_InitStructure);  //³õÊ¼»¯PB11

}

#if 0
void uart1_NVIC_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
	   //Usart1 NVIC ÅäÖÃ
	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	  //ÉèÖÃNVICÖÐ¶Ï·Ö×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶	 0-3;
	   
	   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		 //×ÓÓÅÏÈ¼¶3
	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQÍ¨µÀÊ¹ÄÜ
	   NVIC_Init(&NVIC_InitStructure);	  //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷

}
#endif
//DMA_Streamx:DMAÊý¾ÝÁ÷,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAÍ¨µÀÑ¡Ôñ,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:ÍâÉèµØÖ·
//mar:´æ´¢Æ÷µØÖ·
//ndtr:Êý¾Ý´«ÊäÁ¿  
void DMA_Config(DMA_Channel_TypeDef *DMA_Streamx,uint32_t par,
					uint32_t mar,uint32_t dir,u16 ndtr)
{  	
	DMA_InitTypeDef  DMA_InitStructure;	
	#if 0
	if((u32)DMA_Streamx>(u32)DMA2)
	//µÃµ½µ±Ç°streamÊÇÊôÓÚDMA2»¹ÊÇDMA1	
	{	  
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
		//DMA2Ê±ÖÓÊ¹ÄÜ 			
	}
	else 	
	{	  
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
		//DMA1Ê±ÖÓÊ¹ÄÜ 	
	} 
	#endif
	DMA_DeInit(DMA_Streamx);		
	//while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//µÈ´ýDMA¿ÉÅäÖÃ 	 
	/* ÅäÖÃ DMA Stream */ 
	
	//DMA_InitStructure.DMA_Channel 					= chx;  							
	//Í¨µÀÑ¡Ôñ  
	DMA_InitStructure.DMA_PeripheralBaseAddr 		= par;								
	//DMAÍâÉèµØÖ·  
	DMA_InitStructure.DMA_MemoryBaseAddr 			= mar;								
	//DMA ´æ´¢Æ÷0µØÖ· 
	DMA_InitStructure.DMA_DIR 					    = dir;								
	//direction of transmit.  
	DMA_InitStructure.DMA_BufferSize 				 = ndtr;								
	//Êý¾Ý´«ÊäÁ¿   
	DMA_InitStructure.DMA_PeripheralInc				= DMA_PeripheralInc_Disable;		
	//ÍâÉè·ÇÔöÁ¿Ä£Ê½  
	DMA_InitStructure.DMA_MemoryInc 				= DMA_MemoryInc_Enable;				
	//´æ´¢Æ÷ÔöÁ¿Ä£Ê½ 
	DMA_InitStructure.DMA_PeripheralDataSize 		= DMA_PeripheralDataSize_Byte;		
	//ÍâÉèÊý¾Ý³¤¶È:8Î» 
	DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;			
	//´æ´¢Æ÷Êý¾Ý³¤¶È:8Î» 
	DMA_InitStructure.DMA_Mode 						= DMA_Mode_Normal;					
	// Ê¹ÓÃÆÕÍ¨Ä£Ê½ 
	//RX
	if((DMA_Streamx ==DMA1_Channel3)||(DMA_Streamx ==DMA1_Channel5)|| \
	   (DMA_Streamx ==DMA1_Channel6)||(DMA_Streamx ==DMA2_Channel3))
		DMA_InitStructure.DMA_Priority 					= DMA_Priority_VeryHigh;	
	else
		//TX
		DMA_InitStructure.DMA_Priority 					= DMA_Priority_Low;
	//ÖÐµÈÓÅÏÈ¼¶  
	//DMA_InitStructure.DMA_FIFOMode 					= DMA_FIFOMode_Disable;          
	//DMA_InitStructure.DMA_FIFOThreshold 			    = DMA_FIFOThreshold_Full;  
	//DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;			
	//´æ´¢Æ÷Í»·¢µ¥´Î´«Êä  
	//DMA_InitStructure.DMA_PeripheralBurst 		    = DMA_PeripheralBurst_Single;		
	//ÍâÉèÍ»·¢µ¥´Î´«Êä  
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA_Streamx, &DMA_InitStructure);  
	//DMA_Cmd(DMA1_Channel2, ENABLE);//·¢ËÍÍ¨µÀ ·¢ËÍµÄÊ±ºòÔÙÊ¹ÄÜ
	if((DMA_Streamx ==DMA1_Channel3)||(DMA_Streamx ==DMA1_Channel5)|| \
	   (DMA_Streamx ==DMA1_Channel6)||(DMA_Streamx ==DMA2_Channel3))
		DMA_Cmd(DMA_Streamx,ENABLE);
}
//¿ªÆôÒ»´ÎDMA´«Êä
void DMA_Enable(DMA_Channel_TypeDef *DMA_Streamx,u16 ndtr)
{ 	
	DMA_Cmd(DMA_Streamx, DISABLE);                      
	//ÏÈ¹Ø±ÕDMA,²ÅÄÜÉèÖÃËü		
	//while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//µÈ´ý´«Êä½áÊø			
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          
	//ÉèÖÃ´«ÊäÊý¾Ý³¤¶È  	
	DMA_Cmd(DMA_Streamx, ENABLE);                      
	//¿ªÆôDMA
}	  

void USART_Config(USART_TypeDef* USARTx)
{
	
	USART_InitTypeDef USART_InitStructure={0};
	USART_InitStructure.USART_BaudRate = 115200;	
	#if (HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1)
	if(debug_uart_id == 1)
	{
		if(USARTx == USART1)
			USART_InitStructure.USART_BaudRate = 115200;	//debug
		else if(USARTx == USART2)
			USART_InitStructure.USART_BaudRate = 115200;//4//module 4g
		else if(USARTx == USART3)
			USART_InitStructure.USART_BaudRate = 9600;
		else if(USARTx == UART4)
			USART_InitStructure.USART_BaudRate = 9600;
		else if(USARTx == UART5)
			USART_InitStructure.USART_BaudRate = 4800;

			
	}
	else if(debug_uart_id == 4)
	{
		if(USARTx == USART1)
			USART_InitStructure.USART_BaudRate = 4800;//485
		else if(USARTx == USART2)
			USART_InitStructure.USART_BaudRate = 115200;	
		else if(USARTx == USART3)
			USART_InitStructure.USART_BaudRate = 9600;//not use
		else if(USARTx == UART4)
			USART_InitStructure.USART_BaudRate = 115200;//debug
		else if(USARTx == UART5)
			USART_InitStructure.USART_BaudRate = 4800;
	}
	else 
	{

		USART_InitStructure.USART_BaudRate = 115200;//115200;		//ËÙÂÊ115200bps
	}
	#elif (HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2)
	//dc
	if(debug_uart_id == 1)
	{
		if(USARTx == USART1)
			USART_InitStructure.USART_BaudRate = 115200;	//debug
		else if(USARTx == USART2)
			USART_InitStructure.USART_BaudRate = 115200;//module 4g
		else if(USARTx == USART3)
			USART_InitStructure.USART_BaudRate = 9600;//dc1
		else if(USARTx == UART4)
			USART_InitStructure.USART_BaudRate = 9600;//dc2
		else if(USARTx == UART5)
			USART_InitStructure.USART_BaudRate = 4800;//not use for dianhuohua

			
	}
	else if(debug_uart_id == 4)
	{
		if(USARTx == USART1)
			USART_InitStructure.USART_BaudRate = 4800;//485 broker
		else if(USARTx == USART2)
			USART_InitStructure.USART_BaudRate = 115200;//module 4g	
		else if(USARTx == USART3)
			USART_InitStructure.USART_BaudRate = 115200;//not use
		else if(USARTx == UART4)
			USART_InitStructure.USART_BaudRate = 115200;//debug
		else if(USARTx == UART5)
			USART_InitStructure.USART_BaudRate = 4800;//not use for dianhuohua
	}
	else 
	{

		USART_InitStructure.USART_BaudRate = 115200;//115200;		//ËÙÂÊ115200bps
	}	
	#endif
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//Êý¾ÝÎ»8Î»
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//Í£Ö¹Î»1Î»
  USART_InitStructure.USART_Parity = USART_Parity_No;				//ÎÞÐ£ÑéÎ»
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //ÎÞÓ²¼þÁ÷¿Ø
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//ÊÕ·¢Ä£Ê½

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);							//ÅäÖÃ´®¿Ú²ÎÊýº¯Êý
 
  
  /* Enable USART1 Receive and Transmit interrupts */
  //	#if (UART1_RX_DMA ==1) ||(UART2_RX_DMA ==1) 
  //  USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
  //rx config
	if(USARTx == USART1)
	{
		#if (UART1_RX_DMA ==1)
		//initialize the DMA channel.	
		DMA_Config(DMA1_Channel5,	
			(uint32_t)&(USART1->DR),	  //´®¿ÚDR¼Ä´æÆ÷						 
			(uint32_t)RxBuffer1,//×Ô¶¨ÒåµÄ½ÓÊÕÊý¾Ýbuf						
			DMA_DIR_PeripheralSRC,//ÍâÉèµ½´æ´¢Æ÷·½Ïò						 
			UART1_RX_BUFFER_LEN);//³¤¶È
		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);//¿ªÆôDMA½ÓÊÕ
		 USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
		#else
	  // ²ÉÓÃ¿ÕÏÐÖÐ¶Ï£¬Ä¿µÄÊÇÔÚ²úÉú¿ÕÏÐÖÐ¶ÏÊ±£¬ËµÃ÷½ÓÊÕ»òÕß·¢ËÍÒÑ¾­½áÊø£¬´ËÊ±¿ÉÒÔ¶ÁÈ¡DMAÖÐµÄÊý¾ÝÁË¡£
		USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);                    //Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
		#endif

	}
	else if(USARTx == USART2)
	{
		#if (UART2_RX_DMA ==1) 
		//initialize the DMA channel.	
		DMA_Config(DMA1_Channel6,	
			(uint32_t)&(USART2->DR),	  //´®¿ÚDR¼Ä´æÆ÷						 
			(uint32_t)RxBuffer2,//×Ô¶¨ÒåµÄ½ÓÊÕÊý¾Ýbuf						
			DMA_DIR_PeripheralSRC,//ÍâÉèµ½´æ´¢Æ÷·½Ïò						 
			UART2_RX_BUFFER_LEN);//³¤¶È
		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//¿ªÆôDMA½ÓÊÕ
		USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	 	 #else
		// ²ÉÓÃ¿ÕÏÐÖÐ¶Ï£¬Ä¿µÄÊÇÔÚ²úÉú¿ÕÏÐÖÐ¶ÏÊ±£¬ËµÃ÷½ÓÊÕ»òÕß·¢ËÍÒÑ¾­½áÊø£¬´ËÊ±¿ÉÒÔ¶ÁÈ¡DMAÖÐµÄÊý¾ÝÁË¡£
		  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);					//Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
 		 #endif
	}
	else if(USARTx == USART3)
	{
	#if (UART3_RX_DMA ==1) 
		//initialize the DMA channel.	
		DMA_Config(UART3_RX_DMA_CHANNEL,	
			(uint32_t)&(USART3->DR),	  //´®¿ÚDR¼Ä´æÆ÷						 
			(uint32_t)RxBuffer3,//×Ô¶¨ÒåµÄ½ÓÊÕÊý¾Ýbuf						
			DMA_DIR_PeripheralSRC,//ÍâÉèµ½´æ´¢Æ÷·½Ïò						 
			UART3_RX_BUFFER_LEN);//³¤¶È
		USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);//¿ªÆôDMA½ÓÊÕ
		USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	 #else
		// ²ÉÓÃ¿ÕÏÐÖÐ¶Ï£¬Ä¿µÄÊÇÔÚ²úÉú¿ÕÏÐÖÐ¶ÏÊ±£¬ËµÃ÷½ÓÊÕ»òÕß·¢ËÍÒÑ¾­½áÊø£¬´ËÊ±¿ÉÒÔ¶ÁÈ¡DMAÖÐµÄÊý¾ÝÁË¡£
		  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);					//Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
	 #endif

	}
	else if(USARTx == UART4)
	{
#if (UART4_RX_DMA ==1) 
		//initialize the DMA channel.	
		DMA_Config(DMA2_Channel3,	
			(uint32_t)&(UART4->DR),	  //´®¿ÚDR¼Ä´æÆ÷						 
			(uint32_t)RxBuffer4,//×Ô¶¨ÒåµÄ½ÓÊÕÊý¾Ýbuf						
			DMA_DIR_PeripheralSRC,//ÍâÉèµ½´æ´¢Æ÷·½Ïò						 
			UART4_RX_BUFFER_LEN);//³¤¶È
		USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);//¿ªÆôDMA½ÓÊÕ
		USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
 #else
		// ²ÉÓÃ¿ÕÏÐÖÐ¶Ï£¬Ä¿µÄÊÇÔÚ²úÉú¿ÕÏÐÖÐ¶ÏÊ±£¬ËµÃ÷½ÓÊÕ»òÕß·¢ËÍÒÑ¾­½áÊø£¬´ËÊ±¿ÉÒÔ¶ÁÈ¡DMAÖÐµÄÊý¾ÝÁË¡£
		  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);					//Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
 #endif

	}

	else if(USARTx == UART5)
	{

		 USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);	
	}


	//tx config
	if(USARTx == USART1)
	{
		#if( UART1_TX_DMA == 1)
			//initialize the DMA channel.	
			DMA_Config(DMA1_Channel4,	
				(uint32_t)&(USART1->DR),	  //´®¿ÚDR¼Ä´æÆ÷						 
				(uint32_t)TxBuffer1,//×Ô¶¨ÒåµÄ½ÓÊÕÊý¾Ýbuf						
				DMA_DIR_PeripheralDST,//ÍâÉèµ½´æ´¢Æ÷·½Ïò						 
				UART1_TX_BUFFER_LEN);//³¤¶È
			USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);//¿ªÆôDMA½ÓÊÕ
			//USART_ITConfig(USARTx, USART_IT_TXE, ENABLE); 

		#else

			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);						//Ê¹ÄÜ·¢ËÍ»º³å¿ÕÖÐ¶Ï   
		
		#endif
	
	}
	else if(USARTx == USART2)
	{
		#if( UART2_TX_DMA == 1)
			//initialize the DMA channel.	
			DMA_Config(DMA1_Channel7,	
				(uint32_t)&(USART2->DR),	  //´®¿ÚDR¼Ä´æÆ÷						 
				(uint32_t)TxBuffer2,//×Ô¶¨ÒåµÄ½ÓÊÕÊý¾Ýbuf						
				DMA_DIR_PeripheralDST,//ÍâÉèµ½´æ´¢Æ÷·½Ïò						 
				UART2_TX_BUFFER_LEN);//³¤¶È
			USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);//¿ªÆôDMA½ÓÊÕ
			//USART_ITConfig(USARTx, USART_IT_TXE, ENABLE); 
		#else

			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);						//Ê¹ÄÜ·¢ËÍ»º³å¿ÕÖÐ¶Ï   
		
		#endif
	}
	else if(USARTx == USART3)
	{
	#if( UART3_TX_DMA == 1)
			//initialize the DMA channel.	
			DMA_Config(UART3_TX_DMA_CHANNEL,	
				(uint32_t)&(USART3->DR),	  //´®¿ÚDR¼Ä´æÆ÷						 
				(uint32_t)TxBuffer3,//×Ô¶¨ÒåµÄ½ÓÊÕÊý¾Ýbuf						
				DMA_DIR_PeripheralDST,//ÍâÉèµ½´æ´¢Æ÷·½Ïò						 
				UART3_TX_BUFFER_LEN);//³¤¶È
			USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);//¿ªÆôDMA½ÓÊÕ
			//USART_ITConfig(USARTx, USART_IT_TXE, ENABLE); 
	#else
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);						//Ê¹ÄÜ·¢ËÍ»º³å¿ÕÖÐ¶Ï   
	#endif
	}
	else if(USARTx == UART4)
	{
#if( UART4_TX_DMA == 1)
			//initialize the DMA channel.	
			DMA_Config(DMA2_Channel5,	
				(uint32_t)&(UART4->DR),	  //´®¿ÚDR¼Ä´æÆ÷						 
				(uint32_t)TxBuffer4,//×Ô¶¨ÒåµÄ½ÓÊÕÊý¾Ýbuf						
				DMA_DIR_PeripheralDST,//ÍâÉèµ½´æ´¢Æ÷·½Ïò						 
				UART4_TX_BUFFER_LEN);//³¤¶È
			USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);//¿ªÆôDMA½ÓÊÕ
			//USART_ITConfig(USARTx, USART_IT_TXE, ENABLE); 
#else
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);						//Ê¹ÄÜ·¢ËÍ»º³å¿ÕÖÐ¶Ï   
#endif
	}

	else if(USARTx == UART5)
	{
		USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
	}


	USART_ClearFlag(USARTx, USART_FLAG_TC); 
	/* Enable the USART1 */
	USART_Cmd(USARTx, ENABLE);	
	
}
/*
#define LEN  200 

//#define LEN (33 +2)
char string[LEN]={0}; 
char string_len=0; 
*/
//static 
int uart1_sendchar(char ch)
{
    USART1->SR;
    USART_SendData(USART1,ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);

    return ch;
}
void uart1_sendstring( char *pt,int len)  
{  


	while((*pt) && (len>0))  
	 {
			uart1_sendchar(*pt++);  
			len-- ;
	 }
	// UART0_PutStr("\n");
	uart1_sendchar('\r');	 
	uart1_sendchar('\n');

}  

int sendchar(char ch)
{  
	//AcquireMutex(mutex_id_uart);
//	while( !(USART1->SR&USART_FLAG_TC));//Êý¾ÝÎ´·¢ËÍÍê

	if(debug_uart_id == 1)
	{
		USART1->SR;
		USART_SendData(USART1,ch);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	}	
	else if(debug_uart_id == 4)
	{
		UART4->SR;
		USART_SendData(UART4,ch);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);
	}
	else
	{

	}
	//ReleaseMutex(mutex_id_uart);
	return ch;  
} 
void uart_sendstring( char *pt,int len)  
{  

#if 0

    while(*pt)  
     sendchar(*pt++);  

   //  sendchar('\n');
    UART0_PutStr("\n");
		
		
			{
	
		if(*pt == '\n')
		{

			sendchar('\r');
			pt++ ;
		}
		else
			sendchar(*pt++);  
	}
#else
	 
	while((*pt) && (len>0))  
	 {
			sendchar(*pt++);  
			len-- ;
	 }
	// UART0_PutStr("\n");
	sendchar('\r');	 
	sendchar('\n');
#endif
}  
#if PRINTF_SUPPORT
void uart_printf( char *fmt,...)  
{ 

/*
	if(low_power_req_flag ==1)
	{
		memset(string,0,100);
		strcpy(string,"LOWPOWER 1");
		uart_sendstring(string, strlen("LOWPOWER 1"));  
		return;
	}
*/
	va_list ap;  
	int len=0;
	 memset(string,0,LEN);
	va_start(ap,fmt);  

	len = vsprintf((char*)string,fmt,ap);  //Use It Will Increase the code size, Reduce the efficiency  
	if((len >0)&&(len <=LEN))
	{
	//	UART0_PutStr("len succ ");
		//"len =%d",len);
		uart_sendstring((char*)string, len);  
	}
	else
	{

		//uart_sendstring(string, LEN);  
		
		//memset(string,0,LEN);
		//strcpy(string,"len error");
		//uart_sendstring(string, strlen("len error"));  
		//snprintf(string,LEN-1,"len =%d error",len);

		uart_sendstring((char*)string, LEN-1);  
	}
	va_end(ap);  

}  
#else
void uart_printf( char *fmt,...)
{
}
#endif
void Get_ChipInfo(void)
{
	uint32_t ChipUniqueID32[3];
	uint16_t ChipUniqueID16[2];
	uint32_t stm32_flash_size;
	
	uint32_t mcu_id;
	uint16_t mcu_dev_id;
	uint16_t mcu_rev_id;
	stm32_flash_size = *((uint16_t*)0x1FFFF7E0);
	uart_printf("STM32_FLASH_SIZE: %dKB", stm32_flash_size);
	

	ChipUniqueID16[0] = *((__IO uint16_t*)0X1FFFF7E8);
	ChipUniqueID16[1] = *((__IO uint16_t*)0X1FFFF7Ea);
	
	ChipUniqueID32[0] = *((__IO uint32_t*)0X1FFFF7ec);
	ChipUniqueID32[1] = *((__IO uint32_t*)0X1FFFF7F0);
	uart_printf("ChipUniqueID: 0x%08x-%08x-%04x-%04x",ChipUniqueID32[1],ChipUniqueID32[0],ChipUniqueID16[1],ChipUniqueID16[0]);

	mcu_id = *((__IO uint32_t*)0Xe0042000);
	//0x10036414
	mcu_rev_id = (mcu_id>>16);
	mcu_dev_id = (mcu_id&0xfff);
	uart_printf("mcu_id: 0x%08x mcu_rev_id: 0x%04x mcu_dev_id: 0x%04x", mcu_id ,mcu_rev_id,mcu_dev_id);
	switch(mcu_dev_id)
	{
		case 0x412:
			uart_printf("In low-density devices");
			switch(mcu_rev_id)
			{
				case 0x1000:
					uart_printf("In low-density devices:Revision A");
					break;
				default:

					uart_printf("fatal devices,mcu_rev_id=0x%x",mcu_rev_id);
					break;			
			}
			break;
		case 0x410:
			uart_printf("In medium-density devices");
			switch(mcu_rev_id)
			{
				case 0x0000:
					uart_printf("In medium-density devices:Revision A");
					break;
				case 0x2000:
					uart_printf("In medium-density devices:Revision B");
					break;
				case 0x2001:
					uart_printf("In medium-density devices:Revision Z");
					break;
				case 0x2003:
					uart_printf("In medium-density devices:Revision Y, 1, 2 or X");
					break;
				default:
					uart_printf("fatal devices,mcu_rev_id=0x%x",mcu_rev_id);
					break;			
			}
			break;
		case 0x414:
			uart_printf("In high-density devices");
			switch(mcu_rev_id)
			{
				case 0x1000:
					uart_printf("In high-density devices:Revision A or 1");
					break;
				case 0x1001:
					uart_printf("In high-density devices:Revision Z");
					break;
				case 0x1003:
					uart_printf("In high-density devices:Revision Y, 1, 2 or X");
					break;
				default:
					uart_printf("fatal devices");
					break;			
			}
			break;
		case 0x430:
			uart_printf("In XL-density devices");
			break;
		case 0x418:
			uart_printf("In connectivity devices");
			break;	
		default:
			uart_printf("fatal devices,mcu_dev_id=0x%x",mcu_dev_id);
			 break;
	}

/*	
	Bits 31:16 REV_ID(15:0) Revision identifier
This field indicates the revision of the device:
In low-density devices:
– 0x1000 = Revision A
In medium-density devices:
– 0x0000 = Revision A
– 0x2000 = Revision B
– 0x2001 = Revision Z
– 0x2003 = Revision Y, 1, 2 or X
In high-density devices:
– 0x1000 = Revision A or 1
– 0x1001 = Revision Z
– 0x1003 = Revision Y, 1, 2 or X
In XL-density devices:
– 0x1000 = Revision A
In connectivity line devices:
– 0x1000 = Revision A
– 0x1001 = Revision Z
*/
}
#if 1
void printf_test(void)
{
	unsigned short int cpu_type = 0x5678;

	unsigned char *p =NULL;

	float fo = 10.55;
	double dou = 110.55;
	//printf("%f %f\n",fo,dou);   
//	uart_printf("kl03 bin build time is %s :%s\n",__DATE__,__TIME__); 

	uart_printf("%s:%s %d\n",__FILE__,__func__,__LINE__);   
	uart_printf("sizeof(char)=%d\n",sizeof(char));
	uart_printf("sizeof(short)=%d\n",sizeof(short));
	uart_printf("sizeof(int)=%d\n",sizeof(int));
	uart_printf("sizeof(long int)=%d\n",sizeof(long int));
	uart_printf("sizeof(long long)=%d\n",sizeof(long long));
 

	uart_printf("sizeof(float)=%d\n",sizeof(float));
	uart_printf("sizeof(double)=%d\n",sizeof(double));
	uart_printf("fo=%5.3f\n",fo);
	uart_printf("dou=%3.2f\n",dou);
	uart_printf("sizeof(p)=%d\n",sizeof(p));
	uart_printf("cpu_type=%d cpu_type=%x cpu_type=0x%04x,address cpu_type=0x%x,is %p\n",cpu_type,cpu_type,cpu_type,&cpu_type,&cpu_type);

	//uart_printf("end");

//	printf("%s %d\n",__func__,__LINE__);   
//	printf("sizeof(char)=%d\n",sizeof(char));
//	printf("sizeof(short)=%d\n",sizeof(short));
//	printf("sizeof(int)=%d\n",sizeof(int));
//	printf("sizeof(long int)=%d\n",sizeof(long int));
	cpu_type =0x1234;
	p = (unsigned char *)&cpu_type;
	
	if(*p == 0x34)
	{
		uart_printf("stm32f103 cpu is little\n");

	}
	else
	{
		uart_printf("stm32f103 cpu is big\n");

	}
}
#endif
#if !defined(RTE_Compiler_EventRecorder)

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
 // AcquireMutex(mutex_id_uart);
 if(debug_uart_id == 1)
 {
	  USART_SendData(USART1, (uint8_t) ch);

	  /* Loop until the end of transmission */
	  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	  {
	  }
 }
 else if(debug_uart_id == 4)
 {
	 USART_SendData(UART4,ch);
	 while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);
 }

 // ReleaseMutex(mutex_id_uart);

  return ch;
}
#endif
void handle_uart_debug_cmd(void)
{
	int ret = 0;
	uint8_t len = 0;
	char*uart_buf = ( char*)RxBuffer1;
	uint8_t uart_buf_len = RxCounter1;
	if(debug_uart_id ==4)
	{
		uart_buf = ( char*)RxBuffer4;
		uart_buf_len = RxCounter4;
	}
//	uint32_t flags = 0;
	//uint32_t timeout = 10;
	if(uart_buf ==NULL)
		printf("uart_buf ==NULL\r\n");
	if(uart_buf_len ==0)
		printf("uart_buf_len ==0\r\n");
	else
		printf("uart_buf_len =%d =%s\r\n",uart_buf_len,uart_buf);
	ret=strncmp(uart_buf,"usb",3);
	if(ret==0)
	{
		printf("uart_buf=%s\r\n",uart_buf);
		goto out;
	}
	ret=strncmp(uart_buf,"debug on",6);
	if(ret==0)
	{
		
	//	debug_uart_id = PRINTF_UART_ID ; 
		printf("debug_uart_id=%d\r\n",debug_uart_id);
		goto out;
	}
	ret=strncmp(uart_buf,"debug off",7);
	if(ret==0)
	{
	//	debug_uart_id = PRINTF_UART_ID ;
		printf("debug_uart_id=%d\r\n",debug_uart_id);
		debug_uart_id = PRINTF_UART_ID_INVALID ; 
		goto out;
	}
	ret=strncmp(uart_buf,"reboot",6);
	if(ret==0)
	{
		printf("reboot...\r\n");
		mcu_sys_soft_reset();
		while(1);

	}
	ret=strncmp(uart_buf,"pc_4g_debug_on",14);
	if(ret==0)
	{
		printf("pc_4g_debug_on\r\n");
		
		gpio2uart2_txpin_config_pc4g_debug(1);
		goto out;
	}
	ret=strncmp(uart_buf,"pc_4g_debug_off",15);
	if(ret==0)
	{
		printf("pc_4g_debug_off\r\n");
		gpio2uart2_txpin_config_pc4g_debug(0);
		goto out;
	}
	ret=strncmp(uart_buf,"app4g_reset_io_high",19);
	if(ret==0)
	{
		printf("app4g_reset_io_high\r\n");
		app4g_reset_io_high();;
		goto out;
	}
	ret=strncmp(uart_buf,"app4g_reset_io_low",18);
	if(ret==0)
	{
		printf("app4g_reset_io_low\r\n");
		app4g_reset_io_low();;
		goto out;
	}
	ret=strncmp(uart_buf,"standby_io_high",15);
	if(ret==0)
	{
		printf("app4g_standby_io_high\r\n");
		app4g_standby_io_high();;
		goto out;
	}
	ret=strncmp(uart_buf,"standby_io_low",14);
	if(ret==0)
	{
		printf("app4g_standby_io_low\r\n");
		app4g_standby_io_low();;
		goto out;
	}
	ret=strncmp(uart_buf,"mqtt_stop",9);
	if(ret==0)
	{
		printf("mqtt_stop\r\n");
		//osThreadSuspend(thread_app);
		dx_set_mqtt_thread_stop();
		goto out;
	}

	ret=strncmp(uart_buf,"mqtt_pause",10);
	if(ret==0)
	{
		printf("mqtt_pause\r\n");
		//osThreadSuspend(thread_app);
		dx_set_mqtt_thread_pause();
		goto out;
	}
	ret=strncmp(uart_buf,"mqtt_run",8);
	if(ret==0)
	{
		printf("mqtt_run\r\n");
		//osThreadResume(thread_app);
		dx_clear_mqtt_thread_pause();
		goto out;
	}

	//osThreadSuspend(thread_uart_msg);
	#if (STM_RTC_ENABLE == 1)
	ret=strncmp(uart_buf,"rtc_show",8);
	if(ret==0)
	{
		st_rtc_show();
		DisplayDate();
		printf("rtc_show\r\n");
		goto out;
	}
	#endif
	ret=strncmp(uart_buf,"uart1_rs485_rx",14);
	if(ret==0)
	{
		app_uart1_rs485_rxtx_io_low_is_rx();
		reset_uart1_rx_buffer();
#if (UART1_RX_DMA ==1)
		DMA_Enable(DMA1_Channel5,UART1_RX_BUFFER_LEN);//¿ªÆôÏÂÒ»´ÎDMA½ÓÊÕ
#endif

		printf("app_uart1_rs485_rxtx_io_low_is_rx\r\n");
		goto out;
	}

	
	ret=strncmp(uart_buf,"uart1_rs485_tx",14);
	if(ret==0)
	{
		app_uart1_rs485_rxtx_io_high_is_tx();
		printf("app_uart1_rs485_rxtx_io_high_is_tx\r\n");
		uart1_cpu_printf("uart1_cpu_printf test\r\n");
		goto out;
	}	
	ret=strncmp(uart_buf,"rs485_broker_info",strlen("rs485_broker_info"));
	if(ret==0)
	{
		app_rs485_broker_info_print();
		printf("app_rs485_broker_info_print\r\n");
		goto out;
	}
	ret=strncmp(uart_buf,"rs485_broker_status",strlen("rs485_broker_status"));
	if(ret==0)
	{
		int i=0;
		uint16_t cur_port_status=0;
		printf("rs485_broker_status\r\n");
		for(i=0; i<RS485_BROKER_NUM; i++)
		{
			ret= app_rs485_broker_port_x_status(i+1,&cur_port_status);
			printf("ret=%d,i=%d,cur_port_status=%d\r\n",ret,i,cur_port_status);
		}
		
		goto out;
	}

	
	
	ret=strncmp(uart_buf,"app_rs485_broker_test",strlen("app_rs485_broker_test"));
	if(ret==0)
	{
		app_rs485_broker_test();
		printf("app_rs485_broker_test\r\n");
		goto out;
	}

	

	ret=strncmp(uart_buf,"dc_port_x_status",strlen("dc_port_x_status"));
	if(ret==0)
	{
		//dc_tx_rx_data(1,DIANCHUAN_CMD_GetAllPortStatus);
		dc_port_status();
		dc_port_x_status(1,NULL);
		printf("dc_port_x_status\r\n");
		goto out;
	}
	ret=strncmp(uart_buf,"dc2_test",8);
	if(ret==0)
	{
		dc_tx_rx_data(2,DIANCHUAN_CMD_GetAllPortStatus);
	
		
		printf("dc2_test\r\n");
		goto out;
	}
	ret=strncmp(uart_buf,"charge_mg_test",strlen("charge_mg_test"));
	if(ret==0)
	{

		charge_mg_init();
		//charge_mg_set_on(1,120,200,100,30);
		charge_mg_set_on(1,3,200,100,30);

		dc_port_x_status(1,NULL);
		dc_set_max_power(1,500);
		//dc_get_total_consumption(1);

		//charge_mg_set_off(1);

		goto out;
	}

	

	ret=strncmp(uart_buf,"audio enable",12);
	if(ret==0)
	{
		audio_pa_io_enable();
		printf("audio_pa_io_enable\r\n");
		goto out;
	}
	ret=strncmp(uart_buf,"audio disable",13);
	if(ret==0)
	{
		printf("audio_pa_io_disable\r\n");
		audio_pa_io_disable();
		goto out;
	}
	ret=strncmp(uart_buf,"ds18b20",7);
	if(ret==0)
	{
		float  f_tem = DS18B20_Get_Temp(NULL,4);
		printf("DS18B20_Get_Temp %f\r\n",f_tem);
		goto out;
	}
	ret=strncmp(uart_buf,"ntc_temp",8);
	if(ret==0)
	{
		int  ntc_temp = get_ntc_temp_adc1_chn9();
		printf("ntc temp %d\r\n",ntc_temp);
		goto out;
	}
	ret=strncmp(uart_buf,"http_test",9);
	if(ret==0)
	{
		#if 1
		memcpy(updata_path,HTTPURL_BAIDU,sizeof(HTTPURL_BAIDU));
		dx_set_mqtt_thread_pause();
		#else
		ret =dx_lte_http_contextid_config();
		if(ret == 0)
		{
			ret = dx_lte_http_url_config(HTTPURL_BAIDU);
			if(ret == 0)
			{
				int file_len = 0;
				ret = dx_lte_http_file_len(&file_len);
				if(ret == 0)
				{
					printf("file_len= %d\r\n",file_len);
					ret =dx_lte_http_getfile(STM32APPBIN,file_len);
					printf("dx_lte_http_getfile=%d\r\n",ret);
				}
				else
					printf("dx_lte_http_file_len=%d\r\n",ret);
					
			}
		}
		#endif
		goto out;

	}
	ret=strncmp(uart_buf,"clearid",7);
	//printf("uart_buf=%s\r\n",uart_buf );
	if(ret==0)
	{
		sFLASH_EraseSector(FLASH_DEVID_ADDRESS);
		goto out;
	}
	ret=strncmp(uart_buf,"clear_rs485id",strlen("clear_rs485id"));
	//printf("uart_buf=%s\r\n",uart_buf );
	if(ret==0)
	{
		sFLASH_EraseSector(FLASH_BROKERID_ADDRESS);
		goto out;
	}

	ret=strncmp(uart_buf,"setid:",6);
	//printf("uart_buf=%s\r\n",uart_buf );
	if(ret==0)
	{
		//gpio_led_on();
		char id[DEVID_SPACE] ={0};
		char id_len =0;
		//char bit_num =0;
		char*p = strstr((const char*)uart_buf, "setid:");
		if(p != NULL)
		{
			dx_set_mqtt_thread_stop();
			osDelay(2000);
			printf("p =%s\r\n",p );
	
			p += strlen("setid:");
			//data_len = 0;
			id_len = 0;
			while(*p != 0x0d)
			{
				//data_len *= 10;
				
				id[id_len+1] = *p;
				p++;
				id_len++;
			}
			id[0] = id_len;
			printf("id=%s,id_len=%d\r\n",&id[1],id_len);
			sFLASH_EraseSector(FLASH_DEVID_ADDRESS);
			sFLASH_WriteBuffer((uint8_t*)id, FLASH_DEVID_ADDRESS, DEVID_SPACE);
			memset(id,0,DEVID_SPACE);
			sFLASH_ReadBuffer((uint8_t*)id, FLASH_DEVID_ADDRESS, DEVID_SPACE);
			printf("id[0]=%d\r\n",id[0]);
			printf("id =%s\r\n",&id[1]);

		}

		goto out;
	}
	ret=strncmp(uart_buf,"led_on",6);
	if(ret==0)
	{
		gpio_led_on();
		goto out;
	}
	ret=strncmp(uart_buf,"led_off",7);
	if(ret==0)
	{
		gpio_led_off();
		goto out;
	}	

	ret=strncmp(uart_buf,"led_show",8);
	if(ret==0)
	{
		TA6932_DisplayPortNumber(1,789);
		goto out;
	}
	ret=strncmp(uart_buf,"status_4g",8);
	if(ret==0)
	{
		printf("dc %d %d\r\n",dc1_exist,dc2_exist);
		printf("statu_4g= %d\r\n",status_4g);
		printf("module4g_init= %d\r\n",module4g_init);
		printf("charge_mode= %d\r\n",charge_mode);
		// charge_mode = CHARGE_MODE_BROKER ;
		printf("mqtt socket= %d\r\n",get_mqtt_transport_sock());
		app4g_run_ok_need_time();
		goto out;
	}
	ret=strncmp(uart_buf,"m4g_imei",8);
	if(ret==0)
	{
		printf("g_imei:%s\n", g_imei);
		goto out;
	}
	ret=strncmp(uart_buf,"m4g_imsi",8);
	if(ret==0)
	{
		printf("g_imsi:%s\n", g_imsi);
		goto out;
	}
	ret=strncmp(uart_buf,"m4g_ccid",8);
	if(ret==0)
	{
		printf("g_sim_ccid:%s\n", g_sim_ccid);
		goto out;
	}

	ret=strncmp(uart_buf,"at_udp_test",11);
	if(ret==0)
	{
		at_udp_ntp_test();
		goto out;
	}
	ret=strncmp(uart_buf,"at_tcp_test",11);
	if(ret==0)
	{
		at_tcp_mqtt_test();
		goto out;
	}	

	ret=strncmp(uart_buf,"uart5_485_tx",12);
	if(ret==0)
	{
		printf("uart5_tx set\r\n");
		app_uart5_rs485_rxtx_io_high_is_tx();
		goto out;
	}
	ret=strncmp(uart_buf,"uart5_485_rx",12);
	if(ret==0)
	{
		printf("uart5_rx set\r\n");
		app_uart5_rs485_rxtx_io_low_is_rx();
		goto out;
	}	
	ret=strncmp(uart_buf,"uart5_broker_tx",15);
	if(ret==0)
	{
		uart2_dma_printf("uart5_broker_tx\r\n");
	#if 1
		//char* test = "uart5 rs485 to uart1 rs485";
		char* test = "uart5 rs485 test";
		memset(TxBuffer5,0,UART5_RX_BUFFER_LEN);
		memcpy(TxBuffer5,test,strlen(test));
		uart5_cpu_send_data(TxBuffer5,strlen(test));
		//uart1_dma_send_data(TxBuffer5,strlen(test));
	#else
		char* test = "uart1 2rs485 to uart5 rs485";
		memset(TxBuffer1,0,UART1_RX_BUFFER_LEN);
		memcpy(TxBuffer1,test,strlen(test));
		//uart5_cpu_send_data(RxBuffer5,strlen(test));
		uart1_dma_send_data(TxBuffer1,strlen(test));
	#endif
		goto out;
	}
	//UARA1 FOR UART2 4G TEST
	len = strlen(MODULE_4G_COMMAND_PREFIX);
	//printf("MODULE_4G_COMMAND_PREFIX=%d\r\n",strlen(MODULE_4G_COMMAND_PREFIX));
	ret=strncmp(uart_buf,MODULE_4G_COMMAND_PREFIX,len);
	if(ret==0)
	{
		//send at command to 4g
		//memmove(RxBuffer1,RxBuffer1+len,RxCounter1-len);
		//memset(&RxBuffer1[RxCounter1-len],0,UART1_RX_BUFFER_LEN-(RxCounter1-len));
		//printf("at command uart_buf=%s ,RxCounter1-len=%d\r\n",uart_buf,RxCounter1-len);
		//printf("RxBuffer1=%s, =%d\r\n",RxBuffer1,(RxCounter1-len));
		// =8cmd=_AT+GMI
		uart_buf = ( char*)(uart_buf+strlen(MODULE_4G_COMMAND_PREFIX));
		printf("uart_buf=%s,len=%d\r\n",uart_buf,RxCounter1-len);
		//printf("strlen(AT_GSN)=%d\r\n",strlen(AT_GSN));
		ret=strncmp(uart_buf,AT_GSN,strlen(AT_GSN)-1);
		if(ret==0)
		{
			printf("dx_get_lte_imei\r\n");
			dx_get_lte_imei();
			goto out;
		}
		ret=strncmp(uart_buf,AT_CSQ,strlen(AT_CSQ)-1);
		if(ret==0)
		{
			
			ret = dx_get_lte_signalQuality();
			printf("dx_get_lte_signalQuality=%d\r\n",ret);
			goto out;
		}
		ret=strncmp(uart_buf,AT_QPINC_READ,strlen(AT_QPINC_READ)-1);
		if(ret==0)
		{
			
			ret = dx_get_lte_pinCount();
			printf("dx_get_lte_pinCount=%d\r\n",ret);
			goto out;
		}
		ret=strncmp(uart_buf,AT_CPIN_READ,strlen(AT_CPIN_READ)-1);
		if(ret==0)
		{
			
			ret = dx_get_lte_pinState();
			printf("dx_get_lte_pinState=%d\r\n",ret);
			goto out;
		}
		ret=strncmp(uart_buf,AT_CLCK_READ,strlen(AT_CLCK_READ)-1);
		if(ret==0)
		{
			
			ret = dx_get_lte_pinSwitch();
			printf("dx_get_lte_pinSwitch=%d\r\n",ret);
			goto out;
		}


		ret=strncmp(uart_buf,AT_CGREG_READ,strlen(AT_CGREG_READ)-1);
		if(ret==0)
		{
			
			ret = dx_get_lte_network_resiger_status();
			printf("dx_get_lte_network_resiger_status=%d\r\n",ret);
			goto out;
		}
		ret=strncmp(uart_buf,AT_CIMI,strlen(AT_CIMI)-1);
		if(ret==0)
		{
			
			ret = dx_interface_get_imsi(g_imsi,16);
			printf("dx_interface_get_imsi=%d g_imsi=%s\r\n",ret,g_imsi);
			goto out;
		}		
		ret=strncmp(uart_buf,AT_QSIMSTAT_READ,strlen(AT_QSIMSTAT_READ)-1);
		if(ret==0)
		{
			
			ret = dx_lte_traffic_GetLteStatus();
			printf("dx_lte_traffic_GetLteStatus=%d\r\n",ret);
			goto out;
		}
		
		ret=strncmp(uart_buf,AT_QCCID,strlen(AT_QCCID)-1);
		if(ret==0)
		{
			
			ret = dx_lte_traffic_GetCCID(g_sim_ccid,SIM_CCID_LEN);
			printf("dx_lte_traffic_GetCCID=%d %s\r\n",ret,g_sim_ccid);
			goto out;
		}
		ret=strncmp(uart_buf,AT_COPS_READ,strlen(AT_COPS_READ)-1);
		if(ret==0)
		{
			char buffer[12]={0};
			 dx_get_lte_network(buffer,sizeof(buffer));
			printf("dx_get_lte_network=%d %s\r\n",ret,buffer);
			goto out;
		}

		

		ret=strncmp(uart_buf,AT_QNWINFO,strlen(AT_QNWINFO)-1);
		if(ret==0)
		{
			char buffer[7]={0};
			dx_get_lte_datacap(buffer,7);
			printf("dx_get_lte_datacap=%d %s\r\n",ret,buffer);
			goto out;
		}
		ret=strncmp(uart_buf,AT_QNTP_WRITE,8);
		if(ret==0)
		{
	
			ret = dx_set_lte_ntp_server();
			printf("dx_set_lte_ntp_server=%d\r\n",ret);
			goto out;
		}

		ret=strncmp(uart_buf,AT_CCLK_READ,strlen("AT+CCLK?"));
		if(ret==0)
		{
			char buffer[22]={0};
			myst  pt;
			dx_get_lte_ntp_time(buffer,22);
			printf("dx_get_lte_ntp_time=%d %s\r\n",ret,buffer);
			
			mytransfor(buffer,&pt);
			
			goto out;
		}	
		ret=strncmp(uart_buf,AT_TCP_TEST,8);
		if(ret==0)
		{

			dx_tcp_test();
			//int sock =transport_open(NULL,0);
			#if 0
			printf("transport_open sock=%d\r\n",sock);
			sock = transport_close(sock);
			printf("transport_close sock=%d\r\n",sock);
			#endif
			goto out;
		}
		ret=strncmp(uart_buf,AT_QPING,strlen("AT+QPING"));
		if(ret==0)
		{

			dx_lte_ping_demo();
			goto out;
		}	



		//ret=strncmp(uart_buf,AT_GMI,strlen(AT_GMI));
		//if(ret==0)
		
		{
			printf("no define at command\r\n");
			//printf("AT_GMI=%s strlen(AT_GMI)=%d\r\n",AT_GMI,strlen(AT_GMI));
		#if 1
			uint8_t rcv_buf[UART2_RX_BUFFER_LEN]={0};
			uint8_t rcv_len = 0;
			printf("AT_cmd=%s,%d ...\r\n",uart_buf,RxCounter1-len);
			ret = AT_cmd((uint8_t*)uart_buf,RxCounter1-len,rcv_buf,&rcv_len);
			//ret = AT_cmd(uart_buf,RxCounter1-len,NULL,NULL);
			//ret = AT_cmd(AT_GMI,strlen(AT_GMI),rcv_buf,&rcv_len);
			//ret = AT_cmd(AT_GMI,strlen(AT_GMI),NULL,NULL);
			if(ret == 0)
			{
				//printf("rcv_buf=%s rcv_len=%d\r\n",rcv_buf,rcv_len);
			}
		#else
			//Request_Manufacturer_Identification
			printf("send at=%s\r\n",uart_buf);
			//uart2_dma_send_data(RxBuffer1,RxCounter1-len);
			//uart2_dma_send_data(uart_buf,RxCounter1-len);
			uart2_dma_send_data(AT_GMI,strlen(AT_GMI));
			flags =osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2_TX_COMPLETE, osFlagsWaitAny, osWaitForever);
			//flags =osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2, osFlagsWaitAny, osWaitForever);
			//rx at command response from 4g module
			printf("*****osEventFlagsWait flags =0x%08x\r\n",flags);
			if(flags == EVENT_FLAGS_UART2_TX_COMPLETE)
			{
				printf("send at end,rx...\r\n");
			}

			
			flags =osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2, osFlagsWaitAny, osWaitForever);
			//flags =osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2, osFlagsWaitAny, osWaitForever);
			//rx at command response from 4g module
			//debug_uart_id = PRINTF_UART_ID ;
			printf("*****osEventFlagsWait flags =0x%08x\r\n",flags);
			if(flags == EVENT_FLAGS_UART2)
			{
				printf("uart2 rx at command response from 4g moduler\r\n");
				uart2_rec_at_cmd_response(NULL,NULL);
			}
			else
			{
				printf("send at command,but 4g moduler no response\r\n");
			}
		#endif
			//#if (UART1_RX_DMA ==1)
			//DMA_Enable(DMA1_Channel7,UART1_RX_BUFFER_LEN);//¿ªÆôÏÂÒ»´ÎDMA½ÓÊÕ
			//#endif
		}

		goto out;
	}
	
	//printf("uart1 rx int test uart_buf=%s uart_buf_len=%d\r\n",uart_buf,uart_buf_len);
out:
	//printf("uart1 rx int test");
	//printf("uart_buf=%s uart_buf_len=%d\r\n",uart_buf,uart_buf_len);

	if(debug_uart_id ==1)
	{
	reset_uart_debug_buffer();
#if (UART1_RX_DMA ==1)
	DMA_Enable(DMA1_Channel5,UART1_RX_BUFFER_LEN);//¿ªÆôÏÂÒ»´ÎDMA½ÓÊÕ
#endif
	}
	else if(debug_uart_id ==4)
	{
	reset_uart4_rx_buffer();
#if (UART4_RX_DMA ==1)
	DMA_Enable(UART4_RX_DMA_CHANNEL,UART4_RX_BUFFER_LEN);//¿ªÆôÏÂÒ»´ÎDMA½ÓÊÕ
#endif
	}

}






