/**
  ******************************************************************************
  * @file    mqtt.h
  * $Author: 飞鸿踏雪 $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   MQTT应用层函数.
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
#ifndef __CJSON_MIDDLEWARE_H
#define __CJSON_MIDDLEWARE_H
/* Includes ------------------------------------------------------------------*/
//#include "includes.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"

#include <string.h>
#include <stdio.h>
#include "cJSON.h"



void myfree(void *ptr);  
void *mymalloc(size_t size);
void dx_cjson_init(void);
//extern char json_buff[512];
void print_heap_info(void);




#define CHARGE_NUM      20

#if 0
#define CUT_WIRE_NUM    20

typedef enum{
   DEV_NONE = 0,
   DEV_TURN_OFF = 1,
   DEV_TURN_ON = 2,
   DEV_OFF_LINE = 3,
}Dev_sta;


typedef struct{
   uint8_t dev_num;
   /*2bit表示状态 0:未占用 1:打开 2:关闭 3:离线*/
   Dev_sta dev_sta[CUT_WIRE_NUM];
   uint16_t notify_interval;
   uint8_t err_code[CUT_WIRE_NUM];
   uint8_t sn[CUT_WIRE_NUM][6];
   uint8_t current[CUT_WIRE_NUM];
   uint8_t current_f[CUT_WIRE_NUM];
   uint16_t voltage[CUT_WIRE_NUM];
   uint16_t watter[CUT_WIRE_NUM];
   uint32_t energy_int[CUT_WIRE_NUM];
   uint8_t energy_f[CUT_WIRE_NUM];
   uint8_t max_current[CUT_WIRE_NUM];
   uint16_t max_vol[CUT_WIRE_NUM];
   uint16_t min_vol[CUT_WIRE_NUM];
   uint32_t alive_tm[CUT_WIRE_NUM];
   uint8_t hw_ver[CUT_WIRE_NUM][3];
   uint8_t sw_ver[CUT_WIRE_NUM][3];
   uint32_t sum_crc;
}
Cut_wire_info;
#endif
//0x01：端口空闲；0x02：端口正在使用；0x03：端口禁用；0x04：端口故障。
#if 1
typedef enum{
    PORT_FREE = 0X1,
    PORT_USEING = 0x02,
    PORT_FORBID = 0x03,
    PORT_FAULT = 0x04,
}Dc_Port_Status;

typedef enum{
    CHARGE_NONE = 0,
    CHARGE_READY = 1,
    CHARGE_RUNING = 2,
    CHARGE_RUNING_ZERO = 3,
    CHARGE_BREAK = 4,
    CHARGE_FINISH = 5,
    CHARGE_MIN = 6,
    CHARGE_MIN_ZERO = 7,
    CHARGE_MAX = 8,
    CHARGE_NO_INSERT = 9,
}Charge_sta;

#endif

//报警类型，数字，1-外界温度超限，2-机箱门被打开，3-停电
typedef enum{
    STA_WARN_TEMP = 1,//OUTDOOR_TEMP
    STA_WARN_OPEN = 2,
    STA_WARN_NO_POWER = 3,
    STA_WARN_SMOKE = 4,
    STA_WARN_BOARD_TEMP = 5,
    STA_WARN_DHH_JC = 6,
}Sta_warning;
//-3-设定开关状态失败，0-正常关闭，1-小于下限功率涓流结束关闭，2-大于上限功率，
//3-设定开关状态成功,4-通电后一定时间没有插入插头自动关闭，5-插头被拔出（弃用，拆分成6和7），6-涓流情况被拔出，7-插头正常被拔出

typedef enum{
	REPORT_TYPE_SET_SW_FAIL = -3,
    REPORT_TYPE_CHARG_FINISH = 0,
    REPORT_TYPE_LESS_MIN_W = 1,
    REPORT_TYPE_MORE_MAX_W = 2,
    REPORT_TYPE_SET_SW_OK = 3,
    REPORT_TYPE_NO_INSERT = 4,
    REPORT_TYPE_PULL_OUT = 5,//废弃
    REPORT_TYPE_MIN_ZERO_PULL_OUT = 6,
    REPORT_TYPE_RUN_ZERO_PULL_OUT = 7,
    REPORT_TYPE_MAX = 8,
}Report_type;


#if 1
typedef struct{
    //Charge_sta sta[CHARGE_NUM];
	Dc_Port_Status dc_port_status[CHARGE_NUM];
    uint16_t max_watter[CHARGE_NUM];
    uint16_t min_watter[CHARGE_NUM];
	uint32_t charge_tm[CHARGE_NUM];//m
	uint32_t trickle_tm[CHARGE_NUM];//trickle charge; Trickle charging; Trickling charging;

	uint32_t start_time[CHARGE_NUM];//s
	uint16_t charge_left_tm[CHARGE_NUM];//m
	
	uint16_t charge_cur_pow[CHARGE_NUM];//m
	
	uint32_t tck_start_tm[CHARGE_NUM];
	uint32_t tck_left_tm[CHARGE_NUM];
	Report_type port_report_type[CHARGE_NUM];
	#if 0
    uint32_t valid_tm[CHARGE_NUM];
    uint32_t sta_tm[CHARGE_NUM];
    uint32_t dev_tck_tm[CHARGE_NUM];
    uint32_t tck_tm[CHARGE_NUM];
    double start_energy[CHARGE_NUM];
	#endif
    float temperature;//current temp
    uint32_t board_temp;
	
    float warn_temp;
    uint32_t ping_tm;
    uint32_t updata_tm;
	uint32_t wait_tm;
}Charge_Info;
#else
struct Charge_Info{
    Charge_sta sta[CHARGE_NUM];
    uint32_t valid_tm[CHARGE_NUM];
    uint32_t sta_tm[CHARGE_NUM];
    uint32_t dev_tck_tm[CHARGE_NUM];
    uint32_t tck_tm[CHARGE_NUM];
    uint16_t max_watter[CHARGE_NUM];
    uint16_t min_watter[CHARGE_NUM];
    double start_energy[CHARGE_NUM];
    float temperature;
    float warn_temp;
    uint32_t ping_tm;
    uint32_t updata_tm;
}__attribute__((align(8)));


#endif
struct student
{
    char name[7];
    uint32_t id;
    char subject[5];
} __attribute__ ((aligned(4))); 



extern Charge_Info charge_Info;
uint8_t get_cur_upgrade_num(void);

extern void json_parse(char *recv, char *topic);
extern char updata_path[200];
extern void json_parse_set_update2(cJSON *root);
extern void json_parse_set_update_test(char *url);

extern int json_online_cmd(uint8_t *get_str,int json_len);
//int json_online_cmd(char *get_str,char* id_str)


//extern int json_sw_change_notify(uint8_t ch, uint8_t *get_str, Report_type type);
extern int json_updata_cut_wire_msg(uint8_t *get_str);
extern void json_parse_set_parm(cJSON *root);
//extern int json_parse_set_sw(cJSON *root, uint8_t *get_ch, uint8_t *get_sta);
extern int json_parse_set_sw(cJSON *root, uint8_t *get_ch, Report_type *get_sta);

extern int json_sw_ctrl_report(uint8_t *get_str, uint8_t *set_ch, Report_type *sta, uint8_t sw_num);
extern int json_station_warning_notify(Sta_warning warn, uint8_t *get_str);

void charge_mg_init(void);
void charge_mg_set_parm(uint32_t heartbeat_tm, uint32_t updata_tm, uint32_t wait_tm,float temperature);

//void charge_mg_set_parm(uint32_t heartbeat_tm, uint32_t updata_tm, float temperature);
uint8_t charge_mg_set_on(uint8_t ch, uint32_t min, uint16_t max_w, uint16_t min_w, uint32_t tck_tm);

uint8_t charge_mg_set_off(uint8_t ch);
void reset_dc_port_para(uint8_t index);

int main_cjson(void);


#endif /* __MAIN_H */

/*********************************END OF FILE**********************************/
