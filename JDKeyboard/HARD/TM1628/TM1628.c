#include "tm1628.h"


/*************************************
�������ƣ�Tm1628_Write_Bit
����˵����д�����ֽ�
�����������������
�������أ�NULL
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
�������ƣ�Tm1628_Write_Command
����˵����д����
�����������������
�������أ�NULL
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
�������ƣ�Tm1628init
����˵����TM1628��ʼ��
����������NULL
�������أ�NULL
*************************************/
void Tm1628init(void)
{
GPIO_InitTypeDef ?GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;	//LB6-->PB8 �˿�����
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	//IO���ٶ�Ϊ10MHz
GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB6 - 8
Tm1628_Write_Command(0x03);//7λ10��
Tm1628_ClearDisplay();	//����
Tm1628_GrayScale(2);	//����2
Tm1628_Fixed(0xc2,0xff);
}


/*************************************
�������ƣ�Tm1628_Fixed
����˵�����̶�д��ʾ
����������(1)data ��ַ
(2)add	����
�������أ�NULL
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
�������ƣ�Tm1628_Continuous
����˵��������д��ʾ
����������(1)data ��ַ ?��C0��ʼ
(2)num	����
(3)add ?����	���14
�������أ�NULL
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
�������ƣ�Tm1628_ClearDisplay
����˵��������
����������NULL
�������أ�NULL
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
�������ƣ�Tm1628_GrayScale
����˵�����������ȵ��� 0 - 9
�������������� 0 - 9
�������أ�NULL
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
