#ifndef __TM1628_H
#define __TM1628_H


#include "sys.h"
#include "delay.h"


#define Tm1628_CKL_H GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET)
#define Tm1628_CKL_L GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET)


#define Tm1628_DIO_H GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_SET)
#define Tm1628_DIO_L GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET)


#define Tm1628_STB_H GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET)
#define Tm1628_STB_L GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET)




#define GrayScale_OFF 0x80 //关显示
#define GrayScale_ON 0x81 //开显示

#define GrayScale1 0x88 //灰度等级1
#define GrayScale2 0x89 //灰度等级2
#define GrayScale3 0x8A //灰度等级3
#define GrayScale4 0x8B //灰度等级4
#define GrayScale5 0x8C //灰度等级5
#define GrayScale6 0x8D //灰度等级6
#define GrayScale7 0x8E //灰度等级7
#define GrayScale8 0x8F //灰度等级8


void Tm1628init(void);//TM1628初始化
void Tm1628_Fixed(uint8_t data, uint8_t add);//固定写显示 data 地址 add	数据
void Tm1628_Continuous(uint8_t data, uint8_t num, uint8_t *add);//连续写显示
void Tm1628_ClearDisplay(void);//清屏
void Tm1628_GrayScale(uint8_t data);//亮度调节
void TM1628_init(void) ;
void KEY_Scan(void) ;
u16 keydata_getcrc(u8 * buf,u8 len);

#endif
