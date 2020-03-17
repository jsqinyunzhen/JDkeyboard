#include "project_config.h"
#include "mod_date_time.h"

#include "mod_time_list.h"
//#include "kl_interrupt.h"

#include "stm32f10x_it.h"
time_unit_t g_Time_list[TIME_UNIT_MAX];


#define SYS_PRINTF uart_printf
void gxos_interrupt_enable(void)
{
	// __enable_irq();
	return;
}

void  gxos_interrupt_disable(void)
{
	// __disable_irq();
	return;
}


/*****************************************************************************
 * Function    : GetControlBlockFromHandle
 
 * Description : 
    get the counter control block whose handle equals to the argument
    
 * Arguments   : 
    [IN]  GXCTR_Handle_t    Handle  Counter device handle
    
 * Returns     : 
    GXCTR_ControlBlock_t*     A pointer of Counter control block
    
 * Other       :  Nothing
 ****************************************************************************/

//定时单元列表初始化
app_error_t time_list_init( void )
{
    u8  i;

    for( i = 0; i < TIME_UNIT_MAX; i++ )
    {
        g_Time_list[i].is_register = 0; 
        g_Time_list[i].is_start = 0;
    }

    return APP_ERROR_TIMER_OK;
}



//注册一个定时单元
#ifdef TIMER_LOW_MEM
//app_error_t time_register( void* pfun, u32  uinterval,u8 priority, time_type_t       type,time_run_mode_t   fun_run_mode, u8* id )
app_error_t time_register(  FunType pfun, u32  uinterval,u8 priority, time_type_t       type,time_run_mode_t   fun_run_mode, u8* id )
{
    u8 node;
    if( uinterval == 0 || pfun == NULL )
    {
      //  SYS_PRINTF( "time_register----error params\n" );
        return APP_ERROR_TIMER_PARAM;
    }

    node = priority;
    if( priority >= TIME_UNIT_MAX )
    {
        //SYS_PRINTF( "timer unit overflow\n" );
        return APP_ERROR_TIMER_PRIORITY;
    }

    gxos_interrupt_disable();


    for( node = 0; node < TIME_UNIT_MAX; node ++ )
    {
        if( g_Time_list[node].is_register == 0 )
        {
            break;
        }
    }

    if( TIME_UNIT_MAX == node )
    {
        gxos_interrupt_enable();  
        //SYS_PRINTF( "timer--unit overflow2\n" );
        return APP_ERROR_TIMER_PRIORITY;
    }
    g_Time_list[node].priority = priority;
    g_Time_list[node].time_id = node;
    g_Time_list[node].pfun = pfun;
    g_Time_list[node].interval = uinterval;
    g_Time_list[node].curvalue = uinterval;
    g_Time_list[node].is_register = 1;
    g_Time_list[node].type = type;
    g_Time_list[node].run_mode = fun_run_mode;
    *id = g_Time_list[node].time_id;

    gxos_interrupt_enable();  

    return APP_ERROR_TIMER_OK;
}
#else
app_error_t time_register( void* pfun, u32               uinterval,u16 priority, time_type_t       type,time_run_mode_t   fun_run_mode, u16* id )
{
    u16 node;
    if( uinterval == 0 || pfun == NULL )
    {
      //  SYS_PRINTF( "time_register----error params\n" );
        return APP_ERROR_TIMER_PARAM;
    }

    node = priority;
    if( priority >= TIME_UNIT_MAX )
    {
        //SYS_PRINTF( "timer---unit overflow\n" );
        return APP_ERROR_TIMER_PRIORITY;
    }

    gxos_interrupt_disable();


    for( node = 0; node < TIME_UNIT_MAX; node ++ )
    {
        if( g_Time_list[node].is_register == 0 )
        {
            break;
        }
    }

    if( TIME_UNIT_MAX == node )
    {
        gxos_interrupt_enable();  
        //SYS_PRINTF( "timer-unit overflow2\n" );
        return APP_ERROR_TIMER_PRIORITY;
    }


    g_Time_list[node].priority = priority;
    g_Time_list[node].time_id = node;
    g_Time_list[node].pfun = pfun;
    g_Time_list[node].interval = uinterval;
    g_Time_list[node].curvalue = uinterval;
    g_Time_list[node].is_register = 1;
    g_Time_list[node].type = type;
    g_Time_list[node].run_mode = fun_run_mode;
    *id = g_Time_list[node].time_id;

    gxos_interrupt_enable();  

    return APP_ERROR_TIMER_OK;
}
#endif




//得到一个定时单元的标识


#ifdef TIMER_LOW_MEM
app_error_t time_get_id( void* pfun, u8* id )
{
    u8 node;
    gxos_interrupt_disable();
    node = 0;
    while( g_Time_list[node].pfun != pfun && node < TIME_UNIT_MAX )
    {
        node++;
    }
    if( node >= TIME_UNIT_MAX )
    {
        gxos_interrupt_enable();
      //  SYS_PRINTF( "time_get_id----cant find pfun\n" );
        return APP_ERROR_TIMER_GET_ID;
    }
    else
    {
        *id = g_Time_list[node].time_id;

        gxos_interrupt_enable();
        return APP_ERROR_TIMER_OK;
    }
//    gxos_interrupt_enable();
}
app_error_t time_start( u8 id )
{
    gxos_interrupt_disable(); 

    if( g_Time_list[id].is_register == 1 )
    {
        g_Time_list[id].is_start = 1; 
        gxos_interrupt_enable();    
        return APP_ERROR_TIMER_OK;
    }
    else
    {
        gxos_interrupt_enable();    
        //SYS_PRINTF( "time_start----%d is not registered\n",id );
        return APP_ERROR_TIMER_START;
    }
//    gxos_interrupt_enable();
}

//使一个定时单元无效
app_error_t time_stop( u8 id )
{
//	if(timer_r_ctrl ==id)
	//{
//led off
	//	mcu_led_onoff(LED_R_PIN,GPIO_LOW);
	//	mcu_led_onoff(LED_B_PIN,GPIO_LOW);
	//}
    gxos_interrupt_disable();
    if( g_Time_list[id].is_start == 1 )
    {
        g_Time_list[id].is_start = 0;
        gxos_interrupt_enable();
        return APP_ERROR_TIMER_OK;
    }
    else
    {
        gxos_interrupt_enable();
      //  SYS_PRINTF( "time_stop----id %d is not start\n" ,id);
        return APP_ERROR_TIMER_END;
    } 
//    gxos_interrupt_enable();
}

//使一个定时单元无效
app_error_t time_restart( u8 id,u32 new_period)
{
    gxos_interrupt_disable();
    if(1)// g_Time_list[id].is_start == 1 )
    {
	g_Time_list[id].is_start = 1;
	g_Time_list[id].curvalue = new_period;//g_Time_list[id].interval ;
	g_Time_list[id].interval  = new_period;
	  gxos_interrupt_enable();
	// SYS_PRINTF( "time_restart %d ms \n" ,g_Time_list[id].curvalue);
	 uart_printf("event restart  tick=%d\n",get_curtime()); 
      
        return APP_ERROR_TIMER_OK;
    }
    else
    {
    //    gxos_interrupt_enable();
     //   SYS_PRINTF( "time_restart----id %d is not registered\n" ,id);
        return APP_ERROR_TIMER_END;
    } 
   // gxos_interrupt_enable();
}
unsigned char time_status_get( u8 id)
{

	return g_Time_list[id].is_start ;
}

unsigned int time_period_get_by_id( u8 id)
{

	return g_Time_list[id].interval ;
}

//isr轮询
app_error_t time_isr_query( void )
{
    u8          node;
    time_unit_t*cur_node    = NULL;

    if( 1 != time_list_flag_query() )
    {
        return APP_NO_ERROR;
    }

    for( node = 0; node < TIME_UNIT_MAX; node++ )
    {
        cur_node = &g_Time_list[node];
        if( cur_node->is_start == 1 )
        {
            if( cur_node->curvalue > 0 )
            {
                cur_node->curvalue --;
            }      
            if( cur_node->run_mode == TIME_RUN_MODE_ISR
             && cur_node->curvalue == 0 )
            {
                cur_node->curvalue = cur_node->interval;        
                if( cur_node->type == ONCE_TIME )
                {
                    cur_node->is_start = 0;
                } 
                if( cur_node->pfun != NULL )
                {
                    ( *cur_node->pfun ) ();
                }
            }
        }
    }

    return APP_ERROR_TIMER_OK;
}


app_error_t time_task_query( void )
{
    u8          node;
    time_unit_t*cur_node    = NULL;
    if( 1 != time_list_flag_query() )
    {
        return APP_NO_ERROR;
    }

 //   gxos_interrupt_disable();

    for( node = 0; node < TIME_UNIT_MAX; node++ )
    {
        cur_node = &g_Time_list[node];
        if( cur_node->is_start == 1
         && cur_node->run_mode == TIME_RUN_MODE_TASK
         && cur_node->curvalue == 0 )
        {
            cur_node->curvalue = cur_node->interval;        
            if( cur_node->type == ONCE_TIME )
            {
                cur_node->is_start = 0;
            } 
            if( cur_node->pfun != NULL )
            {
                ( *cur_node->pfun ) ();
            }
        }
    }

 //   gxos_interrupt_enable();
    return APP_ERROR_TIMER_OK;
}





//释放一个定时单元
app_error_t time_release( u8 id )
{
    gxos_interrupt_disable(); 
    g_Time_list[id].is_register = 0;
    g_Time_list[id].is_start = 0;
    gxos_interrupt_enable();

    return APP_ERROR_TIMER_OK;
}

#else
app_error_t time_get_id( void* pfun, u16* id )
{
    u16 node;
    gxos_interrupt_disable();
    node = 0;
    while( g_Time_list[node].pfun != pfun && node < TIME_UNIT_MAX )
    {
        node++;
    }
    if( node >= TIME_UNIT_MAX )
    {
        gxos_interrupt_enable();
      //  SYS_PRINTF( "time_get_id----cant find pfun\n" );
        return APP_ERROR_TIMER_GET_ID;
    }
    else
    {
        *id = g_Time_list[node].time_id;

        gxos_interrupt_enable();
        return APP_ERROR_TIMER_OK;
    }
//    gxos_interrupt_enable();
}
//开始一个定时单元
app_error_t time_start( u16 id )
{
    gxos_interrupt_disable(); 

    if( g_Time_list[id].is_register == 1 )
    {
        g_Time_list[id].is_start = 1; 
        gxos_interrupt_enable();    
        return APP_ERROR_TIMER_OK;
    }
    else
    {
        gxos_interrupt_enable();    
        //SYS_PRINTF( "time_start----%d is not registered\n",id );
        return APP_ERROR_TIMER_START;
    }
//    gxos_interrupt_enable();
}

//使一个定时单元无效
app_error_t time_stop( u16 id )
{
    gxos_interrupt_disable();
    if( g_Time_list[id].is_start == 1 )
    {
        g_Time_list[id].is_start = 0;
        gxos_interrupt_enable();
        return APP_ERROR_TIMER_OK;
    }
    else
    {
        gxos_interrupt_enable();
      //  SYS_PRINTF( "time_stop----id %d is not start\n" ,id);
        return APP_ERROR_TIMER_END;
    } 
//    gxos_interrupt_enable();
}

//使一个定时单元无效
app_error_t time_restart( u16 id,u32 new_period)
{
    gxos_interrupt_disable();
    if(1)// g_Time_list[id].is_start == 1 )
    {
	g_Time_list[id].is_start = 1;
	g_Time_list[id].curvalue = new_period;//g_Time_list[id].interval ;
	g_Time_list[id].interval  = new_period;
	  gxos_interrupt_enable();
	// SYS_PRINTF( "time_restart %d ms \n" ,g_Time_list[id].curvalue);
	 //uart_printf("event restart  tick=%d\n",get_curtime()); 
      
        return APP_ERROR_TIMER_OK;
    }
    else
    {
    //    gxos_interrupt_enable();
     //   SYS_PRINTF( "time_restart----id %d is not registered\n" ,id);
        return APP_ERROR_TIMER_END;
    } 
   // gxos_interrupt_enable();
}


//isr轮询
app_error_t time_isr_query( void )
{
    u8          node;
    time_unit_t*cur_node    = NULL;

    if( 1 != time_list_flag_query() )
    {
        return APP_NO_ERROR;
    }

    for( node = 0; node < TIME_UNIT_MAX; node++ )
    {
        cur_node = &g_Time_list[node];
        if( cur_node->is_start == 1 )
        {
            if( cur_node->curvalue > 0 )
            {
                cur_node->curvalue --;
            }      
            if( cur_node->run_mode == TIME_RUN_MODE_ISR
             && cur_node->curvalue == 0 )
            {
                cur_node->curvalue = cur_node->interval;        
                if( cur_node->type == ONCE_TIME )
                {
                    cur_node->is_start = 0;
                } 
                if( cur_node->pfun != NULL )
                {
                    ( *cur_node->pfun ) ();
                }
            }
        }
    }

    return APP_ERROR_TIMER_OK;
}


app_error_t time_task_query( void )
{
    u8          node;
    time_unit_t*cur_node    = NULL;

 //   gxos_interrupt_disable();

    for( node = 0; node < TIME_UNIT_MAX; node++ )
    {
        cur_node = &g_Time_list[node];
        if( cur_node->is_start == 1
         && cur_node->run_mode == TIME_RUN_MODE_TASK
         && cur_node->curvalue == 0 )
        {
            cur_node->curvalue = cur_node->interval;        
            if( cur_node->type == ONCE_TIME )
            {
                cur_node->is_start = 0;
            } 
            if( cur_node->pfun != NULL )
            {
                ( *cur_node->pfun ) ();
            }
        }
    }

 //   gxos_interrupt_enable();
    return APP_ERROR_TIMER_OK;
}





//释放一个定时单元
app_error_t time_release( u16 id )
{
    gxos_interrupt_disable(); 
    g_Time_list[id].is_register = 0;
    g_Time_list[id].is_start = 0;
    gxos_interrupt_enable();

    return APP_ERROR_TIMER_OK;
}


#endif


