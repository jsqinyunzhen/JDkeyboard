#include <string.h>

//#include "include.h"

//#include "MKL_ISR.h"


#include "project_config.h"



#ifndef __MOD_DATE_TIME_H__
#define __MOD_DATE_TIME_H__

#define APP_NO_ERROR  0

typedef enum time_error_e
{
    APP_ERROR_TIMER_OK              = 10,
    APP_ERROR_TIMER_PARAM          ,
    APP_ERROR_TIMER_PRIORITY,
    APP_ERROR_TIMER_START,
    APP_ERROR_TIMER_GET_ID,
    APP_ERROR_TIMER_END
}   time_error_t;

typedef enum timing_mode_e
{
    TIMING_MODE_OFF,
    TIMING_MODE_ONCE,
    TIMING_MODE_DAILY
}   timing_mode_t;


typedef enum gmt_usage_e
{
    GMT_USAGE_ON,
    GMT_USAGE_OFF
}   gmt_usage_t;





typedef enum time_type_e
{
    ONCE_TIME,
    PERIOD_TIME
}    time_type_t;



typedef enum time_run_mode_e
{
    TIME_RUN_MODE_ISR,
    TIME_RUN_MODE_TASK
}   time_run_mode_t;


/*

typedef struct time_s
{
    u32 year    : 12;
    u32 month   : 4;
    u32 day     : 5;
    u32 hour    : 5;
    u32 minute  : 6;
} com_time_t;


typedef struct time_hm_s
{
    u8          hour;
    u8          minute;
} time_hm_t;


*/



void        time_list_flag_start( void );

void        time_list_flag_stop( void );

unsigned char          time_list_flag_query( void );






#endif/*__MOD_DATE_TIME_H__*/

