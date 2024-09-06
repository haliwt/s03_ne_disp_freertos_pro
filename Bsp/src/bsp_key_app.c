#include "bsp.h"

uint16_t k;
uint8_t keyvalue;
uint8_t decade_hour;
uint8_t unit_hour;
uint8_t decade_temp;
uint8_t decade_minute;
uint8_t unit_minute;
uint8_t decade_second;
uint8_t unit_second;
uint8_t unit_temp ;

uint8_t keyvalue;

uint8_t rx_data_from_main_flag ;



void (*single_ai_fun)(uint8_t cmd);
void (*single_add_fun)(void);
void (*single_buzzer_fun)(void);
void (*sendAi_usart_fun)(uint8_t senddat);
void (*beijing_time_fun)(void);




static void Beijing_Time_Display(void);




/************************************************************************
	*
	*Function Name: void Process_Key_Handler(uint8_t keylabel)
	*Function : key by pressed which is key numbers process 
	*Input Ref: key be pressed value 
	*Return Ref:No
	*
************************************************************************/
void Beijing_Time_Init(void)
{

	Beijing_Time_Display_Handler(Beijing_Time_Display);

}



void power_off_handler(void)
{
    
    if(run_t.power_off_id_flag == 1){   
        run_t.power_off_id_flag++;     
        Power_Off_Fun();
        SendData_PowerOnOff(0);

        LED_MODEL_OFF();
        POWER_ON_LED();

        run_t.wifi_led_fast_blink_flag=0;
        run_t.setup_temperature_value=0;
        run_t.timer_time_hours =0;
        run_t.timer_time_minutes =0;


    
        run_t.fan_warning=0;
        run_t.ptc_warning = 0;



        //run_t.gModel =1; //WT.EDIT 2022.09.01
        run_t.gPlasma=0;
        run_t.gDry =0;
        run_t.gUltransonic =0;

        run_t.gPower_On=0;


        run_t.timer_timing_define_flag = timing_not_definition;

        run_t.disp_wind_speed_grade =30;	

        run_t.fan_off_60s =0;
        run_t.gFan_RunContinue=1;
	}
    
	lcd_power_off_donot_fan_Fun();
    
     Breath_Led();
	if(run_t.gFan_RunContinue == 1){
           if(run_t.fan_off_60s < 61){
		      LCD_BACK_LIGHT_ON();
		      LCD_Display_Wind_Icon_Handler();
           	}
		   else {
               
               
               run_t.gFan_RunContinue =0;
			   Lcd_PowerOff_Fun();

		   }
	}
}

/************************************************
 * 
 *Function Name:
 *
 * 
*************************************************/
void power_key_short_fun(void)
{
  
   
   if(gpro_t.app_power_on_flag == 0){

    	run_t.gModel =1; //WT.EDIT 2022.09.01
    	run_t.gPlasma=1;
    	run_t.gDry =1;
    	run_t.gUltransonic =1;

    }
	
	gpro_t.set_temp_value_success =0;
    run_t.setup_temperature_value=0; // //WT.EDIT 2023.01.31
    run_t.disp_wind_speed_grade =100;
	

     run_t.setup_timer_timing_item=0;

	
     run_t.gTimer_timing=0;

	 run_t.timer_time_hours =0;
	 run_t.timer_time_minutes =0;
	 run_t.setup_timer_timing_item=0;

	 run_t.timer_timing_define_flag = timing_not_definition;

	 

	 if(lcd_t.display_beijing_time_flag == 0 ){

	 run_t.gTimer_disp_time_sencods=0;
	 run_t.dispTime_hours=0;
     run_t.dispTime_minutes=0;
	 
	  lcd_t.number5_low=(run_t.dispTime_hours ) /10;
     lcd_t.number5_high =(run_t.dispTime_hours) /10;

	 lcd_t.number6_low = (run_t.dispTime_hours ) %10;;
	 lcd_t.number6_high = (run_t.dispTime_hours ) %10;
     
     lcd_t.number7_low = (run_t.dispTime_minutes )/10;
	 lcd_t.number7_high = (run_t.dispTime_minutes )/10;

	 lcd_t.number8_low = (run_t.dispTime_minutes )%10;
	 lcd_t.number8_high = (run_t.dispTime_minutes )%10;
	
	 
	 }
    
	run_t.power_off_id_flag =1;
 
   

    run_t.gModel =1; //AI mode
    run_t.display_set_timer_timing=beijing_time ;
  
  
     Lcd_PowerOn_Fun();
    
}

void power_key_long_fun(void)
{
    
    run_t.wifi_link_net_success =0;
    run_t.gTimer_wifi_connect_counter=0;
  
    run_t.wifi_receive_led_fast_led_flag=0; //adjust if mainboard receive of connect wifi of signal
    run_t.wifi_led_fast_blink_flag=1;
  
}

void mode_key_short_fun(void)
{
   if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
		   if(run_t.display_set_timer_timing == beijing_time){
		
               //timer time + don't has ai item
               run_t.display_set_timer_timing = timer_time;
			   run_t.gModel=2;
               if(wifi_link_net_state() == 1){
                  SendData_Set_Command(0x27,0x02); //MODE_NOT AI,BUR NO_BUZZER);
                  osDelay(5);
               }
		 
		
               
		   	}
		    else if(run_t.display_set_timer_timing == timer_time){
                //beijing time + ai item
                run_t.display_set_timer_timing = beijing_time;
             
	           run_t.gModel=1;
               if(wifi_link_net_state() ==1){
			        SendData_Set_Command(0x27,0x01); //MODE_AI,BUR NO_BUZZER);
                    osDelay(5);
                }
			
				
			}
			
			
          }		



}


void add_key_fun(void)
{
     static uint8_t power_on_fisrt_flag ;
   //  static uint8_t temp_bit_1_hours,temp_bit_2_hours,temp_bit_1_minute,temp_bit_2_minute;
    
		
	 SendData_Buzzer();
     osDelay(5);


    switch(run_t.setup_timer_timing_item){

	case 0: //set temperature value add number

		
		run_t.wifi_set_temperature ++;
        if(run_t.wifi_set_temperature < 20){
		    run_t.wifi_set_temperature=20;
		}
		
		if(run_t.wifi_set_temperature > 40)run_t.wifi_set_temperature= 20;

		if(power_on_fisrt_flag ==0){
		     power_on_fisrt_flag ++;
	     	run_t.wifi_set_temperature =40;


	      }
    
	    decade_temp =  run_t.wifi_set_temperature / 10 ;
		unit_temp =  run_t.wifi_set_temperature % 10; //
        
		lcd_t.number1_low=decade_temp;
		lcd_t.number1_high =decade_temp;

		lcd_t.number2_low = unit_temp;
		lcd_t.number2_high = unit_temp;

        gpro_t.temp_key_set_value = 1;
         gpro_t.gTimer_set_temp_times = 0;
			
	
	   break;

	   case 1:
		  
			run_t.gTimer_key_timing =0;
         
			run_t.timer_time_hours++ ;//run_t.dispTime_minutes = run_t.dispTime_minutes + 60;
		    if(run_t.timer_time_hours > 24){ //if(run_t.dispTime_minutes > 59){

                 run_t.timer_time_hours=0;//run_t.dispTime_hours =0;
                

			}
		
        #if 0
          
			temp_bit_2_hours = run_t.timer_time_hours /10 ;
			temp_bit_1_hours = run_t.timer_time_hours %10;
           // HAL_Delay(20);
			run_t.timer_time_minutes  =0;

			temp_bit_2_minute =0;
			temp_bit_1_minute =0;
            
			lcd_t.number5_low=temp_bit_2_hours;
			lcd_t.number5_high =temp_bit_2_hours;

			lcd_t.number6_low = temp_bit_1_hours;
			lcd_t.number6_high = temp_bit_1_hours;

			lcd_t.number7_low=temp_bit_2_minute;
			lcd_t.number7_high =temp_bit_2_minute;

			lcd_t.number8_low = temp_bit_1_minute;
			lcd_t.number8_high = temp_bit_1_minute;
      #endif 

		break;
		}	
	
}



void dec_key_fun(void)
{
    static uint8_t power_on_fisrt_flag;
   //   static uint8_t temp_bit_1_hours,temp_bit_2_hours,temp_bit_1_minute,temp_bit_2_minute;
    if(run_t.gPower_On ==1){
	   	if(run_t.ptc_warning ==0){
	   	SendData_Buzzer();
        osDelay(5);
	     switch(run_t.setup_timer_timing_item){

		   case 0: 
	
	   
			run_t.wifi_set_temperature--;
			if(run_t.wifi_set_temperature<20) run_t.wifi_set_temperature=40;
	        if(run_t.wifi_set_temperature >40)run_t.wifi_set_temperature=40;

			if(power_on_fisrt_flag ==0){
				power_on_fisrt_flag ++;
			  run_t.wifi_set_temperature =40;


			}

	        decade_temp =  run_t.wifi_set_temperature / 10;
			unit_temp =  run_t.wifi_set_temperature % 10; //
         //    HAL_Delay(5);
			lcd_t.number1_low=decade_temp;
			lcd_t.number1_high =decade_temp;

			lcd_t.number2_low = unit_temp;
			lcd_t.number2_high = unit_temp;
			
		
            //run_t.setup_temperature_value =1;
             gpro_t.temp_key_set_value = 1;
             gpro_t.gTimer_set_temp_times = 0;
	
		    break;

			case 1:
	    
			 
				run_t.gTimer_key_timing =0;
           
				run_t.timer_time_hours -- ;//run_t.dispTime_minutes = run_t.dispTime_minutes - 1;
				if(run_t.timer_time_hours < 0){//if(run_t.dispTime_minutes < 0){

				    run_t.timer_time_hours =24;//run_t.dispTime_hours --;
					
					
				}
                
          #if 0
				    temp_bit_2_minute = run_t.timer_time_hours /10 ;
					temp_bit_1_minute = run_t.timer_time_hours %10;
               
					temp_bit_2_hours = run_t.timer_time_hours /10 ;
					temp_bit_1_hours = run_t.timer_time_hours  %10;
                    //HAL_Delay(20);
					run_t.timer_time_minutes  =0;

					temp_bit_2_minute=0;
					temp_bit_1_minute=0;
                 

					lcd_t.number5_low=temp_bit_2_hours;
					lcd_t.number5_high =temp_bit_2_hours;

					lcd_t.number6_low = temp_bit_1_hours;
					lcd_t.number6_high = temp_bit_1_hours;

					lcd_t.number7_low=temp_bit_2_minute;
					lcd_t.number7_high =temp_bit_2_minute;

					lcd_t.number8_low = temp_bit_1_minute;
					lcd_t.number8_high = temp_bit_1_minute;
                 
             #endif 
             break;

	    	}
	   	  }

    }
}
/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
 void Power_Off_Fun(void)
{
	
        //run_t.gModel =1; //WT.EDIT 2022.09.01
		run_t.gPlasma=0;
		run_t.gDry =0;
		run_t.gUltransonic =0;

		run_t.gPower_On=0;
		
		run_t.wifi_led_fast_blink_flag=0;
		run_t.timer_timing_define_flag = timing_not_definition;
		
		run_t.disp_wind_speed_grade =30;	
		
		run_t.fan_off_60s =0;
		

} 






/*********************************************************************************
 * 
 * Function Name:static void Beijing_Time_Dispaly(void)
 * 
 * 
 * 
**********************************************************************************/
static void Beijing_Time_Display(void)
{
	
	    if(run_t.gTimer_disp_timer_seconds >59){ //minute

			run_t.gTimer_disp_timer_seconds=0;
            run_t.dispTime_minutes ++;
           
            
			if(run_t.dispTime_minutes > 59){
				run_t.dispTime_minutes=0;
				run_t.dispTime_hours ++;
			   
				if(run_t.dispTime_hours >24){
					run_t.dispTime_hours=0;

					}

			}
	    	}
            
            if(run_t.gPower_On == power_on) {
				Setup_Timer_Times_Donot_Display();
				lcd_t.number5_low=(run_t.dispTime_hours ) /10;
				lcd_t.number5_high =(run_t.dispTime_hours) /10;

				lcd_t.number6_low = (run_t.dispTime_hours ) %10;;
				lcd_t.number6_high = (run_t.dispTime_hours ) %10;

				lcd_t.number7_low = (run_t.dispTime_minutes )/10;
				lcd_t.number7_high = (run_t.dispTime_minutes )/10;

				lcd_t.number8_low = (run_t.dispTime_minutes )%10;
				lcd_t.number8_high = (run_t.dispTime_minutes )%10;
			}

}

void Beijing_Time_Display_Handler(void(*beijing_time_handler)(void))
{
	beijing_time_fun = beijing_time_handler;

}





/****************************************************************
 * 
 * Function Name:
 * Function :function of pointer 
 * 
 * 
****************************************************************/
void Single_Add_RunCmd(void(*addHandler)(void))
{
    single_add_fun = addHandler;   

}

void Single_SendBuzzer_RunCmd(void(*buzzerHandler)(void))
{
	single_buzzer_fun = buzzerHandler;

}
void Single_SendAi_Usart_RunCmd(void(*sendaiHandler)(uint8_t seddat))
{
    sendAi_usart_fun = sendaiHandler;

}










