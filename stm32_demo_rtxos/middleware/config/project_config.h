
#ifndef _PROJECT_CONFIG_H_
#define _PROJECT_CONFIG_H_
#include <stdarg.h>  
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>
#include "st_printf.h"

#include "cmsis_os2.h"
//#include "rtx_lib.h"
//#include "app4g.h"




#define  HWSERIAL_SOFTVERSION    0x313C

#define  VER_PRO_NET         "1.0.1.12"

#define  HW_BOARD_TYPE_IS_STM32DEMO     1
#define  HW_BOARD_TYPE_IS_STM32PRODUCT1    2
#define  HW_BOARD_TYPE_IS_STM32PRODUCT_V2    3

#define  HW_BOARD_TYPE    HW_BOARD_TYPE_IS_STM32PRODUCT_V2
//#define  HW_BOARD_TYPE    HW_BOARD_TYPE_IS_STM32PRODUCT1

#define  SPI_FLASH_CPU     1
#define  SPI_FLASH_DMA     2
#define  SPI_FLASH_OP_MODE     SPI_FLASH_CPU

#define SPI_FLASH_DMA_RX_TX_INT 0
//
#define USE_SOFTTIMER_DIY 1


#if (USE_SOFTTIMER_DIY ==1)
extern unsigned char timer_dma_pcm;
extern unsigned char timer_detect;

extern unsigned char timer_y_ctrl;
extern unsigned char led_mode ;
extern unsigned char times;
extern unsigned char timer_kb_openport;

#define TIMER_LOW_MEM

#else
extern osTimerId_t timer_dma_pcm ;
#endif

#define FLASH_VALID_JUDGE 0
#define TIME_VALID_JUDGE 1

#define DEAD_TIME "99/11/14,06:00:09+32"
//#define DEAD_TIME "19/09/04,06:00:09+32"
#define STM_RTC_ENABLE 1
//typedef uint32_t bool;
//#define FALSE 0
//#define TRUE 1
//<contextID>           Integer type. The context ID. The range is 1-16. 
//<connectID>          Integer type. The socket service index. The range is 0-11. 


#define MQTT_TCP_CONTEXT_ID  1
#define QNTP_UDP_CONTEXT_ID  2
#define HTTP_TCP_CONTEXT_ID  3
#define NTP_UDP_CONTEXT_ID   4
#define PING_CONTEXT_ID      5

//#define QNTP_UDP_CONNECT_ID  0
#define MQTT_TCP_CONNECT_ID  1
#define HTTP_TCP_CONNECT_ID  2
#define NTP_UDP_CONNECT_ID   3
#define PING_CONNECT_ID  4

extern osThreadId_t thread_main;
extern osThreadId_t thread_uart_msg;
extern osThreadId_t thread_app;
extern osThreadId_t thread_app_dc12;

#define MAIN_STACK_SIZE 200
#define UART2_MSG_STACK_SIZE 200
#define APP_MQTT_STACK_SIZE 200
#define APP_DC12_STACK_SIZE 200

extern uint64_t thread1_stk_1[MAIN_STACK_SIZE];
extern uint64_t thread1_stk_2[UART2_MSG_STACK_SIZE];
extern uint64_t thread1_stk_3[APP_MQTT_STACK_SIZE];
extern uint64_t thread1_stk_4[APP_DC12_STACK_SIZE];

#define MCU_USING_UTILS_STACK_CHK
#define MCU_USING_MODULE_BACKTRACE			/* backtrace功能 */


extern osEventFlagsId_t evt_id_uart; 
extern osEventFlagsId_t evt_id_uart1;
extern osEventFlagsId_t evt_id_uart3;
extern osEventFlagsId_t evt_id_uart4;
extern osEventFlagsId_t evt_id_uart5;

extern osEventFlagsId_t evt_id_app;

#define EVENT_FLAGS_UART1 0x01000001U
#define EVENT_FLAGS_UART1_TX_COMPLETE 0x01000002U //rx _COMPLETE

#define EVENT_FLAGS_UART2_TX_COMPLETE 0x02000001U //rx _COMPLETE
#define EVENT_FLAGS_UART2 0x02000002U //rx _COMPLETE
//ARM_USART_EVENT_TX_COMPLETE

#define EVENT_FLAGS_UART3_TX_COMPLETE 0x04000001U //rx _COMPLETE
#define EVENT_FLAGS_UART3 0x04000002U //rx _COMPLETE

#define EVENT_FLAGS_UART4_TX_COMPLETE 0x04000004U //rx _COMPLETE
#define EVENT_FLAGS_UART4 0x04000008U //rx _COMPLETE

#define EVENT_FLAGS_UART5_TX_COMPLETE 0x04000010U 
#define EVENT_FLAGS_UART5 0x04000020U

#define EVENT_FLAGS_APP_KEY_ENTER_SHORT 0x10000001U
#define EVENT_FLAGS_APP_KEY_ENTER_LONG  0x10000002U
#define EVENT_FLAGS_APP_DC1_INSERT  0x10000004U
#define EVENT_FLAGS_APP_DC1_REMOVE  0x10000008U

#define EVENT_FLAGS_APP_DC2_INSERT  0x10000010U
#define EVENT_FLAGS_APP_DC2_REMOVE  0x10000020U
#define EVENT_FLAGS_APP_SMOKE_DETECT  0x10000040U
#define EVENT_FLAGS_APP_SMOKE_DETECT_NO  0x10000080U

#define EVENT_FLAGS_APP_DC_COMPLETE  0x10000100U
#define EVENT_FLAGS_APP_DC_REPORT_ERROR  0x10000200U
#define EVENT_FLAGS_APP_KEYBOARD_ON  0x10000400U


#define EVENT_FLAGS_APP ( \
						EVENT_FLAGS_APP_KEY_ENTER_SHORT|EVENT_FLAGS_APP_KEY_ENTER_LONG| \
						EVENT_FLAGS_APP_DC1_INSERT|EVENT_FLAGS_APP_DC1_REMOVE| \
						EVENT_FLAGS_APP_DC2_INSERT|EVENT_FLAGS_APP_DC2_REMOVE| \
						EVENT_FLAGS_APP_SMOKE_DETECT|EVENT_FLAGS_APP_SMOKE_DETECT_NO| \
						EVENT_FLAGS_APP_DC_COMPLETE|EVENT_FLAGS_APP_DC_REPORT_ERROR|  \
						EVENT_FLAGS_APP_KEYBOARD_ON \
						)
	
#define  PRINTF_SUPPORT 1
#define  KB_DEBUG 1
#define  PRINTF_UART_ID_1 1
#define  PRINTF_UART_ID_4 4

#define  PRINTF_UART_ID_INVALID 0XFF

#define  CHARGE_MODE_DC1 1
#define  CHARGE_MODE_DC2 2
#define  CHARGE_MODE_DC 3
#define  CHARGE_MODE_BROKER 4
#define  CHARGE_MODE_INVALID 5
extern volatile  unsigned char charge_mode;


extern unsigned short int HWSerial;
extern uint8_t debug_uart_id;
#ifndef LEN  
#define LEN  200 
#endif
//#define LEN (33 +2)
extern unsigned char string[]; 
extern char string_len; 


#define UART1_RX_DMA 1
#define UART1_TX_DMA 0

#define UART2_RX_DMA 1
#define UART2_TX_DMA 1

#define UART3_RX_DMA 1
#define UART3_TX_DMA 0
#define UART4_RX_DMA 1
#define UART4_TX_DMA 0
#define UART5_RX_DMA 0
#define UART5_TX_DMA 0


#define UART1_RX_BUFFER_LEN 64
#define UART1_TX_BUFFER_LEN 32

#define UART2_RX_BUFFER_LEN 320
#define UART2_TX_BUFFER_LEN 600

#define UART3_RX_BUFFER_LEN 64
#define UART3_TX_BUFFER_LEN 64

#define UART4_RX_BUFFER_LEN 64
#define UART4_TX_BUFFER_LEN 64

#define UART5_RX_BUFFER_LEN 16
#define UART5_TX_BUFFER_LEN 16


#define UART3_TX_DMA_CHANNEL DMA1_Channel2
#define UART3_RX_DMA_CHANNEL DMA1_Channel3
#define UART4_TX_DMA_CHANNEL DMA2_Channel5
#define UART4_RX_DMA_CHANNEL DMA2_Channel3

#define UART3_RX_COMMAND_TIMEOUT (5*1000)
#define UART4_RX_COMMAND_TIMEOUT (5*1000)
//rs485
#define UART1_RX_COMMAND_TIMEOUT (5*1000)

#define  WDT_TIMEOUT_S (10)


#define MCU_HEADER_MAIGC (0x53564B48)  /* "HKVS" */

/*
@u32Magic: a symbol which indicates a valid mcu firmware.
@u32RawDataLen: mcu meta data length, not include header itself.
@u16Crc16: verify mcu meta data integrity.
@u16McuVersion: mcu firmware version.
@u8ArrayBuildTime: mcu firmware build time.
 */
typedef struct _McuFirmwareHeader {
	u32 u32Magic; /* HKVS */
	u32 u32RawDataLen;
	u16 u16Crc16;
	u16 u16McuVersion;
	u8  u8ArrayBuildTime[16];
}T_McuFirmwareHeader, *pT_McuFirmwareHeader;

#define UPDATE_MCUAPP_FLAG_DATA 0x4004


/* Define the APP start address -------------------------------*/
#define  IAP_ADDR 0X08000000
#define ApplicationAddress    0x08020000 //128KB
#define ApplicationSize       0x0030000 
#define ApplicationSize_other       0x0020000 

#define ApplicationAddress_end    (0x8080000+ApplicationSize)

/* IAP command------------------------------------------------ */
#define IAP_FLAG_ADDR   (uint32_t)(ApplicationAddress - 1024 * 2)//App区域和Bootloader区域共享信息的地址(暂定大小为2K)


#define  MCU_HEADER 28
//#define APP_OFFSET (ApplicationAddress-IAP_ADDR+MCU_HEADER)
#define APP_OFFSET (ApplicationAddress-IAP_ADDR)
//#define  RT_APP_PART_ADDR  (ApplicationAddress+28)

#define PROGRAM_IS_BOOT 1
#define PROGRAM_IS_APP 2

#define PROGRAM_MODE  PROGRAM_IS_APP 
//#define PROGRAM_MODE  PROGRAM_IS_APP 


#define  SPI_FLASH_SIZE   0x800000  //8MB
//FOR PCM_DATA
#define  FLASH_PCM_DATA_ADDRESS     0
//FOR APP_BAK_BIN
#define  FLASH_APP_BAK_BIN_ADDRESS     0x400000//--4MB
#define  FLASH_APP_BAK_BIN_END_ADDRESS     (0x400000+ApplicationSize)//192KB

#define  FLASH_MCU_HEADER_ADDRESS  0x500000//--5MB
#define  FLASH_DEVID_ADDRESS  0x600000//--6MB
#define  FLASH_BROKERID_ADDRESS  0x700000//--7MB

#define DEVID_SPACE 24
//for test
#define  FLASH_TEST_Address     0x780000
#if 1
#define  FLASH_WriteAddress     FLASH_TEST_Address
#define  FLASH_ReadAddress      FLASH_TEST_Address
#define  FLASH_SectorToErase    FLASH_TEST_Address
#endif
#include "stm32_eval_spi_flash.h"

#if defined(USE_STM32100B_EVAL) || defined(USE_STM32100E_EVAL)
  #define  sFLASH_ID       sFLASH_M25P128_ID
#else

	#if(HW_BOARD_TYPE	== HW_BOARD_TYPE_IS_STM32PRODUCT1)

		#define  sFLASH_ID  sFLASH_GD25Q64C_ID //STM32PRODUCT1
	#elif(HW_BOARD_TYPE	== HW_BOARD_TYPE_IS_STM32PRODUCT_V2)
		
		#define  sFLASH_ID	sFLASH_GD25Q64C_ID //sFLASH_WB25Q64JV_ID //STM32PRODUCT1

	#elif(HW_BOARD_TYPE	== HW_BOARD_TYPE_IS_STM32DEMO)

		#define  sFLASH_ID	sFLASH_SST25VF016B_ID 
	#endif
#endif



#define PCM_IS_CONFIGUREOK 1
#define PCM_IS_CHAEGE_PUB  2
#define  PCM_IS_CHAEGE_ALL 3
#define  PCM_IS_CHAEGE_FULL_PART 4
#define  PCM_IS_NUM6 5

#define  PCM_IS_IN_FLASH 6

//#define PCM_IS  PCM_IS_CONFIGUREOK 
//#define PCM_IS  PCM_IS_CHAEGE_FULL_PART 
#define PCM_IS  PCM_IS_IN_FLASH//PCM_IS_IN_FLASH 

#define PCM_ROM2RAM_DMA_SUPPORT 1
#define PCM_ROM2RAM_DMA_CHANNEL DMA1_Channel2
#define PCM_ROM2RAM_DMA_CHANNEL_INDEX 2


//extern uint8_t timer_dma_pcm;
//extern uint8_t timer_event_check;
//res
#if( PCM_IS  != PCM_IS_IN_FLASH )
extern const unsigned char audio_device_open_pcm[];
extern unsigned int audio_device_open_pcm_len;

extern const unsigned char audio_temp_high_pcm[];
extern unsigned int audio_temp_high_pcm_len;
#endif
typedef enum {
	MCU_INIT_STATE = 1,
	MCU_POLLING_KEY,
	MCU_POLLING_EVENT,
	MCU_POLLING_PWR_SRC,
	MCU_LOWPOWER,
	MCU_STATE_MAX
}E_McuState;
#if 0
typedef enum
{
		 AUDIO_DEVICE_OPEN=1,
		 AUDIO_SMOKE_TOO_HIGH,
		 AUDIO_INNER_TEMP_TOO_HIGH,//65度
		 AUDIO_CHARGE_METHOD,
		 AUDIO_CHARGE_START,
		 AUDIO_CHARGE_END,
		 AUDIO_CHARGE_AO,
		 AUDIO_CHARGE_BUG,
		 AUDIO_NUM_1,
		 AUDIO_NUM_2,
		 AUDIO_NUM_3,
		 AUDIO_NUM_4,
		 AUDIO_NUM_5,
		 AUDIO_NUM_6,
		 AUDIO_NUM_7,
		 AUDIO_NUM_8,
		 AUDIO_NUM_9,
		 AUDIO_NUM_10,
		 AUDIO_NUM_11,
		 AUDIO_NUM_12,
		 AUDIO_NUM_13,
		 AUDIO_NUM_14,
		 AUDIO_NUM_15,
		 AUDIO_NUM_16,
		 AUDIO_NUM_17,
		 AUDIO_NUM_18,
		 AUDIO_NUM_19,
		 AUDIO_NUM_20,
		 AUDIO_END,
}AUDIO_OUT_TYPE;
#endif
typedef enum
{
	MCU_EVENT_NULL = 0x00,

	MCU_EVENT_AUDIO_NUM_1= 0x01,
	MCU_EVENT_AUDIO_NUM_2,
	MCU_EVENT_AUDIO_NUM_3,
	MCU_EVENT_AUDIO_NUM_4,
	MCU_EVENT_AUDIO_NUM_5,
	MCU_EVENT_AUDIO_NUM_6,
	MCU_EVENT_AUDIO_NUM_7,
	MCU_EVENT_AUDIO_NUM_8,
	MCU_EVENT_AUDIO_NUM_9,
	MCU_EVENT_AUDIO_NUM_10,
	MCU_EVENT_AUDIO_NUM_11,
	MCU_EVENT_AUDIO_NUM_12,
	MCU_EVENT_AUDIO_NUM_13,
	MCU_EVENT_AUDIO_NUM_14,
	MCU_EVENT_AUDIO_NUM_15,
	MCU_EVENT_AUDIO_NUM_16,
	MCU_EVENT_AUDIO_NUM_17,
	MCU_EVENT_AUDIO_NUM_18,
	MCU_EVENT_AUDIO_NUM_19,
	MCU_EVENT_AUDIO_NUM_20,

	MCU_EVENT_AUDIO_DEVICE_OPEN,
	MCU_EVENT_AUDIO_SMOKE_TOO_HIGH,
	MCU_EVENT_AUDIO_INNER_TEMP_TOO_HIGH,
	MCU_EVENT_AUDIO_SAFE,
	MCU_EVENT_AUDIO_CHARGE_METHOD,
	MCU_EVENT_AUDIO_CHARGE_START,
	MCU_EVENT_AUDIO_CHARGE_END,
	MCU_EVENT_AUDIO_CHARGE_AO,
	MCU_EVENT_AUDIO_CHARGE_BUG,
	//MCU_EVENT_AUDIO_NUM_1_CHARGE_START,
	MCU_EVENT_TOP,
	#if 0
	MCU_EVENT_KEY1,
	MCU_EVENT_KEY2,
	MCU_EVENT_LOW_BAT,
	MCU_EVENT_DAY_NIGHT,
	MCU_EVENT_LIGHT_LUM_OK,
	MCU_EVENT_PMU_INT_GET,
	MCU_EVENT_SLEEP_QUERY,
	MCU_EVENT_WAKEUP_OV_ONE_HOUR,
	MCU_EVENT_WAKEUP_TIMER,
	MCU_EVENT_PIR_TIMER,
	#endif
}MCU_EVENT_TYPE;

extern volatile uint32_t u32mcu_eventStatus;
#define CHK_EVENT(irq)    ((u32mcu_eventStatus >> irq) & 0x1)
#define SET_EVENT(irq)    (u32mcu_eventStatus |= ( 0x1 << irq))
#define CLR_EVENT(irq)    (u32mcu_eventStatus &= ~(0x1 << irq))
#define CLR_ALL_EVENT()   (u32mcu_eventStatus = 0)
#define CHK_ALL_EVENT()   (u32mcu_eventStatus == 0)

void mcu_sys_soft_reset(void);

extern volatile char audio_status;
#if (USE_SOFTTIMER_DIY ==0)

void init_timer_trig_dma_pcm(void);
void restart_timer_trig_dma_pcm(void);
void reset_timer_trig_dma_pcm(void);
int get_status_timer_trig_dma_pcm(void);
#endif
void play_audio_start(void);
void play_audio_end(void);
char play_audio_status(void);
void mcu_check_event(void);


typedef uint8_t bool;
#define FALSE 0
#define TRUE 1

#ifdef MCU_USING_UTILS_STACK_CHK
void dump_app_info(void);
uint32_t get_text_start(void);
uint32_t get_text_end(void);
uint32_t get_text_size(void);

uint32_t get_ram_start(void);
uint32_t get_ram_size(void);
uint32_t get_ram_end(void);

uint32_t get_data_start(void);
uint32_t get_data_end(void);
uint32_t get_data_size(void);


uint32_t get_rwdata_start(void);
uint32_t get_rwdata_end(void);
uint32_t get_rwdata_size(void);


//uint32_t get_rodata_start(void);
//uint32_t get_rodata_end(void);
//uint32_t get_rodata_size(void);


uint32_t get_bss_start(void);
uint32_t get_bss_end(void);
uint32_t get_bss_size(void);

uint32_t get_stack_end(void);
uint32_t get_stack_size(void);
uint32_t get_stack_start(void);

uint32_t get_heap_end(void);
uint32_t get_heap_size(void);
uint32_t get_heap_start(void);

uint32_t system_get_stack_magic(void);
uint8_t system_stack_is_overflow(void);
void system_stack_chk_init(void);
void system_stack_chk_size(void);

#endif
#ifdef MCU_USING_MODULE_BACKTRACE
void dump_memeory(uint32_t addr, uint32_t len);
#endif
void hex_dump(unsigned char* data, int bytes);

int hex_dump_test(void);

int dx_memmem(char * a, int alen, char * b, int blen);

osMutexId_t CreateMutex (const osMutexAttr_t *mutex_def);
void AcquireMutex (osMutexId_t mutex_id);
void ReleaseMutex (osMutexId_t mutex_id);

/*
extern osMutexId mutex_id_uart;  

void CreateMutexUart (void);
void AcquireMutex (osMutexId mutex_id);
void ReleaseMutex (osMutexId mutex_id) ;
*/
#endif
