#include <stdio.h>
#include <string.h>
#include "ds18b20.h"
#include "stm32f10x.h"
uint8_t serial_1[8]= {0x28,0x2d,0x9a,0xdd,0x02,0x00,0x00,0x3b};
uint8_t serial_2[8]= {0x28,0x3b,0x2b,0xbc,0x02,0x00,0x00,0x4f};
uint8_t serial_3[8]= {0x28,0x00,0x49,0x1b,0x03,0x00,0x00,0x4c}; //序列号，需要根据自己的DS18B20修改，具体读取方法，会有另一篇介绍。

uint8_t serial_4[8]= {0x28,0xff,0xd9,0x4c,0x31,0x17,0x03,0x02};


#define Delay_Us Delay_us
static void DS18B20_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
    GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);
}

static void DS18B20_Mode_IPU(void) //使DS18B20-DATA引脚变为输入模式
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

static void DS18B20_Mode_Out_PP(void) //使DS18B20-DATA引脚变为输出模式
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

static void DS18B20_Rst(void)//主机给从机发送复位脉冲
{
    DS18B20_Mode_Out_PP();
    DS18B20_DATA_OUT(HIGH);//主机在产生复位信号后，需将总线拉高
    Delay_Us(2);
    DS18B20_DATA_OUT(LOW);
    Delay_Us(750);//主机至少产生480us的低电平复位信号
    DS18B20_DATA_OUT(HIGH);//主机在产生复位信号后，需将总线拉高
    //Delay_Us(15);//从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
    Delay_us(60);//15-60us
}

static uint8_t DS18B20_Presence(void) //检测从机给主机返回的存在脉冲
{
    uint8_t pulse_time = 0;

    DS18B20_Mode_IPU(); //主机设置为上拉输入

    //等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号?
    //如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲

    while( DS18B20_DATA_IN() && pulse_time<100 )
    {
        pulse_time++;
        Delay_Us(1);
    }
    if( pulse_time >=100 ) //经过100us后，存在脉冲都还没有到来
        return 1;
    else
        pulse_time = 0;
    while( !DS18B20_DATA_IN() && pulse_time<240 ) //存在脉冲到来，且存在的时间不能超过240us?
    {
        pulse_time++;
        Delay_Us(1);
    }
    if( pulse_time >=240 )
        return 1;
    else
        return 0;
}

static uint8_t DS18B20_Read_Bit(void) //从DS18B20读取一个bit
{
    uint8_t dat;

    DS18B20_Mode_Out_PP(); //读0和读1的时间至少要大于60us
    DS18B20_DATA_OUT(LOW); //读时间的起始：必须由主机产生 >1us <15us 的低电平信号
    Delay_Us(10);
    DS18B20_Mode_IPU(); //设置成输入，释放总线，由外部上拉电阻将总线拉高
    if( DS18B20_DATA_IN() == SET )
        dat = 1;
    else
        dat = 0;
    Delay_Us(45); //这个延时参数请参考时序图

    return dat;
}

uint8_t DS18B20_Read_Byte(void) //从DS18B20读一个字节，低位先行
{
    uint8_t i, j, dat = 0;

    for(i=0; i<8; i++)
    {
        j = DS18B20_Read_Bit();
        dat = (dat) | (j<<i);
    }

    return dat;
}

void DS18B20_Write_Byte(uint8_t dat) //写一个字节到DS18B20，低位先行
{
    uint8_t i, testb;

    DS18B20_Mode_Out_PP();
    for( i=0; i<8; i++ )
    {
        testb = dat&0x01;
        dat = dat>>1;
        if (testb) //写0和写1的时间至少要大于60us
        {
            DS18B20_DATA_OUT(LOW);
            Delay_Us(8);
            DS18B20_DATA_OUT(HIGH);
            Delay_Us(58);
        }
        else
        {
            DS18B20_DATA_OUT(LOW);
            Delay_Us(70);
            DS18B20_DATA_OUT(HIGH);
            Delay_Us(2);
        }
    }
}

void DS18B20_Start(void)
{
    DS18B20_Rst();
    DS18B20_Presence();
    DS18B20_Write_Byte(0XCC); //跳过 ROM ?
    DS18B20_Write_Byte(0X44); //开始转换?
}

uint8_t DS18B20_Init(void)
{
    DS18B20_GPIO_Config();
    DS18B20_Rst();

    return DS18B20_Presence();
}

void DS18B20_Match_Serial(uint8_t a)//匹配序列号
{
    uint8_t i;
    DS18B20_Rst();
    DS18B20_Presence();
    DS18B20_Write_Byte(0X55); //匹配序列号指令
    if(a==1)
    {
        for(i=0; i<8; i++)
            DS18B20_Write_Byte(serial_1[i]);
    }
    else if(a==2)
    {
        for(i=0; i<8; i++)
            DS18B20_Write_Byte(serial_2[i]);
    }
    else if(a==3)
    {
        for(i=0; i<8; i++)
            DS18B20_Write_Byte(serial_3[i]);
    }
    else if(a==4)
    {
        for(i=0; i<8; i++)
            DS18B20_Write_Byte(serial_4[i]);
    }

}


//存储的温度是16 位的带符号扩展的二进制补码形式
//当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位

// ? ? ? ? |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
//低字节 ?| 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |

// ? ? ? ?|-----符号位：0->正 ?1->负-------|-----------整数-----------|
//高字节 ?| ?s ?| ?s ?| ?s ?| ?s ?| ? ?s ? | ? 2^6 ?| ? 2^5 ?| ? 2^4 ?|
//温度 = 符号位 + 整数 + 小数*0.0625


float DS18B20_Get_Temp(uint8_t *a,uint8_t b)
{
    uint8_t tpmsb, tplsb;
    signed short s_tem;
    float f_tem;
    int temp_num;

    DS18B20_Rst();

    tpmsb = DS18B20_Presence();
    printf("tpmsb=%d\r\n",tpmsb);
	if(tpmsb == 1)
		return -100;
	//if(b != 0)
	
    DS18B20_Write_Byte(0XCC); // 跳过 ROM?
    DS18B20_Match_Serial(b); //匹配序列号
    DS18B20_Write_Byte(0X44); // 开始转换?
	
    DS18B20_Rst();
    tpmsb =DS18B20_Presence();
    printf("tpmsb=%d\r\n",tpmsb);
	if(tpmsb == 1)
		return -100;
    DS18B20_Write_Byte(0XCC); //跳过 ROM ?
    DS18B20_Match_Serial(b); //匹配序列号
    DS18B20_Write_Byte(0XBE); //读温度值?

    tplsb = DS18B20_Read_Byte();
    tpmsb = DS18B20_Read_Byte();

    s_tem = tpmsb<<8;
    s_tem = s_tem | tplsb;

    if( s_tem < 0 ) //负温度
    {
        f_tem = (~s_tem+1) * 0.0625;
        temp_num = (~s_tem+1) * 0.0625*10;
        //go_temp= temp_num;
        if(a)
        {
            if(temp_num>=1000)
            {
                a[0]='-';
                a[1]= temp_num/1000+'0';
                a[2]= temp_num%1000/100+'0';
                a[3]= temp_num%100/10+'0';
                a[4]='.';
                a[5]= temp_num%10+'0';
                a[6]= '\0';
            }
            else
            {
                a[0]='-';
                a[1]= temp_num/100+'0';
                a[2]= temp_num%100/10+'0';
                a[3]='.';
                a[4]=temp_num%10+'0';
                a[5]= '\0';
            }
        }
    }
    else
    {
        f_tem = s_tem * 0.0625;
        temp_num = s_tem * 0.0625*10;
        //go_temp= temp_num;
        if(a)
        {

            if(temp_num>=1000)
            {
                a[0]='+';
                a[1]= temp_num/1000+'0';
                a[2]= temp_num%1000/100+'0';
                a[3]= temp_num%100/10+'0';
                a[4]='.';
                a[5]= temp_num%10+'0';
                a[6]= '\0';
            }
            else
            {
                a[0]='+';
                a[1]= temp_num/100+'0';
                a[2]= temp_num%100/10+'0';
                a[3]='.';
                a[4]=temp_num%10+'0';
                a[5]= '\0';
            }
        }
    }
    return f_tem;
}

void read_serial(uint8_t *serial) //读取序列号
{
    uint8_t i;
    DS18B20_Rst();
    DS18B20_Presence();
    DS18B20_Write_Byte(0X33); //读取序列号指令
    for(i=0; i<8; i++)
        serial[i] = DS18B20_Read_Byte();
}


int main_ds18b20_test(void)
{
    //int go_num;

    //DS18B20_GPIO_Config();

    int i = 0;
    uint8_t temp_ds18b20[8]= {0};

    read_serial(temp_ds18b20);
    for(i=0; i<8; i++)
        printf("temp_ds18b20[%d]=0x%02x ",i,temp_ds18b20[i]);
    printf("\r\n");
    //printf("多个DS18B20温度读取实验！\r\n");
    printf("DS18B20_Get_Temp\r\n");
/*

temp_ds18b20[0]=0x28
temp_ds18b20[1]=0xff
temp_ds18b20[2]=0x29
temp_ds18b20[3]=0x69
temp_ds18b20[4]=0x31
temp_ds18b20[5]=0x17
temp_ds18b20[6]=0x03
temp_ds18b20[7]=0x55
tpmsb=0


*/
    memset(temp_ds18b20,0,sizeof(temp_ds18b20));
    DS18B20_Get_Temp(temp_ds18b20,4);

    printf("%s\r\n",temp_ds18b20);

    printf("DS18B20_Get_Temp %f\r\n",DS18B20_Get_Temp(temp_ds18b20,4));
    //Delay_Ms(200);
    return 0;
}
