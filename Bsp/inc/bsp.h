#ifndef __BSP_H
#define __BSP_H
#include "main.h"

#include "bsp_cmd_link.h"
#include "bsp_delay.h"
#include "bsp_ctl.h"
#include "bsp_display.h"
#include "bsp_led.h"
#include "bsp_key_app.h"
#include "bsp_key.h"
#include "bsp_lcd.h"
#include "bsp_wifi.h"
#include "bsp_lcd_app.h"
#include "bsp_freertos.h"


#include "gpio.h"
#include "usart.h"




#define  USE_FreeRTOS      1

#if USE_FreeRTOS == 1
	#include "FreeRTOS.h"
    #include "task.h"
    #include "cmsis_os.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif


typedef struct _process_state{

   uint8_t disp_rx_cmd_done_flag;
   uint8_t gTimer_mode_key_long;
   uint8_t temp_key_set_value ;
   uint8_t set_temp_value_success;
   uint8_t humidity_real_value;
   uint8_t  temp_real_value;
   uint8_t  app_power_on_flag;


   //
   uint8_t gTimer_set_temp_times;
   uint8_t gTimer_temp_copare_value;
   uint8_t gTimer_fan_to_ptc_warning;
   


}process_state;


extern process_state gpro_t;


void bsp_init(void);

void mode_key_long_fun(void);

void set_temperature_compare_value_fun(void);

void display_timer_and_beijing_time_handler(void);





#endif

