#include "project_config.h"

#include "mod_date_time.h"

#include "mod_time_list.h"

static u8   s_time_list_query   = 0;



void time_list_flag_start( void )
{
    s_time_list_query = 1;
    return;
}

void time_list_flag_stop( void )
{
    s_time_list_query = 0;
    return;
}

u8 time_list_flag_query( void )
{
    return s_time_list_query;
}

