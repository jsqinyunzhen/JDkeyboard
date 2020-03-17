#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "stm32f10x.h"


#define CABINETNUM (1)
extern uint32_t sys_ms_cnt;
extern u8 LCD_ID;

typedef enum{ 
    DSP_CABINET_1 = GPIO_Pin_15,
    DSP_CABINET_2 = GPIO_Pin_2,
    DSP_CABINET_3 = GPIO_Pin_3,
    DSP_CABINET_4 = GPIO_Pin_4,
    DSP_CABINET_5 = GPIO_Pin_5,
    DSP_CABINET_6 = GPIO_Pin_6,
    DSP_CABINET_7 = GPIO_Pin_7,
    DSP_CABINET_8 = GPIO_Pin_8,
}DisplayCabinetEnGPIO;

#define DISPLAY_PORT1  GPIOC
#define DISPLAY_PORT2  GPIOB
#define DISPLAY_EN    DispCabinet[CurDispNum].en //GPIO_Pin_10
//#define DISPLAY_EN    GPIO_Pin_1


typedef enum{ 
    BAT_EMPTY = 0,//空仓
    BAT_CHARGING ,//充电中
    BAT_POWER_FULL ,//充电满
    BAT_RESERVED,//预约
    BAT_ERROR, //故障
}Battery_tatus;

typedef struct _DisplayCabinet
{
    u16 name[10];
    DisplayCabinetEnGPIO en; //柜子的使能脚
    Battery_tatus cs; //柜子的状态
    int soc;
    u16 power; //柜子的充电功率
    u8 updatescreen;
    u8 lcd_timeout;
}DisplayCabinet;




extern DisplayCabinet DispCabinet[];
extern u8 CurDispNum;

void Display_Init(void);
extern void Display_AEChar(u16 x,u16 y,u8 size,u16 num);
extern void Display_CabinetStatus(void);
extern void Display_InitializeEN(void);
extern u8 *  Display_GetBatteryStatusImage(u8 id);
extern u16 *  Display_GetBatteryStatusChar(u8 id);
u8 *  Display_BatterySOCImage(u8 id);
void  Display_BatterySOCPercent(u8 id);
u8 *  Display_GetNumberImage(u8 id);
void Display_Id(void);
#endif
