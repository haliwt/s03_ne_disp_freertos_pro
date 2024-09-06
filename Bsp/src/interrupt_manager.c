#include "bsp.h"


/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timing 10ms interrupt call back function 
	*
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint16_t tm0;
  static uint8_t tm1,tm2;
    
   if(htim->Instance==TIM17){
    
    tm0++;  //10ms
	tm1++;
    tm2++;

    if(tm2> 9){
        tm2=0;
    lcd_t.gTimer_wifi_500ms++;
	lcd_t.gTimer_fan_10ms++;

     }
	
    if(tm1>99){ //100ms 
       tm1=0;
       lcd_t.gTimer_colon_ms++ ;
	   run_t.gTimer_numbers_one_two_blink++;
	   run_t.gTimer_digital5678_ms++;
       run_t.gTimer_ptc_fan_blink_warning++ ;
    }

    if(tm0>999){ //1000 *1ms = 1000ms = 1s
		tm0=0;
		
		 gpro_t.gTimer_mode_key_long++;
         gpro_t.gTimer_set_temp_times++;
         gpro_t.gTimer_temp_copare_value++ ;
         gpro_t.gTimer_fan_to_ptc_warning++ ;
         //
		run_t.fan_off_60s++;
        run_t.gTimer_wifi_connect_counter++;
		run_t.gTimer_disp_time_sencods++;
		run_t.gTimer_key_timing++;
		run_t.gTimer_disp_timer_seconds ++;
		run_t.gTimer_ptc_fan_warning++;
        run_t.gTimer_timing++;
	    run_t.gTimer_work_break_times++;
		 
	}

   }
 }


