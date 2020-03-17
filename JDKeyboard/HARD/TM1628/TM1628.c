#include "tm1628.h"


/*************************************
函数名称：Tm1628_Write_Bit
函数说明：写单个字节
函数参数：命令参数
函数返回：NULL
https://wenku.baidu.com/view/28badf3da4e9856a561252d380eb6294dd8822ce.html
*************************************/
void Tm1628_Write_Bit(uint8_t data)
{
    uint8_t i;
    Tm1628_CKL_H;
    Tm1628_DIO_H;
    for(i = 0; i < 8; i++)
    {

        Tm1628_CKL_L;
        delay_us(100);
        if((data & 0x01) == 1)
        Tm1628_DIO_H;
        else
        Tm1628_DIO_L;
        data = data	>> 1;
        Tm1628_CKL_H;
        delay_us(100);
    }	
}

/*************************************
函数名称：Tm1628_Write_Command
函数说明：写命令
函数参数：命令参数
函数返回：NULL
*************************************/
void Tm1628_Write_Command(u8 unm)
{
Tm1628_STB_L;
delay_ms(1);
Tm1628_Write_Bit(unm);
Tm1628_STB_H;
delay_ms(1);	
}


/*************************************
函数名称：Tm1628init
函数说明：TM1628初始化
函数参数：NULL
函数返回：NULL
*************************************/
void Tm1628init(void)
{
GPIO_InitTypeDef ?GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;	//LB6-->PB8 端口配置
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	//IO口速度为10MHz
GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB6 - 8
Tm1628_Write_Command(0x03);//7位10段
Tm1628_ClearDisplay();	//清屏
Tm1628_GrayScale(2);	//亮度2
Tm1628_Fixed(0xc2,0xff);
}


/*************************************
函数名称：Tm1628_Fixed
函数说明：固定写显示
函数参数：(1)data 地址
(2)add	数据
函数返回：NULL
*************************************/
void Tm1628_Fixed(uint8_t data, uint8_t add)
{
Tm1628_Write_Command(0x44);
Tm1628_STB_L;
delay_ms(1);
Tm1628_Write_Bit(data);
Tm1628_Write_Bit(add);
Tm1628_STB_H;
delay_ms(1);
}


/*************************************
函数名称：Tm1628_Continuous
函数说明：连续写显示
函数参数：(1)data 地址 ?从C0开始
(2)num	数据
(3)add ?数量	最大14
函数返回：NULL
*************************************/


void Tm1628_Continuous(uint8_t data, uint8_t num, uint8_t *add)
{
uint8_t i;
Tm1628_Write_Command(0x40);
Tm1628_STB_L;
delay_ms(1);
Tm1628_Write_Bit(data);
for(i = 0; i < num; i++)
{
Tm1628_Write_Bit(add[i]);
}
Tm1628_STB_H;
delay_ms(1);
}
/*************************************
函数名称：Tm1628_ClearDisplay
函数说明：清屏
函数参数：NULL
函数返回：NULL
*************************************/


void Tm1628_ClearDisplay(void)
{
    uint8_t i;
    Tm1628_Write_Command(0x40);
    Tm1628_STB_L;
        delay_ms(1);
        Tm1628_Write_Bit(0xC0);
    for(i = 0; i < 14; i++)
    {
        Tm1628_Write_Bit(0x00);
    }
    Tm1628_STB_H;
    delay_ms(1);
}

/*************************************
函数名称：Tm1628_GrayScale
函数说明：用于亮度调节 0 - 9
函数参数：亮度 0 - 9
函数返回：NULL
*************************************/
void Tm1628_GrayScale(uint8_t data)
{
switch(data)
{
case(0): Tm1628_Write_Command(GrayScale_ON); ?	break;
case(1): Tm1628_Write_Command(GrayScale1); break;
case(2): Tm1628_Write_Command(GrayScale2); break;
case(3): Tm1628_Write_Command(GrayScale3);	break;	
case(4): Tm1628_Write_Command(GrayScale4);	break;
case(5): Tm1628_Write_Command(GrayScale5);	break;
case(6): Tm1628_Write_Command(GrayScale6);	break;
case(7): Tm1628_Write_Command(GrayScale7);	break;
case(8): Tm1628_Write_Command(GrayScale8);	break;
}

}
