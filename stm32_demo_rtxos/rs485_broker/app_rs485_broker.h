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

#ifndef _APP_RS485_BROKER_H_
#define _APP_RS485_BROKER_H_




void app_uart5_rs485_rxtx_io_init(void);
void app_uart5_rs485_rxtx_io_high_is_tx(void);
void app_uart5_rs485_rxtx_io_low_is_rx(void);

void app_uart1_rs485_rxtx_io_init(void);
void app_uart1_rs485_rxtx_io_high_is_tx(void);
void app_uart1_rs485_rxtx_io_low_is_rx(void);

#define RS485_BROKER_NUM   10
typedef struct{
	uint8_t used_flag;
	uint8_t channel_index;
    uint8_t id[4];
	//uint8_t broker_port_status;
	/*
	uint8_t voltage;
	uint8_t current;
	uint8_t current_f;
	uint8_t power;
	uint8_t power_f;	  
	uint8_t energy_int;
	uint8_t energy_f;
	*/
	/*
	uint16_t max_watter;
	uint16_t min_watter;
	uint32_t start_time;//s
	uint32_t charge_tm;//m
	uint16_t charge_left_tm;//m
	uint16_t charge_cur_pow;//m
	uint32_t trickle_tm;
	*/

}rs485_broker_Info;

extern rs485_broker_Info cur_rs485_broker_Info[RS485_BROKER_NUM];

int uart5_sendchar(char ch);
void uart5_cpusendstring( char *pt,int len)  ;
void uart5_cpu_printf( char *fmt,...) ; 
uint16_t uart5_cpu_send_data(unsigned char* buffer, uint16_t size);
void uart5_rec_rs485_response(void);
void uart1_rec_rs485_response(void);

#define BREAKER_TXCMDLINKCNT     20   //发送断路器数据帧的队列长度，暂时设定最大20
#define BREAKER_FRAMELEN     (0x0F)

#define BREAKER_FRAME_HEAD (0x5A)

#define BREAKER_CMD_ReportID (0xA1)   // 断路器主动 上报  ID
#define BREAKER_CMD_ReportIDACK (0xA0)   //回复 A1
#define BREAKER_CMD_OpenBreaker (0xA2)         //网关发送 【 打开电路器 】 命令
#define BREAKER_CMD_OpenBreakerACK (0xA3)         //回复A2
#define BREAKER_CMD_ReadBreaker (0xA4)      //网关 发送 【 查询 】 命令
#define BREAKER_CMD_ReadBreakerACK (0xA5)      //回复 A5
#define BREAKER_CMD_CloseBreaker (0xA6)      // 网关 发送 【 关闭断路 】 器命令
#define BREAKER_CMD_CloseBreakerACK (0xA7)      //回复A6
#define BREAKER_CMD_Calibrate (0xAA)      //发送校准命令
#define BREAKER_CMD_CalibrateACK (0xAB)      // 回复 AA

typedef struct _Breaker_Frame_type{
		u8 head;
		u8 len;
		u8 cmd;
		
		u8 id1;
		u8 id2;
		u8 id3;
		u8 id4;
		
		u8 voltage;
		u8 voltage2;
		u8 current_i;
		u8 current_d;
		u8 power_i;
		//u8 power_d;
		u8 Electricity_i;
		u8 Electricity_d;
		
		//u8 error_code;
		u8 crc;
} Breaker_Frame_t;

typedef struct Breaker_Frame_Type
{
    union {
        Breaker_Frame_t frame;
        u8 buf[BREAKER_FRAMELEN];
    }u;
}Breaker_Frame;  //断路器数据帧格式

typedef enum {
	STATE_MGAIC1 = 0,
	STATE_LEN,
	STATE_CMD1,
	STATE_ID1,//4//--4B
	STATE_ID2,//4//--4B
	STATE_ID3,//4//--4B
	STATE_ID4,//4//--4B

	STATE_V,//4
	STATE_V2,//4
	STATE_I_I,
	STATE_I_D,
	STATE_P_I,//not use
	//STATE_P_D,
	STATE_E_I,//not use
	STATE_E_D,//not use
	STATE_CRC,
}E_RecvState;
#define MSG_FIXED_LEN (0xF)
#define MAGIC1   (0x5A) 
#define CMD_MASK   (0xA0)
void Breaker_ConstructTxCmdFrame(Breaker_Frame *frame,u8 cmd,u8 * id,u8 *data,u8 datalen);
u8 Breaker_CRC8(u8 *ptr, u8 len);

void app_rs485_broker_init(void);
void app_rs485_broker_info_print(void);
int app_rs485_broker_start_power(uint8_t ch);
int app_rs485_broker_stop_power(uint8_t ch);
int app_rs485_broker_lookup_all_info(void);

uint8_t app_rs485_broker_id_response_parse(uint8_t*pdata,uint8_t data_len);
void  app_rs485_broker_test(void);
int app_rs485_broker_port_x_status(uint8_t port_num,uint16_t* pport_status);


#define MAGIC_START   (0x7a) 
#define MSG_CMD (0x00)
#define MSG_LEN (0x05)
#define MAGIC_END   (0x67)
#define STATE_JC_SAFE  (0xaa)
#define STATE_JC_ALARM  (0x55)


typedef enum {
	STATE_JC_MGAIC_START = 0,
	STATE_JC_CMD,
	STATE_JC_LEN,
	STATE_JC_DATA0,
	STATE_JC_DATA1,
	STATE_JC_DATA2,
	STATE_JC_DATA3,
	STATE_JC_DATA4,
	STATE_JC_CRC0,
	STATE_JC_CRC1,
	STATE_JC_MGAIC_END,
}STATE_JC_State;

typedef struct JC_Frame{
		u8 start;
		u8 cmd;
		u8 len;

		u8 data0;
		u8 data1;
		u8 data2;
		u8 data3;
		u8 data4;

		u8 crc0;
		u8 crc1;
		u8 end;
} JC_Frame_t;

typedef struct KB_Frame{
		u8 start;
		u8 cmd;
		u8 len;
		u8 data[10];
		u8 crc0;
		u8 crc1;
		u8 end;
} KB_Frame_t;

#endif

