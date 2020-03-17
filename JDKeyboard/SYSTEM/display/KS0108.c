//-------------------------------------------------------------------------------------------------
// Graphic LCD C library
// (c) Rados³aw Kwiecieñ, radek@dxp.pl
//-------------------------------------------------------------------------------------------------
#include "KS0108.h"
#include "delay.h"
#include "font5x8.h"
#include "display.h"
//-------------------------------------------------------------------------------------------------
// Reference to MCU-specific external functions
//-------------------------------------------------------------------------------------------------
extern void GLCD_InitializePorts(void);
extern void GLCD_WriteData(unsigned char);
extern void GLCD_WriteCommand(unsigned char, unsigned char);
extern unsigned char GLCD_ReadData(void);
extern unsigned char GLCD_ReadStatus(unsigned char);
//-------------------------------------------------------------------------------------------------
unsigned char screen_x = 0, screen_y = 0; // screen coordinates

//-------------------------------------------------------------------------------------------------
// Initialization interface and LCD controller
//-------------------------------------------------------------------------------------------------
extern u8 CurDispNum;

void GLCD_Initialize(void)
{
	unsigned char i;
	unsigned char j;

	GLCD_InitializePorts();
	for(j = 0; j < CABINETNUM; j++)
	{
        CurDispNum = j;
        for(i = 0; i < 3; i++)
            GLCD_WriteCommand((DISPLAY_ON_CMD | ON), i);
	}
}
//-------------------------------------------------------------------------------------------------
// Sets screen coordinates and address 
//-------------------------------------------------------------------------------------------------
void GLCD_GoTo(unsigned char x, unsigned char y)
{
unsigned char i;
screen_x = x;
screen_y = y;

for(i = 0; i < KS0108_SCREEN_WIDTH/64; i++)
{
  GLCD_WriteCommand(DISPLAY_SET_Y | 0,i);
  GLCD_WriteCommand(DISPLAY_SET_X | y,i);
  GLCD_WriteCommand(DISPLAY_START_LINE | 0,i);
}
GLCD_WriteCommand((DISPLAY_SET_Y | (x % 64)), (x / 64));
GLCD_WriteCommand((DISPLAY_SET_X | y), (x / 64));
}
//-------------------------------------------------------------------------------------------------
// Sets text coordinates and address 
//-------------------------------------------------------------------------------------------------
void GLCD_TextGoTo(unsigned char x, unsigned char y)
{
GLCD_GoTo(x*6,y);
}
//-------------------------------------------------------------------------------------------------
// Clears screen
//-------------------------------------------------------------------------------------------------
void GLCD_ClearScreen(void)
{
unsigned char i, j;
for(j = 0; j < KS0108_SCREEN_HEIGHT/8; j++)
	{
	GLCD_GoTo(0,j);
	for(i = 0; i < KS0108_SCREEN_WIDTH; i++)
 	{
	GLCD_WriteData(0x00);
	delay_us(1);
	}
	}
}
u16 GLCD_change(u8 x1,u8 x2)
{
    u16 x =0;
    x = ((u16)x1 << 8) | x2;
    x = x>>2;
    return x;
}

#if 0
//-------------------------------------------------------------------------------------------------
// Writes char to screen memory
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar(char charToWrite)
{
char i;
//char *pchar = 0;
charToWrite -= 32;
//pchar = (char*)(asc2_1206[0] + charToWrite);

for(i = 0; i < 5; i++) 
  GLCD_WriteData(GLCD_ReadByteFromROMMemory((char *)(font5x8 + (5 * charToWrite) + i))); 
  
  //GLCD_WriteData(GLCD_ReadByteFromROMMemory((char *)(pchar + i))); 
  GLCD_WriteData(0x00);
}


void GLCD_WriteLong(unsigned int intTowrite)
{
unsigned int Data_val[10]={0};
unsigned int i=0;
while(intTowrite/10!=0)
{
Data_val[i++]=intTowrite%10;
intTowrite=intTowrite/10;
delay_us(10);
}
Data_val[i++]=intTowrite%10;
while(i--)
{
  GLCD_WriteChar(Data_val[i]+0x30);
  delay_us(10);
}
}

void GLCD_Writefloat( float intTowrite)
{
unsigned int Data_val[10]={0};
unsigned int i=0,floa=0,in=0;
in=(intTowrite*100)/100;
floa=intTowrite*100;
floa=floa%100;
while(in/10!=0)
{
Data_val[i++]=in%10;
in=in/10;
delay_us(10);
}
Data_val[i++]=in%10;
while(i--)
{
  GLCD_WriteChar(Data_val[i]+0x30);
  delay_us(10);
}
GLCD_WriteChar('.');
while(floa/10!=0)
{
Data_val[i++]=floa%10;
floa=floa/10;
delay_us(10);
}
Data_val[i++]=floa%10;
while(i--)
{
  GLCD_WriteChar(Data_val[i]+0x30);
  delay_us(10);
}



}
#endif
void GLCD_WriteChar16x8(char charToWrite,unsigned char x, unsigned char y)
{
    char i;
    char *pchar = 0;
    unsigned char dy,dx;

    unsigned char  j;
    u16 c = 0;
    //unsigned char xxx [] ={0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xCC,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00};
    charToWrite -= ' ';
    if(charToWrite > sizeof(asc2_1608)/sizeof(asc2_1608[0]))
    {
        charToWrite = sizeof(asc2_1608)/sizeof(asc2_1608[0])-1;
    }
    pchar = (char*)(asc2_1608[charToWrite]);
    dy = 16;
    dx = 8;
  
    for(j = 0; j < dy / 8; j++)
    {
        GLCD_GoTo(x,y + j);
        //GLCD_TextGoTo(x,y + j);
        for(i = 0; i < dx; i++) 
        {
            c = GLCD_change(*(pchar+i+8),*(pchar+i));
            //GLCD_WriteData(c);//*(pchar+ i*2+j)
            if(j == 1)
            {
               c = c >> 8;
            }
            GLCD_WriteData((u8)c);
        }
    }



  
  //GLCD_WriteData(0x00);
}


//-------------------------------------------------------------------------------------------------
// Write null-terminated string to screen memory
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString(char * stringToWrite)
{
    unsigned char y = screen_y;
    
    while(*stringToWrite)
    {
      //GLCD_WriteChar(*stringToWrite++);
      GLCD_WriteChar16x8(*stringToWrite++,screen_x,y);
    }
    
}
//-------------------------------------------------------------------------------------------------
// Sets or clears pixel at (x,y)
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
unsigned char tmp;
GLCD_GoTo(x, y/8);
tmp = GLCD_ReadData();
GLCD_GoTo(x, y/8);
tmp = GLCD_ReadData();
GLCD_GoTo(x, y/8);
if(color)
	tmp |= (1 << (y%8));
else
	tmp &= ~(1 << (y%8));
GLCD_WriteData(tmp);
}
//-------------------------------------------------------------------------------------------------
// Displays bitmap at (x,y) and (dx,dy) size
//-------------------------------------------------------------------------------------------------
void GLCD_Bitmap(char * bmp, unsigned char x, unsigned char y, unsigned char dx, unsigned char dy)
{
    unsigned char i, j;
    for(j = 0; j < dy / 8; j++)
    {
        GLCD_GoTo(x,y + j);
        for(i = 0; i < dx; i++) 
        GLCD_WriteData(GLCD_ReadByteFromROMMemory(bmp++));
    }
}
//-------------------------------------------------------------------------------------------------
// End of file KS0108.c
//-------------------------------------------------------------------------------------------------





