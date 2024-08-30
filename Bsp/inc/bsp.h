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


}process_state;


extern process_state gpro_t;


void bsp_init(void);

void mode_key_long_fun(void);

void disp_timer_time_and_fan_icon_handler(void);


#endif

