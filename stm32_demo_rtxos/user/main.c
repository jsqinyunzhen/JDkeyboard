

/* --------------------------------------------------------------------------
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *      Name:    BLinky.c
 *      Purpose: RTX example program
 *
 *---------------------------------------------------------------------------*/

#include <stdio.h>

#include "stm32f10x.h"

#include "rtx_os.h"                  // ARM::CMSIS:RTOS2:Keil RTX5
#include "cmsis_os.h"

#include "project_config.h"

#include "st_printf.h"                  // ARM::CMSIS:RTOS2:Keil RTX5
#include "st_rtc.h"
#include "clock_calendar.h"
#include "st_gpio.h"
#include "stm32_eval_spi_flash.h"



#include "stm32f10x_ip_dbg.h"
#include "st_backupreg.h"
#include "st_rtc.h"
#include "st_iwdg.h"
#include "stm32f10x_it.h"
#include "st_adc.h"
#include "st_audio_product.h"
#include "pack.h"
#if (USE_SOFTTIMER_DIY ==1)
#include "mod_date_time.h"
#include "mod_time_list.h"
#endif
#include "app4g.h"
#include "app_rs485_broker.h"
#include "ds18b20.h"
#include "ta6932.h"
#include <time.h>
#if defined(RTE_Compiler_EventRecorder)
#include "EventRecorder.h"
#include "EventRecorderConf.h"                       // Keil::ARM_Compiler
#endif
#include "app4g.h"
#include "cjson_middleware.h"
#include "mqtt_app.h"
#include "transport.h"
#include "dianchuan.h"
#include "st_ext_spi_flash.h"
#include "st_flash.h"

#if (USE_SOFTTIMER_DIY ==1)
u8 timer_dma_pcm;
u8 timer_detect;
u8 timer_y_ctrl= 0;
u8 timer_kb_openport= 0;

u8 led_mode = 0;
unsigned char  times =0;

#else
osTimerId_t timer_dma_pcm ;
#endif
//uint8_t timer_event_check = 0;
volatile uint32_t u32mcu_eventStatus = 0;
volatile char audio_status = 0;
volatile uint8_t Onkey_index;

uint8_t debug_uart_id=0xff;
//#define LEN (33 +2)
unsigned char string[LEN];
char string_len;

//uint32_t startflag __attribute__((at(0x2000F000)));
osThreadId_t thread_main;
osThreadId_t thread_uart_msg;

osThreadId_t thread_app;
osThreadId_t thread_app_dc12;

uint64_t thread1_stk_1[MAIN_STACK_SIZE];
uint64_t thread1_stk_2[UART2_MSG_STACK_SIZE];
uint64_t thread1_stk_3[APP_MQTT_STACK_SIZE];
uint64_t thread1_stk_4[APP_DC12_STACK_SIZE];

//static
//uint64_t thread1_stk_1[100];
static osRtxThread_t thread1_tcb;

//static
//uint64_t thread1_stk_2[100];
static osRtxThread_t thread2_tcb;

//static
//uint64_t thread1_stk_3[700];
static osRtxThread_t thread3_tcb;
static osRtxThread_t thread4_tcb;

const osThreadAttr_t thread1_attr = {
    .name = "main",
    .attr_bits = osThreadJoinable,
    .stack_mem  = &thread1_stk_1[0],
    .stack_size = sizeof(thread1_stk_1),
    .cb_mem  = &thread1_tcb,
    .cb_size = sizeof(thread1_tcb),
    .priority = osPriorityAboveNormal1                    //Set initial thread priority to high
};

const osThreadAttr_t thread2_attr = {
    .name = "app_uart_msg",
    .attr_bits = osThreadJoinable,
    .stack_mem  = &thread1_stk_2[0],
    .stack_size = sizeof(thread1_stk_2),
    .cb_mem  = &thread2_tcb,
    .cb_size = sizeof(thread2_tcb),
    .priority = osPriorityAboveNormal                    //Set initial thread priority to high
};
const osThreadAttr_t thread3_attr = {
    .name = "app_mqtt",
    .attr_bits = osThreadJoinable,
    .stack_mem  = &thread1_stk_3[0],
    .stack_size = sizeof(thread1_stk_3),
    .cb_mem  = &thread3_tcb,
    .cb_size = sizeof(thread3_tcb),
    .priority = osPriorityAboveNormal2                    //Set initial thread priority to high
};
const osThreadAttr_t thread4_attr = {
    .name = "app_dc12",//uart3 uart4
    .attr_bits = osThreadJoinable,
    .stack_mem  = &thread1_stk_4[0],
    .stack_size = sizeof(thread1_stk_4),
    .cb_mem  = &thread4_tcb,
    .cb_size = sizeof(thread4_tcb),
    .priority = osPriorityAboveNormal3                    //Set initial thread priority to high
};


#if 1
const osMutexAttr_t os_mutex_def_mutex_uart = {
    "mutex_uart",                          // human readable mutex name
    osMutexRecursive | osMutexPrioInherit,    // attr_bits
    NULL,                                     // memory for control block
    0U                                        // size for control block
};
#else

osMutexDef (mutex_uart);
#endif

osMutexId mutex_uart_id;

#if 1
//osSemaphoreAttr_t
const osSemaphoreDef_t os_semaphore_def_sem_uart= { "sem_uart", 0U, NULL, NULL };

#else
osSemaphoreDef (sem_uart);
#endif

osSemaphoreId_t multiplex_id;

osMutexId_t CreateMutex (const osMutexAttr_t *mutex_def)
{
    osMutexId mutex_id;

    //mutex_id = osMutexCreate(mutex_def);

    mutex_id = osMutexNew(mutex_def);
    if (mutex_id != NULL)  {
        // Mutex object created
        printf("mutex_id_uart =0x%08x\r\n",(unsigned int)mutex_id);
    }
    return mutex_id;

}
void AcquireMutex (osMutexId_t mutex_id)
{
    osStatus status;
    if (mutex_id != NULL)
    {
        status = osMutexAcquire(mutex_id,osWaitForever);
        if (status != osOK)
        {
            // handle failure code
            printf("AcquireMutex fail=0x%08x\r\n",(unsigned int)mutex_id);
        }
    }
}
void ReleaseMutex (osMutexId_t mutex_id)
{
    osStatus status;

    if (mutex_id != NULL)
    {
        status = osMutexRelease(mutex_id);
        if (status != osOK)
        {
            // handle failure code
            printf("ReleaseMutex fail=0x%08x\r\n",(unsigned int)mutex_id);
        }
    }
}

osEventFlagsId_t evt_id_app;
osEventFlagsAttr_t os_event_flag_app = {
    "event_fg_app",					   // human readable mutex name
    0,	  // attr_bits
    NULL,									  // memory for control block
    0U										  // size for control block
};

osEventFlagsId_t evt_id_uart=NULL;
osEventFlagsAttr_t os_event_flag_uart = {
    "event_fg_uart", 						 // human readable mutex name
    0,	// attr_bits
    NULL, 									// memory for control block
    0U										// size for control block
};

osEventFlagsId_t evt_id_uart1=NULL;
osEventFlagsAttr_t os_event_flag_uart1 = {
    "event_fg_uart1", 						 // human readable mutex name
    0,	// attr_bits
    NULL, 									// memory for control block
    0U										// size for control block
};
osEventFlagsId_t evt_id_uart3=NULL;
osEventFlagsAttr_t os_event_flag_uart3 = {
    "event_fg_uart3", 						 // human readable mutex name
    0,	// attr_bits
    NULL, 									// memory for control block
    0U										// size for control block
};
osEventFlagsId_t evt_id_uart4=NULL;
osEventFlagsAttr_t os_event_flag_uart4 = {
    "event_fg_uart4", 						 // human readable mutex name
    0,	// attr_bits
    NULL, 									// memory for control block
    0U										// size for control block
};
osEventFlagsId_t evt_id_uart5=NULL;
osEventFlagsAttr_t os_event_flag_uart5 = {
    "event_fg_uart5", 						 // human readable mutex name
    0,	// attr_bits
    NULL, 									// memory for control block
    0U										// size for control block
};

void therad_base_api_demo(void)
{
    const char * thread_name = NULL;
    uint32_t thread_count = 0;
    uint32_t stack_size = 0;

    osThreadId_t thread_id = 0;
    osThreadState_t os_thread_state = osThreadReserved;
    osStatus_t os_state = osStatusReserved;
    osPriority_t os_pri = osPriorityReserved;

    thread_name= osThreadGetName(thread_app);
    if(thread_name)
        printf("osThreadGetName thread_name =%s\r\n",thread_name);
    thread_id =  osThreadGetId();
    printf("osThreadGetId thread_id =0x%08x thread_app=0x%08x\r\n",(unsigned int)thread_id,(unsigned int)thread_app);

    os_thread_state =  osThreadGetState(thread_app);
    printf("osThreadGetState =%d\r\n",os_thread_state);

    stack_size =  osThreadGetStackSize(thread_app);
    printf("osThreadGetStackSize =%d\r\n",stack_size);

    os_pri =  osThreadGetPriority(thread_app);
    printf("osThreadGetPriority =%d\r\n",os_pri);


    os_state = osThreadSetPriority(thread_app, osPriorityBelowNormal);  // Set thread priority
    if (os_state == osOK) {
        // Thread priority changed to BelowNormal
        printf("Thread priority changed to BelowNormal\r\n");
    }
    else {
        // Failed to set the priority
        printf("Failed to set the priority\r\n");
    }
    os_pri =  osThreadGetPriority(thread_app);
    printf("osThreadGetPriority =%d\r\n",os_pri);

    thread_count = osThreadGetCount();
    printf("osThreadGetCount thread_count =%d\r\n",thread_count);
}


void osRtxIdleThread (void *argument)
{
    (void)argument;
//	uint32_t tick_cur = 0;
//	uint32_t tick_cur2 = 0;
    for (;;)
    {
//	tick_cur = osKernelGetTickCount();
        //printf("@@@@@@@ osKernelGetTickCount =%d\r\n",tick_cur);
        //osDelay(1000);
        __WFE();                            /* Enter sleep mode                   */
        //  tick_cur2 = osKernelGetTickCount();
        //printf("@@@@@@@ tick_cur =%d tick_cur2 =%d\r\n",tick_cur,tick_cur2);
    }
}
MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

#if defined(RTE_CMSIS_RTOS)
void thread_app_cb(const void  *arg)
#else
//http://cache.baiducontent.com/c?m=9d78d513d98207f04fece47f0d01d7174617c6252bd6a0542894c81bcd3f1316506793ac20560705a387273b54b8482caaa77565377267ebc898d61dcabbe36e729f27452141da1d45825fb8cb3732c151875b99ef4ea1fdb06e84aea382820204de1445248faacd1c4203cb1ced0974a6e38e48620252edb16733bb4e775ece7e1fab13a4a075311081819f060ed42ba43e4cc1b834c67a49e256e2455b7317f65bb10c177d66f74e51ff44370f93eb&p=89759a45d69a5aee40bbc7710f4f&newp=8c3bc40785cc43ff57ed9466515492695d0fc20e3bd3d201298ffe0cc4241a1a1a3aecbf22231104d0c1796c00a44c59e1f632703d0034f1f689df08d2ecce7e&user=baidu&fm=sc&query=STM32+MQTTSerialize%5Fpingreq+Unable+to+connect%2C+return+code&qid=f2b389bf00030632&p1=3
void thread_app_cb( void  *arg)
#endif
{
    //Var_ThreadExec += (arg == NULL) ? (1) : (2);
    // uint32_t tick_cur = 0;
    unsigned char *buf = mqtt_core_buff;
    int buflen = sizeof(mqtt_core_buff);
    int len = 0;
    int rc = 0;
#if 1
    char* will_topics_str=mymalloc(50);
    char* will_topics=mymalloc(50);
    //MQTTPacket_connectData* pdata =(MQTTPacket_connectData*)mymalloc(sizeof(MQTTPacket_connectData));

#else
    char will_topics_str[50];
    char will_topics[50];
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
#endif

    unsigned char sessionPresent, connack_rc;
    MQTTString topicStrings[] = {MQTTString_initializer, MQTTString_initializer, MQTTString_initializer};
    uint8_t top_pos;
    uint8_t top_num = (sizeof(sub_topics) / sizeof(sub_topics[0]));
    int req_qos[] = {2,2,2};

    int subcount;
    int granted_qos[3];
    unsigned short submsgid;

    unsigned char dup;
    int qos;
    unsigned char retained;
    unsigned short msgid;
    int payloadlen_in;
    unsigned char* payload_in;
    MQTTString receivedTopic;

    uint8_t  msgtypes = CONNECT;
    int mqtt_tcp_socket = 0;

#if 0
check_imei:

    if (dx_check_imei(g_imei))
    {
        // snprintf(imei, imei_len, "%s", g_imei);
        printf("imei haved get ,is %s\r\n",g_imei);
    }
    else
    {
        printf("check_imei...\r\n");
        osDelay(1000);
        goto check_imei;
    }
#endif
    dx_lte_init_check();
check_module4g_init:

    if(module4g_init ==1)
    {
        // snprintf(imei, imei_len, "%s", g_imei);
        printf("module4g_init_ok ==1\r\n");
        osDelay(1000);
    }
    else
    {
        printf("check_module4g_init_ok\r\n");
        osDelay(1000);
        goto check_module4g_init;
    }
    memset(g_imei,0,16);

    //osThreadSuspend(thread_uart_msg);
    //mqtt_tcp_socket = transport_open("\"mqtt.jindoo.jopool.net\"",MQTT_PORT);



#if 0
    mqtt_tcp_socket = transport_open("\"mqtt.fluux.io\"",1883);
    printf("mqtt_tcp_socket %d\r\n",mqtt_tcp_socket);
    if(mqtt_tcp_socket < 0)
    {
        return ;
    }

    //data.MQTTVersion =3;
    data.clientID.cstring = "me";
    data.keepAliveInterval = 20;
    data.cleansession = 1;
    data.username.cstring = "testuser";
    data.password.cstring = "testpassword";
#else
    mqtt_tcp_socket = transport_open("\"mqtt.jindoo.jopool.net\"",MQTT_PORT);
    printf("mqtt_tcp_socket %d\r\n",mqtt_tcp_socket);
    if(mqtt_tcp_socket < 0)
    {
        return ;
    }
    {
        uint8_t id[DEVID_SPACE]= {0};
        sFLASH_ReadBuffer((uint8_t*)id, FLASH_DEVID_ADDRESS, DEVID_SPACE);
        printf("id[0]=%d\r\n",id[0]);
        printf("id =%s\r\n",&id[1]);
        if((id[0] >=5)&&(id[0] <=20))
        {
            memcpy(g_imei,&id[1],id[0]);
        }
        else
            memcpy(g_imei,MODULE4G_IMEI_SN_TEST,strlen(MODULE4G_IMEI_SN_TEST));
    }
    //memcpy(g_imei,DEVICE_TEST_UUID,strlen(DEVICE_TEST_UUID));
    //  memcpy(g_imei,MODULE4G_IMEI_SN_ERROR_TEST,strlen(MODULE4G_IMEI_SN_ERROR_TEST));
    mqtt_set(g_imei);
    charge_mg_init();
    printf("client_id = %s\r\n",client_id);
    sprintf(will_topics_str, WILL_STR, client_id);
    sprintf(will_topics, WILL_TOPIC, client_id);
#if 0

    MQTTPacket_connectData* pdata =(MQTTPacket_connectData*)mymalloc(sizeof(MQTTPacket_connectData));
    memset(pdata,0,sizeof(MQTTPacket_connectData));
    pdata->clientID.cstring = client_id;
    pdata->keepAliveInterval = MQTT_KEEP_ALIVE;
    pdata->cleansession = 1;
    pdata->username.cstring = MQTT_USERNAME;
    pdata->password.cstring = MQTT_PASSWORD;
    pdata->willFlag = 1;
    pdata->will.topicName.cstring = will_topics;
    pdata->will.message.cstring = will_topics_str;
    pdata->will.qos = 2;
    pdata->will.retained = 0;

#else


    data.clientID.cstring = client_id;
    data.keepAliveInterval = MQTT_KEEP_ALIVE;
    data.cleansession = 1;
    data.username.cstring = MQTT_USERNAME;
    data.password.cstring = MQTT_PASSWORD;
    data.willFlag = 1;
    data.will.topicName.cstring = will_topics;
    data.will.message.cstring = will_topics_str;
    data.will.qos = 2;
    data.will.retained = 0;
#endif
#endif
    printf("sizeof(MQTTPacket_connectData)=%d\r\n",sizeof(MQTTPacket_connectData));
    uint32_t curtick = osKernelGetTickCount();
    static uint32_t bef_sec_update=0;
    static uint32_t bef_sec_check_temp= 0;
	static uint32_t bef_sec_check_board_temp= 0;
    uint32_t now_sec;

    while(1)
    {
        //mqtt_pause
        if( dx_get_mqtt_thread_status() == 2)
        {
            osDelay(10000);
            continue;

        }
        else if( dx_get_mqtt_thread_status() == 1)
        {
            transport_close(mqtt_tcp_socket);

            int ret =dx_lte_http_contextid_config();
            if(ret == 0)
            {
                fw_upgeade_start();
                //ret = dx_lte_http_url_config(HTTPURL_BAIDU);

                //ret = dx_lte_http_url_config(HTTPURL_JD_TEST);
                if(strlen(updata_path) == 0)
                    ret = dx_lte_http_url_config(HTTPURL_JD_UPGRADE_TEST);
                else
                    ret = dx_lte_http_url_config(updata_path);
                //ret = dx_lte_http_url_config(HTTPURL_BAIDU);
                if(ret == 0)
                {
                    int file_len = 0;
                    ret = dx_lte_http_file_len(&file_len);
                    if(ret == 0)
                    {
                        printf("file_len= %d\r\n",file_len);
                        ret =dx_lte_http_getfile(STM32APPBIN,file_len);
                        printf("dx_lte_http_getfile=%d\r\n",ret);
                        if(ret == 0)
                        {
                            printf("reboot, to update stm fw...\r\n");

                            IAP_WriteFlag(UPDATE_MCUAPP_FLAG_DATA);
                            mcu_sys_soft_reset();
                            while(1);
                        }
                        else
                        {
                            goto reboot;
                        }
                    }
                    else
                    {
                        printf("dx_lte_http_file_len=%d\r\n",ret);
                        goto reboot;
                    }
                }
                else
                {
                    printf("dx_lte_http_url_config=%d\r\n",ret);
                    goto reboot;
                }


            }
            else
                goto reboot;
reboot:
            printf("dx_lte upgrade fail\r\n");
            mcu_sys_soft_reset();
            while(1);
            //osDelay(1000);
            //continue;

        }
        // debug_uart_id = PRINTF_UART_ID;
        // tick_cur = osKernelGetTickCount();
        //printf("$$$$$$thread_app_cb osKernelGetTickCount =%d\r\n",tick_cur);
        //printf("osEventFlagsGet=0x%x\r\n",osEventFlagsGet(evt_id_app));

        switch(msgtypes)
        {
        case CONNECT:
            len = MQTTSerialize_connect(buf, buflen, &data);
            //len = MQTTSerialize_connect(buf, buflen, pdata);
            MQTT_PRINT_DEBUG("send CONNECT data:%d\r\n",len);

            rc = transport_sendPacketBuffer(mqtt_tcp_socket, (unsigned char*)buf, len);
            if (rc == 0)
            {
                mqtt_sta = MQTT_CON;
                MQTT_PRINT_DEBUG("send CONNECT Successfully\r\n");
            }
            else
            {
                mqtt_sta = MQTT_DISCON;
                MQTT_PRINT_DEBUG("send CONNECT failed=%d\r\n",rc);
            }
            myfree(will_topics_str);
            myfree(will_topics);
            //myfree(pdata);
            //MQTT_PRINT_DEBUG("MQTT concet to server!\r\n");
            msgtypes = 0;
            break;
        case CONNACK:
            if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
            {
                MQTT_PRINT_DEBUG("Unable to connect, return code %d\r\n", connack_rc);
            }
            else
            {
                MQTT_PRINT_DEBUG("MQTT is concet OK!\r\n");
            }

            mqtt_sta = MQTT_CON_ACK;
            msgtypes = SUBSCRIBE;
            break;
        case SUBSCRIBE:
            // topicString.cstring = "ledtest";
            //len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);
            //MQTT_PRINT_DEBUG("client subscribe:[%s]",topicString.cstring);
            for(top_pos = 0; top_pos < top_num; top_pos++)
            {
                topicStrings[top_pos].cstring = sub_topics[top_pos];
                MQTT_PRINT_DEBUG("sub topic %d %s\r\n", top_pos, sub_topics[top_pos]);
            }


            len = MQTTSerialize_subscribe(buf, buflen, 0, 1, top_num, topicStrings, req_qos);
            rc = transport_sendPacketBuffer(mqtt_tcp_socket, (unsigned char*)buf, len);
            if (rc == 0)
                MQTT_PRINT_DEBUG("send SUBSCRIBE Successfully\r\n");
            else
                MQTT_PRINT_DEBUG("send SUBSCRIBE failed\r\n");

            msgtypes = 0;
            mqtt_sta = MQTT_SUB;
            break;
        case SUBACK:
            //rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
            rc = MQTTDeserialize_suback(&submsgid, (sizeof(sub_topics)/sizeof(sub_topics[0])), &subcount, granted_qos, buf, buflen);
            for(top_pos=0; top_pos<sizeof(sub_topics)/sizeof(sub_topics[0]); top_pos++)
                MQTT_PRINT_DEBUG("granted_qos[%d]=%d\r\n", top_pos,granted_qos[top_pos]);
            MQTT_PRINT_DEBUG("submsgid:%d, subcount:%d,rc=%d\r\n", submsgid, subcount,rc);

            mqtt_sta = MQTT_OK;

            //{
#if 1
            rc = mqtt_link_pub_online();
            if (rc == 0)
                printf("send pub online Successfully\r\n");
            else
                printf("send pub online failed\r\n");

#else
            char* pjson_buff=json_buff;
            int json_len = sizeof(json_buff);
            MQTTString topicString = MQTTString_initializer;
            static unsigned short packet_id = 0;

            memset(pjson_buff,0,json_len);
            if(mqtt_sta != MQTT_OK)
                return;

            printf("mqtt_link_pub_online\r\n");
            //json init
            json_len = json_online_cmd(pjson_buff);
            if(json_len	> 0)
            {
                printf("cmd_len: %d\r\n", json_len);
                printf("cur json_buff is: %s \r\n", pjson_buff);

            }
            else
                printf("ret: %d\r\n", json_len);

            //mqtt
            packet_id++;
            topicString.cstring = pub_topics[3];
            qos = 1;
            len = MQTTSerialize_publish(buf, buflen, 0, qos, 0, packet_id, topicString, (unsigned char*)pjson_buff, json_len);

            rc = transport_sendPacketBuffer(mqtt_tcp_socket, (unsigned char*)buf, len);
            if (rc == 0)
                printf("send pub online Successfully\r\n");
            else
                printf("send pub online failed\r\n");
#endif
            //}
            msgtypes = 0;
            break;


        case PUBLISH:
            rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,&payload_in, &payloadlen_in, buf, buflen);
            printf("MQTTDeserialize_publish: %d\r\n", rc);
            if(rc == 1)
                printf("MQTTDeserialize_publish ok\r\n");
            else
                printf("MQTTDeserialize_publish fail\r\n");
            //dup=0 qos=d retained=0, msgid=0
            MQTT_PRINT_DEBUG("dup=%d qos=%d,retained=%d, msgid=%d\r\n", dup,qos,retained,msgid);
            MQTT_PRINT_DEBUG("message arrived : %s,len=%d\r\n", payload_in,payloadlen_in);
            if(receivedTopic.cstring)
                MQTT_PRINT_DEBUG("receivedTopic cstring=%s\r\n",(uint8_t *)receivedTopic.cstring);
            else
                MQTT_PRINT_DEBUG("receivedTopic cstring is null\r\n");
            MQTT_PRINT_DEBUG("receivedTopic=%s len=%d\r\n",(uint8_t *)receivedTopic.lenstring.data, receivedTopic.lenstring.len);
            //[09-16-10:26:15]message arrived : {"ct":120,"ht":20,"t":1568600766,"temp":80,"ut":180,"wt":130}
            //[09-16-10:26:15]receivedTopic=dc/cs/param/865860040521701{"ct":120,"ht":20,"t":1568600766,"temp":80,"ut":180,"wt":130} len=27

            if(strstr((const char*)receivedTopic.lenstring.data,sub_topics[0]))
            {
                MQTT_PRINT_DEBUG("sub_topics[0]\r\n");
//[09-24-14:42:38]receivedTopic=dc/cs/set/865860040521701{"did":"865860040521701","st":[{"apow":400,"cn":0,"ipow":20,"opt":480,"sst":1,"tck":60}],"t":1569307271} len=25
                json_parse((char*)payload_in,sub_topics[0]);
            }
            else if(strstr((const char*)receivedTopic.lenstring.data,sub_topics[1]))
            {
                MQTT_PRINT_DEBUG("sub_topics[1]\r\n");
                json_parse((char*)payload_in,sub_topics[1]);

            }
            else if(strstr((const char*)receivedTopic.lenstring.data,sub_topics[2]))
            {
                MQTT_PRINT_DEBUG("sub_topics[2]\r\n");
                json_parse((char*)payload_in,sub_topics[2]);

            }

            if(qos == 1)
            {
                MQTT_PRINT_DEBUG("publish qos is 1,send publish ack.\r\n");
                memset(buf,0,buflen);
                len = MQTTSerialize_ack(buf,buflen,PUBACK,dup,msgid);	//publish ack
                rc = transport_sendPacketBuffer(mqtt_tcp_socket, (unsigned char*)buf, len);
                if (rc == 0)
                    MQTT_PRINT_DEBUG("send PUBACK Successfully\r\n");
                else
                    MQTT_PRINT_DEBUG("send PUBACK failed\r\n");
            }
            msgtypes = 0;
            break;

        case PUBACK:
            MQTT_PRINT_DEBUG("PUBACK!\r\n");
            msgtypes = 0;
            break;
        case PUBREC:
            MQTT_PRINT_DEBUG("PUBREC!\r\n");	 //just for qos2
            break;
        case PUBREL:
            MQTT_PRINT_DEBUG("PUBREL!\r\n");		//just for qos2
            break;
        case PUBCOMP:
            MQTT_PRINT_DEBUG("PUBCOMP!\r\n");		  //just for qos2
            break;
        case PINGREQ:
            len = MQTTSerialize_pingreq(buf, buflen);
            rc = transport_sendPacketBuffer(mqtt_tcp_socket, (unsigned char*)buf, len);
            if (rc == 0)
                MQTT_PRINT_DEBUG("send PINGREQ Successfully\r\n");
            else
                MQTT_PRINT_DEBUG("send PINGREQ failed=%d\r\n",rc);
            MQTT_PRINT_DEBUG("time to ping mqtt server to take alive=%d\r\n",len);
            msgtypes = 0;
            ping_tm_out_cnt ++;
            if(ping_tm_out_cnt >= PING_TM_OUT_MAX)
            {
                mqtt_sta = MQTT_DISCON;
                ping_tm_out_cnt = 0;
                MQTT_PRINT_DEBUG("ping_tm_out_cnt=%d\r\n",ping_tm_out_cnt);
                printf("MQTT_DISCON reboot...\r\n");
                mcu_sys_soft_reset();

            }
            break;

        case PINGRESP:
            MQTT_PRINT_DEBUG("mqtt server Pong\r\n");
            ping_tm_out_cnt = 0;
            mqtt_sta = MQTT_OK;
            msgtypes = 0;
            break;
        default:
            MQTT_PRINT_DEBUG("msgtypes error=%d \r\n",msgtypes);
            break;

        }
        //debug_uart_id = 0xff;
        if(get_mqtt_transport_sock() >= 0)
        {
            memset(buf,0,buflen);
            MQTT_PRINT_DEBUG("MQTTPacket_read start\r\n");
            rc=MQTTPacket_read(buf, buflen, transport_getdata);
            if(rc >0)
            {
#if 0
                int i =0;
                for(i=0; i<buflen; i++)
                {
                    printf("buf[%02d]=0x%02x,",i,buf[i]);
                    if((i+1)%8 == 0)
                        printf("\r\n");
                }
#endif
                msgtypes = rc;
                MQTT_PRINT_DEBUG("MQTT is get recv=0x%x:\r\n",msgtypes);
            }
            else
            {
                MQTT_PRINT_DEBUG("MQTTPacket_read rc=%d\r\n",rc);
                //other mqtt msg dealwith
                //check ping
                if(charge_Info.ping_tm !=0)
                {
                    if((osKernelGetTickCount() - curtick) >(charge_Info.ping_tm/2*1000))
                    {
                        if(msgtypes == 0)
                        {
                            curtick = osKernelGetTickCount();
                            msgtypes = PINGREQ;
                            MQTT_PRINT_DEBUG("ping start=%d\r\n",get_curtime2());
                            continue;
                        }

                    }
                }
                //timer update
                if(charge_Info.updata_tm>0)
                {
                    now_sec = RTC_GetCounter();
                    //if(((now_sec - bef_sec_update) >= (charge_Info.updata_tm/2)) ||(get_cur_upgrade_num() !=0))
                    if(((now_sec - bef_sec_update) >= (charge_Info.updata_tm)) ||(get_cur_upgrade_num() !=0))
                    {
                        //print_heap_info();
                        bef_sec_update = now_sec;
                        rc = mqtt_updata_pro();
                        if (rc == 0)
                        {
                            MQTT_PRINT_DEBUG("mqtt_updata_pro Successfully=%d\r\n",now_sec);
                        }
                        else
                        {
                            MQTT_PRINT_DEBUG("mqtt_updata_pro=%d...\r\n",rc);
                        }
                        //print_heap_info();
                        continue;
                    }
                }

                if(charge_Info.warn_temp > 0)
                {
					
                    now_sec = RTC_GetCounter();
                    if((now_sec - bef_sec_check_temp) > 20)
                    {
                        bef_sec_check_temp = now_sec;
                        //int  ntc_temp = get_ntc_temp_adc1_chn9();
                        static char send_cnt =0;
						int DS18B20_temp = (int)DS18B20_Get_Temp(NULL,4);
						int warn_temp = (int)charge_Info.warn_temp;
						MQTT_PRINT_DEBUG("DS18B20_temp=%d ,%d\r\n",DS18B20_temp,warn_temp);
						if(DS18B20_temp == -100)
						{
							send_cnt = 0;
							continue;
						}
						charge_Info.temperature = DS18B20_temp;
                       
                        //charge_Info.temperature = ds18b20_get_temp()  / 10.0f;
                        //PRINT_DEBUG("temp:%f\r\n",charge_Info.temperature);
                        //if(charge_Info.temperature >= charge_Info.warm_temp)
                        if(DS18B20_temp >= warn_temp)
                        {
                            send_cnt++;
                            if(send_cnt >= 3)
                            {
                            	send_cnt = 0;
                                mqtt_pub_station_warning(STA_WARN_TEMP);
                                //play audio
                                //nvc040_play_append_byte(SP_INDEX_TEMP_WARNING);
                                SET_EVENT(MCU_EVENT_AUDIO_INNER_TEMP_TOO_HIGH);
                                SET_EVENT(MCU_EVENT_AUDIO_SAFE);
                                continue;
                            }
                        }
                        else
                        {
                            send_cnt = 0;
                        }

                    }
                }
				 if(charge_Info.warn_temp > 0)
                {
                    now_sec = RTC_GetCounter();
                    if((now_sec - bef_sec_check_board_temp) > 20)
                    {
                        bef_sec_check_board_temp = now_sec;
						 static char cnt =0;
                        int  ntc_temp = get_ntc_temp_adc1_chn9();
						//int ntc_temp = (int)DS18B20_Get_Temp(NULL,4);
                        int warn_temp = (int)charge_Info.warn_temp;
                       
                        MQTT_PRINT_DEBUG("ntc_temp=%d ,%d\r\n",ntc_temp,warn_temp);

                        //charge_Info.temperature = ds18b20_get_temp()  / 10.0f;
                        //PRINT_DEBUG("temp:%f\r\n",charge_Info.temperature);
                        //if(charge_Info.temperature >= charge_Info.warm_temp)
                        if(ntc_temp >= warn_temp)
                        {
                            cnt++;
                            if(cnt >= 2)
                            {
                            	cnt = 0;
                            	//charge_Info.temperature = ntc_temp;
								charge_Info.board_temp = ntc_temp;
                                mqtt_pub_station_warning(STA_WARN_BOARD_TEMP);
                                //play audio
                                //nvc040_play_append_byte(SP_INDEX_TEMP_WARNING);
                                SET_EVENT(MCU_EVENT_AUDIO_INNER_TEMP_TOO_HIGH);
                                SET_EVENT(MCU_EVENT_AUDIO_SAFE);
                                continue;
                            }
                        }
                        else
                        {
                            cnt = 0;
                        }

                    }
                }
				if( get_smoke_happen() == 1)
				{
					clear_smoke_happen();
					mqtt_pub_station_warning(STA_WARN_SMOKE);
					SET_EVENT(MCU_EVENT_AUDIO_SMOKE_TOO_HIGH);
					SET_EVENT(MCU_EVENT_AUDIO_SAFE);

					continue;
				}
				if( get_dhh_happen() == 1)
				{
					clear_dhh_happen();
					mqtt_pub_station_warning(STA_WARN_DHH_JC);
					//SET_EVENT(MCU_EVENT_AUDIO_CHARGE_AO);
					SET_EVENT(MCU_EVENT_AUDIO_SAFE);

					continue;
				}

				
#if 1
                {
                    static uint8_t index = 0;
                    int ret = 0;
                    uint8_t  index_status = 0;
                    uint16_t cur_power = 0;
                    uint16_t cur_port_status = 0;
                    uint32_t cur_time_sec = 0;
                    uint16_t charge_left_tm = 0;
                    //for(; index<channel_num; index++)
                    {
                        if(charge_Info.dc_port_status[index] == PORT_USEING)
                        {
							if(charge_mode == CHARGE_MODE_DC )
							{
	                            index_status= get_dc_port_time_complete ();
	                            if(index_status == (index+1))
	                            {

	                                if(dc_port_complete_reason == CHARGE_COMPLETE_REASON_IS_TIME_POWER_COMPLETE)
	                                {
	                                    index_status = REPORT_TYPE_CHARG_FINISH;
	                                }
	                                else if(dc_port_complete_reason == CHARGE_COMPLETE_REASON_IS_USER_MANU_STOP)
	                                {
	                                    if(charge_Info.tck_start_tm[index] == 0)
	                                        index_status = REPORT_TYPE_MIN_ZERO_PULL_OUT;
	                                    else
	                                        index_status = REPORT_TYPE_RUN_ZERO_PULL_OUT;
	                                }
	                                else
	                                {
	                                    index_status = REPORT_TYPE_CHARG_FINISH;
	                                }
	                                reset_dc_port_time_complete();

	                                goto DC_COMPLETE;

	                            }
							}
							if(charge_mode == CHARGE_MODE_DC )
								ret = dc_port_x_status(index+1,&cur_port_status);
							else
								ret= app_rs485_broker_port_x_status(index+1,&cur_port_status);
                            if(ret ==0)
                            {
                                //led update
                                cur_time_sec = RTC_GetCounter();//need dw
#if 0
                                charge_left_tm = (charge_Info.charge_tm[index]-(cur_time_sec-charge_Info.start_time[index])/60);
                                charge_Info.charge_left_tm[index] =charge_left_tm;
                                TA6932_DisplayPortNumber(index+1,charge_left_tm);
#else							
						
								charge_left_tm =  (charge_Info.charge_tm[index]-(cur_time_sec-charge_Info.start_time[index])/60);
								
								if(charge_Info.wait_tm >0)
								{
									TA6932_DisplayPortNumber(index+1,charge_left_tm);
								}
								if(charge_mode == CHARGE_MODE_BROKER )
								{
									if(cur_port_status != 0)
										charge_Info.charge_left_tm[index] =charge_left_tm;
								}


                                if((charge_Info.tck_start_tm[index] == 0) && (cur_port_status != 0))
                                {
                                    //normal mode
                                    //charge_left_tm = (charge_Info.charge_tm[index]-(cur_time_sec-charge_Info.start_time[index])/60);
                                    charge_Info.charge_left_tm[index] =charge_left_tm;
                                    //TA6932_DisplayPortNumber(index+1,charge_left_tm);
                                    if(charge_mode == CHARGE_MODE_BROKER )
									{
										if(charge_Info.wait_tm >0)
										{
											if(charge_left_tm <= 0)
											{
												index_status = REPORT_TYPE_CHARG_FINISH;
												goto DC_COMPLETE;

											}
										}
									}
                                }

                                if((charge_Info.tck_start_tm[index] != 0) && (cur_port_status != 0))
                                {
                                    //trickle_tm mode
                                    charge_left_tm = (charge_Info.trickle_tm[index]-(cur_time_sec-charge_Info.tck_start_tm[index])/60);
									if(charge_left_tm == charge_Info.trickle_tm[index])
									{
										charge_left_tm = charge_Info.trickle_tm[index]-1;

									}
                                    charge_Info.tck_left_tm[index] =charge_left_tm;
                                    //TA6932_DisplayPortNumber(index+1,charge_left_tm);
                                    if(charge_Info.tck_left_tm[index] <=0)
                                    {
                                        index_status = REPORT_TYPE_LESS_MIN_W;
                                        goto DC_COMPLETE;

                                    }


                                }

#endif

								if(charge_mode == CHARGE_MODE_BROKER )
									cur_power = (uint16_t) (charge_Info.charge_cur_pow[index]);
								else
                               		 cur_power = (uint16_t) (charge_Info.charge_cur_pow[index]*0.1);
                                if((cur_port_status != 0)&&
                                        (cur_power >= charge_Info.min_watter[index])&&
                                        (cur_power <= charge_Info.max_watter[index]))
                                {
                                    //normal work
                                    index_status = REPORT_TYPE_SET_SW_OK;
                                    if(charge_Info.port_report_type[index] != (Report_type)index_status)
                                    {
                                        //goto	mqtt_report;

                                    }
                                    goto out;

                                }
                                else if((cur_port_status != 0)&& (cur_power >= charge_Info.max_watter[index]))
                                {
                                    index_status = REPORT_TYPE_MORE_MAX_W;
                                    goto DC_COMPLETE;
                                }

                                else if((cur_port_status != 0)&&(cur_power <= charge_Info.min_watter[index]))
                                {
                                    index_status = REPORT_TYPE_SET_SW_OK;
                                    //charge_Info.wait_tm
                                    //START ONCE TIMER TO CHECK CHARGE
                                    cur_time_sec = RTC_GetCounter();
                                    if(charge_Info.tck_start_tm[index] == 0)
                                    {
                                        charge_Info.tck_start_tm[index] = cur_time_sec;
										charge_Info.tck_left_tm[index] =charge_Info.trickle_tm[index] -1;
                                    }


                                    if(((cur_time_sec-charge_Info.tck_start_tm[index])/60) >= charge_Info.trickle_tm[index])
                                    {
                                        index_status = REPORT_TYPE_LESS_MIN_W;
                                        goto DC_COMPLETE;

                                    }
                                    if(charge_Info.port_report_type[index] != (Report_type)index_status)
                                    {
                                        //goto	mqtt_report;

                                    }
                                    goto out;


                                }
                                else if(cur_port_status == 0)
                                {
                                    //index_status = REPORT_TYPE_NO_INSERT;


                                    if( charge_Info.trickle_tm[index] != charge_Info.tck_left_tm[index])
                                    {
                                        index_status = REPORT_TYPE_MIN_ZERO_PULL_OUT ;
                                        goto DC_COMPLETE;
                                    }

                                    if( charge_Info.charge_tm[index] != charge_Info.charge_left_tm[index])
                                    {
                                        index_status = REPORT_TYPE_RUN_ZERO_PULL_OUT ;
                                        goto DC_COMPLETE;
                                    }
                                    else
                                    {
                                        if(charge_Info.wait_tm >0)
                                        {
                                            cur_time_sec = RTC_GetCounter();
                                            if((cur_time_sec-charge_Info.start_time[index]) >= charge_Info.wait_tm)
                                            {
                                                index_status = REPORT_TYPE_NO_INSERT;
                                                goto DC_COMPLETE;

                                            }

                                        }
                                        goto out;

                                    }

                                }
DC_COMPLETE:
								if(charge_mode == CHARGE_MODE_DC )
                               	 dc_stop_power(index+1);
								else
									app_rs485_broker_stop_power(index+1);
                                reset_dc_port_para(index);
								if(charge_Info.wait_tm >0)
                                	TA6932_DisplayPortNumber(index+1,index+1);

                                SET_EVENT(index+1);
                                SET_EVENT(MCU_EVENT_AUDIO_CHARGE_END);

                                if(evt_id_app)
                                {
                                    osEventFlagsSet(evt_id_app, EVENT_FLAGS_APP_DC_COMPLETE);
                                }
//mqtt_report:
                                mqtt_pub_sw_report(&index,(Report_type*)&index_status,1);
                                charge_Info.port_report_type[index] = (Report_type)index_status;

                            }
                        }

                       // break;
                    }
out:
                    if(index >= channel_num)
                    {
                        index =0;
                    }
					else
					{
						index++;

					}
                }
#endif

            }
        }

        //osThreadYield();
        osDelay(1000);
    }
}


__NO_RETURN void thread_app_dc12_cb (void *argument)
{
    // const char *event_name;
//    uint32_t flags =0;

//   uint8_t data[] = {0x00};
//   uint8_t sessionid[] = {0x00,0x00,0x00,0x00,0x00,0x00};
    //  uint8_t dc1_2 =1;


    /*
    while(1)
    {
        uart3_dma_printf("uart3_dma_printf test\r\n");
    	uart3_cpu_printf("uart3_cpu_printf test\r\n");
    	osDelay(1000);
    }
    */
    // gpio_dc_detect_exti_config();
    for (;;)
    {

        //dc1--uart3 ,dc2--uart4(debug)
        gpio_dc_detect_value();
        //printf("dc %d %d\r\n",dc1_exist,dc2_exist);
        osDelay(5000);
        // continue;


    }
}

#if defined(RTE_CMSIS_RTOS)
osThreadDef (thread_app_cb, osPriorityBelowNormal, 1, 0);
#endif
uint32_t uart_thread_times = 0;

__NO_RETURN void thread_uart_cb (void *argument)
{
//    uint32_t tick_cur = 0;
//    osStatus_t val;
    uint32_t flags = 0;

    mutex_uart_id = CreateMutex(osMutex(mutex_uart));
    if(mutex_uart_id)
    {
        printf("CreateMutex  mutex_uart_id=0x%08x\r\n",(unsigned int)mutex_uart_id);
        printf("osMutexGetName name =%s\r\n",osMutexGetName (mutex_uart_id)?osMutexGetName (mutex_uart_id):"name is null");
        printf("osMutexGetOwner Owner ==0x%08x\r\n",(unsigned int)osMutexGetOwner (mutex_uart_id));

    }
    else
    {
        printf("CreateMutex mutex_uart fail\r\n");
        while(1);
    }
    //multiplex_id = osSemaphoreNew(3U, 3U, NULL);

    multiplex_id = osSemaphoreNew(3U, 3U, osSemaphore(sem_uart));
    //multiplex_id = osSemaphoreNew(3U, 3U, NULL);
    if (multiplex_id ) {
        //; // Semaphore object not created, handle failure
        printf("osSemaphoreNew  multiplex_id=0x%08x\r\n",(unsigned int)multiplex_id);
        printf("osMutexGetName name =%s\r\n",osSemaphoreGetName (multiplex_id));
        printf("osSemaphoreGetCount Count ==0x%08x\r\n",osSemaphoreGetCount (multiplex_id));
    }
    else
    {
        printf("osSemaphoreNew multiplex_id fail\r\n");
        while(1);
    }

    for (;;)
    {
#if 0
        AcquireMutex(mutex_uart_id);
        uart_thread_times ++;
        printf("osMutexGetOwner Owner ==0x%08x\r\n",(unsigned int)osMutexGetOwner (mutex_uart_id));
        printf("##########thread_uart_cb uart_thread_times =%d\r\n",uart_thread_times);
        ReleaseMutex(mutex_uart_id);


        val = osSemaphoreAcquire(multiplex_id, osWaitForever);
        switch (val) {
        case osOK:
            ; // Use protected code here...
            //osSemaphoreRelease(sid_Semaphore);			   // return a token back to a semaphore
            break;
        case osErrorResource:
            break;
        case osErrorParameter:
            break;
        default:
            break;
        }

        // do something
        osSemaphoreRelease(multiplex_id);
#endif
        //set event
#if 1
        if(debug_uart_id ==1)
            flags = EVENT_FLAGS_UART1|EVENT_FLAGS_UART1_TX_COMPLETE|EVENT_FLAGS_UART5;
        else if(debug_uart_id ==4)
        {
            flags = EVENT_FLAGS_UART4|EVENT_FLAGS_UART4_TX_COMPLETE|EVENT_FLAGS_UART5;
        }
#else

        flags = EVENT_FLAGS_UART1|EVENT_FLAGS_UART1_TX_COMPLETE| \
                EVENT_FLAGS_UART2|EVENT_FLAGS_UART2_TX_COMPLETE| \
                EVENT_FLAGS_UART5;
#endif
        if(debug_uart_id ==1)
        {
            flags = osEventFlagsWait(evt_id_uart1, flags, osFlagsWaitAny, osWaitForever);
            if((flags&EVENT_FLAGS_UART1) == EVENT_FLAGS_UART1)
            {
                printf("uart%d rx debug command\r\n",debug_uart_id);
                handle_uart_debug_cmd();
            }
#if 1
        if((flags&EVENT_FLAGS_UART5) == EVENT_FLAGS_UART5)
        {
            //rx at 485 response from broker
            printf("uart5 rx 485 response from broker\r\n");
            uart5_rec_rs485_response();
        }
#endif
        }
        else if(debug_uart_id ==4)
        {
        	#if 0
            flags = EVENT_FLAGS_UART1;
            flags = osEventFlagsWait(evt_id_uart1, flags, osFlagsWaitAny, 1000);//osWaitForever
            //flags = osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2, osFlagsWaitAny, osWaitForever);
            //handle event
            // debug_uart_id = PRINTF_UART_ID;
            printf("*****uart cb evt_id_uart1 flags =0x%08x\r\n",flags);
            if(flags != osFlagsErrorTimeout)
            {

                if((flags&EVENT_FLAGS_UART1) == EVENT_FLAGS_UART1)
                {
                    uart1_rec_rs485_response();

                    char*uart_buf = ( char*)RxBuffer1;
                    uint8_t uart_buf_len = RxCounter1;
                    app_rs485_broker_id_response_parse(uart_buf,uart_buf_len);

                    app_uart1_rs485_rxtx_io_low_is_rx();
                    reset_uart1_rx_buffer();
#if (UART1_RX_DMA ==1)
                    DMA_Enable(DMA1_Channel5,UART1_RX_BUFFER_LEN);//开启下一次DMA接收
#endif
                }
            }
			#endif
            flags = EVENT_FLAGS_UART4|EVENT_FLAGS_UART4_TX_COMPLETE|EVENT_FLAGS_UART5;
            flags = osEventFlagsWait(evt_id_uart4, flags, osFlagsWaitAny, 1000);
            //flags = osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2, osFlagsWaitAny, osWaitForever);
            //handle event
            // debug_uart_id = PRINTF_UART_ID;
            printf("*****uart cb evt_id_uart4 flags =0x%08x\r\n",flags);
            if(flags != osFlagsErrorTimeout)
            {
                if((flags&EVENT_FLAGS_UART4) == EVENT_FLAGS_UART4)
                {
                    printf("uart%d rx debug command\r\n",debug_uart_id);
                    handle_uart_debug_cmd();
                }
#if 1
        if((flags&EVENT_FLAGS_UART5) == EVENT_FLAGS_UART5)
        {
            //rx at 485 response from broker
            printf("uart5 rx 485 response from broker\r\n");
            uart5_rec_rs485_response();
        }
#endif
            }
            osDelay(1000);
        }

        //else if(flags == EVENT_FLAGS_UART2)

        //debug_uart_id = 0xff;

        //  tick_cur = osKernelGetTickCount();
        //  printf("##########thread_uart_cb osKernelGetTickCount =%d\r\n",tick_cur);

        // osDelayUntil (tick_cur+500);
        //osThreadYield();
        //osDelay(1000);


    }
}
__NO_RETURN void thread_main_cb (void *argument)
{
    uint32_t tick_cur = 0;
    static uint8_t led_staus = 0;
//    static uint8_t event_index =MCU_EVENT_AUDIO_NUM_1;
    uint32_t flags = 0;
    /*
    SetDate_Alarm(1);
    SetDate(3,6,2019);
    SetAlarm(0,0,10);
    */
    const char * event_name = NULL;

    evt_id_app =osEventFlagsNew(&os_event_flag_app);
    if (evt_id_app == NULL)
    {
        // Failed to get the event flags object name
    }
    else
        printf("osEventFlagsNew  evt_id_app=0x%08x\r\n",(unsigned int)evt_id_app);


    event_name = osEventFlagsGetName(evt_id_app);
    if (event_name)
    {
        // Failed to get the event flags object name
        printf("osEventFlagsGetName  event_name=%s\r\n",event_name);
    }

    printf("osEventFlagsGet  evt_id_app=0x%08x\r\n",osEventFlagsGet(evt_id_app));
    osDelay(1000);

    // IWDG_Config_Timeout(WDT_TIMEOUT_S);
    uart_printf("IWDG_Config_Timeout get_curtime=%d",get_curtime());
    //close printf
    //debug_uart_id = 0xff;

    gpio_dc_detect_exti_config();
    for (;;)
    {
        tick_cur = osKernelGetTickCount();
        // debug_uart_id = PRINTF_UART_ID;
        led_staus ++;
        if(led_staus >=20)
        {
            led_staus =0;
            printf("**********thread_main osKernelGetTickCount =%d\r\n",tick_cur);
            //dc_lte_runtime_valid_judge();
        }
        //  IWDG_Feed();
        //flags = osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART, osFlagsWaitAny, osWaitForever);
        //handle event
        //printf("*****osEventFlagsWait flags =0x%08x\r\n",flags);
        //st_rtc_show();
        //RTC_Application();
#if 0
        if(led_staus == 10)
        {
            led_staus = 0;
            gpio_led_on();
        }
        else
        {
            led_staus ++;
            gpio_led_off();
        }
#endif
        //gpio_key_value();

        //get_ntc_temp_adc1_chn9();


#if( USE_SOFTTIMER_DIY ==1)
        time_task_query();
#endif
        mcu_check_event();

        //restart_timer_trig_dma_pcm();
        flags = EVENT_FLAGS_APP;

        //		EVENT_FLAGS_APP_KEY_ENTER_SHORT|  \
        ////       EVENT_FLAGS_APP_DC1_INSERT|EVENT_FLAGS_APP_DC1_REMOVE| \
        //       EVENT_FLAGS_APP_DC2_INSERT|EVENT_FLAGS_APP_DC2_REMOVE| \
        //       ;
        flags = osEventFlagsWait(evt_id_app, flags, osFlagsWaitAny, 1000);
		//flags = osEventFlagsWait(evt_id_app, flags, osFlagsWaitAny, 2);
        //printf("osErrorTimeout=0x%08x %d\r\n",osErrorTimeout,osErrorTimeout);
        if((int32_t)flags == osError)
            printf("main thread flags osError\r\n");
        else if((int32_t)flags == osErrorTimeout)
            printf("main thread flags osErrorTimeout=%d\r\n",get_curtime2());
        else
        {
            printf("main thread flags=0x%x\r\n",flags);
            if((flags &EVENT_FLAGS_APP_KEY_ENTER_SHORT)==EVENT_FLAGS_APP_KEY_ENTER_SHORT)
            {
                printf("key enter short\r\n");
                //key_driver();
                //rs485 add mode
                if( charge_mode == CHARGE_MODE_BROKER )
                {
                    printf("rs485 add mode\r\n");
					dx_set_mqtt_thread_stop();
					reset_uart1_rx_buffer();
#if (UART1_RX_DMA ==1)
					DMA_Enable(DMA1_Channel5,UART1_RX_BUFFER_LEN);//开启下一次DMA接收
#endif
					app_uart1_rs485_rxtx_io_low_is_rx();
				
                    rs485_set_start();
                }
				#if 0
                if(play_audio_status() ==0)
                {
                    printf("event_index=%d\r\n",event_index);
                    SET_EVENT(event_index);
                    event_index ++;
                    if(event_index > MCU_EVENT_TOP)
                    {
                        event_index = MCU_EVENT_AUDIO_NUM_1;
                    }
                }
				#endif
                //gpio_led_tog();

            }
            if((flags &EVENT_FLAGS_APP_KEY_ENTER_LONG)==EVENT_FLAGS_APP_KEY_ENTER_LONG)
            {
                printf("key enter long\r\n");
                mcu_sys_soft_reset();
            }
            
            if((flags &EVENT_FLAGS_APP_KEYBOARD_ON)==EVENT_FLAGS_APP_KEYBOARD_ON)
            {
                printf("On keyboard  dc_port_status = %d\r\n",charge_Info.dc_port_status[Onkey_index]);
                 printf("charge_tm===== = %d\r\n",charge_Info.charge_tm[Onkey_index]);
                if(charge_Info.dc_port_status[Onkey_index] == PORT_USEING)//
                {
                    uint32_t cur_time_sec = 0;
                    uint16_t charge_left_tm = 0;

                    cur_time_sec = RTC_GetCounter();//need dw							
                    charge_left_tm =  (charge_Info.charge_tm[Onkey_index]-(cur_time_sec-charge_Info.start_time[Onkey_index])/60);
                    if(charge_mode == CHARGE_MODE_BROKER )
                    {
                        //app_rs485_broker_start_power(Onkey_index+1);
                        charge_Info.charge_left_tm[Onkey_index] =charge_left_tm;
                    }
                    else
                    {
                        dc_start_power(Onkey_index+1,charge_left_tm);
                    }
                                      
                 }
                else if(charge_Info.dc_port_status[Onkey_index] <= PORT_FREE)
                {
                    cJSON *root =NULL, *sta_item, *ch_msg_item;
                    //[09-24-14:42:38]receivedTopic=dc/cs/set/865860040521701{"did":"865860040521701","st":[{"apow":400,"cn":0,"ipow":20,"opt":480,"sst":1,"tck":60}],"t":1569307271} len=25
                    root = cJSON_CreateObject();
                    cJSON_AddStringToObject(root, "did", client_id);
                    cJSON_AddNumberToObject(root, "t",RTC_GetCounter());

                    sta_item = cJSON_CreateArray();
                    if(sta_item == NULL)
                    {
                        printf("thread main sta_item err\r\n");
                        cJSON_Delete(root);
                        root =NULL;
                    }
                    else
                    {
                        cJSON_AddItemToObject(root, "st", sta_item);
                    }
                    ch_msg_item = cJSON_CreateObject();
                    if(ch_msg_item == NULL)
                    {
                        printf("thread main ch_msg_item err\r\n");
                        cJSON_Delete(root);
                        root =NULL;
                    }
                    else
                    {
                        cJSON_AddItemToArray(sta_item, ch_msg_item);
                        cJSON_AddNumberToObject(ch_msg_item,"cn", Onkey_index);
                        cJSON_AddNumberToObject(ch_msg_item,"sst", 1);
                        cJSON_AddNumberToObject(ch_msg_item,"apow", 400);
                        cJSON_AddNumberToObject(ch_msg_item,"ipow", 20);
                        cJSON_AddNumberToObject(ch_msg_item,"tck", 60);
                        cJSON_AddNumberToObject(ch_msg_item,"opt", charge_Info.charge_tm[Onkey_index]);
                    }

                    if(root != NULL)
                    {
                        char * out = cJSON_Print(root);
                        if(out)
                        {
                            //char payload_in[200]={0};
                            
                            //strcat(payload_in, sub_topics[0]);
                            //strcat(payload_in, out);
                            printf("create js string is %s\r\n",out);
                            json_parse((char*)out,sub_topics[0]);
                            myfree(out);
                        }
                        cJSON_Delete(root);
                    }
                }
            }


            if((flags &EVENT_FLAGS_APP_DC1_REMOVE)==EVENT_FLAGS_APP_DC1_REMOVE)
            {
                printf("main dc1 remove\r\n");

            }
            if((flags &EVENT_FLAGS_APP_DC1_INSERT)==EVENT_FLAGS_APP_DC1_INSERT)
            {
                printf("main dc1 insert\r\n");


            }

            if((flags &EVENT_FLAGS_APP_DC2_REMOVE)==EVENT_FLAGS_APP_DC2_REMOVE)
            {
                printf("main dc2 remove\r\n");

            }
            if((flags &EVENT_FLAGS_APP_DC2_INSERT)==EVENT_FLAGS_APP_DC2_INSERT)
            {
                printf("main dc2 insert\r\n");

            }
            if((flags &EVENT_FLAGS_APP_SMOKE_DETECT)==EVENT_FLAGS_APP_SMOKE_DETECT)
            {
                printf("main SMOKE_DETECT\r\n");
				osDelay(300);
                //if(gpio_smoke_detect_value() ==1)
               // SET_EVENT(MCU_EVENT_AUDIO_SMOKE_TOO_HIGH);
              //  SET_EVENT(MCU_EVENT_AUDIO_SAFE);
				uint8_t gpio_value = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);
				uart_printf("int GPIO_Pin_9=%d",gpio_value);

				if(gpio_value == 1)
					set_smoke_happen();


                gpio_led_on();
            }
            if((flags &EVENT_FLAGS_APP_SMOKE_DETECT_NO)==EVENT_FLAGS_APP_SMOKE_DETECT_NO)
            {
                printf("main SMOKE_DETECT_NO\r\n");
                //if(gpio_smoke_detect_value() ==1)
                gpio_led_off();

            }
            if((flags &EVENT_FLAGS_APP_DC_COMPLETE)==EVENT_FLAGS_APP_DC_COMPLETE)
            {
                printf("DC_COMPLETE\r\n");
            }
            if((flags &EVENT_FLAGS_APP_DC_REPORT_ERROR)==EVENT_FLAGS_APP_DC_REPORT_ERROR)
            {
                printf("DC_REPORT_ERROR\r\n");
            }

        }
        extern volatile uint8_t pc_4g_debug_on;
        if(pc_4g_debug_on ==1 )
        {
            flags = EVENT_FLAGS_UART2|EVENT_FLAGS_UART2_TX_COMPLETE;

            flags = osEventFlagsWait(evt_id_uart, flags, osFlagsWaitAny, osWaitForever);
            //flags = osEventFlagsWait(evt_id_uart, EVENT_FLAGS_UART2, osFlagsWaitAny, osWaitForever);
            //handle event
            // debug_uart_id = PRINTF_UART_ID;
            printf("***** osEventFlagsWait flags =0x%08x\r\n",flags);
            if(flags == EVENT_FLAGS_UART2)
            {
                //rx at 485 response from broker
                printf("uart2_rec_at_cmd_response\r\n");
                uart2_rec_at_cmd_response(NULL,NULL);
            }

        }
        else
        {
            osDelay(500);
        }

        //osDelay(10);
    }
}
/*----------------------------------------------------------------------------
 *      Memory Pool creation & usage
 *---------------------------------------------------------------------------*/
#define MEMPOOL_OBJECTS 16                      // number of Memory Pool Objects
typedef struct {                                // object data type
    uint8_t Buf[32];
    uint8_t Idx;
} MEM_BLOCK_t;
osMemoryPoolId_t mpid_MemPool;                  // memory pool id
int Init_MemPool (void) {

    const char * pname =NULL;
    mpid_MemPool = osMemoryPoolNew(MEMPOOL_OBJECTS, sizeof(MEM_BLOCK_t), NULL);
    if (mpid_MemPool == NULL)
    {
        ; // MemPool object not created, handle failure
    }
    else
        printf("osMemoryPoolNew  mpid_MemPool=0x%08x\r\n",(unsigned int)mpid_MemPool);
    pname = osMemoryPoolGetName(mpid_MemPool);
    if(pname)
        printf("osMemoryPoolGetName=0x%s\r\n",pname);

    printf("MemoryPool GetCapacity=%d BlockSize=%d GetCount=%d GetSpace=%d\r\n",\
           osMemoryPoolGetCapacity(mpid_MemPool),\
           osMemoryPoolGetBlockSize(mpid_MemPool),\
           osMemoryPoolGetCount(mpid_MemPool),\
           osMemoryPoolGetSpace(mpid_MemPool));
    return(0);
}
#define MESSAGE_QUEUE_OBJECTS 16                      // number of Memory Pool Objects
#define MESSAGE_QUEUE_OBJECT_SIZE 4                      // number of Memory Pool Objects

osMessageQueueId_t message_queue_id = 0;
typedef struct {								 // object data type
    uint8_t Buf[8];
    uint8_t Idx;
} MSGQUEUE_OBJ_t;


int init_message_queue (void)
{
    const char * pname =NULL;
    MSGQUEUE_OBJ_t msg;
    osStatus_t status ;
    message_queue_id = osMessageQueueNew(MESSAGE_QUEUE_OBJECTS, sizeof(MSGQUEUE_OBJ_t), NULL);
    if (message_queue_id == NULL)
    {
        ; // MemPool object not created, handle failure
    }
    else
        printf("osMessageQueueNew =0x%08x\r\n",(unsigned int)message_queue_id);
    pname= osMessageQueueGetName(message_queue_id);
    if(pname)
        printf("osMessageQueueGetName=0x%s\r\n",pname);

    printf("MessageQueue GetCapacity=%d MsgSize=%d GetCount=%d GetSpace=%d\r\n",\
           osMessageQueueGetCapacity(message_queue_id),\
           osMessageQueueGetMsgSize(message_queue_id),\
           osMessageQueueGetCount(message_queue_id),\
           osMessageQueueGetSpace(message_queue_id));

    memset(&msg,0,sizeof(msg));
    msg.Buf[0] = 0x11U;										 // do some work...
    msg.Buf[7] = 0x88U;										 // do some work...
    msg.Idx	= 0U;
    status = osMessageQueuePut(message_queue_id, &msg, 0U, 0U);
    switch (status)
    {
    case osOK:
        break;
    case osErrorTimeout:
        break;
    case osErrorResource:
        break;
    case osErrorParameter:
        break;

    default:
        break;
    }

    printf("GetCount =%d\r\n",osMessageQueueGetCount(message_queue_id));
    memset(&msg,0,sizeof(msg));
    status = osMessageQueueGet(message_queue_id, &msg, NULL, 0U);	 // wait for message
    switch (status)
    {
    case osOK:
        printf("msg 0=0x%02x 7=0x%02x\r\n",msg.Buf[0],msg.Buf[7]);
        break;
    case osErrorTimeout:
        break;
    case osErrorResource:
        break;
    case osErrorParameter:
        break;

    default:
        break;
    }

    return(0);
}




#if (USE_SOFTTIMER_DIY ==0)


osTimerId timer_id =NULL;
#if defined(RTE_CMSIS_RTOS)
void timer_callback(const void  *pram)

#else
void timer_callback( void	*pram)
#endif
{
    unsigned int case_num = 0;
    //printf("t cb tick =%d\r\n",osKernelGetTickCount());
    MEM_BLOCK_t *pMem;
    osStatus_t status;

    if(pram)
    {
        printf("timer pram=%d\r\n",*(unsigned int*)pram);
        case_num = *(unsigned int*)pram;
    }
    else
        return ;



    pMem = (MEM_BLOCK_t *)osMemoryPoolAlloc(mpid_MemPool, 0U);  // get Mem Block
    if (pMem != NULL)
    {

    }



    if (pMem != NULL)
    {   // Mem Block was available
        pMem->Buf[0] = 0x55U;									  // do some work...
        pMem->Idx	 = 0U;

        status = osMemoryPoolFree(mpid_MemPool, pMem);			  // free mem block
        switch (status)
        {
        case osOK:
            break;
        case osErrorParameter:
            break;
        case osErrorNoMemory:
            break;
        default:
            break;
        }
    }



    switch(case_num)
    {

    case 0:

        //GPIOB->ODR ^= 0x8;


        break;

    case 1:

        //GPIOB->ODR ^= 0x4;
        printf(" One-shoot timer 1=%d\r\n",osKernelGetTickCount());
        break;

    case 2:

        //GPIOB->ODR ^= 0x2;
        printf(" Periodic timer 2=%d\r\n",osKernelGetTickCount());
        break;

    case 3:
        printf(" Periodic timer 3=%d\r\n",osKernelGetTickCount());
        break;
    case 4:
        printf(" Periodic timer 4=%d\r\n",osKernelGetTickCount());
        reset_timer_trig_dma_pcm();
        //restart_timer_trig_dma_pcm();
        break;

    default:
        break;

    }
    //osTimerStart( timer_id, 100 );
}

#if defined(RTE_CMSIS_RTOS)
osTimerDef(timer_data,timer_callback);
osTimerDef (Timer1, timer_callback);                      // define timers
osTimerDef (Timer2, timer_callback);
#endif
uint32_t  exec1;                                           // argument for the timer call back function
uint32_t  exec2;                                           // argument for the timer call back function
uint32_t  exec3;                                           // argument for the timer call back function
uint32_t  exec4;

void TimerCreate_example (void)
{
#if 0
    osTimerId id1;                                           // timer id
    osTimerId id2;                                           // timer id

    // Create one-shoot timer
    exec1 = 1;
#if defined(RTE_CMSIS_RTOS)
    id1 = osTimerCreate (osTimer(Timer1), osTimerOnce, &exec1);
#else
    id1 =osTimerNew(timer_callback,osTimerOnce, &exec1,NULL);
#endif
    if (id1 != NULL)  {
        // One-shoot timer created
        printf("osTimerCreate  id1=0x%08x\r\n",(unsigned int)id1);
    }

    // Create periodic timer
    exec2 = 2;
#if defined(RTE_CMSIS_RTOS)
    id2 = osTimerCreate (osTimer(Timer2), osTimerPeriodic, &exec2);
#else
    id2 =osTimerNew(timer_callback,osTimerPeriodic, &exec2,NULL);
#endif
    if (id2 != NULL)  {
        // Periodic timer created
        printf("osTimerCreate  id2=0x%08x\r\n",(unsigned int)id2);
    }

    printf("cur tick =%d\r\n",osKernelGetTickCount());
    osTimerStart(id1, 500);
    osTimerStart(id2, 1500);


    exec3 =3;
#if defined(RTE_CMSIS_RTOS)
    timer_id = osTimerCreate(osTimer(timer_data),osTimerPeriodic,&exec3);
#else
    timer_id  =osTimerNew(timer_callback,osTimerPeriodic, &exec3,NULL);
#endif
    if (timer_id != NULL)
    {
        // Periodic timer created
        printf("osTimerCreate	id2=0x%08x\r\n",(unsigned int)timer_id);
        printf("3 cur tick =%d\r\n",osKernelGetTickCount());
        osTimerStart(timer_id,1000);
    }
#endif

    init_timer_trig_dma_pcm();
    if(get_status_timer_trig_dma_pcm() == 0)
    {
        printf("timer_trig_dma_pcm is not running\r\n");
    }
    else
    {
        printf("timer_trig_dma_pcm	is running\r\n");
    }

    //restart_timer_trig_dma_pcm();

}
#endif

#if 1

/*----------------------------------------------------------------------------
 *      Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
    osVersion_t version= {0};
    char id_buf[20]= {0};
    uint32_t id_size = sizeof(id_buf);
    osStatus_t ret = osStatusReserved;
    uint32_t tick_cur = 0;
    int32_t sl = osStatusReserved;
    const char *event_name;
    //CreateMutexUart();
    //while(1)
    ret = osKernelGetInfo(&version,id_buf,id_size);
    if(osOK == ret )
    {
        printf("os api=%d kernel=%d\r\n",version.api,version.kernel);
        printf("os id =%s\r\n",id_buf);
    }


    tick_cur = osKernelGetSysTimerCount();
    printf("osKernelGetSysTimerCount =%d\r\n",tick_cur);

    tick_cur = osKernelGetSysTimerFreq();
    printf("osKernelGetSysTimerFreq =%d\r\n",tick_cur);

    tick_cur = osKernelGetTickCount();
    printf("osKernelGetTickCount =%d\r\n",tick_cur);

    tick_cur = osKernelGetTickFreq();
    printf("osKernelGetSysTimerFreq =%d\r\n",tick_cur);

    Init_MemPool();
    init_message_queue();
    //crrate thread
#if 1

    evt_id_uart1 =osEventFlagsNew(&os_event_flag_uart1);
    if (evt_id_uart1 == NULL)
    {
        // Failed to get the event flags object name
    }
    else
        printf("osEventFlagsNew  evt_id_uart1=0x%08x\r\n",(unsigned int)evt_id_uart1);


    event_name = osEventFlagsGetName(evt_id_uart1);
    if (event_name)
    {
        // Failed to get the event flags object name
        printf("osEventFlagsGetName  event_name=%s\r\n",event_name);
    }

    printf("osEventFlagsGet  evt_id_uart1=0x%08x\r\n",osEventFlagsGet(evt_id_uart1));

    evt_id_uart =osEventFlagsNew(&os_event_flag_uart);
    if (evt_id_uart == NULL)
    {
        // Failed to get the event flags object name
    }
    else
        printf("osEventFlagsNew  evt_id_uart=0x%08x\r\n",(unsigned int)evt_id_uart);


    event_name = osEventFlagsGetName(evt_id_uart);
    if (event_name)
    {
        // Failed to get the event flags object name
        printf("osEventFlagsGetName  event_name=%s\r\n",event_name);
    }

    printf("osEventFlagsGet  evt_id_uart=0x%08x\r\n",osEventFlagsGet(evt_id_uart));

    evt_id_uart3 =osEventFlagsNew(&os_event_flag_uart3);
    if (evt_id_uart3 == NULL)
    {
        // Failed to get the event flags object name
    }
    else
        printf("osEventFlagsNew  evt_id_uart3=0x%08x\r\n",(unsigned int)evt_id_uart3);


    event_name = osEventFlagsGetName(evt_id_uart3);
    if (event_name)
    {
        // Failed to get the event flags object name
        printf("osEventFlagsGetName  event_name=%s\r\n",event_name);
    }

    printf("osEventFlagsGet  evt_id_uart3=0x%08x\r\n",osEventFlagsGet(evt_id_uart3));



    evt_id_uart4 =osEventFlagsNew(&os_event_flag_uart4);
    if (evt_id_uart4 == NULL)
    {
        // Failed to get the event flags object name
    }
    else
        printf("osEventFlagsNew  evt_id_uart4=0x%08x\r\n",(unsigned int)evt_id_uart4);


    event_name = osEventFlagsGetName(evt_id_uart4);
    if (event_name)
    {
        // Failed to get the event flags object name
        printf("osEventFlagsGetName  event_name=%s\r\n",event_name);
    }

    printf("osEventFlagsGet  evt_id_uart4=0x%08x\r\n",osEventFlagsGet(evt_id_uart4));

    evt_id_uart5 =osEventFlagsNew(&os_event_flag_uart5);
    if (evt_id_uart5 == NULL)
    {
        // Failed to get the event flags object name
    }
    else
        printf("osEventFlagsNew  evt_id_uart5=0x%08x\r\n",(unsigned int)evt_id_uart5);


    event_name = osEventFlagsGetName(evt_id_uart5);
    if (event_name)
    {
        // Failed to get the event flags object name
        printf("osEventFlagsGetName  event_name=%s\r\n",event_name);
    }

    printf("osEventFlagsGet  evt_id_uart5=0x%08x\r\n",osEventFlagsGet(evt_id_uart5));

    osDelay(1000);


    sl = osKernelLock();
    printf("osKernelLock sl=%d\r\n",sl);
    thread_main = osThreadNew(thread_main_cb, "main", &thread1_attr);    // Create thread with statically allocated stack memory
    printf("osThreadGetId  thread_main=0x%08x\r\n",(unsigned int)thread_main);

    thread_uart_msg = osThreadNew(thread_uart_cb, "uart", &thread2_attr);    // Create thread with statically allocated stack memory
    printf("osThreadGetId  thread_uart_msg=0x%08x\r\n",(unsigned int)thread_uart_msg);
#if defined(RTE_CMSIS_RTOS)
    thread_app = osThreadCreate (osThread(thread_app_cb), "app_m");
#else
    thread_app = osThreadNew(thread_app_cb, "app_m", &thread3_attr);    // Create thread with statically allocated stack memory
    //thread_app = osThreadNew (thread_app_cb, "app", NULL);
#endif
    //printf("osThreadCreate  thread_app=0x%08x\r\n",(unsigned int)thread_app);
    //thread_app = osThreadNew(thread_app_cb, "app", &thread3_attr);    // Create thread with statically allocated stack memory
    printf("osThreadGetId  thread_app=0x%08x\r\n",(unsigned int)thread_app);


    // thread_app_dc12 = osThreadNew(thread_app_dc12_cb, "app_d", &thread4_attr);	  // Create thread with statically allocated stack memory
    // printf("osThreadGetId  thread_app_dc12=0x%08x\r\n",(unsigned int)thread_app_dc12);



    sl = osKernelRestoreLock(sl);
    printf("osKernelRestoreLock sl=%d\r\n",sl);
    osDelay(1000);
    /*
    os_state = osThreadSetPriority(thread_app, osPriorityBelowNormal);  // Set thread priority
    if (os_state == osOK) {
    // Thread priority changed to BelowNormal
    	printf("Thread priority changed to BelowNormal\r\n");
    }
    else {
    // Failed to set the priority
    	printf("Failed to set the priority=%d\r\n",os_state);
    }
    */
#if 0
    thread_uart_msg = osThreadNew(thread_func, "uart", &thread2_attr);    // Create thread with statically allocated stack memory
    printf("osThreadGetId  thread_uart_msg=0x%08x\r\n",(unsigned int)thread_uart_msg);
    os_state = osThreadSetPriority(thread_uart_msg, osPriorityNormal);  // Set thread priority
    if (os_state == osOK) {
        // Thread priority changed to BelowNormal
        printf("Thread priority changed to osPriorityNormal\r\n");
    }
    else {
        // Failed to set the priority
        printf("Failed to set the priority=%d\r\n",os_state);
    }
#endif

#endif


    osDelay(osWaitForever);

    while(1);
}
void STM3F10X_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //Usart1 NVIC 配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	  //设置NVIC中断分组2:2位抢占优先级，2位响应优先级	 0-3;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		 //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		 //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器


    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		 //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		 //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    /* Enable the RTC Alarm Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

#if(UART1_TX_DMA == 1)
    /* Enable DMA1 channel4 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
#if(UART2_TX_DMA == 1)
    /* Enable DMA1 channel7 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#if(UART3_TX_DMA == 1)
    /* Enable DMA1 channel7 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
#if(UART4_TX_DMA == 1)
    /* Enable DMA1 channel7 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

    //NVIC_IRQChannelPreemptionPriority ==1
#if( USE_SOFTTIMER_DIY ==1)
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif


#if ((HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1)||  \
	 (HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2))
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//EXTI15_10_IRQn;			   //????2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   //????? 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			   //????2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			   //??
    NVIC_Init(&NVIC_InitStructure);
#endif
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			   //????3
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   //????? 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			   //????0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			   //??
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the EXTI9-5 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;			   //????9-5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   //????? 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			   //????1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			   //??
    NVIC_Init(&NVIC_InitStructure);

}
void mcu_sys_soft_reset(void)
{

    printf("\r\n\r\n\r\n%s\r\n\r\n\r\n",__FUNCTION__);
    osDelay(1000);
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}
//void (*debug_dma_printf)( char *fmt,...);
volatile  unsigned char charge_mode =CHARGE_MODE_INVALID;
int dx_memmem(char * a, int alen, char * b, int blen)
{
    int i, j;
    for (i = 0; i <= (alen-blen); ++ i)
    {
        for (j = 0; j < blen; ++ j)
        {
            if (a[i + j] != b[j])
            {
                break;
            }
        }
        if (j >= blen)
        {
            return i;
        }
    }
    return -1;
}
#if(PROGRAM_MODE == PROGRAM_IS_APP )

uint32_t runBootloaderAddress;
void (*runBootloader)(void * arg);

#define     __IO    volatile
typedef  void (*pFunction)(void);
//#define APPLICATION_ADDRESS   (uint32_t)0x08040000
pFunction Jump_To_Application;
uint32_t JumpAddress;

#endif
uint32_t checkontime =0;
#if (USE_SOFTTIMER_DIY ==1)
void timer_keyboard_openport( void)
#else
void timer_keyboard_openport( void	*pram)
#endif
{
	if(checkontime)
	{
		if(checkontime > charge_Info.charge_tm[Onkey_index])
		{
			charge_Info.charge_tm[Onkey_index] = checkontime;
			osEventFlagsSet(evt_id_app,EVENT_FLAGS_APP_KEYBOARD_ON);
		}
		checkontime = 0;
	}
	else
	{
		osEventFlagsSet(evt_id_app,EVENT_FLAGS_APP_KEYBOARD_ON);
	}
}
int main (void)
{
    //uint32_t tick_cur = 0;
    char* p = NULL;
//	GPIO_InitTypeDef GPIO_InitStructure;
    //osStatus_t os_state = osStatusReserved;
    osThreadId main_id;//创建线程句柄
    uint32_t FlashID;
    unsigned char*pcm_flash_address;
    unsigned int pcm_flash_len;
    unsigned char ret =0;

    char test_signed = 0xff;
    signed char test_signed3 = 0xff;

    uint8_t test_signed2 = 0;
    test_signed2 = test_signed;
    // System Initialization
    SystemCoreClockUpdate();
#if(PROGRAM_MODE == PROGRAM_IS_APP )
    NVIC_SetVectorTable(NVIC_VectTab_FLASH,APP_OFFSET);
#endif


#if 0//defined(RTE_Compiler_EventRecorder)
    EventRecorderInitialize(EventRecordAll, 1);  // initialize and start Event Recorder
    EventRecorderStart();
#endif
#ifdef MCU_USING_UTILS_STACK_CHK
    system_stack_chk_init();
#endif

    osKernelInitialize();  // Initialize CMSIS-RTOS
    // ...
#if 0
    /* 初始化 SysTick 滴答定时器*/
    if(SysTick_Config(SystemCoreClock/1000))//1s/x=所设置的时间间隔   本程序x=1000,故1/1000=1ms;
    {
        //如果设置失败则进入死循环
        while (1);
    }
#endif
    //rcc set
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                           |RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD
                           |RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|
                           RCC_APB1Periph_UART4|RCC_APB1Periph_UART5, ENABLE);
    /*Enables the clock to Backup and power interface peripherals    */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1|RCC_AHBPeriph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    //nvic init
    FlashID=NVIC_GetPriorityGrouping();
    STM3F10X_NVIC_Configuration();
#if (USE_SOFTTIMER_DIY ==1)
    //init_tim1_for_softtimer(1000);
    init_tim1_for_softtimer(100);
#endif
    gpio_dc_detect_init();
    gpio_dc_detect_value();
    if(dc1_exist == 1 && dc2_exist ==1)//都不存在，RS 485 断路器
    {
        debug_uart_id = PRINTF_UART_ID_4;
    }
    else
    {

        debug_uart_id = PRINTF_UART_ID_1;
    }

    //debug_uart_id = PRINTF_UART_ID_1;
    //debug_uart_id = PRINTF_UART_ID_4;

#if ((HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1) || \
	  (HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2) )
    //dc
    if(debug_uart_id == PRINTF_UART_ID_1)
    {
        charge_mode = CHARGE_MODE_DC ;
        //debug
        gpio2uart1_pin_config();
        USART_Config(USART1);

        //debug_dma_printf = uart1_dma_printf;

        //init dc1 uart

        gpio2uart3_pin_config();
        USART_Config(USART3);
        //init dc2 uart

        gpio2uart4_pin_config();
        USART_Config(UART4);

#if 1
		mcu_uart5_control_init();
		gpio2uart5_pin_config();
		USART_Config(UART5);
		app_uart5_rs485_rxtx_io_init();
		app_uart5_rs485_rxtx_io_low_is_rx();
				//	app_uart5_rs485_rxtx_io_high_is_tx();
#endif


        printf("*************cur is dc ,%d,%d\r\n",dc1_exist,dc2_exist);
        if(dc1_exist == 0)
            ta6932_channel_num_init(10);

        if((dc1_exist == 0) &&(dc2_exist == 0))
            ta6932_channel_num_init(20);

    }
    //rs485 broker
    else if(debug_uart_id == PRINTF_UART_ID_4)
    {
        //debug
        charge_mode = CHARGE_MODE_BROKER ;
        gpio2uart4_pin_config();
        USART_Config(UART4);

        //debug_dma_printf = uart4_dma_printf;



        printf("*************cur is rs485 broker\r\n");
        printf("*************dc ,%d,%d\r\n",dc1_exist,dc2_exist);
        ta6932_channel_num_init(10);

#if 1
        //broker
        //uart1  init RS585 uart for broker

        gpio2uart1_pin_config();
        USART_Config(USART1);
        app_uart1_rs485_rxtx_io_init();
        app_uart1_rs485_rxtx_io_low_is_rx();

        reset_uart1_rx_buffer();
#if (UART1_RX_DMA ==1)
        DMA_Enable(DMA1_Channel5,UART1_RX_BUFFER_LEN);//开启下一次DMA接收
#endif


        //app_uart1_rs485_rxtx_io_high_is_tx();
#if 1
		mcu_uart5_control_init();
        gpio2uart5_pin_config();
        USART_Config(UART5);
        app_uart5_rs485_rxtx_io_init();
        app_uart5_rs485_rxtx_io_low_is_rx();
        //	app_uart5_rs485_rxtx_io_high_is_tx();
#endif


#if 0
        app_uart1_rs485_rxtx_io_high_is_tx();

        while(1)
        {
            printf("printf 1234567890asdfghjkl\r\n");
            uart4_dma_printf("uart4_dma_printf 1234567890asdfghjkl\r\n");
            uart4_cpu_printf("uart4_cpu_printf 1234567890asdfghjkl\r\n");


            uart1_cpu_printf("uart1_cpu_printf 1234567890asdfghjkl\r\n");
            //uart5_cpu_printf("uart5_cpu_printf 1234567890asdfghjkl\r\n");
        }
#endif

#endif

        //read id info from ext flash
    }
#endif
#if 0
//strlen(HTTPURL_JD_TEST)=84 sizeof(HTTPURL_JD_TEST)=85
    printf("strlen(HTTPURL_JD_TEST)=%d sizeof(HTTPURL_JD_TEST)=%d\r\n",strlen(HTTPURL_JD_TEST),sizeof(HTTPURL_JD_TEST));
//test
    char file_read_ack[]= {0x0d,0x0a,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x20,
                           0x31,0x36,0x0d,0x0a,

                           0x47,0x49,0x46,0x38,0x39,0x61,
                           0x0e,0x01,0x81,0x00,0xb3,0x00,0x00,0xf1,0xec,0xf7,

                           0x0d,0x0a,0x4f,0x4b,0x0d,0x0a,0x00
                          };

    if (strlen((const char*)file_read_ack) > 0 && strstr((const char*)file_read_ack, "OK"))
    {
        printf("file_read_ack ok\r\n");
    }
    else
    {
        printf("strstr fail=%d\r\n",strlen((const char*)file_read_ack));
#if 1
        //void *memmem(const void *haystack_start, size_t haystack_len, const void *needle_start, size_t needle_len);
        int res  =dx_memmem(file_read_ack,sizeof(file_read_ack),"OK",2);
        if(res < 0)
        {
            printf("memmem fail\r\n");
        }
        else
        {
            printf("memmem OK=%d\r\n",res);
        }
#endif
    }
    json_parse_set_update_test(HTTPURL_BAIDU);
#endif
    //hex_dump_test();
    {
        float sp = 36.516470;
        //
        //		sp=36.500000
        sp=( (float)( (int)( (sp+0.005)*100 ) ) )/100;
        printf("sp=%f\r\n",sp);
    }
    //init 4g uart
#if 0
    gpio2uart2_pin_config();
    gpio2uart2_txpin_config_pc4g_debug(1);
#else
    gpio2uart2_pin_config();
    USART_Config(USART2);
    //gpio2uart2_txpin_config_pc4g_debug(1);
#endif


//	gpio2uart3_pin_config_gpio(0);//tx 1 rx0
    //gpio2uart3_pin_config_gpio(0);//tx 1 rx0
    //gpio2uart4_pin_config_gpio(0);
    //while(1);

    //uart1_dma_printf("uart1_dma_printf send");
    //uart2_dma_printf("uart2_dma_printf send");
    //dc_tx_rx_data_test();
#if 0
    while(1)
    {
        //uart5_cpu_printf("uart5_cpu_printf send");
        //if(RxCounter5)
        {

            uart4_cpu_printf("RxBuffer4=%s RxCounter4=%d\r\n",RxBuffer5,RxCounter5);

            //uart5_rec_rs485_response();
            //RxCounter5 = 0;
        }
        osDelay(1000);
    }
#endif


    //while(1);
    /*
    osDelay(300);
    osDelay(300);
    osDelay(300);
    osDelay(300);
    app4g_reset_io_low();
    */
    //while(1);



#if 0
    while(1)
    {
        //uart5_cpu_printf("uart5_cpu_printf send");
        //if(RxCounter5)
        {

            uart5_cpu_printf("RxBuffer5=%s RxCounter5=%d\r\n",RxBuffer5,RxCounter5);

            //uart5_rec_rs485_response();
            //RxCounter5 = 0;
        }
        osDelay(1000);
    }
#endif
#if 0

    printf("uart1 work normal\r\n");
    uart2_dma_printf("uart2 work normal\r\n");
    while(1)
    {
        //uart5_cpu_printf("uart5_cpu_printf send");
        if(RxCounter1)
        {

            uart2_dma_printf("RxCounter1=%d\r\n",RxCounter1);

            if(RxCounter1 ==16)
                handle_uart_debug_cmd();
            RxCounter1= 0;
        }
        if(RxCounter5)
        {

            uart2_dma_printf("RxCounter5=%d\r\n",RxCounter5);
            RxCounter5 = 0;
        }
        //handle_uart_debug_cmd();
        osDelay(1000);
    }
#endif



#if 0//(UART1_TX_DMA == 1)
    //while(1)
    {
        uint16_t len_u16 =0;
        len_u16 = DMA_GetCurrDataCounter(DMA1_Channel4);
        uart1_dma_printf("DMA1_Channel4 =%d\r\n",len_u16);

        len_u16 = DMA_GetCurrDataCounter(DMA1_Channel7);
        uart1_dma_printf("DMA1_Channel7 =%d\r\n",len_u16);

        uart1_dma_send_data("USART1 dma send test\r\n",strlen("USART1 dma send test\r\n"));
        if(USART_GetITStatus(USART1,USART_IT_TC)!= RESET)
        {
            uart1_dma_send_data("USART1 USART_IT_TC\r\n",strlen("USART1 USART_IT_TC\r\n"));
        }
        if(USART_GetITStatus(USART1,USART_IT_TXE)!= RESET)
        {
            uart1_dma_send_data("USART1 USART_IT_TXE\r\n",strlen("USART1 USART_IT_TXE\r\n"));
        }
        if(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= RESET)
        {
            uart1_dma_send_data("USART1 USART_FLAG_TC\r\n",strlen("USART1 USART_FLAG_TC\r\n"));
        }
        if(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!= RESET)
        {
            uart1_dma_send_data("USART1 USART_FLAG_TXE\r\n",strlen("USART1 USART_FLAG_TXE\r\n"));
        }

        printf("uart1_cpu_printf TxCounter1 1 =%d\r\n",TxCounter1);
        uart1_dma_printf("uart1_dma_printf TxCounter1 1 =%d\r\n",TxCounter1);
        DMA_SetCurrDataCounter(DMA1_Channel7,0);
        len_u16 = DMA_GetCurrDataCounter(DMA1_Channel7);
        uart1_dma_printf("uart1_dma_printf DMA1_Channel7 =%d\r\n",len_u16);
        uart1_dma_printf("TxCounter1 2=%d\r\n",TxCounter1);


        uart2_cpu_printf("uart2_cpu_printf TxCounter1 1 =%d\r\n",TxCounter1);
        uart2_dma_printf("uart2_dma_printf TxCounter1 1 =%d\r\n",TxCounter1);

    }
    while(1);
#endif
    //uart1_dma_printf("uart1_dma_printf\r\n");

    //debug_dma_printf("uart1_dma_printf\r\n");
    printf("PriorityGrouping=%d\r\n",FlashID);

    printf("HWSerial_SoftVersion=0x%04x VER_PRO_NET=%s\r\n",HWSERIAL_SOFTVERSION,VER_PRO_NET);
    printf("stmcu bin build time is %s :%s\r\n",__DATE__,__TIME__);
    uart_printf("STM32F10x_StdPeriph_Driver V%d.%d.%d",__STM32F10X_STDPERIPH_VERSION_MAIN, \
                __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
#ifdef MCU_USING_UTILS_STACK_CHK
    dump_app_info();
#endif
#if 1
    {
        uint8_t clk_sou;
        RCC_ClocksTypeDef RCC_Clocks = {0};
        uart_printf(" HSE_VALUE=%d",HSE_VALUE);
        RCC_GetClocksFreq(&RCC_Clocks);


        uart_printf(" SYSCLK_Frequency=%d",RCC_Clocks.SYSCLK_Frequency);
        uart_printf(" HCLK_Frequency=%d",RCC_Clocks.HCLK_Frequency);
        uart_printf(" PCLK1_Frequency=%d",RCC_Clocks.PCLK1_Frequency);
        uart_printf(" PCLK2_Frequency=%d",RCC_Clocks.PCLK2_Frequency);
        uart_printf(" ADCCLK_Frequency=%d",RCC_Clocks.ADCCLK_Frequency);
        clk_sou = RCC_GetSYSCLKSource();
        switch(clk_sou)
        {
        case 0x00:
            uart_printf("HSI used as system clock");
            break;
        case 0x04:
            uart_printf("HSE used as system clock");
            break;
        case 0x08:
            uart_printf("PLL used as system clock");
            break;
        default:
            break;
        }
    }
    /*
        //mco test
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					 //PA8配置为通用推挽输出
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        //RCC_MCOConfig(RCC_MCO_HSE);
        //RCC_MCOConfig(RCC_MCO_SYSCLK);
        RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);
        while(1);
    	*/
#endif

    printf("get_curtime2()=%d\r\n",get_curtime2());
    printf("test_signed=%d\r\n",test_signed);
    printf("test_signed2=%d\r\n",test_signed2);
    printf("test_signed3=%d\r\n",test_signed3);
    printf("get_curtime2()=%d\r\n",get_curtime2());
#if (osCMSIS < 0x20000U)
    printf("(osCMSIS < 0x20000U)  osCMSIS=0x%x\r\n",osCMSIS);
#else
    printf("(osCMSIS >= 0x20000U) osCMSIS=0x%x\r\n",osCMSIS);
#endif
#if(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32DEMO)
    printf("(HW_BOARD_TYPE	==	HW_BOARD_TYPE_IS_STM32DEMO\r\n");
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1)
    printf("HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT1\r\n");
#elif(HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2)
    printf("HW_BOARD_TYPE  ==  HW_BOARD_TYPE_IS_STM32PRODUCT_V2\r\n");
#endif
    // strlen=40
    printf("baidu strlen=%d",strlen("https://www.baidu.com/img/baidu_logo.gif"));
    printf("download strlen=%d",strlen("http://common-download.oss-cn-hangzhou.aliyuncs.com/jindoo/charge-1.0.0.4-ENCODE.bin"));

    /* Check if the system has resumed from IWDG reset */

    //jlink download and sw reset
    if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
        //这是外部RST管脚复位
        uart_printf("RCC_FLAG_PINRST\r\n");
        /* Clear reset flags */
        RCC_ClearFlag();

    }
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
        //这是上电复位
        uart_printf("RCC_FLAG_PORRST\r\n");
        /* Clear reset flags */
        RCC_ClearFlag();

    }

    if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
    {
        //这是软件复位
        uart_printf("RCC_FLAG_SFTRST\r\n");
        /* Clear reset flags */
        RCC_ClearFlag();

    }
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
        /* IWDGRST flag set */
        /* Turn on LED1 */
        // STM_EVAL_LEDOn(LED1);
        uart_printf("RCC_FLAG_IWDGRST\r\n");
        /* Clear reset flags */
        RCC_ClearFlag();
    }
    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
    {
        uart_printf("RCC_FLAG_WWDGRST\r\n");
        /* Clear reset flags */
        RCC_ClearFlag();
    }
    if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
    {
        uart_printf("RCC_FLAG_LPWRRST\r\n");
        /* Clear reset flags */
        RCC_ClearFlag();
    }
    //test
    // charge_mg_init();

    app4g_standby_io_init();
    app4g_standby_io_low();
    printf("app4g_standby_io_low get_curtime2()=%d\r\n",get_curtime2());
    //osDelay(1000);
    Delay1ms(100);
    app4g_standby_io_high();
    printf("app4g_standby_io_high get_curtime2()=%d\r\n",get_curtime2());
//	app4g_standby_io_low get_curtime2()=8
//	app4g_standby_io_high get_curtime2()=8

    app4g_reset_start_time();
    app4g_reset_io_init();
#if 1
    app4g_reset_io_high();
    //osDelay(300);
    Delay1ms(30);
    app4g_reset_io_low();

#else
    app4g_reset_io_low();
    osDelay(300);
    app4g_reset_io_high();
#endif



#if 1

    Get_ChipInfo();
#ifdef __CC_ARM
    uart_printf("define __CC_ARM");
#else
    uart_printf(" no define __CC_ARM");
#endif

#ifdef __GNUC__
    uart_printf("define __GNUC__");
#else
    uart_printf(" no define __GNUC__");
#endif
#if defined(__NO_INIT)
    uart_printf("define __NO_INIT");
#else
    uart_printf(" no define __NO_INIT");
#endif

    debug();
    //get_dbgmcu_value();
    //set_dbgmcu_value(8);
#if (STM_RTC_ENABLE ==1)
    //st_BackupReg_init();
    // st_rtc_init();
    RTC_Configuration();
    __nop();
    printf(" RTC_Configuration end\r\n");
    //RTC_NVIC_Configuration_calendar();
    //SetAlarm(0,0,50);
    //SetDate_Alarm(0);
    //SetDate(3,6,2019);

    //SetDate_Alarm(1);
    //SetDate(3,6,2019);
#endif
#endif
    printf("led init\r\n");
    gpio_led_init();
    gpio_led_on();
    printf("key init\r\n");
    gpio_key_init();
    gpio_key_exti_config();
    gpio_key_value();

#if 1
#if (USE_SOFTTIMER_DIY ==1)
    /*time_list*/
    time_list_init();
    //time_register( timer_dma_pcm_data,1, 1, ONCE_TIME, TIME_RUN_MODE_TASK,&timer_dma_pcm);
    time_register( timer_dma_pcm_data,1, 1, ONCE_TIME, TIME_RUN_MODE_ISR,&timer_dma_pcm);
    time_stop(timer_dma_pcm);

    time_register( timer_key_detect,1, 2, PERIOD_TIME, TIME_RUN_MODE_ISR,&timer_detect);
    time_stop(timer_detect);


    led_poweron();
    time_register( mcu_led_y_fast_ctrl, 100,3, PERIOD_TIME, TIME_RUN_MODE_ISR,&timer_y_ctrl );
    time_start( timer_y_ctrl );

    time_register( timer_keyboard_openport,1, 3, ONCE_TIME, TIME_RUN_MODE_ISR,&timer_kb_openport);
    time_stop(timer_kb_openport);
 
    //rs485_set_start();
    time_list_flag_start();
#else
    TimerCreate_example();
#endif

    printf("gpio_smoke_detect_init\r\n");
    gpio_smoke_detect_init();
    gpio_smoke_detect_exti_config();
    if(gpio_smoke_detect_value()==1)
        gpio_led_on();
    else
        gpio_led_off();

    printf("NTC TEMP ADC  init\r\n");
    st_adc_init();
    printf("os init\r\n");

    p=malloc(128);
    if(p ==NULL)
        printf(" malloc fail\r\n");
    else
    {

        printf(" malloc ok\r\n");

        p = realloc(p,256);
        if(p ==NULL)
            printf(" realloc fail\r\n");
        else
        {

            printf(" realloc ok\r\n");
            free(p);
            p = NULL;
        }


    }
    dx_cjson_init();
#if 0
    DS18B20_GPIO_Output_test();
#else
    ret = DS18B20_Init();
    printf("DS18B20_Init ret=%d\r\n",ret);
    if(ret == 0)
    {
        printf("DS18B20_Init ok\r\n");
        //main_ds18b20_test();
		int i = 0;
		uint8_t temp_ds18b20[8]= {0};
		
		read_serial(temp_ds18b20);
		for(i=0; i<8; i++)
			printf("temp_ds18b20[%d]=0x%02x",i,temp_ds18b20[i]);
		printf("\r\n");
		memcpy(serial_4,temp_ds18b20,8);
        float  f_tem = DS18B20_Get_Temp(NULL,4);
        printf("DS18B20_Get_Temp %f\r\n",f_tem);
    }
    TA6932_Init();
    printf("TA6932_Init \r\n");
    {
        //u16 number[TA6932_LEDCOUNT]= {999,888,777,666,555,444,333,222,111,321};
        u16 number[TA6932_LEDCOUNT]= {1,2,3,4,5,6,7,8,9,10,
                                      11,12,13,14,15,16,17,18,19,20
                                     };
        printf("TA6932_DisplayAllNumber \r\n");
        TA6932_DisplayAllNumber(number);
    }
    //while(1);
#endif
    //for spi flash test
    main_ext_spi_flash();
    //while(1);
#if 0
    /* Initialize the SPI FLASH driver */
    sFLASH_Init();

    /* Get SPI Flash ID */
    FlashID = sFLASH_ReadID();
    //  u8 fac_id= 0;//BFH: 工程码SST
    //u8 dev_id= 0;//41H: 器件型号SST25VF016B

    //printf("fac_id=0x%x,dev_id=0x%x",fac_id,dev_id);
    printf("FlashID=0x%08x\r\n",FlashID);

    /* Check the SPI Flash ID */
    if (FlashID == sFLASH_WB25Q64JV_ID)
    {
        /* OK: Turn on LD1 */
        // STM_EVAL_LEDOn(LED1);
        printf("FLASHis WB25Q64JV\r\n");
    }
    else if (FlashID == sFLASH_GD25Q64C_ID)
    {
        /* OK: Turn on LD1 */
        // STM_EVAL_LEDOn(LED1);
        printf("FLASHis gd24q64c\r\n");
    }
    else
    {
        printf("FLASH ID=0x%x ,type not support \r\n",FlashID);
    }
#if (FLASH_VALID_JUDGE ==1 )
    if (FlashID != sFLASH_ID)
    {
        printf("FLASH ID=0x%x ,cur board not support \r\n",FlashID);
        while (1);
    }
#endif

#endif
    //httpget file  check
#if 0
    {

        unsigned int charge_1_0_0_4_ENCODE_bin_len = 55312;
        unsigned int baidu_logo_gif_len = 1489;
        unsigned int file_len = baidu_logo_gif_len;
        int page_num =0;
        int i = 0;

        p=malloc(file_len);
        if(p ==NULL)
            printf(" malloc fail=%d\r\n",file_len);
        else
        {
            printf(" malloc ok =%d\r\n",file_len);

        }


        page_num = file_len/sFLASH_SPI_PAGESIZE;
        printf("page_num=%d ,%d\r\n",page_num,file_len%sFLASH_SPI_PAGESIZE);
        for(i=0; i<page_num; i++)
        {
            sFLASH_ReadBuffer((uint8_t*)p+sFLASH_SPI_PAGESIZE*i,
                              FLASH_APP_BAK_BIN_ADDRESS+sFLASH_SPI_PAGESIZE*i, sFLASH_SPI_PAGESIZE);

            dump_memeory((uint32_t)p+sFLASH_SPI_PAGESIZE*(i),sFLASH_SPI_PAGESIZE);
        }
        printf("left len=%d,i=%d\r\n",file_len%sFLASH_SPI_PAGESIZE,i);
        if(file_len%sFLASH_SPI_PAGESIZE)
        {

            sFLASH_ReadBuffer((uint8_t*)p+sFLASH_SPI_PAGESIZE*i,
                              FLASH_APP_BAK_BIN_ADDRESS+sFLASH_SPI_PAGESIZE*i, file_len%sFLASH_SPI_PAGESIZE);

            dump_memeory((uint32_t)p+sFLASH_SPI_PAGESIZE*i,file_len%sFLASH_SPI_PAGESIZE);
        }
    }
#endif

    printf("IAP_ReadFlag=0x%04x \r\n",IAP_ReadFlag());
    main_parse_flash_pcm_info();


    if(MCU_EVENT_TOP > 31)
    {
        printf("MCU_EVENT_TOP=%d,too big \r\n",MCU_EVENT_TOP);
        while(1);
    }
    if(AUDIO_END > 31)
    {
        printf("AUDIO_END=%d,too big \r\n",AUDIO_END);
        while(1);
    }
    if(charge_mode == CHARGE_MODE_BROKER )
    {
        app_rs485_broker_init();
        app_rs485_broker_info_print();
    }


    //get_pcm_data_by_name("any",NULL,NULL);
#if(PCM_IS == PCM_IS_IN_FLASH)
    flash_paudio_pcm_init();
    ret = get_pcm_data_by_name(FILE_AUDIO_SMOKE_TOO_HIGH,&pcm_flash_address,&pcm_flash_len);
    if(ret == 0)
    {
        printf("%s pcm_flash_address=0x%x,pcm_flash_len=0x%x\r\n",\
               FILE_AUDIO_SMOKE_TOO_HIGH,pcm_flash_address,pcm_flash_len);
    }
#endif

    //main_ext_spi_flash();
    /*
    init_timer_trig_dma_pcm();
    if(get_status_timer_trig_dma_pcm() == 0)
    {
    	printf("timer_trig_dma_pcm is not running\r\n");
    }
    else
    {
    	printf("timer_trig_dma_pcm  is running\r\n");
    }
    restart_timer_trig_dma_pcm();
    */
#if 0
    app4g_reset_io_init();
    app4g_reset_io_low();
    osDelay(150);
    osDelay(150);
    app4g_reset_io_high();
#endif
#if 1
    // st_dac_init();
    // Escalatar_Init();
    //pwm_tim_test();
    // set_pwm_period_init(TIM3,1000,5);
    //   play_cur_tone();
    //  main_dma_rom2ram();
    //  merger_audio_pcm(1,AUDIO_INNER_TEMP_TOO_HIGH);
    //init pwm audio
    audio_pa_io_init();
    audio_pa_io_enable();
    // audio_pa_io_disable();
    //while(1);
    //set_pwm_period_init_tim34(32000);
    set_pwm_period_init_tim34(132000);
    //187.38KHZ
    SET_EVENT(MCU_EVENT_AUDIO_DEVICE_OPEN);
    //SET_EVENT(MCU_EVENT_AUDIO_INNER_TEMP_TOO_HIGH);
    //SET_EVENT(MCU_EVENT_AUDIO_NUM_1_CHARGE_START);
    //SET_EVENT(MCU_EVENT_AUDIO_NUM_6);

    //	pcm_data_tim3_reable();
    //  set_tim4_pwm_duty(64);
#endif
    /*获取main线程的ID号*/
#endif
    main_id = osThreadGetId();
    printf("osThreadGetId  main_id=0x%08x\r\n",(unsigned int)main_id);
#ifdef MCU_USING_UTILS_STACK_CHK
    system_stack_chk_size();
#endif
    osThreadNew(app_main, NULL, NULL);    // Create application main thread
    if (osKernelGetState() == osKernelReady)
    {
        printf("os start\r\n");
        osKernelStart();                    // Start thread execution
    }

    while(1);
}

#endif

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    //while (1)
    //{}
    if(file)
    {
        uart_printf("file=%s, line=%d",file,  line);
    }
    else
    {
        uart_printf("invalid,file =%s line=%d",file,  line);
    }
}
#endif
#if (USE_SOFTTIMER_DIY ==0)
volatile uint8_t timer_dma_pcm_run_flag =0;
void reset_timer_trig_dma_pcm(void)
{
    timer_dma_pcm_run_flag =0;

}

void init_timer_trig_dma_pcm(void)
{
    //osTimerId id1;	// timer id

    // Create one-shoot timer
    exec4 = 4;
    //timer_dma_pcm =osTimerNew(timer_callback /*timer_dma_pcm_data*/,osTimerOnce, &exec4,NULL);
    timer_dma_pcm =osTimerNew(timer_dma_pcm_data /*timer_dma_pcm_data*/,osTimerOnce, &exec4,NULL);
    if (timer_dma_pcm != NULL)
    {
        // One-shoot timer created
        printf("osTimerCreate  timer_dma_pcm=0x%08x\r\n",(unsigned int)timer_dma_pcm);
    }
    reset_timer_trig_dma_pcm();

}
/// \return 0 not running, 1 running.

int get_status_timer_trig_dma_pcm(void)
{
    //printf("get_status_timer_trig_dma_pcm=%d\r\n",timer_dma_pcm_run_flag);

    return timer_dma_pcm_run_flag;//osTimerIsRunning(timer_dma_pcm);
}


void restart_timer_trig_dma_pcm(void)
{
    printf("restart_timer_trig_dma_pcm=%d\r\n",timer_dma_pcm_run_flag);
    if(timer_dma_pcm_run_flag == 0)
    {
        timer_dma_pcm_run_flag = 1;
        osTimerStart(timer_dma_pcm,10);
        //osTimerStart(timer_dma_pcm,1);
    }
}
#endif

void play_audio_start(void)
{
    audio_status=1;
    audio_pa_io_enable();

}
void play_audio_end(void)
{
    audio_status=0;
    audio_pa_io_disable();

}
char play_audio_status(void)
{
    return audio_status;
}
void mcu_check_event(void)
{
    static unsigned char event_step = 0;
    int i =MCU_EVENT_AUDIO_NUM_1;
    for(i=MCU_EVENT_AUDIO_NUM_1; i<MCU_EVENT_TOP; i++)
    {
        if(CHK_EVENT(i))
        {
            if((play_audio_status() == 0) && (event_step ==0))
            {
                play_audio_start();
                merger_audio_pcm(1,(AUDIO_OUT_TYPE)i);
                //set_pwm_period_init_tim34(32000);
                CLR_EVENT(i);
                pcm_data_tim3_reable();
            }
            return;
        }
    }
#if 0
    if(CHK_EVENT(MCU_EVENT_AUDIO_DEVICE_OPEN))//MCU_EVENT_PIR_TIMER
    {
        if((play_audio_status() == 0) && (event_step ==0))
        {
            play_audio_start();
            merger_audio_pcm(1,AUDIO_DEVICE_OPEN);
            //set_pwm_period_init_tim34(32000);
            CLR_EVENT(MCU_EVENT_AUDIO_DEVICE_OPEN);
            pcm_data_tim3_reable();
        }
    }
    else if(CHK_EVENT(MCU_EVENT_AUDIO_NUM_6))//MCU_EVENT_PIR_TIMER
    {
        if((play_audio_status() == 0) && (event_step ==0))
        {
            play_audio_start();
            merger_audio_pcm(1,AUDIO_NUM_6);
            //set_pwm_period_init_tim34(32000);
            CLR_EVENT(MCU_EVENT_AUDIO_NUM_6);
            pcm_data_tim3_reable();
        }
    }
    else if(CHK_EVENT(MCU_EVENT_AUDIO_CHARGE_END))//MCU_EVENT_PIR_TIMER
    {
        if((play_audio_status() == 0) && (event_step ==0))
        {
            play_audio_start();
            if(CHK_EVENT(MCU_EVENT_AUDIO_NUM_1))
            {
                CLR_EVENT(MCU_EVENT_AUDIO_NUM_1);
                merger_audio_pcm(1,AUDIO_NUM_1);
            }
            else if(CHK_EVENT(MCU_EVENT_AUDIO_NUM_6))
            {
                CLR_EVENT(MCU_EVENT_AUDIO_NUM_6);
                merger_audio_pcm(1,AUDIO_NUM_6);
            }
            CLR_EVENT(MCU_EVENT_AUDIO_CHARGE_END);
            pcm_data_tim3_reable();
        }

    }

    else if(CHK_EVENT(MCU_EVENT_AUDIO_INNER_TEMP_TOO_HIGH))//MCU_EVENT_PIR_TIMER
    {
        if((play_audio_status() == 0) && (event_step ==0))
        {
            play_audio_start();
            merger_audio_pcm(1,AUDIO_INNER_TEMP_TOO_HIGH);
            CLR_EVENT(MCU_EVENT_AUDIO_INNER_TEMP_TOO_HIGH);
            pcm_data_tim3_reable();
        }

    }
    else if(CHK_EVENT(MCU_EVENT_AUDIO_NUM_1_CHARGE_START))//MCU_EVENT_PIR_TIMER
    {

        if((play_audio_status() == 0) && (event_step ==0))
        {
            event_step = 1;
            play_audio_start();
            merger_audio_pcm(1,AUDIO_NUM_1);
            pcm_data_tim3_reable();
        }
        else if((play_audio_status() == 0) && (event_step ==1))
        {
            event_step = 2;
            play_audio_start();
            merger_audio_pcm(1,AUDIO_CHARGE_START);
            pcm_data_tim3_reable();
        }
        else if((play_audio_status() == 0) && (event_step ==2))
        {
            event_step = 0;
            CLR_EVENT(MCU_EVENT_AUDIO_NUM_1_CHARGE_START);
        }

    }
#endif
}
#ifdef MCU_USING_UTILS_STACK_CHK
#define CUR_CHIP_STM32F103VE
#if defined(CUR_CHIP_STM32F103VE)
#define MCU_FLASH_SIZE		0x80000
#define MCU_RAM_SIZE		0x10000
#endif

#define RAM_STACK_MAGIC		0xFEEFA55A
static uint32_t *g_stack_magic;

#define CURRENT_PLATFORM_KEIL_STM32

#if defined(CURRENT_PLATFORM_KEIL_STM32)
extern uint32_t __initial_sp;
extern uint32_t	__heap_base;
extern uint32_t	__heap_limit;
extern uint32_t Stack_Size = 0x00000600;
extern uint32_t Heap_Size = 0x00002000;

extern unsigned int Image$$ER_IROM1$$Base;
extern unsigned int Image$$ER_IROM1$$Limit;
extern unsigned int Image$$ER_IROM1$$Length;

extern unsigned int Image$$RW_IRAM1$$Base;
extern unsigned int Image$$RW_IRAM1$$Limit;
extern unsigned int Image$$RW_IRAM1$$Length;

extern unsigned int Image$$RW_IRAM1$$RW$$Base;
extern unsigned int Image$$RW_IRAM1$$RW$$Limit;
extern unsigned int Image$$RW_IRAM1$$RW$$Length;

extern unsigned int Image$$RW_IRAM1$$RO$$Base;
extern unsigned int Image$$RW_IRAM1$$RO$$Limit;
extern unsigned int Image$$RW_IRAM1$$RO$$Length;

extern unsigned int Image$$RW_IRAM1$$ZI$$Base;
extern unsigned int Image$$RW_IRAM1$$ZI$$Limit;
extern unsigned int Image$$RW_IRAM1$$ZI$$Length;

#define APP_TEXT_START		FLASH_BASE
#define APP_TEXT_LEN		Image$$ER_IROM1$$Length
#define APP_TEXT_END		Image$$ER_IROM1$$Limit

#define APP_RAM_START		Image$$RW_IRAM1$$Base
#define APP_RAM_LEN		Image$$RW_IRAM1$$Length
#define APP_RAM_END		Image$$RW_IRAM1$$ZI$$Limit

#define APP_RWDATA_START		Image$$RW_IRAM1$$RW$$Base
#define APP_RWDATA_LEN		Image$$RW_IRAM1$$RW$$Length
#define APP_RWDATA_END		Image$$RW_IRAM1$$RW$$Limit

//#define APP_RODATA_START	Image$$RW_IRAM1$$RO$$Base
//#define APP_RODATA_LEN		Image$$RW_IRAM1$$RO$$Length
//#define APP_RODATA_END		Image$$RW_IRAM1$$RO$$Limit

#define APP_BSS_START		Image$$RW_IRAM1$$ZI$$Base
#define APP_BSS_LEN		Image$$RW_IRAM1$$ZI$$Length
#define APP_BSS_END		Image$$RW_IRAM1$$ZI$$Limit

//#define APP_DATA_START	Image$$RW_IRAM1$$ZI$$Base
//#define APP_DATA_END		Image$$RW_IRAM1$$ZI$$Limit

#define APP_STACK_SIZE		Stack_Size
#define APP_STACK_END		__initial_sp

#define APP_HEAP_SIZE		Heap_Size


#endif

uint32_t get_text_start(void)
{
    return APP_TEXT_START;
}

uint32_t get_text_end(void)
{
    return (uint32_t)&APP_TEXT_END;
}

uint32_t get_text_size(void)
{
    return (uint32_t)&APP_TEXT_LEN;
}

uint32_t get_ram_start(void)
{
    return (uint32_t)&APP_RAM_START;
}

uint32_t get_ram_size(void)
{
    return (uint32_t)MCU_RAM_SIZE;
}

uint32_t get_ram_end(void)
{
    return get_ram_start() + get_ram_size();
}

uint32_t get_data_start(void)
{
    return (uint32_t)&APP_RAM_START;
    //return get_ram_start();
}

uint32_t get_data_end(void)
{
    uint32_t addr = (uint32_t)&APP_RAM_END;


#if 0//defined(CURRENT_PLATFORM_KEIL_STM32)
    addr -= Stack_Size;
#endif
    return addr;
}
uint32_t get_data_size(void)
{
    uint32_t addr = (uint32_t)&APP_RAM_LEN;


#if 0//defined(CURRENT_PLATFORM_KEIL_STM32)
    addr -= Stack_Size;
#endif
    return addr;
}



uint32_t get_rwdata_start(void)
{
    return (uint32_t)&APP_RWDATA_START;
    //return get_ram_start();
}

uint32_t get_rwdata_end(void)
{
    uint32_t addr = (uint32_t)&APP_RWDATA_END;

    return addr;
}
uint32_t get_rwdata_size(void)
{
    uint32_t addr = (uint32_t)&APP_RWDATA_LEN;

    return addr;
}
/*
uint32_t get_rodata_start(void)
{
	return (uint32_t)&APP_RODATA_START;
	//return get_ram_start();
}

uint32_t get_rodata_end(void)
{
	uint32_t addr = (uint32_t)&APP_RODATA_END;

	return addr;
}
uint32_t get_rodata_size(void)
{
	uint32_t addr = (uint32_t)&APP_RODATA_LEN;

	return addr;
}
*/
uint32_t get_bss_start(void)
{
    return (uint32_t)&APP_BSS_START;
    //return get_ram_start();
}

uint32_t get_bss_end(void)
{
    uint32_t addr = (uint32_t)&APP_BSS_END;
#if 1
    addr -= Heap_Size;
    addr -= Stack_Size;
#endif

    return addr;
}
uint32_t get_bss_size(void)
{
    uint32_t addr = (uint32_t)&APP_BSS_LEN;
#if 1
    addr -= Heap_Size;
    addr -= Stack_Size;
#endif

    return addr;
}


uint32_t get_heap_end(void)
{

    return (uint32_t)&__heap_limit;

}

uint32_t get_heap_size(void)
{
    return (uint32_t)APP_HEAP_SIZE;
}

uint32_t get_heap_start(void)
{
    return (uint32_t)&__heap_base;
}

uint32_t get_stack_end(void)
{

    return (uint32_t)&APP_STACK_END;

}

uint32_t get_stack_size(void)
{
    return (uint32_t)APP_STACK_SIZE;
}

uint32_t get_stack_start(void)
{
    return get_stack_end() - get_stack_size();
}


void dump_app_info(void)
{
    printf("\r\n\r\n");
    printf("===========  app info  ===========\r\n");
    printf("Flash size = 0x%04x, RAM size = 0x%04x\r\n", MCU_FLASH_SIZE, MCU_RAM_SIZE);
    printf("-text[0x%08x, 0x%08x, 0x%08x]\r\n", get_text_start(), get_text_end(), get_text_size());
    printf(" ram[0x%08x, 0x%08x, 0x%08x]\r\n", get_ram_start(), get_ram_end(), get_ram_size());
    printf(" -data[0x%08x, 0x%08x, 0x%08x]\r\n", get_data_start(), get_data_end(), get_data_size());
    printf(" --rwdata[0x%08x, 0x%08x, 0x%08x]\r\n", get_rwdata_start(), get_rwdata_end(), get_rwdata_size());
    //printf(" --rodata[0x%08x, 0x%08x, 0x%08x]\r\n", get_rodata_start(), get_rodata_end(), get_rodata_size());
    printf(" --bss[0x%08x, 0x%08x, 0x%08x]\r\n", get_bss_start(), get_bss_end(), get_bss_size());
    printf(" --heap[0x%08x, 0x%08x, 0x%08x]\r\n", get_heap_start(), get_heap_end(), get_heap_size());
    printf(" --stack[0x%08x, 0x%08x, 0x%08x]\r\n", get_stack_start(), get_stack_end(), get_stack_size());
    printf("===========    END     ===========\r\n");
    printf("\r\n\r\n");
}




uint32_t system_get_stack_magic(void)
{
    return *g_stack_magic;
}

uint8_t system_stack_is_overflow(void)
{
    return (*g_stack_magic != RAM_STACK_MAGIC);
}

void system_stack_chk_init(void)
{
    uint32_t *pramdata = NULL;
    int i =0;
    pramdata =(uint32_t *)get_stack_start();
    for(i=0; i<APP_STACK_SIZE/4; i+=4)
    {
        *(pramdata+i) = RAM_STACK_MAGIC;
    }

    //memset((void *)get_stack_start(), 0xFF, 0x80);
    g_stack_magic = (uint32_t *)get_stack_start();
    *g_stack_magic = RAM_STACK_MAGIC;
}
void system_stack_chk_size(void)
{

    uint32_t *pramdata = NULL;
    int i =0;
    int unuse_stack_size =0;
    pramdata =(uint32_t *)get_stack_start();
    for(i=0; i<APP_STACK_SIZE; i+=4)
    {
        if(*(pramdata+i) == RAM_STACK_MAGIC)
            unuse_stack_size += 4;
        else
            break;
    }
    printf("unuse_stack_size=0x%x\r\n",unuse_stack_size);
}

#endif

#ifdef MCU_USING_MODULE_BACKTRACE

#if __STDC_VERSION__ < 199901L
#error "must be C99 or higher. try to add '-std=c99' to compile parameters"
#endif
#define OS_Printf printf
void dump_memeory(uint32_t addr, uint32_t len)
{
    uint32_t i, j, tmp, line_num, line_max = 16;
    uint32_t old_addr;
    char *p;

    old_addr = addr;
    //自动字节对齐
    tmp = addr % 0x10;
    if (tmp) {
        addr -= tmp;
        len += tmp;
    }

    line_num = len / line_max;
    if (len % line_max) {
        line_num++;
    }

    OS_Printf("\r\n====Disp Memory  0x%08x,len=%d=================\r\n",addr,len);
    for (i = 0; i < line_num; ++i) {
        OS_Printf("0x%08x:  ", addr);
        if (i == line_num - 1) {
            line_max = (len % line_max) ? (len % line_max) : line_max;
        }
        for (j = 0; j < line_max; ++j) {
            p = (char *)addr;
            if (addr < old_addr) {
                OS_Printf("--  ");
            } else {
                OS_Printf("%02x  ", *p);
            }
            addr++;
        }
        OS_Printf("|\r\n");
    }
    OS_Printf("\r\n");
}
#define COLUMN 16
#define VERBOSE_VIEW
void hex_dump(unsigned char* data, int bytes)
{
    int i, j;
    char c;

    for (i = 0; i < bytes; i++) {
        if (!(i % 8) && i)
            printf(" ");
        if (!(i % COLUMN) && i) {
            printf("  ");
            for (j = 0; j < COLUMN; j++) {
                c = data[i+j-COLUMN];
                if ((c < 0x20) || (c >= 0x7f))
                    c = '.';
#if defined(VERBOSE_VIEW)
                printf("%c", c);
#endif
            }
            printf("\r\n");
        }
        printf("%.2x ", data[i]);
    }
    j = (bytes % COLUMN);
    j = (j != 0 ? j : COLUMN);
    for (i = j; i < COLUMN; i++) {
        if (!(i % 8) && i)
            printf(" ");
        printf("   ");
    }
    printf("   ");
    for (i = bytes - j; i < bytes; i++) {
        c = data[i];
        if ((c < 0x20) || (c >= 0x7f))
            c = '.';
#if defined(VERBOSE_VIEW)
        printf("%c", c);
#endif
    }
    printf("\r\n");
}
int hex_dump_test(void)
{
    char buf[100] = {'\0'};
    int i ;
    for(i = 0; i < 100; i++) {
        buf[i] = i;
    }
    hex_dump((unsigned char*)buf, sizeof(buf));
    return 0;
}

#if 0
__attribute__((naked)) int add(int i, int j); /* Declaring a function with __attribute__((naked)). */

__attribute__((naked)) int add(int i, int j)
{
    __asm("ADD r0, r1, #1"); /* Basic assembler statements are supported. */

    /*  Parameter references are not supported inside naked functions: */
#if 0
    __asm (
        "ADD r0, %[input_i], %[input_j]"       /* Assembler statement with parameter references */
        :                                      /* Output operand parameter */
        : [input_i] "r" (i), [input_j] "r" (j) /* Input operand parameter */
    );
#endif

    /*  Mixing C code is not supported inside naked functions: */
    /*  int res = 0;
        return res;
    */

}
#endif
//__attribute__((naked))
void except_Handler(void)
{
    __asm__("nop");
    uint32_t max_len, start_addr;
    start_addr = get_data_start();
    max_len = get_data_size();
#ifdef NOT_USE_PRINTF
    print_cur_backtrace();
#endif
    dump_memeory(start_addr, max_len);
    while (1);
}
#endif


