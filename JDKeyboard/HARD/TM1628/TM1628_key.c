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

//������ƶ˿�

//sbit DIO =P2^0; 
//sbit CLK =P2^1; 
//sbit STB =P2^2; 

#define DIO PBout(7)
#define CLK PBout(6)
#define STB PBout(5)


//��������

uchar const CODE[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0xef,0x6f}; //���������0-9�ı���

uchar KEY[5]={0};  //Ϊ�洢����ֵ��������
u8 gKeyPress = 0;
u8 gKeyPress2 = 0;
u8 gKeyRelease = 1; 
u8 gKeyOn = 0; 
extern uint8_t gDevIsSlave ;
keydata uart1_tx = {0};
u8 testkey = 0;
//��TM1628����8λ���ݴӵ�λ��ʼ------------------------ 

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

 

//��TM1628��������-------------------------------------- 

static void command(uchar com) 

{ 

  STB=1; 

  STB=0; 

  TM1628_send_8bit(com); 

} 

void TM1628_init(void) 
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	//LB6-->PB8 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ10MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB6 - 8
    
    STB = 1;
    STB = 1;
    STB = 1;
}

//��ȡ����ֵ������KEY[]���飬�ӵ��ֽڿ�ʼ���ӵ�λ��ʼ---

void read_key() 

{ 

  uchar i,j; 

  command(0x42);  //����������

  DIO=1;      //��DIO�ø�  

  for(j=0;j<5;j++)//������ȡ5���ֽ� 

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

 

//��ʾ����,1-7λ�������ʾ����0-6-------------------------------------------------------- 
#if 0
static void display() 

{ 

  uchar i; 

  command(0x03);      //������ʾģʽ��7λ10��ģʽ 
  command(0x40);      //������������,���õ�ַ�Զ���1ģʽ 
  command(0xc0);      //������ʾ��ַ����00H��ʼ 

  for(i=0;i<7;i++)     //������ʾ���� 
  { 
    TM1628_send_8bit(CODE[i]);      //��00H��ż����ַ����ʾ���� 
    TM1628_send_8bit(0);  //��ΪSEG9-14��δ�õ�������������ַ��ȫ��0��  
  } 

  command(0x8F);      //��ʾ�����������ʾ������Ϊ���� 

  //read_key();       //������ֵ 

  STB=1; 

} 

 #endif

//����������------------------------------------------------- 
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
        
        if(gKeyPress2 != 0 && 0 != gKeyPress &&gKeyPress !=  gKeyPress2)//������
        {
        	gKeyOn = 0;
           	gKeyRelease = 1;
           	gKeyPress2 = 0;
        }
	//gKeyOn == 0 &&
	if( gKeyPress == gKeyPress2 && gKeyPress != 0 && gKeyRelease != 0) //����һ�ΰ�����������ִ��
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
		else  if(kb_getMode() != STATE_WAITKEY)//�ڶ��ΰ�����ȡ������
		{
		      	kb_SetMode(STATE_FINISH);
		}
	}
    }

} 

 /*

//������------------------------------------------------------- 

void main() 

{ 

  display();  //��ʾ 

  while(1) 

  { 

    read_key();      //������ֵ 

    KEY_Scan();    //��������    

  } 

} 
*/
