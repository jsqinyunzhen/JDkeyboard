//  #include "stm32f10x_lib.h"
#include "sys.h"
#include "delay.h"
#include "display.h"

//#define KS0108_PORT  GPIOE
//#define KS0108_PORT  GPIOD
#define KS0108_PORT  GPIOA

#define KS0108_PORT_RW  GPIOC
#define KS0108_RS    GPIO_Pin_13
#define KS0108_RW    GPIO_Pin_14
#define KS0108_EN    DispCabinet[CurDispNum].en //GPIO_Pin_10

#define KS0108_PORT_CS  GPIOB
#define KS0108_REST   GPIO_Pin_1
#define KS0108_CS1   GPIO_Pin_0
#define KS0108_CS2   GPIO_Pin_2
#define KS0108_CS3   GPIO_Pin_10

#define KS0108_D0    0

#define DISPLAY_STATUS_BUSY	0x80
//#define GLCD_DELAY_BUSY

extern unsigned char screen_x;
extern unsigned char screen_y;

GPIO_InitTypeDef GPIO_InitStructure;
extern u32 sys_ms_cnt;

//-------------------------------------------------------------------------------------------------
// Delay function /for 8MHz/
//-------------------------------------------------------------------------------------------------
void GLCD_Delay(void)
{
  delay_us(1);
}
//-------------------------------------------------------------------------------------------------
// Enalbe Controller (0-2)
//-------------------------------------------------------------------------------------------------
void GLCD_EnableController(unsigned char controller)
{
switch(controller){
	case 0 : GPIO_ResetBits(KS0108_PORT_CS, KS0108_CS1); break;
	case 1 : GPIO_ResetBits(KS0108_PORT_CS, KS0108_CS2); break;
	case 2 : GPIO_ResetBits(KS0108_PORT_CS, KS0108_CS3); break;
	}
}
//-------------------------------------------------------------------------------------------------
// Disable Controller (0-2)
//-------------------------------------------------------------------------------------------------
void GLCD_DisableController(unsigned char controller)
{
switch(controller){
	case 0 : GPIO_SetBits(KS0108_PORT_CS, KS0108_CS1); break;
	case 1 : GPIO_SetBits(KS0108_PORT_CS, KS0108_CS2); break;
	case 2 : GPIO_SetBits(KS0108_PORT_CS, KS0108_CS3); break;
	}
}
//-------------------------------------------------------------------------------------------------
// Read Status byte from specified controller (0-2)
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadStatus(unsigned char controller)
{
unsigned char status;

GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = 0xFF << KS0108_D0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
GPIO_Init(KS0108_PORT, &GPIO_InitStructure);

GPIO_SetBits(KS0108_PORT_RW, KS0108_RW);
GPIO_ResetBits(KS0108_PORT_RW, KS0108_RS);
GLCD_EnableController(controller);
GLCD_Delay();
if(DSP_CABINET_1 == DISPLAY_EN)
{
    GPIO_SetBits(DISPLAY_PORT1, DISPLAY_EN);
}
else
{
    GPIO_SetBits(DISPLAY_PORT2, DISPLAY_EN);
}
GLCD_Delay();
status = ((GPIO_ReadInputData(KS0108_PORT) >> KS0108_D0) & 0xFF);
if(DSP_CABINET_1 == DISPLAY_EN)
{
    GPIO_ResetBits(DISPLAY_PORT1, DISPLAY_EN);
}
else
{
    GPIO_ResetBits(DISPLAY_PORT1, DISPLAY_EN);
}

GLCD_DisableController(controller);
return status;
}
//-------------------------------------------------------------------------------------------------
// Write command to specified controller
//-------------------------------------------------------------------------------------------------
u32 LCD_delay1 = 0;
void GLCD_WriteCommand(unsigned char commandToWrite, unsigned char controller)
{
    LCD_delay1 = sys_ms_cnt;
    #ifdef GLCD_DELAY_BUSY
    while(GLCD_ReadStatus(controller)&DISPLAY_STATUS_BUSY)
    {
        if((sys_ms_cnt - LCD_delay1) > 2)
        {
            DispCabinet[CurDispNum].lcd_timeout = 1;
            return;
        }
    }
    #endif
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin  = (0xFF << KS0108_D0);
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(KS0108_PORT, &GPIO_InitStructure);

GPIO_ResetBits(KS0108_PORT_RW, KS0108_RS | KS0108_RW);
GLCD_Delay();
GLCD_EnableController(controller);
GLCD_Delay();
GPIO_SetBits(KS0108_PORT, (commandToWrite << KS0108_D0));
commandToWrite ^= 0xFF;
GPIO_ResetBits(KS0108_PORT, (commandToWrite << KS0108_D0));
GLCD_Delay();
if(DSP_CABINET_1 == DISPLAY_EN)
{
    GPIO_SetBits(DISPLAY_PORT1, DISPLAY_EN);
}
else
{
    GPIO_SetBits(DISPLAY_PORT2, DISPLAY_EN);
}
GLCD_Delay();
if(DSP_CABINET_1 == DISPLAY_EN)
{
    GPIO_ResetBits(DISPLAY_PORT1, DISPLAY_EN);
}
else
{
    GPIO_ResetBits(DISPLAY_PORT1, DISPLAY_EN);
}
GLCD_Delay();
GLCD_DisableController(controller);
}

//-------------------------------------------------------------------------------------------------
// Read data from current position
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadData(void)
{
unsigned char tmp;

#ifdef GLCD_DELAY_BUSY
u32 LCD_delay = sys_ms_cnt;

while(GLCD_ReadStatus(screen_x / 64)&DISPLAY_STATUS_BUSY)
{
    if((sys_ms_cnt - LCD_delay) > 2)
    {
        DispCabinet[CurDispNum].lcd_timeout = 1;
        return 0;
    }
}
#endif

GPIO_StructInit(&GPIO_InitStructure);  
GPIO_InitStructure.GPIO_Pin = 0xFF << KS0108_D0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
GPIO_Init(KS0108_PORT, &GPIO_InitStructure);

GPIO_SetBits(KS0108_PORT_RW, KS0108_RS | KS0108_RW);

GLCD_EnableController(screen_x / 64);
GLCD_Delay();
if(DSP_CABINET_1 == DISPLAY_EN)
{
    GPIO_SetBits(DISPLAY_PORT1, DISPLAY_EN);
}
else
{
    GPIO_SetBits(DISPLAY_PORT2, DISPLAY_EN);
}
GLCD_Delay();
tmp = ((GPIO_ReadInputData(KS0108_PORT) >> KS0108_D0) & 0xFF);
if(DSP_CABINET_1 == DISPLAY_EN)
{
    GPIO_ResetBits(DISPLAY_PORT1, DISPLAY_EN);
}
else
{
    GPIO_ResetBits(DISPLAY_PORT1, DISPLAY_EN);
}
GLCD_DisableController(screen_x / 64);
screen_x++;
return tmp;
}
//-------------------------------------------------------------------------------------------------
// Write data to current position
//-------------------------------------------------------------------------------------------------
void GLCD_WriteData(unsigned char dataToWrite)
{
#ifdef GLCD_DELAY_BUSY
    u32 LCD_delay = sys_ms_cnt;

    while(GLCD_ReadStatus(screen_x / 64)&DISPLAY_STATUS_BUSY)
    {
        if((sys_ms_cnt - LCD_delay) > 2)
        {
            DispCabinet[CurDispNum].lcd_timeout = 1;
            return;
        }
    }
#endif

GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = (0xFF << KS0108_D0);
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(KS0108_PORT, &GPIO_InitStructure);

GPIO_ResetBits(KS0108_PORT_RW, KS0108_RW);
GLCD_Delay();
GPIO_SetBits(KS0108_PORT_RW, KS0108_RS);
GLCD_Delay();
GPIO_SetBits(KS0108_PORT, (dataToWrite << KS0108_D0));
dataToWrite ^= 0xFF;
GPIO_ResetBits(KS0108_PORT, (dataToWrite << KS0108_D0));
GLCD_Delay();
GLCD_EnableController(screen_x / 64);
GLCD_Delay();
//GPIO_SetBits(DISPLAY_PORT, DISPLAY_EN);
if(DSP_CABINET_1 == DISPLAY_EN)
{
    GPIO_SetBits(DISPLAY_PORT1, DISPLAY_EN);
}
else
{
    GPIO_SetBits(DISPLAY_PORT2, DISPLAY_EN);
}
GLCD_Delay();
//GPIO_ResetBits(DISPLAY_PORT, DISPLAY_EN);
if(DSP_CABINET_1 == DISPLAY_EN)
{
    GPIO_ResetBits(DISPLAY_PORT1, DISPLAY_EN);
}
else
{
    GPIO_ResetBits(DISPLAY_PORT1, DISPLAY_EN);
}
GLCD_Delay();
GLCD_DisableController(screen_x / 64);
screen_x++;
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_InitializePorts(void)
{
vu32 i;

if(KS0108_PORT == GPIOE)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
}
else
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin   =  (0xFF << KS0108_D0);
GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;
GPIO_Init(KS0108_PORT, &GPIO_InitStructure);

GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin   =  KS0108_RS|KS0108_RW;
GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;
GPIO_Init(KS0108_PORT_RW, &GPIO_InitStructure);

GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin   =  DSP_CABINET_1;
GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;
GPIO_Init(DISPLAY_PORT1, &GPIO_InitStructure);

GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin   =  DSP_CABINET_2|DSP_CABINET_3|DSP_CABINET_4|DSP_CABINET_5|DSP_CABINET_6|DSP_CABINET_7|DSP_CABINET_8;
GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;
GPIO_Init(DISPLAY_PORT2, &GPIO_InitStructure);


GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin   =  KS0108_CS1|KS0108_CS2|KS0108_CS3;
GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;
GPIO_Init(KS0108_PORT_CS, &GPIO_InitStructure);

GPIO_SetBits(KS0108_PORT, (0xFF << KS0108_D0));
GPIO_SetBits(KS0108_PORT_CS, KS0108_CS1 | KS0108_CS2 | KS0108_CS3 |KS0108_REST);
GPIO_SetBits(KS0108_PORT_RW, KS0108_RS | KS0108_RW );

}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadByteFromROMMemory(char * ptr)
{
  return *(ptr);
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
