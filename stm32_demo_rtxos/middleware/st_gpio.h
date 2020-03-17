#ifndef _ST_GPIO_H_
#define _ST_GPIO_H_

void gpio_led_init(void);
void gpio_led_on(void);
void gpio_led_off(void);
void gpio_led_tog(void);

void EXTI5_Config(void);

//void EXTI2_3_Config(void);
void EXTI2_Config(void);
void EXTI3_Config(void);
void gpio_key_init(void);
void gpio_key_value(void);
void gpio_key_exti_config(void);
void timer_key_detect( void);

void led_poweron(void);

void rs485_set_start(void);

void rs485_ok_led_fast(void);
void rs485_fail_led_fast(void);
void mcu_led_y_fast_ctrl( void );
void fw_upgeade_start(void);

void gpio_smoke_detect_init(void);

void gpio_smoke_detect_exti_config(void);

int gpio_smoke_detect_value(void);


//void gpio_key_NVIC_Configuration(void);
#endif
