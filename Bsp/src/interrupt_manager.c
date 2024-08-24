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
		
		
		run_t.fan_off_60s++;
        run_t.gTimer_wifi_connect_counter++;
		run_t.dispTime_seconds++;
		run_t.gTimer_temp_delay++ ;
		run_t.gTimer_connect_wifi ++;
        run_t.gTimer_set_temp_times++;
        run_t.gTimer_key_timing++;
		run_t.gTimer_minute_Counter ++;
		run_t.gTimer_ptc_fan_warning++;

		run_t.gTimer_usart_error++;
	    run_t.gTimer_detected_power_key++;
    
          run_t.gTimer_timing++;
	    run_t.gTimer_wifi_led_blink++;
        run_t.gTimer_first_power_on_flag++;
        run_t.gTimer_first_power_on_counter++;
        run_t.gTimer_first_power_off_flag++;

	    if(run_t.dispTime_seconds >59){//60s ->1 minute 
	      run_t.dispTime_seconds=0;
		  run_t.gTimer_work_break_times++;
		 
		 }

		
		
	   
     

   }
 }
}

