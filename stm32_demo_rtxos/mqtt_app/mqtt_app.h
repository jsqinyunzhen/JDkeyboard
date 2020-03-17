/**
  ******************************************************************************
  * @file    mqtt.h
  * $Author: ·ÉºèÌ¤Ñ© $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   MQTTÓ¦ÓÃ²ãº¯Êý.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, EmbedNet</center>
  *<center><a href="http:\\www.embed-net.com">http://www.embed-net.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MQTT_H
#define __MQTT_H
/* Includes ------------------------------------------------------------------*/
//#include "includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <stdio.h>
#include <time.h>
#include "MQTTPacket.h"
#include "project_config.h"
#include "cjson_middleware.h"

typedef enum{
    MQTT_DISCON = 0,
    MQTT_CON = 1,
    MQTT_CON_ACK = 2,
    MQTT_SUB = 3,
    MQTT_OK = 4
}Mqtt_sta;



extern char sub_topics[3][50];
extern char pub_topics[5][50];
extern Mqtt_sta mqtt_sta;

extern char client_id[];
/* Exported Functions --------------------------------------------------------*/
//extern void mqtt_dev_info_save(uint8_t *dev_id, uint8_t *hw_ver);
//extern void mqtt_dev_info_restore(void);
extern void mqtt_set(char *imei);
//extern int mqtt_subscrib(char *pTopic,char *pMessage);
//extern int mqtt_connect_pro(void);
//extern void mqtt_ping(void);
//extern void mqtt_process(void);
extern int mqtt_link_pub_online(void);
extern int mqtt_updata_pro(void);

//extern void socket_set_data(unsigned char * buf, int count);
//extern int socket_get_data(unsigned char * buf, int count);
extern int mqtt_publish(char *pTopic, uint8_t qos, uint8_t *data, uint16_t data_len);
//extern void uart_msg_pro(void);
extern void mqtt_network_check(void);
extern void mqtt_pub_station_warning(Sta_warning warn);

#if 0

extern void mqtt_pub_sw_finish(uint8_t ch);
extern void mqtt_pub_max_watter(uint8_t ch);
extern void mqtt_pub_no_insert(uint8_t ch);
extern void mqtt_pub_full_charge_ok(uint8_t ch);
extern void mqtt_pub_pull_out(uint8_t ch);
extern void mqtt_pub_min_zero_pull_out(uint8_t ch);
extern void mqtt_pub_run_zero_pull_out(uint8_t ch);
#endif

extern void mqtt_pub_sw_report(uint8_t * set_ch,Report_type * sta,uint8_t sw_num);

extern void mqtt_led_dir(void);

//#define  transport_open   Connect_Server



#if 1
#define MQTT_USERNAME      "client"
#define MQTT_PASSWORD      "client@jindoo)!@!"
#endif
#if 0
#define MQTT_USERNAME      ""
#define MQTT_PASSWORD      ""
#endif

//20 (30s)  60(90s)  120(180s)
#define MQTT_KEEP_ALIVE      60
//10000 (10s)  30000(30s)  60000(60s)
#define PING_INTERVAL_TM            30000
//
#define PING_CONFIRM_TM            5000

#define TOPIC_SUB_SER_TO_LINK_SET_SW       "dc/cs/set/%s"
#define TOPIC_SUB_SER_TO_LINK_SET_PAR       "dc/cs/param/%s"
#define TOPIC_SUB_SER_TO_LINK_SET_UPDATE       "dc/cs/update/%s"

#define TOPIC_PUB_LINK_TO_SER_TYPE_REPORT     "dc/cs/state/%s"
//#define TOPIC_PUB_LINK_TO_SER_TYPE_ADP     "dc/cs/warning/%s/adapter"
#define TOPIC_PUB_LINK_TO_SER_TYPE_STA     "dc/cs/warning/%s/station"
#define TOPIC_PUB_LINK_TO_SER_ONLINE        "dc/cs/online/%s"
#define TOPIC_PUB_LINK_TO_SER_REPORT        "dc/cs/report/%s"

#define WILL_STR      "{\"did\":\"%s\"}"
#define WILL_TOPIC     "dc/cs/will/%s"

#define MODULE4G_IMEI_SN_ERROR_TEST  "865860040521999"
#define MODULE4G_IMEI_SN_TEST  "865860040521701"
#define DEVICE_TEST_UUID  "0571TST001"


#define NET_DATA_LEN 1400
#define JSON_DATA_LEN 1000

extern uint8_t mqtt_core_buff[NET_DATA_LEN];
extern uint8_t mqtt_user_buff[JSON_DATA_LEN];

#define PING_TM_OUT_MAX 4
extern uint8_t ping_tm_out_cnt;

extern uint8_t module4g_init ;

//#define MQTT_DEBUG
#ifdef MQTT_DEBUG
#define MQTT_PRINT_DEBUG(format, args...)   printf(format, ##args)
#else
#define MQTT_PRINT_DEBUG(format, args...)
#endif

#endif /* __MAIN_H */

/*********************************END OF FILE**********************************/
