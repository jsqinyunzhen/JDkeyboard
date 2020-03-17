#include "font.h"
#include "image.h"
#include "KS0108.h"
#include "string.h"
#include "delay.h"
#include "display.h"
#include "stdio.h"
#include <stdlib.h>

extern u8 CurDispNum ;
DisplayCabinet DispCabinet[CABINETNUM] = {0};
u8 CurDispNum = 0;

void Display_InitializeEN(void)
{
    u32 i = 0;
    DisplayCabinetEnGPIO engpio[]= {DSP_CABINET_1,DSP_CABINET_2,DSP_CABINET_3,DSP_CABINET_4,DSP_CABINET_5,DSP_CABINET_6,DSP_CABINET_7,DSP_CABINET_8};
    //u16 port[][3] = {L"一号",L"二号",L"三号",L"四号",L"五号",L"六号",L"七号",L"八号"};
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_15;//|DSP_CABINET_2|DSP_CABINET_3|DSP_CABINET_4|DSP_CABINET_5|DSP_CABINET_6|DSP_CABINET_7|DSP_CABINET_8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    for(i = 0;i < CABINETNUM; i++)
    {
    //    memcpy(DispCabinet[i].name,port[i],4);
        DispCabinet[i].en = engpio[i];
    }
    
    GPIO_ResetBits(GPIOC,DSP_CABINET_1);//|DSP_CABINET_2|DSP_CABINET_3|DSP_CABINET_4|DSP_CABINET_5|DSP_CABINET_6|DSP_CABINET_7|DSP_CABINET_8
}
#define DISP_EMPTY_X 34
#define DISP_EMPTY_Y 0
#define DISP_EMPTY_DX 158
#define DISP_EMPTY_DY 64

void Display_BatteryStatus(u8 id)
{
	  u8 *pImage = 0;
    if(id >= CABINETNUM)
    {
       // printf("Display_BatteryStatus ID Error!\r\n");
        return;
    }
  //  printf("LCD = %d,status = %d\r\n",id,DispCabinet[id].cs);

    switch(DispCabinet[id].cs)
    {
        case BAT_EMPTY:
        case BAT_ERROR:
        {
            pImage = Display_GetBatteryStatusImage(id);
            GLCD_Bitmap((char *)pImage,DISP_EMPTY_X,DISP_EMPTY_Y,DISP_EMPTY_DX,DISP_EMPTY_DY);
        }
        break;
        
        case BAT_POWER_FULL:
        case BAT_CHARGING:
        case BAT_RESERVED:
        {
            pImage = Display_GetBatteryStatusImage(id);
            if(DispCabinet[id].updatescreen == 1)
            {
                GLCD_Bitmap((char *)pImage,DISP_EMPTY_X,DISP_EMPTY_Y,DISP_EMPTY_DX,DISP_EMPTY_DY);
                DispCabinet[id].updatescreen = 0;
            }
            Display_BatterySOCImage(id);
            Display_BatterySOCPercent(id);
        }
        break;
    }
    
}
void Display_AEChar(u16 x,u16 y,u8 size,u16 code)
{  							  
	u8 * AECode = 0;
    if(size == 32 || size == 29 )
    {
        AECode = Font32_GetAEchar_bmp(code);
    }
    else
    {
        AECode = Font16_GetAEchar_bmp(code);
    }
	
	if(AECode == 0)
	{
		return;
	}
    GLCD_Bitmap((char *)AECode,x,y,size,size+3);
    return;   
}   
u32 xx = 0;



#define DISP_FULL_CHAR1_X 5
#define DISP_FULL_CHAR_Y 10
#define DISP_FULL_CHAR_DX 32
#define DISP_FULL_CHAR_DY 32


/*获取电池状态字符串*/
u16 *  Display_GetBatteryStatusChar(u8 id)
{
#if 0
    switch(DispCabinet[id].cs)
    {
        case BAT_EMPTY:
        {
            static u16 BatChar[] = {L"空仓"};
            return BatChar;
        }
        case BAT_POWER_FULL:
        {
            static u16 BatChar[] = {L"已充满"};
            return BatChar;
        }
        case BAT_CHARGING:
        {
            static u16 BatChar[] = {L"充电中"};
            return BatChar;
        }
        case BAT_RESERVED:
        {
            static u16 BatChar[] = {L"预约"};
            return BatChar;
        }
        case BAT_ERROR:
        {
            static u16 BatChar[] = {L"故障"};
            return BatChar;
        }
    }
    #endif
		return 0;
}
u8 *  Display_GetBatteryStatusImage(u8 id)
{
    switch(DispCabinet[id].cs)
    {
        case BAT_EMPTY:
        {
            extern u8 empty_image[];
            return empty_image;
        }
        case BAT_POWER_FULL:
        {
            extern u8 full_image[];
            return full_image;
        }
        case BAT_CHARGING:
        {
            extern u8 charging_image[];
            return charging_image;
        }
        case BAT_RESERVED:
        {
            extern u8 reserved_image[];
            return reserved_image;
        }
        case BAT_ERROR:
        {
           // extern u8 error_image[];
            //return error_image;
        }
    }
    return 0;
}
u8 *  Display_GetNumberImage(u8 id)
{
   u8 * pimage[10] ={
    number00_image,number01_image,number02_image,number03_image,number04_image,number05_image,
    number06_image,number07_image,number08_image,number09_image
   };
    return pimage[id%10];
}

#define BATTERY_IMAGE_X 137
#define BATTERY_IMAGE_Y 4
#define BATTERY_IMAGE_DX 12
#define BATTERY_IMAGE_DY 24

u8 *  Display_BatterySOCImage(u8 id)
{
    #if 0
    if(DispCabinet[id].soc <= 0)
    {
        extern u8 battery_precent0[];
        GLCD_Bitmap((char *)battery_precent0,BATTERY_IMAGE_X,BATTERY_IMAGE_Y,BATTERY_IMAGE_DX,BATTERY_IMAGE_DY);
    }
    else if(DispCabinet[id].soc <= 30)
    {
        extern u8 battery_precent25[];
        GLCD_Bitmap((char *)battery_precent25,BATTERY_IMAGE_X,BATTERY_IMAGE_Y,BATTERY_IMAGE_DX,BATTERY_IMAGE_DY);
    }
    else if(DispCabinet[id].soc <= 60)
    {
        extern u8 battery_precent50[];
        GLCD_Bitmap((char *)battery_precent50,BATTERY_IMAGE_X,BATTERY_IMAGE_Y,BATTERY_IMAGE_DX,BATTERY_IMAGE_DY);
    }
    else if(DispCabinet[id].soc <= 99)
    {
        extern u8 battery_precent75[];
        GLCD_Bitmap((char *)battery_precent75,BATTERY_IMAGE_X,BATTERY_IMAGE_Y,BATTERY_IMAGE_DX,BATTERY_IMAGE_DY);
    }
    #endif
    
    if(DispCabinet[id].soc < 100)
    {
        extern u8 battery_charging[];
        GLCD_Bitmap((char *)battery_charging,BATTERY_IMAGE_X,BATTERY_IMAGE_Y,BATTERY_IMAGE_DX,BATTERY_IMAGE_DY);
    }
    else
    {
        extern u8 battery_precent100[];
        GLCD_Bitmap((char *)battery_precent100,BATTERY_IMAGE_X,BATTERY_IMAGE_Y,BATTERY_IMAGE_DX,BATTERY_IMAGE_DY);
    }
		return 0;
}
#define SOC_PERCENT_X 184
#define SOC_PERCENT_Y 5
#define SOC_PERCENT_DX 64
#define SOC_PERCENT_DY 16

void  Display_BatterySOCPercent(u8 id)
{
    char percent[8] = {' ',' ',' ',' ',0};
    u8 x = SOC_PERCENT_X;
    
    GLCD_GoTo(x-8*strlen(percent),SOC_PERCENT_Y);
    GLCD_WriteString(percent);

    sprintf(percent,"%d%%",DispCabinet[id].soc);
    GLCD_GoTo(x-8*strlen(percent),SOC_PERCENT_Y);
    GLCD_WriteString(percent);
}



void Display_CabinetDataTest(u8 num)
{
  //  GLCD_Bitmap((char *)bg_image,0,0,192,64);
//GLCD_ClearScreen();
/*  statusStr = Display_GetBatteryStatusChar(id);
  if(statusStr != 0)
  {
      Display_AEChar(DISP_FULL_CHAR1_X,  DISP_FULL_CHAR_Y,29,*statusStr);
      Display_AEChar(DISP_FULL_CHAR1_X+32,DISP_FULL_CHAR_Y,29,*(statusStr+1));
      Display_AEChar(DISP_FULL_CHAR1_X+64,DISP_FULL_CHAR_Y,29,*(statusStr+2));    
  }*/
#if 1 //测试
    DispCabinet[0].cs = BAT_ERROR;
    DispCabinet[1].cs = BAT_POWER_FULL;

    DispCabinet[0].soc = 50;
    DispCabinet[1].soc = 100;
#endif

    //xx += 32;
    if(xx > 160)
    {
        xx = 0;
    }

}
void Display_ChangeCurLcdId(void)
{
    u8 i = 0;
    for(i = 0; i < CABINETNUM; i++)
    {
        CurDispNum ++;
        CurDispNum %= CABINETNUM;
        if(DispCabinet[CurDispNum].lcd_timeout == 0)
        {
            break;
        }
    }
    if(i == CABINETNUM)
    {
      //  printf("ALL LCD Timeout!\r\n");
    }
}
u8   display_id_flag = 1;

void Display_CabinetStatus()
{
    static u32 display_delay = 0;


    if((sys_ms_cnt-display_delay) > 1000) //每50MS更新一个充电柜的状态，总共8个柜子，需要400ms循环一次
    {
        display_delay = sys_ms_cnt;                                
        Display_ChangeCurLcdId();
        // Display_CabinetDataTest(CurDispNum);
        Display_BatteryStatus(CurDispNum);
        if(display_id_flag)
        {
            display_id_flag =0;
            Display_Id();
        }
#if 0
        LCD_ID ++;display_id_flag =1;
        DispCabinet[CurDispNum].cs ++;
        DispCabinet[CurDispNum].cs %=BAT_ERROR;
        DispCabinet[CurDispNum].updatescreen = 1;
#endif
#if 0 //测试使用
{
        Battery_tatus old_cs = 0;
        old_cs = DispCabinet[CurDispNum].cs;
        DispCabinet[CurDispNum].cs = (rand())%5;
        if(old_cs != DispCabinet[CurDispNum].cs)
        {
            DispCabinet[CurDispNum].updatescreen = 1;
        }
        if(DispCabinet[CurDispNum].cs == BAT_POWER_FULL)
        {
            DispCabinet[CurDispNum].soc = 100;
        }
        else
        {
            DispCabinet[CurDispNum].soc = 0;
        }
}
#endif
    }
}
extern uint8_t Version[];
void Display_Id(void)
{
    u8 *pImage = 0;
    pImage = Display_GetNumberImage((LCD_ID+1)/10);//imageid++%8
    GLCD_Bitmap((char *)pImage,5,3,12,24);
     pImage = Display_GetNumberImage((LCD_ID+1)%10);//imageid++%8
    GLCD_Bitmap((char *)pImage,5+13,3,12,24);
}
void Display_Init(void)
{
    u8 i = 0;
	
    CurDispNum = CABINETNUM -1;
	
    for(i = 0; i < CABINETNUM; i++)
    {
        Display_ChangeCurLcdId();
        delay_ms(1);
        GLCD_ClearScreen();
        delay_ms(5);
        DispCabinet[i].updatescreen = 1;
        DispCabinet[i].cs = BAT_EMPTY;
    }
    CurDispNum = 0;

    #if 0
    //printf("LCD %s init success\r\n",Version);
    for(i = 0; i < CABINETNUM; i++)
    {
        if(DispCabinet[i].lcd_timeout == 1)
        {
            //printf("LCD %d query timeout!\r\n",i);
        }
    }
    #endif
}
