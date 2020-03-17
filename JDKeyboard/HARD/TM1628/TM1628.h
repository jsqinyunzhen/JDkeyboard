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




#define GrayScale_OFF 0x80 //����ʾ
#define GrayScale_ON 0x81 //����ʾ

#define GrayScale1 0x88 //�Ҷȵȼ�1
#define GrayScale2 0x89 //�Ҷȵȼ�2
#define GrayScale3 0x8A //�Ҷȵȼ�3
#define GrayScale4 0x8B //�Ҷȵȼ�4
#define GrayScale5 0x8C //�Ҷȵȼ�5
#define GrayScale6 0x8D //�Ҷȵȼ�6
#define GrayScale7 0x8E //�Ҷȵȼ�7
#define GrayScale8 0x8F //�Ҷȵȼ�8


void Tm1628init(void);//TM1628��ʼ��
void Tm1628_Fixed(uint8_t data, uint8_t add);//�̶�д��ʾ data ��ַ add	����
void Tm1628_Continuous(uint8_t data, uint8_t num, uint8_t *add);//����д��ʾ
void Tm1628_ClearDisplay(void);//����
void Tm1628_GrayScale(uint8_t data);//���ȵ���
void TM1628_init(void) ;
void KEY_Scan(void) ;
u16 keydata_getcrc(u8 * buf,u8 len);

#endif
