/**
  ******************************************************************************
  * @file    ADC/ADC1_DMA/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#ifndef _APP4G_H_
#define _APP4G_H_




void app4g_reset_io_init(void);
void app4g_reset_io_high(void);
void app4g_reset_io_low(void);

void app4g_standby_io_init(void);//power
void app4g_standby_io_high(void);
void app4g_standby_io_low(void);
	

void app4g_reset_start_time(void);
void app4g_reset_ok_time(void);
void app4g_run_ok_time(void);
uint32_t app4g_run_ok_need_time(void);

uint16_t uart1_dma_send_data(unsigned char* buffer, uint16_t size);
void uart1_dma_printf( char *fmt,...) ;
void uart1_cpu_printf( char *fmt,...) ;

int uart3_sendchar(char ch);
void uart3_cpusendstring( unsigned char *pt,int len)  ;
void uart3_dma_printf( char *fmt,...);
void uart3_cpu_printf( char *fmt,...);
int uart4_sendchar(char ch);
void uart4_cpusendstring( unsigned char *pt,int len)  ;
void uart4_dma_printf( char *fmt,...);
void uart4_cpu_printf( char *fmt,...);


int uart2_sendchar(char ch);
void uart2_cpusendstring( unsigned char *pt,int len)  ;
void uart2_cpu_printf( char *fmt,...) ; 

uint16_t uart2_dma_send_data(unsigned char* buffer, uint16_t size);//dma
void uart2_dma_printf( char *fmt,...) ;

uint16_t uart3_dma_send_data(unsigned char* buffer, uint16_t size);
uint16_t uart4_dma_send_data(unsigned char* buffer, uint16_t size);


extern int8_t status_4g;
#ifndef MODULE_4G_COMMAND_PREFIX
#define MODULE_4G_COMMAND_PREFIX "app4g_"
#endif

#define  MODULE_4G_POWER_OK  "\r\nRDY\r\n"
//+QIURC: "pdpdeact",1

//rcv_len=24
#define  MODULE_4G_PDPDEACT "+QIURC: \"pdpdeact\",1"


#define ATE0 "ATE0\r\n"
#define ATE1 "ATE1\r\n"

#define  AT_GMI  "AT+GMI\r\n" // Request_Manufacturer_Identification
#define  AT_GSN  "AT+GSN\r\n" 
#define  AT_CSQ  "AT+CSQ\r\n"
#define  AT_CGREG_READ  "AT+CGREG?\r\n"

#define  AT_CIMI  "AT+CIMI\r\n"
#define  AT_QSIMSTAT_READ  "AT+QSIMSTAT?\r\n"
#define  AT_QCCID  "AT+QCCID\r\n"


#define  AT_COPS_READ  "AT+COPS?\r\n"

#define  AT_QNWINFO  "AT+QNWINFO\r\n"
#define  AT_QPINC_READ  "AT+QPINC?\r\n"
#define  AT_CPIN_READ  "AT+CPIN?\r\n"
#define  AT_CLCK_READ  "AT+CLCK"


#define AT_QNTP_WRITE "AT+QNTP=1,\"182.92.12.11\",123\r\n"
#define AT_CCLK_READ "AT+CCLK?\r\n"
/*
uart2 uart_buf_len =48 =AT+CCLK?
+CCLK: "19/09/04,02:17:33+32"

OK
*/
#define AT_CGPADDR_READ "AT+CGPADDR=1\r\n" //获取4G模块的IP地址


/*
+CGPADDR: 1,"10.202.45.115"

OK

*/

#if 0
AT+QIOPEN=<contextID>,<connectID
>,<service_type>,<IP_address>/<dom
ain_name>,<remote_port>[,<local_po
rt>[,<access_mode>]] 

	AT+QICLOSE=<connectID>[,<timeout
	>] 
	1,0,"TCP","mqtt.jindoo.jopool.net",1883,0,0"

app4g_AT+QIOPEN=1,0,"TCP","mqtt.fluux.io",1883,0,0
app4g_AT+QIOPEN=1,0,"TCP","mqtt.jindoo.jopool.net",1883,0,0
app4g_AT+QISTATE?

#endif
#define AT_TCP_TEST "tcp_test"

//#define AT_QIOPEN   "AT+QIOPEN=1,0,\"TCP\",\"mqtt.jindoo.jopool.net\",1883,0,0\r\n"
#define AT_QIOPEN   "AT+QIOPEN=1,1,\"TCP\",\"mqtt.jindoo.jopool.net\",1883,0,0\r\n"

//AT+QIOPEN=1,-1,"UDP",182.92.12.11,123,0,0

#define AT_QICLOSE "AT+QICLOSE=1\r\n"
#define AT_QISTATE "AT+QISTATE?\r\n"

//app4g_AT+QPING=1,"www.baidu.com"
//PING_CONTEXT_ID
#define AT_QPING "AT+QPING=4,\"www.baidu.com\"\r\n"
#define AT_QIACT "AT+QIACT?\r\n"

//AT+QHTTPCFG="contextid",1 
#define AT_QHTTPCFG "AT+QHTTPCFG=\"contextid\",%d\r\n"
#define HTTPURL_BAIDU "https://www.baidu.com/img/baidu_logo.gif"

#define HTTPURL_JD_TEST "http://common-download.oss-cn-hangzhou.aliyuncs.com/jindoo/charge-1.0.0.4-ENCODE.bin"

#define HTTPURL_JD_UPGRADE_TEST "http://dl.jopool.net/jindoo/stm32f103apprtx_0x3132.bin"
#define AT_QHTTPGET "AT+QHTTPGET=%d\r\n"


 
#define AT_QFDEL  "AT+QFDEL=\"%s\"\r\n"
#define AT_QFDEL_UFS  "AT+QFDEL=\"*\"\r\n"
#define AT_QFDEL_RAM  "AT+QFDEL=\"RAM:*\"\r\n"


#define AT_QFLST "AT+QFLST=\"*\"\r\n"
#define AT_QFLST_RAM "AT+QFLST=\"RAM:*\"\r\n"

#define AT_QFLDS "AT+QFLDS=\"UFS\"\r\n"
#define AT_QFLDS_RAM "AT+QFLDS=\"RAM\"\r\n"

#define STM32APPBIN  "stm32_app.bin"

extern char g_imei[] ;
extern char g_imsi[] ;
#define SIM_CCID_LEN                          (20)
extern char g_sim_ccid[SIM_CCID_LEN] ;

extern uint8_t cur_at_rec_len;
//void uart2_rec_at_cmd_response(void);

void uart2_rec_at_cmd_response(uint8_t *rcv_buf, uint8_t* rcv_len) ;

int AT_cmd(uint8_t *cmd, uint8_t cmd_len, uint8_t *rcv_buf, uint8_t* rcv_len)  ;
int at_cmd_ack(uint8_t *cmd, uint16_t cmd_len, uint8_t *rcv_buf,uint8_t* rcv_len,uint8_t *ack, uint32_t time_out);

int dx_check_imei(char *imei);

void dx_get_lte_imei(void);
int dx_get_lte_signalQuality(void);
int dx_get_lte_pinCount(void);
int dx_get_lte_pinState(void);

int dx_get_lte_pinSwitch(void);

int dx_get_lte_network_resiger_status(void);

int dx_interface_get_imsi(char *imsi, int len);
int dx_lte_traffic_GetLteStatus(void);
/*
    0:ok
    1:sim is not insert
    2:other error
*/
int dx_lte_traffic_GetLteStatus(void);
int dx_lte_traffic_GetCCID(char *ccid, int ccid_len);
void dx_get_lte_network(char *network, int network_len);

void dx_get_lte_datacap(char *datacap, int datacap_len);
int dx_set_lte_ntp_server(void);

int dx_get_lte_ntp_time(char *datacap, int datacap_len);
int dx_set_lte_ate0(void);
//Configure Parameters of a TCP/IP Context 
int dx_set_lte_pdp(void);


typedef struct{
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;
	int tzone;
}myst;
int mytransfor(char *src,myst *pt);
int dx_tcp_test(void);
int dx_lte_ping_demo(void);

int dx_get_lte_qiact(void);
int dx_lte_init_check(void);

extern uint8_t at_tx_buffer[];
extern uint8_t at_rx_buffer[];
void dc_lte_runtime_valid_judge(void);
int dx_lte_http_url_config(char* purl);
int dx_lte_http_contextid_config(void);

int dx_lte_http_file_len(int* plen);
int dx_lte_http_getfile(char* filename,int file_len);



void dx_set_mqtt_thread_stop(void);

void dx_set_mqtt_thread_pause(void);
void dx_clear_mqtt_thread_pause(void);
char dx_get_mqtt_thread_status(void);

#endif

