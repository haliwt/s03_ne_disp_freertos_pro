#include "bsp.h"

RUN_T run_t;

static void Receive_Wifi_Cmd(uint8_t cmd);
static void Setup_Timer_Times(void);
//void Setup_Timer_Times_Donot_Display(void);
static void Works_Counter_Time(void);

uint8_t temp;


/******************************************************************************
*
*Function Name:void receive_data_fromm_mainboard(uint8_t *pdata,uint8_t len)
*Funcion: handle of tall process 
*
*
******************************************************************************/
void receive_data_fromm_mainboard(uint8_t *pdata)
{
     uint8_t hum1,hum2,temp1,temp2,decade_temp,unit_temp; 

    switch(pdata[2]){

     case 0:


     break;

     case 0x01: //表示开机指令

        if(pdata[3] == 0x01){ //open


        }
        else if(pdata[3] == 0x0){ //close 



        }

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){

            run_t.gDry =1 ;//&& run_t.gPlasma ==1  && run_t.gUltransonic==1
        }
        else if(pdata[3] == 0x0){


            run_t.gDry =0;

        }

     break;

     case 0x03: //PLASMA 打开关闭指令

        if(pdata[3] == 0x01){
        run_t.gPlasma =1;


        }
        else if(pdata[3] == 0x0){

         run_t.gPlasma =0;

        }


     break;


      case 0x04: //ultrasonic  打开关闭指令

        if(pdata[2] == 0x01){  //open 
            run_t.gUltransonic=1;

        }
        else if(pdata[2] == 0x0){ //close 

          run_t.gUltransonic=0;

        }


     break;


      case 0x05: // link wifi command

        if(pdata[3] == 0x01){  // link wifi 

             

        }
        else if(pdata[3] == 0x0){ //don't link wifi 



        }


     break;

      case 0x1A: //温度数据

        if(pdata[4] == 0x02){ //数据
        
           
             
             gpro_t.humidity_real_value = pdata[5];
             gpro_t.temp_real_value = pdata[6];

             //humidity_value 

            hum1 =  gpro_t.humidity_real_value/ 10;
            hum2 =  gpro_t.humidity_real_value % 10;

           lcd_t.number3_low= hum1;
		   lcd_t.number3_high =hum1;

		   lcd_t.number4_low = hum2;
	       lcd_t.number4_high = hum2;

            //temperature_value 
        //  temp = pdata[6];
          temp1 =   gpro_t.temp_real_value/ 10;
          temp2   = gpro_t.temp_real_value% 10;

           lcd_t.number1_low= temp1;
		   lcd_t.number1_high =temp1;

		   lcd_t.number2_low = temp2;
	       lcd_t.number2_high = temp2;

        }
        else if(pdata[4] == 0x01){ //数据)

             
            
        }
      break;

      case 0x1B: //湿度数据

        if(pdata[2] == 0x0F){ //数据
           

        }
      break;

      case 0x1C: //表示时间：小时，分，秒

        if(pdata[4] == 0x03){ //数据

            lcd_t.display_beijing_time_flag= 1;

            run_t.dispTime_hours  =  pdata[5];
            run_t.dispTime_minutes = pdata[6];
            run_t.gTimer_disp_time_sencods =  pdata[7];


        }
      break;

        case 0x1D: //表示日期： 年，月，日

        if(pdata[2] == 0x0F){ //数据

             
            
        }
      break;

      case 0x1F: //link wifi if success .
        
      if(pdata[3] == 0x01){  // link wifi 
      
           run_t.wifi_connect_flag =1 ;      
      
        }
        else if(pdata[3] == 0x0){ //don't link wifi 
      
           run_t.wifi_connect_flag =0 ;     
      
         }
        


      break;

     case 0x3A: // smart phone APP set temperature value 
        
      
        run_t.wifi_set_temperature = pdata[3];

        decade_temp =  run_t.wifi_set_temperature / 10 ;
		unit_temp =  run_t.wifi_set_temperature % 10; //
        
		lcd_t.number1_low=decade_temp;
		lcd_t.number1_high =decade_temp;

		lcd_t.number2_low = unit_temp;
		lcd_t.number2_high = unit_temp;

        run_t.setup_temperature_value =1;
      break;
     
     }

 }



// BCC校验函数
uint8_t bcc_check(const unsigned char *data, int len) {
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}


/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Receive_Wifi_Cmd(uint8_t cmd)
{
	switch(cmd){


		   case WIFI_POWER_ON: //turn on 
		 	
         
	         
		      run_t.wifi_connect_flag =1;
			  run_t.gPower_On = 1;
			  
				run_t.gModel =1;
				run_t.display_set_timer_timing=beijing_time ;
	
   
			  cmd=0xff;

	         break;

			 case WIFI_POWER_OFF: //turn off 
                
			   run_t.wifi_connect_flag =1;
		
				

      
				
              cmd=0xff;

			 break;

			case WIFI_MODE_1: //AI turn on -> AI icon display 
                if(run_t.gPower_On==1){
			
					if(run_t.display_set_timer_timing == beijing_time){

					//timer time + don't has ai item
					run_t.display_set_timer_timing = timer_time;
					run_t.gModel=0;

					}
					else if(run_t.display_set_timer_timing == timer_time){
					//beijing time + ai item
					run_t.display_set_timer_timing = beijing_time;

					run_t.gModel=1;

					}
		    	}
			break;

			 case WIFI_MODE_2: //icon don't display 
                 if(run_t.gPower_On==1){
					  if(run_t.display_set_timer_timing == beijing_time){

						//timer time + don't has ai item
						run_t.display_set_timer_timing = timer_time;
						run_t.gModel=0;

						}
						else if(run_t.display_set_timer_timing == timer_time){
						//beijing time + ai item
						run_t.display_set_timer_timing = beijing_time;

						run_t.gModel=1;

						}
			 	   
                 }
             break;

			 case WIFI_KILL_ON: //kill turn on plasma
			  if(run_t.gPower_On==1){
                    run_t.gPlasma = 1;
			        run_t.gFan_RunContinue =0;
                 // HAL_Delay(200);
                } 
			 break;

			 case WIFI_KILL_OFF: //kill turn off
                if(run_t.gPower_On==1){
			 	  run_t.gPlasma =0;
				  
		          run_t.gFan_RunContinue =0;
                   // HAL_Delay(200);
                }
			 break;

			 case WIFI_PTC_ON://dry turn on
                if(run_t.gPower_On==1){
			        run_t.gDry =1;
                    run_t.gFan_RunContinue =0;
                   // HAL_Delay(200);
                 
                }
			 break;

			 case WIFI_PTC_OFF: //dry turn off
               
			 	if(run_t.gPower_On==1){
					run_t.gDry=0;
                 
		            run_t.gFan_RunContinue =0;
                    //HAL_Delay(200);
			 	}

			 break;

			 case WIFI_SONIC_ON:  //drive bug
		
				 if(run_t.gPower_On==1){		   
				  run_t.gUltransonic =1; //turn on 
			
				 run_t.gFan_RunContinue =0;
                
			    }

			 break;

			 case WIFI_SONIC_OFF: //drive bug turn off
			 	if(run_t.gPower_On==1){
				    run_t.gUltransonic=0;
					run_t.gFan_RunContinue =0;
                   
			   }
			 break;


			 case PTC_ERROR:
			 	
			 	  run_t.gDry=0;
				  run_t.ptc_too_hot_flag =1;
			      run_t.ptc_warning =1;

			 break;

			 case FAN_ERROR:
			 	
			 	 run_t.ptc_too_hot_flag =1;
			 	 run_t.disp_wind_speed_grade=0;
	
				 run_t.fan_warning =1;
			 break;


			 case FAN_REMOVE_ERROR:
			 	 run_t.disp_wind_speed_grade=100;
				 if( run_t.ptc_warning ==0)run_t.ptc_too_hot_flag =0;
				 run_t.fan_warning =0;

			 break;
				default :
                  cmd =0;
			 break;

			 
        }
   
}


/************************************************************************
	*
	*Function Name: void Wifi_Key_Fun(void)
	*
	*
	*
	*
************************************************************************/  
void Timing_Handler(void)
{
     switch(run_t.display_set_timer_timing ){//run_t.setup_timer_timing_item
         
     case beijing_time:
       beijing_time_fun();

						 
    break;
    
    case timer_time:
	
		Setup_Timer_Times();
		Works_Counter_Time();
	
     
     break;
		
    }
}

/*************************************************************************
	*
	*Functin Name:static void Setup_Timer_Times(void)
	*Function : set up timer timing function
	*
	*
	*
*************************************************************************/
void Setup_Timer_Times(void)
{


      if(run_t.gTimer_timing > 59){ //
        
        run_t.gTimer_timing =0;
		 run_t.timer_time_minutes --;
	    if(run_t.timer_time_minutes < 0){
		     run_t.timer_time_hours -- ;
			 run_t.timer_time_minutes =59;
           
			if(run_t.timer_time_hours < 0 ){

	           if(run_t.timer_timing_define_flag == timing_success){
			    run_t.timer_time_hours=0;
				run_t.timer_time_minutes=0;
		
				Power_Off_Fun();
			

			    run_t.gFan_RunContinue=1;
				run_t.fan_off_60s = 0;
	           
	          
                
                }
                 else{
     
                     run_t.timer_time_hours =0;
                     run_t.timer_time_minutes =0;
				     run_t.display_set_timer_timing=beijing_time;
                     run_t.gModel=1;
					 SendData_Set_Command(0x27,0x0); //MODE_AI_NO_BUZZER);
                 }
                            
                
                }
              }
            
		     }

	     
     
   
			lcd_t.number5_low=(run_t.timer_time_hours ) /10;
			lcd_t.number5_high =(run_t.timer_time_hours) /10;

			lcd_t.number6_low = (run_t.timer_time_hours ) %10;;
			lcd_t.number6_high = (run_t.timer_time_hours ) %10;

			lcd_t.number7_low = (run_t.timer_time_minutes )/10;
			lcd_t.number7_high = (run_t.timer_time_minutes)/10;

			lcd_t.number8_low = (run_t.timer_time_minutes)%10;
			lcd_t.number8_high = (run_t.timer_time_minutes )%10;
}
/*************************************************************************
	*
	*Functin Name:void Setup_Timer_Times_Donot_Display(void)
	*
	*
	*
	*
*************************************************************************/       
void Setup_Timer_Times_Donot_Display(void)
{
   if(run_t.gTimer_timing > 59){ //
        
        run_t.gTimer_timing =0;
		 run_t.timer_time_minutes --;
	    if(run_t.timer_time_minutes < 0){
		     run_t.timer_time_hours -- ;
			 run_t.timer_time_minutes =59;
           
			if(run_t.timer_time_hours < 0 ){

	           if(run_t.timer_timing_define_flag == timing_success){
			    run_t.timer_time_hours=0;
				run_t.timer_time_minutes=0;
			
				Power_Off_Fun();

			
				run_t.gPower_On =0 ;
			    run_t.gFan_RunContinue=1;
				run_t.fan_off_60s = 0;
	           
	          
                
                }
                 else{
     
                     run_t.timer_time_hours =0;
                     run_t.timer_time_minutes =0;
                 
                 }
                            
                
                }
              }
            
		     }




}

/***************************************************************
 * 
 * Function Name:
 * 
 *
 * 
 **************************************************************/
static void Works_Counter_Time(void)
{
  if(run_t.timer_timing_define_flag == timing_success){
	  if(run_t.gTimer_disp_timer_seconds >59){ //minute
		
		run_t.gTimer_disp_timer_seconds=0;
        run_t.dispTime_minutes ++;
       
          
		if(run_t.dispTime_minutes > 59){
			run_t.dispTime_minutes=0;
			run_t.dispTime_hours ++;
		    run_t.works_counter_time_value++;
		if(run_t.dispTime_hours >24){
			run_t.dispTime_hours=0;

		}

		}


	  }
  }
}


