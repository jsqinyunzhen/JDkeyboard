
/****************************************************************************************
* 版权所有 (C)2015,杭州海康威视数字技术股份有限公司。
* 文件名称  : main.c
* 内容摘要  : main file
* 作    者  : xushixiong
* 创建日期  : 2016/06/08
****************************************************************************************/

/* 包含头文件 *****************************************************************/


#include <stdarg.h>  
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <ctype.h>




#ifndef __ST__FLASH__
#define __ST__FLASH__

//#define BufferSize     10
/* 变量 ----------------------------------------------------------------------*/
//static __IO uint32_t TimingDelay;

uint16_t IAP_ReadFlag(void);
void IAP_WriteFlag(uint16_t flag);




#endif

