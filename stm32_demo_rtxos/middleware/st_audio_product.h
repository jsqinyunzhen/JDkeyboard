
#ifndef __ST_AUDIO_PRODUCT_H
#define __ST_AUDIO_PRODUCT_H


#include "project_config.h"


typedef enum
{

		 AUDIO_NUM_1=1,
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
		 AUDIO_DEVICE_OPEN,
		 AUDIO_SMOKE_TOO_HIGH,
		 AUDIO_INNER_TEMP_TOO_HIGH,//65¶È
		 AUDIO_SAFE,
		 AUDIO_CHARGE_METHOD,
		 AUDIO_CHARGE_START,
		 AUDIO_CHARGE_END,
		 AUDIO_CHARGE_AO,
		 AUDIO_CHARGE_BUG,

		 AUDIO_END,
}AUDIO_OUT_TYPE;






#if(PCM_IS == PCM_IS_CONFIGUREOK)

extern unsigned int configure_ok_pcm_len;
extern const unsigned char configure_ok_pcm[];

#elif(PCM_IS == PCM_IS_CHAEGE_PUB)

extern unsigned int charge_part1_pcm_len;
extern const unsigned char  charge_part1_pcm[];

#elif(PCM_IS == PCM_IS_CHAEGE_ALL)
extern unsigned int charge_pcm_len;
extern const unsigned char charge_pcm[] ;
#elif(PCM_IS == PCM_IS_CHAEGE_FULL_PART)
extern const unsigned char audio_charge_start_pcm[];
extern unsigned int audio_charge_start_pcm_len;

extern const unsigned char audio_device_open_pcm[] ;
extern  unsigned int audio_device_open_pcm_len ;

extern const unsigned char audio_num1_pcm[] ;
extern  unsigned int audio_num1_pcm_len ;
#elif(PCM_IS == PCM_IS_NUM6)
extern unsigned int audio_num6_pcm_len;
extern const unsigned char audio_num6_pcm[]; 

#elif(PCM_IS == PCM_IS_IN_FLASH)


#define FILE_AUDIO_NUM_1 "audio_num1.pcm"
#define FILE_AUDIO_NUM_2 "audio_num2.pcm"
#define FILE_AUDIO_NUM_3 "audio_num3.pcm"
#define FILE_AUDIO_NUM_4 "audio_num4.pcm"
#define FILE_AUDIO_NUM_5 "audio_num5.pcm"
#define FILE_AUDIO_NUM_6 "audio_num6.pcm"
#define FILE_AUDIO_NUM_7 "audio_num7.pcm"
#define FILE_AUDIO_NUM_8 "audio_num8.pcm"
#define FILE_AUDIO_NUM_9 "audio_num9.pcm"
#define FILE_AUDIO_NUM_10 "audio_num10.pcm"
#define FILE_AUDIO_NUM_11 "audio_num11.pcm"
#define FILE_AUDIO_NUM_12 "audio_num12.pcm"
#define FILE_AUDIO_NUM_13 "audio_num13.pcm"
#define FILE_AUDIO_NUM_14 "audio_num14.pcm"
#define FILE_AUDIO_NUM_15 "audio_num15.pcm"
#define FILE_AUDIO_NUM_16 "audio_num16.pcm"
#define FILE_AUDIO_NUM_17 "audio_num17.pcm"
#define FILE_AUDIO_NUM_18 "audio_num18.pcm"
#define FILE_AUDIO_NUM_19 "audio_num19.pcm"
#define FILE_AUDIO_NUM_20 "audio_num20.pcm"

#define FILE_AUDIO_DEVICE_OPEN "audio_device_open.pcm"
#define FILE_AUDIO_SMOKE_TOO_HIGH "audio_smoke.pcm"
#define FILE_AUDIO_INNER_TEMP_TOO_HIGH "audio_temp_high.pcm"//"audio_temp_high1.pcm" "audio_temp_high1.pcm"
#define FILE_AUDIO_SAFE "audio_safe.pcm"

#define FILE_AUDIO_CHARGE_METHOD "audio_choise_charge_method.pcm"
#define FILE_AUDIO_CHARGE_START "audio_charge_start.pcm"
#define FILE_AUDIO_CHARGE_END "audio_charge_end.pcm"
#define FILE_AUDIO_CHARGE_AO "audio_a_o.pcm"
#define FILE_AUDIO_CHARGE_BUG "audio_charge_bug.pcm"

extern unsigned char* paudio_pcm[]; 

#else


	#error "fail define data"
#endif


extern __IO uint32_t CurrDataCounterBegin ;
extern __IO uint32_t CurrDataCounterEnd ; /* This variable should not be initialized to 0 */

void audio_pa_io_init(void);
 void audio_pa_io_disable(void);
 void audio_pa_io_enable(void);

void set_pwm_period_init_tim34(uint32_t fre);
void pcm_data_tim3_reable(void);
void set_tim4_pwm_duty(uint8_t num);

unsigned char get_audio_pcm_ram_address_len(AUDIO_OUT_TYPE audio_type,unsigned char** pram_address,unsigned int*plen );

unsigned int get_pcm_len(void);
unsigned char get_pcm_value(unsigned int  pos);
void pcm_buffer_status(void);

//uint32_t dma_copy_data_start(uint8_t* SRC_Const_Buffer,uint8_t* DST_Buffer,uint32_t BufferSize);
#if(PCM_IS == PCM_IS_IN_FLASH)
void flash_paudio_pcm_init(void);
#endif

void merger_audio_pcm(unsigned char index,AUDIO_OUT_TYPE audio_type);
//void timer_dma_pcm_data( void );
#if (USE_SOFTTIMER_DIY ==1)
void init_tim1_for_softtimer(uint32_t fre);

void timer_dma_pcm_data( void	);
#else
void timer_dma_pcm_data( void	*pram);

#endif



#endif

