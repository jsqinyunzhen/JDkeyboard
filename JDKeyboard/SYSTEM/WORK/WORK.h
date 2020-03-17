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
	STATE_KEYON,//等待按键
	STATE_WAITCARD1,//等待刷卡
	STATE_WAITCARD1_end,//第一次刷卡完成
	STATE_WAITCARD2, //等待第二次刷卡
	STATE_FINISH, //刷卡完成
}keyboard_mode;

void kb_mode_process(void);
void kb_SetMode(keyboard_mode  m);
keyboard_mode  kb_getMode(void);

extern keyboard_mode kb_mode;

#endif

