/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: tea5767.c
* 内容简述:
*       本程序包含了TEA5767的底层驱动函数
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-06 sun68  创建该文件
*
*/


/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "project_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* I2C控制线的定义 */

#define TEA_SCL_IN  {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=8<<8;}
#define TEA_SCL_OUT {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=3<<8;}

#define TEA_SDA_IN  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define TEA_SDA_OUT {GPIOB->CRH&=0XFFFF0fFF;GPIOB->CRH|=3<<12;}

#define SCL_H       GPIOB->BSRR = GPIO_Pin_10			   
#define SCL_L       GPIOB->BRR  = GPIO_Pin_10 
   
#define SDA_H        GPIOB->BSRR = GPIO_Pin_11 
#define SDA_L         GPIOB->BRR  = GPIO_Pin_11 

#define SCL_read    GPIOB->IDR  & GPIO_Pin_10
#define SDA_read     GPIOB->IDR  & GPIO_Pin_11




#define Tea5767_WriteAddress1    0xc0
#define Tea5767_ReadAddress1     0xc1

/* Private variables ---------------------------------------------------------*/
uint8_t Tx1_Buffer[] = {0XF0,0X2C,0XD0,0X12,0X40};
uint8_t Rx1_Buffer[] = {0XF0,0X2C,0XD0,0X12,0X40};
uint8_t Tx2_Buffer[] = {0X00,0Xc0,0xe0,0X41,0X6e,0X7e};
//uint8_t Rx1_Buffer[BufferSize1], Rx2_Buffer[BufferSize2];	  

unsigned long   FM_FREQ=105300000;		  //默认西安交通广播98.8MHz
//unsigned long   FM_FREQ=91600000;
unsigned long FM_PLL;
unsigned char PLL_HIGH=0;
unsigned char PLL_LOW=0;
unsigned char len=0; 
static unsigned char rec_f=2;
unsigned char ch2=0;

void I2C_GPIO_Config(void);


void I2C_delay(void);
bool I2C_Start(void);

void I2C_Stop(void);

void I2C_Ack(void);

void I2C_NoAck(void);


bool I2C_WaitAck(void);
void I2C_SendByte(u8 SendByte);
void I2C_FM_Init(void);
bool I2C_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite);
//extern void Delay(__IO uint32_t nCount);
static void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
void FM_Configuration(void);

/****************************************************************************
* 名    称：void FM_Configuration(void)
* 功    能：I2C FM收音机模块TEA5767控制线的初始化 
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void FM_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;    
  /* 配置PB10,PB11为I2C的 SCL SDL */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;		    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;//GPIO_Mode_IPU;                                               
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/****************************************************************************
* 名    称：void I2C_EE_Init()
* 功    能：FM 初始化
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void I2C_FM_Init(void)
{
  /* I2C控制线配置 */
  FM_Configuration();

  /* I2C 初始化 */
  //I2C_Configuration();  
}

/****************************************************************************
* 名    称：bool I2C_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
* 功    能：I2C写
* 入口参数：uint8_t* pBuffer --待写入的数组  uint8_t WriteAddr--器件地址  uint8_t NumByteToWrite--写入的字节数
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
bool I2C_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(WriteAddr);                         //器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	 //等待应答
		while(NumByteToWrite--)
		{
		  I2C_SendByte(* pBuffer);
		  I2C_WaitAck();
          pBuffer++;
		}
	  I2C_Stop(); 	
	  return TRUE;
}

/****************************************************************************
* 名    称：void I2C_delay(void)
* 功    能：I2C 控制延时函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void I2C_delay(void)
{
/*	
   u8 i=100*10; 
   while(i) 
   { 
     i--; 
   } 
   */
   Delay1ms(1);
}
/****************************************************************************
* 名    称：bool I2C_Start(void)
* 功    能：I2C起始状态
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
bool I2C_Start(void)
{
	TEA_SDA_OUT
	SDA_H;						//SDA置高
	
	TEA_SCL_OUT
	SCL_H;						//SCL置高
	
	I2C_delay();
	
	TEA_SDA_IN
	if(!SDA_read)return FALSE;	//SDA线为低电平则总线忙,退出
	
	TEA_SDA_OUT
	SDA_L;
	
	I2C_delay();
	TEA_SDA_IN
	if(SDA_read) return FALSE;	//SDA线为高电平则总线出错,退出
	
	TEA_SDA_OUT
	SDA_L;						//SDA置低
	I2C_delay();
	return TRUE;
}
/****************************************************************************
* 名    称：void I2C_Stop(void)
* 功    能：I2C停止状态
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void I2C_Stop(void)
{
	TEA_SCL_OUT
	SCL_L;				  
	I2C_delay();
	TEA_SDA_OUT
	SDA_L;
	I2C_delay();
	TEA_SCL_OUT
	SCL_H;
	I2C_delay();
	TEA_SDA_OUT
	SDA_H;
	I2C_delay();
}
/****************************************************************************
* 名    称：void I2C_Ack(void)
* 功    能：I2C ACK应答
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void I2C_Ack(void)
{	
	TEA_SCL_OUT
	SCL_L;
	I2C_delay();
	
	TEA_SDA_OUT
	SDA_L;
	I2C_delay();
	
	TEA_SCL_OUT
	SCL_H;
	I2C_delay();
	
	TEA_SCL_OUT
	SCL_L;
	I2C_delay();
}
/****************************************************************************
* 名    称：void I2C_NoAck(void)
* 功    能：I2C 无应答
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void I2C_NoAck(void)
{	
	TEA_SCL_OUT
	SCL_L;
	I2C_delay();
	
	TEA_SDA_OUT
	SDA_H;
	I2C_delay();
	
	TEA_SCL_OUT
	SCL_H;
	I2C_delay();
	
	TEA_SCL_OUT
	SCL_L;
	I2C_delay();
}
/****************************************************************************
* 名    称：bool I2C_WaitAck(void)
* 功    能：I2C等待应答
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
bool I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	TEA_SCL_OUT
	SCL_L;
	I2C_delay();
	
	TEA_SDA_OUT
	SDA_H;			
	I2C_delay();
	
	TEA_SCL_OUT
	SCL_H;
	I2C_delay();
	
	TEA_SDA_IN
	if(SDA_read)
	{
	  TEA_SCL_OUT
      SCL_L;
      return FALSE;
	}
	TEA_SCL_OUT
	SCL_L;
	return TRUE;
}
/****************************************************************************
* 名    称：void I2C_SendByte(u8 SendByte)
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
		TEA_SCL_OUT
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
	  {
		  TEA_SDA_OUT
        SDA_H;  
	  }
      else 
		 {
		  TEA_SDA_OUT
        SDA_L; 
		 }			 
        SendByte<<=1;
        I2C_delay();
		 TEA_SCL_OUT
		SCL_H;
        I2C_delay();
    }
	TEA_SCL_OUT
    SCL_L;
}
/****************************************************************************
* 名    称：u8 I2C_ReceiveByte(void)
* 功    能：I2C接收字节
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
u8 I2C_ReceiveByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;
	TEA_SDA_OUT
    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;   
		TEA_SCL_OUT		
      SCL_L;
      I2C_delay();
		TEA_SCL_OUT
	  SCL_H;
      I2C_delay();	
		
		TEA_SDA_IN
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

/****************************************************************************
* 名    称：bool I2C_ReadByte(u8* pBuffer,   u8 length,   u8 DeviceAddress)
* 功    能：I2C 读
* 入口参数：u8* pBuffer-- 数组     u8 length--读出的字节数  u8 DeviceAddress--器件地址
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/	
bool I2C_ReadByte(u8* pBuffer,   u8 length,   u8 DeviceAddress)
{		
    if(!I2C_Start())return FALSE;
    I2C_SendByte(DeviceAddress);                            //器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
	    
		while(length--)
		{
		  *pBuffer = I2C_ReceiveByte();
     	  if(length == 1)I2C_NoAck();
     	  else I2C_Ack(); 
          pBuffer++;
        
		}
	  I2C_Stop(); 	
	  return TRUE;
}


/****************************************************************************
* 名    称：void SetPLL(void)
* 功    能：tea5767设置FM频率
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void SetPLL(void)
{	     
   FM_PLL=(unsigned long)((4000*(FM_FREQ/1000+225))/32768); 	         //计算PLL值
   if(rec_f==2) 
	   PLL_HIGH=(unsigned char)(((FM_PLL >> 8)&0X3f)|0xc0);	 //PLL高字节值--搜索模式静音
   else 
	   PLL_HIGH=(unsigned char)((FM_PLL >> 8)&0X3f);	                 //PLL高字节值
   Tx1_Buffer[0]=PLL_HIGH;		 						                 //I2C第一字节值
   PLL_LOW=(unsigned char)FM_PLL;			      		                 //PLL低字节值
   Tx1_Buffer[1]= PLL_LOW;						                         //I2C第二字节值
   I2C_Write(Tx1_Buffer, Tea5767_WriteAddress1, 5); 					 //写入tea767
}
int main_fm(unsigned char rec_f)
{ float a=0;
  unsigned long fm_ch[50];
  unsigned char ch1=0;
	uint8_t TxBuffer1[] = "奋斗版STM32开发板TEA5767的演示DEMO";
  //RCC_Configuration();
  //NVIC_Configuration();
  FM_Configuration();  
  //Usart1_Init();  	  
  printf("\r\n 奋斗版STM32开发板TEA5767的演示DEMO \n");
  printf("\r\n");
  printf("\r\n");
  printf("\r\n H(h)---帮助                    S(s)---搜索节目  \n");
  printf("\r\n D(d)---显示有效节目            xxP(xxp)---播放选定的节目(如12P) \n");
  printf("\r\n xx.xM(xx.xm)---直接选定频率(如98.8M)      \n");
  printf("\r\n");
  printf("\r\n");
  I2C_Write(Tx1_Buffer, Tea5767_WriteAddress1, 5);   	//设置TEA5767默认的5个字节数据
  SetPLL();												//设置初始默认频率
  
 // while (1)
  {	
  	if(rec_f!=0){
		
		if(rec_f==1){                   //直接输入频率
			a=atof(TxBuffer1);
			FM_FREQ=a*1000000;
			SetPLL();
			//printf(&TxBuffer1[0],len+2);
			printf("\r 当前FM频率是:   %g\n MHz \n",a); 
		}
		else if(rec_f==2){                   //搜索
			//printf("Search......",15+2);
			printf("\n 搜索FM节目! \n");
			Tx1_Buffer[0] = 0XF0; 
			//I2C_Write(Tx1_Buffer, Tea5767_WriteAddress1, 5); 
			//Delay(0xffffff);
			FM_FREQ=87500000;
			FM_FREQ=FM_FREQ+100000;				
		  	SetPLL(); 
			Delay(0x6fffff);
			I2C_ReadByte(Rx1_Buffer,5,Tea5767_ReadAddress1);
			ch2=0;
		 	while(1){
		    //IF<51  IF>55  LEVEL<9
fm_pub:
				FM_FREQ=FM_FREQ+100000;			
		  		if(FM_FREQ>108000000)		
				{
					FM_FREQ=105300000; 
					break;
				}  
				SetPLL(); 
				Delay(0x0fffff);
				I2C_ReadByte(Rx1_Buffer,5,Tea5767_ReadAddress1);				
				a=FM_FREQ;
				a=a/1000000;				
				if((Rx1_Buffer[0]&0x3f)!=(Tx1_Buffer[0]&0x3f)||(Rx1_Buffer[1]!=Tx1_Buffer[1])||(Rx1_Buffer[1]&0x80!=0x80)||Rx1_Buffer[2]<50||Rx1_Buffer[2]>=56||(Rx1_Buffer[3]>>4)<7||(Rx1_Buffer[3]>>4)>14){
				    
					printf("\n 当前FM频率是:   %g MHz \r\n",a); 
								
				}
				else {					
					
					printf("\n 当前FM频率是:   %g MHz     有信号!!!    %u  %u\r\n",a, Rx1_Buffer[2],Rx1_Buffer[3]>>4);
					fm_ch[ch2]= FM_FREQ; 					
					ch2++;
					goto fm_pub;
				}
				
				
			}
			if(FM_FREQ!=105300000) goto fm_pub;
			printf("\r\n 有效的FM频率总共有:   %u 个 \n",ch2); 
			ch1=ch2;
			while(ch1--){
				a=fm_ch[ch1];
				a=a/1000000;	
				printf("\r\n %u  FM频率: %g  MHz \n",ch1,a); 
			}				
		}
		else if(rec_f==3){                   //显示有效频率
			printf("\r\n 有效的FM频率总共有:   %u 个 \n",ch2); 
			ch1=ch2;
			while(ch1--){
				a=fm_ch[ch1];
				a=a/1000000;	
				printf("\r\n %u  FM频率: %g  MHz \n",ch1,a); 
			}				
		}
		else if(rec_f==4){                   //显示有效频率
			ch1=atoi(TxBuffer1);
			FM_FREQ=fm_ch[ch1];
			a=fm_ch[ch1];
			a=a/1000000;
			printf("\r\n 当前FM频率是:   %g\n MHz \n",a); 
		}
	    else if(rec_f==5){                   //显示有效频率
			printf("\r\n 奋斗版STM32开发板TEA5767的演示DEMO \n");
  			printf("\r\n");
  			printf("\r\n");
  			printf("\r\n H(h)---帮助                    S(s)---搜索节目  \n");
  			printf("\r\n D(d)---显示有效节目            xxP(xxp)---播放选定的节目(如12P) \n");
  			printf("\r\n xx.xM(xx.xm)---直接选定频率(如98.8M)      \n");
  			printf("\r\n");
  			printf("\r\n");
		}
	
		rec_f=0;
		SetPLL();
	}
  }
  return 0;
}

