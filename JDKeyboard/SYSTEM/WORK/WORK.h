#ifndef _WORK_H
#define _WORK_H

#include "stm32f10x.h"
#include "stdio.h"

void LED_IO_INIT(void);
void TIME4_init(u16 arr,u16 psc);
void TIM4_IRQHandler(void);
u16 STMFLASH_ReadHalfWord(u32 faddr);
void STMFLASH_WriteHalfWord(u32 WriteAddr,u16 WriteData);
void iwdg_init(void);
void usart1_sendPaySuccess(u8* pid_overage,u8 pay);
extern u32 sys_ms_cnt;


typedef enum {
	STATE_WAITKEY= 0 ,
	STATE_KEYON,//�ȴ�����
	STATE_WAITCARD1,//�ȴ�ˢ��
	STATE_WAITCARD1_end,//��һ��ˢ�����
	STATE_WAITCARD2, //�ȴ��ڶ���ˢ��
	STATE_FINISH, //ˢ�����
}keyboard_mode;

void kb_mode_process(void);
void kb_SetMode(keyboard_mode  m);
keyboard_mode  kb_getMode(void);

extern keyboard_mode kb_mode;

#endif

