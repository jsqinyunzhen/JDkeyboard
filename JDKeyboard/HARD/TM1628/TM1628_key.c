/*
https://wenku.baidu.com/view/00ede55dfad6195f312ba658.html
 */ 
#include "SYS.h"
#include "TM1628.h"
#include "USART.h"
#include "WORK.h"
#include "stdio.h"



#define uchar unsigned char 
#define uint  unsigned int 

//定义控制端口

//sbit DIO =P2^0; 
//sbit CLK =P2^1; 
//sbit STB =P2^2; 

#define DIO PBout(7)
#define CLK PBout(6)
#define STB PBout(5)


//定义数据

uchar const CODE[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0xef,0x6f}; //共阴数码管0-9的编码

uchar KEY[5]={0};  //为存储按键值开辟数组
u8 gKeyPress = 0;
u8 gKeyPress2 = 0;
u8 gKeyRelease = 1; 
u8 gKeyOn = 0; 
extern uint8_t gDevIsSlave ;
keydata uart1_tx = {0};
u8 testkey = 0;
//向TM1628发送8位数据从低位开始------------------------ 

void TM1628_send_8bit(uchar dat) 

{ 

    uchar i; 

    for(i=0;i<8;i++) 
    { 
        if(dat&0x01) 
        {
            DIO=1; 
        }
        else 
        {
            DIO=0; 
        }

    CLK=0; 
    CLK=1; 

    dat=dat>>1; 

    } 

} 

 

//向TM1628发送命令-------------------------------------- 

static void command(uchar com) 

{ 

  STB=1; 

  STB=0; 

  TM1628_send_8bit(com); 

} 

void TM1628_init(void) 
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	//LB6-->PB8 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为10MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB6 - 8
    
    STB = 1;
    STB = 1;
    STB = 1;
}

//读取按键值并存入KEY[]数组，从低字节开始，从低位开始---

void read_key() 

{ 

  uchar i,j; 

  command(0x42);  //读键盘命令

  DIO=1;      //将DIO置高  

  for(j=0;j<5;j++)//连续读取5个字节 

    for(i=0;i<8;i++)  
    { 
        KEY[j]=KEY[j]>>1; 
        CLK=0; 
        CLK=1; 
        if(DIO) 
        KEY[j]=KEY[j]|0X80; 
    } 

  STB=1; 
  if(PAin(15) == 0)
  {
    KEY[1]  = 1;
  }
} 

 

//显示函数,1-7位数码管显示数字0-6-------------------------------------------------------- 
#if 0
static void display() 

{ 

  uchar i; 

  command(0x03);      //设置显示模式，7位10段模式 
  command(0x40);      //设置数据命令,采用地址自动加1模式 
  command(0xc0);      //设置显示地址，从00H开始 

  for(i=0;i<7;i++)     //发送显示数据 
  { 
    TM1628_send_8bit(CODE[i]);      //从00H起，偶数地址送显示数据 
    TM1628_send_8bit(0);  //因为SEG9-14均未用到，所以奇数地址送全“0”  
  } 

  command(0x8F);      //显示控制命令，打开显示并设置为最亮 

  //read_key();       //读按键值 

  STB=1; 

} 

 #endif

//按键处理函数------------------------------------------------- 
extern uint8_t gDevIsSlave;
extern u32 sys_ms_cnt ;
u16 keydata_getcrc(u8 * buf,u8 len)
{
	u8 i =0;
	u16 crc = 0;
	for(i = 0; i < len; i++)
	{
			crc += buf[i];
	}
	return crc;
}

void KEY_Scan(void) 
{ 
    static u32 keyProcessWait = 0;

    if(sys_ms_cnt-keyProcessWait > 10)
    {
        keyProcessWait = sys_ms_cnt;
        read_key();
        gKeyPress2 = gKeyPress;
        gKeyPress = 0;
        switch(KEY[0])
        {
            case 0x01:gKeyPress = 1;break;
            //case 0x02:gKeyPress = 2;break;
            case 0x08:gKeyPress = 2;break;
           // case 0x10:gKeyPress = 4;break;
        }
        switch(KEY[1])
        {
            case 0x01:gKeyPress = 3;break;
            //case 0x02:gKeyPress = 6;break;
            case 0x08:gKeyPress = 4;break;
            //case 0x10:gKeyPress = 8;break;
        }
        switch(KEY[2])
        {
            case 0x01:gKeyPress = 5;break;
            //case 0x02:gKeyPress = 10;break;
            case 0x08:gKeyPress = 6;break;
            //case 0x10:gKeyPress = 12;break;
        }
        switch(KEY[3])
        {
            case 0x01:gKeyPress = 7;break;
            //case 0x02:gKeyPress = 14;break;
            case 0x08:gKeyPress = 8;break;
            //case 0x10:gKeyPress = 16;break;
        }
        switch(KEY[4])
        {
            case 0x01:gKeyPress = 9;break;
            //case 0x02:gKeyPress = 18;break;
            case 0x08:gKeyPress = 10;break;
           // case 0x10:gKeyPress = 20;break;
        }
        
	 if(gKeyPress2 != 0 && 0 == gKeyPress ) 
        {
					 gKeyRelease = gKeyPress2;
            gKeyPress2 = 0;
            printf("%d KeyRelease \r\n",gKeyRelease);
        }
        
        if(gKeyPress2 != 0 && 0 != gKeyPress &&gKeyPress !=  gKeyPress2)//检测错误
        {
        	gKeyOn = 0;
           	gKeyRelease = 1;
           	gKeyPress2 = 0;
        }
	//gKeyOn == 0 &&
	if( gKeyPress == gKeyPress2 && gKeyPress != 0 && gKeyRelease != 0) //按下一次按键，长按不执行
	{

		gKeyOn = gKeyPress;
		if(gDevIsSlave)
		{
			gKeyOn +=10;
		}
		printf("%d gKeyPress,%d  KeyOn, kb mode = %d\r\n",gKeyPress,gKeyOn,kb_getMode());
		gKeyRelease = 0;
		
		if(kb_getMode() == STATE_WAITKEY)
		{//0x7A	 CMD	 LEN	 DATA	CHECKSUM	 0x67
			u16 crc =0x0;

			uart1_tx.f.head =0x7A;
			uart1_tx.f.cmd= 0x01;
			uart1_tx.f.len = 0x02;
			uart1_tx.f.data[0] = gKeyOn;
			uart1_tx.f.data[1] = 0x00;

			crc  = keydata_getcrc(uart1_tx.buf+1,uart1_tx.f.len+2);
			uart1_tx.f.crc1 = crc&0xff;
			uart1_tx.f.crc2 = (crc>>8)&0xff;
			uart1_tx.f.end = 0x67;
			memcpy(uart1_tx.buf+(uart1_tx.f.len+3),uart1_tx.buf+(sizeof(uart1_tx.buf)-3),3);
			usart1_send(uart1_tx.buf,uart1_tx.f.len+6);
			kb_SetMode(STATE_KEYON);

		}
		else  if(kb_getMode() != STATE_WAITKEY)//第二次按键是取消按键
		{
		      	kb_SetMode(STATE_FINISH);
		}
	}
    }

} 

 /*

//主函数------------------------------------------------------- 

void main() 

{ 

  display();  //显示 

  while(1) 

  { 

    read_key();      //读按键值 

    KEY_Scan();    //按键处理    

  } 

} 
*/
