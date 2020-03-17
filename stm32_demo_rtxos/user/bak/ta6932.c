#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_it.h"

#include "ta6932.h"

#define ta6932_mosi_l()      (GPIO_ResetBits(GPIOB,GPIO_Pin_5))
#define ta6932_mosi_h()      (GPIO_SetBits(GPIOB,GPIO_Pin_5))

#define ta6932_sck_l()      (GPIO_ResetBits(GPIOB,GPIO_Pin_3))
#define ta6932_sck_h()      (GPIO_SetBits(GPIOB,GPIO_Pin_3))


#define STB_IO_REVERT 1
#if (STB_IO_REVERT == 1)


#define ta6932_cs_l_a()      (GPIO_SetBits(GPIOD,GPIO_Pin_3))
#define ta6932_cs_h_a()      (GPIO_ResetBits(GPIOD,GPIO_Pin_3))

#define ta6932_cs_l_b()      (GPIO_SetBits(GPIOD,GPIO_Pin_4))
#define ta6932_cs_h_b()      (GPIO_ResetBits(GPIOD,GPIO_Pin_4))

#define ta6932_cs_l_c()      (GPIO_SetBits(GPIOD,GPIO_Pin_5))
#define ta6932_cs_h_c()      (GPIO_ResetBits(GPIOD,GPIO_Pin_5))

#define ta6932_cs_l_d()      (GPIO_SetBits(GPIOD,GPIO_Pin_6))
#define ta6932_cs_h_d()      (GPIO_ResetBits(GPIOD,GPIO_Pin_6))

#else


#define ta6932_cs_l_a()      (GPIO_ResetBits(GPIOD,GPIO_Pin_3))
#define ta6932_cs_h_a()      (GPIO_SetBits(GPIOD,GPIO_Pin_3))

#define ta6932_cs_l_b()      (GPIO_ResetBits(GPIOD,GPIO_Pin_4))
#define ta6932_cs_h_b()      (GPIO_SetBits(GPIOD,GPIO_Pin_4))

#define ta6932_cs_l_c()      (GPIO_ResetBits(GPIOD,GPIO_Pin_5))
#define ta6932_cs_h_c()      (GPIO_SetBits(GPIOD,GPIO_Pin_5))

#define ta6932_cs_l_d()      (GPIO_ResetBits(GPIOD,GPIO_Pin_6))
#define ta6932_cs_h_d()      (GPIO_SetBits(GPIOD,GPIO_Pin_6))
#endif
#define delay_us Delay_us

Display_num dis_num = {0};
                                            /*0       1       2     3      4     5      6       7      8      9       A    b       c      d       e       f      -       .       */   
const unsigned char  tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40,0xef};
u16 ta6932_number[TA6932_LEDCOUNT] = {0};

void TA6932_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    u8 i = 0;

   // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);

 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
   
    //PB3 ��SPI_SCK PB5 ��SPI1_MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   // GPIO_SetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_5);	////sck h mosi h
	//GPIO_ResetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_5);	//sck h mosi l

    //PD3456 ����Ƭѡ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //GPIO_SetBits(GPIOD, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);	
	//GPIO_ResetBits(GPIOD, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);	

	//TEST
	ta6932_mosi_h();
	ta6932_sck_h();
	//ta6932_mosi_l();
	//ta6932_sck_l();

	ta6932_cs_h_a();
	ta6932_cs_h_b();
	ta6932_cs_h_c();
	ta6932_cs_h_d();


	/*
    for(i = 0; i < TA6932_LEDCOUNT; i++)
    {
        ta6932_number[i] = 0;
    }
    
    TA6932_DisplayAllNumber(ta6932_number);
	*/
}

void TA6932_Write_Byte(unsigned char dat)
{
    unsigned char i;
    
    for(i=0;i<8;i++)
    {
        if(dat&0x01)
        {
            ta6932_sck_l();
            delay_us(10);
            ta6932_mosi_h();
            delay_us(10);
			delay_us(10);
            ta6932_sck_h();
            delay_us(10);
			delay_us(10);
        }
        else
        {
            ta6932_sck_l();
            delay_us(10);
            ta6932_mosi_l();
            delay_us(10);
			delay_us(10);
            ta6932_sck_h();
            delay_us(10);
			delay_us(10);
        }
        dat>>=1;
    }
}

/*��ʾÿ��ta6932��led 
dat ����ʾ�����ݽṹ��20���˿ڵ�����
num ��Ƭѡ��
*/
void TA6932_Display_Number(Display_num *dat,u8 stb)
{
    u8 i =0;
    u8 j =0;
    
    
    switch(stb)
    {
        case 1:
            ta6932_cs_l_a();
            delay_us (10);
            TA6932_Write_Byte(0x40);
            ta6932_cs_h_a();
            ta6932_sck_h();
            delay_us (10);
			
            ta6932_cs_l_a();
            delay_us (10);	
            TA6932_Write_Byte(0xC0);
	        ta6932_sck_h();
            delay_us (10);
			
            for(i=0;i<5;i++)
            {
                for(j=0;j<3;j++)
                {
                	printf("dat[%d][%d]=0x%x \r\n",i,j,dat->num[i][j]);
                    TA6932_Write_Byte(dat->num[i][j]);
                }
            }		
            ta6932_cs_h_a();
            ta6932_sck_h();
            delay_us (10);
            ta6932_cs_l_a();	
            delay_us (10);
            TA6932_Write_Byte(0x8f);
            ta6932_cs_h_a();
            ta6932_sck_h();		
            break;
            
        case 2:
            ta6932_cs_l_b();
            delay_us (10);
            TA6932_Write_Byte(0x40);
            ta6932_cs_h_b();
            ta6932_sck_h();
            delay_us (10);
            ta6932_cs_l_b();
            delay_us (10);	
            TA6932_Write_Byte(0xC0);
            for(i=0;i<5;i++)
            {
                for(j=0;j<3;j++)
                {
                	printf("dat[%d][%d]=0x%x \r\n",i,j,dat->num[i][j]);
                    TA6932_Write_Byte(dat->num[i+5][j]);
                }
            }	
            ta6932_cs_h_b();
            ta6932_sck_h();
            delay_us (10);
            ta6932_cs_l_b();	
            delay_us (10);
            TA6932_Write_Byte(0x8f);
            ta6932_cs_h_b();
            ta6932_sck_h();			
            break;
            
        case 3:
            ta6932_cs_l_c();
            delay_us (10);
            TA6932_Write_Byte(0x40);
            ta6932_cs_h_c();
            ta6932_sck_h();
            delay_us (10);
            ta6932_cs_l_c();
            delay_us (10);	
            TA6932_Write_Byte(0xC0);
            for(i=0;i<5;i++)
            {
                for(j=0;j<3;j++)
                {
                    TA6932_Write_Byte(dat->num[i+10][j]);
                }
            }	
            ta6932_cs_h_c();
            ta6932_sck_h();
            delay_us (10);
            ta6932_cs_l_c();	
            delay_us (10);
            TA6932_Write_Byte(0x8f);
            ta6932_cs_h_c();
            ta6932_sck_h();
        break;
        
        case 4:
            ta6932_cs_l_d();
            delay_us (10);
            TA6932_Write_Byte(0x40);
            ta6932_cs_h_d();
            ta6932_sck_h();
            delay_us (10);
            ta6932_cs_l_d();
            delay_us (10);	
            TA6932_Write_Byte(0xC0);
            for(i=0;i<5;i++)
            {
                for(j=0;j<3;j++)
                {
                    TA6932_Write_Byte(dat->num[i+15][j]);
                }
            }	
            ta6932_cs_h_d();
            ta6932_sck_h();
            delay_us (10);
            ta6932_cs_l_d();	
            delay_us (10);
            TA6932_Write_Byte(0x8f);
            ta6932_cs_h_d();
            ta6932_sck_h();			
            break ;
        
        default :
            break;
    }
}
/*��ʾ����20���˿ڵ����֣�number��һ��u16 number[20]������ */
void TA6932_DisplayAllNumber(u16 *number)
{
    u16 num = 0;
    u16 temp = 0;
    u8 i = 0;
    
    if(number == NULL)
    {
        return ;
    }
    
    memcpy(ta6932_number,number,sizeof(ta6932_number));
    
    for(i = 0; i < TA6932_LEDCOUNT; i++ )
    {
        num = ta6932_number[i];
        if(num == 0xffff)
        {
            dis_num.num[i][0] = tab[16];
            dis_num.num[i][1] = tab[16];
            dis_num.num[i][2] = tab[16];
        }
        else
        {
            /*��*/
            temp = num %1000 / 100;
            dis_num.num[i][0] = tab[temp];
            /*ʮ*/
            temp = num %100 / 10;
            dis_num.num[i][1] = tab[temp];
            /*��*/
            temp = num %10;
            dis_num.num[i][2] = tab[temp];
        }
    }
	if(TA6932_LEDCOUNT == 10)    
   	{
	    TA6932_Display_Number(&dis_num, 1);
	    TA6932_Display_Number(&dis_num, 2);

   	}
   	else if(TA6932_LEDCOUNT == 20)    
   	{
	    TA6932_Display_Number(&dis_num, 1);
	    TA6932_Display_Number(&dis_num, 2);
	    TA6932_Display_Number(&dis_num, 3);
	    TA6932_Display_Number(&dis_num, 4);
   	}
}
/*��ʾĳ���˿ڵ����֣�
port�Ƕ˿ں� [1~20]
number����ʾ���ݣ�������������
*/
void TA6932_DisplayPortNumber(u8 port, u16 number)
{
    if(port < 1 || port > TA6932_LEDCOUNT)
    {
        return;
    }
    port = (port-1)%TA6932_LEDCOUNT;
     
    ta6932_number[port] = number;
    TA6932_DisplayAllNumber(ta6932_number);
}
