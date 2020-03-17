#include <string.h>

//#include "include.h"

//#include "MKL_ISR.h"


//#include "c20_board_config.h"


#ifndef _MOD_TIME_LIST_H
#define _MOD_TIME_LIST_H


#include "mod_date_time.h"

/* Exported Types ---------------------------------------------------------- */

/* Common unsigned types */
#ifndef DEFINED_u8
#define DEFINED_u8
typedef unsigned char  u8;
#endif

#ifndef DEFINED_u16
#define DEFINED_u16
typedef unsigned short u16;
#endif

#ifndef DEFINED_u32
#define DEFINED_u32
typedef unsigned int   u32;
#endif

/* Common signed types */
#ifndef DEFINED_s8
#define DEFINED_s8
typedef signed char  s8;
#endif

#ifndef DEFINED_s16
#define DEFINED_s16
typedef signed short s16;
#endif

#ifndef DEFINED_s32
#define DEFINED_s32
typedef signed int   s32;
#endif

typedef unsigned int app_error_t;

#if(S_SW_PERIOD_RECORD_DEBUG == 1)
#define   TIME_UNIT_MAX       (7)        //时间单元最大数值 
#else
#define   TIME_UNIT_MAX       (6)        //时间单元最大数值 
#endif
//#define   TIME_UNIT_MAX       (5)        //时间单元最大数值 

#define     TIME_CTR2_SECOND    (1000)
#define     TIME_CTR2_MINUTE    (60*TIME_CTR2_SECOND)

#ifdef TIMER_LOW_MEM
//Program Size: Code=15116 RO-data=940 RW-data=116 ZI-data=1372  

//typedef void (*FunType)(int ); //②. 定义一个函数指针类型FunType,与①函数类型一至

typedef void (*FunType)( void );
typedef struct time_unit_s
{
  //  void( * pfun )( void );
    FunType pfun; 
    u8             is_register; //注册标识
    u8             is_start;  //有效标识
    u8             time_id;  //定时标识
    u8             priority;
    u32             interval;
    u32             curvalue;
    time_type_t     type;  //类型是定时或超时
    time_run_mode_t run_mode;
} time_unit_t;



app_error_t     time_list_init( void );   // 定时列表初始化
//app_error_t     time_register( void* pfun, u32 uinterval, u8 priority,time_type_t type, time_run_mode_t fun_run_mode,u8* id );  //注册一个定时单元
app_error_t time_register(  FunType pfun, u32  uinterval,u8 priority, time_type_t       type,time_run_mode_t   fun_run_mode, u8* id );
app_error_t     time_start( u8 id );  //使能一个定时单元
app_error_t     time_stop( u8 id );  //使不能一个定时单元
//app_error_t     time_restart( u16 id );
app_error_t time_restart( u8 id,u32 new_period);
unsigned char time_status_get( u8 id);
unsigned int time_period_get_by_id( u8 id);

app_error_t     time_isr_query( void );
app_error_t     time_task_query( void ); 
app_error_t     time_release( u8 id );
app_error_t     time_get_id( void* pfun, u8* id );
#else
//Program Size: Code=15116 RO-data=940 RW-data=124 ZI-data=1396  
typedef struct time_unit_s
{
    void( * pfun )( void );
    u16             is_register; //注册标识
    u16             is_start;  //有效标识
    u16             time_id;  //定时标识
    u16             priority;
    u32             interval;
    u32             curvalue;
    time_type_t     type;  //类型是定时或超时
    time_run_mode_t run_mode;
} time_unit_t;



app_error_t     time_list_init( void );   // 定时列表初始化
app_error_t     time_register( void* pfun, u32 uinterval, u16 priority,
                               time_type_t type, time_run_mode_t fun_run_mode,
                               u16* id );  //注册一个定时单元
app_error_t     time_start( u16 id );  //使能一个定时单元
app_error_t     time_stop( u16 id );  //使不能一个定时单元
//app_error_t     time_restart( u16 id );
app_error_t time_restart( u16 id,u32 new_period);
app_error_t     time_isr_query( void );
app_error_t     time_task_query( void ); 
app_error_t     time_release( u16 id );
app_error_t     time_get_id( void* pfun, u16* id );
#endif
void gxos_interrupt_enable(void);
void  gxos_interrupt_disable(void);

#endif

