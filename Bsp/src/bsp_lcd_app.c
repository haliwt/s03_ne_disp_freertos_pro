#include "bsp.h"


static void Display_Kill_Dry_Ster_Icon(void);
static void disp_fan_speed_level(void);

static void donot_disp_T13_icon_fan_speed_level(void);

static void disp_fan_leaf_run_icon(void);

/*************************************************************************
 	*
 	* Function Name:void disp_temp_humidity_value(void)
 	* Function :lcd display panel 
 	* Input Ref:NO
 	* Return Ref:NO
 	* 
*************************************************************************/ 
void disp_temp_humidity_wifi_icon_handler(void)
{
    
     static uint8_t timer_timg_flag;
	
	 TIM1723_Write_Cmd(0x00);
	 TIM1723_Write_Cmd(0x40);
	 TIM1723_Write_Cmd(0x44);

   /***********************setup temperature value ********************************/
	 //digital 1,2 ->display "temperature"  blink  
	
	 if(run_t.setup_temperature_value ==1){
	     static uint8_t number_blink_times;
	 if(run_t.gTimer_numbers_one_two_blink < 6  ){ //disp number
	     //display address 0xC2
	     if(run_t.gDry ==1 && run_t.gPlasma ==1  && run_t.gUltransonic==1)
		 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
         else if(run_t.gDry ==0 && run_t.gPlasma ==1 && run_t.gUltransonic==1)
		 	TM1723_Write_Display_Data(0xC2,((0X01+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
         else if(run_t.gDry ==0 && run_t.gPlasma ==0 && run_t.gUltransonic==1)
		 	TM1723_Write_Display_Data(0xC2,(0X01+BUG_Symbol+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 else if(run_t.gDry ==0 && run_t.gPlasma ==0 && run_t.gUltransonic==0){
		 	TM1723_Write_Display_Data(0xC2,(0X01+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 else if(run_t.gDry ==1 && run_t.gPlasma ==1 && run_t.gUltransonic==0){
		 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 else if(run_t.gDry ==0 && run_t.gPlasma ==1 && run_t.gUltransonic==0){
		 	TM1723_Write_Display_Data(0xC2,((0X01+KILL_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 else if(run_t.gDry ==1 && run_t.gPlasma ==0 && run_t.gUltransonic==0){
		 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 else if(run_t.gDry ==1 && run_t.gPlasma ==0 && run_t.gUltransonic==1){
		 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 //display addres 0xC3 -> AI icon
		 if(run_t.gModel ==1)
	        TM1723_Write_Display_Data(0xC3,(lcdNumber1_Low[lcd_t.number1_low]+AI_Symbol+lcdNumber2_High[lcd_t.number2_high]) & 0xff);//display  "AI icon
         else
		 	TM1723_Write_Display_Data(0xC3,(lcdNumber1_Low[lcd_t.number1_low]+AI_NO_Symbol+lcdNumber2_High[lcd_t.number2_high]) & 0xfe);//display  "AI icon

		 //display address 0xC4 -> temperature icon T7
		 TM1723_Write_Display_Data(0xC4,(0x01+lcdNumber2_Low[lcd_t.number2_low]+lcdNumber3_High[lcd_t.number3_high])&0xff);//display "t,c"
		 
		 
	 	 }//don't Display numbers
		 else if(run_t.gTimer_numbers_one_two_blink > 5  && run_t.gTimer_numbers_one_two_blink <11){ //don't display 
            //display address 0xC2 -> 
	          if(run_t.gDry ==1 && run_t.gPlasma ==1  && run_t.gUltransonic==1)
			 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
	         else if(run_t.gDry ==0 && run_t.gPlasma ==1 && run_t.gUltransonic==1)
			 	TM1723_Write_Display_Data(0xC2,((0X01+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
	         else if(run_t.gDry ==0 && run_t.gPlasma ==0 && run_t.gUltransonic==1)
			 	TM1723_Write_Display_Data(0xC2,((0X01+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 else if(run_t.gDry ==0 && run_t.gPlasma ==0 && run_t.gUltransonic==0){
			 	TM1723_Write_Display_Data(0xC2,((0X01)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 else if(run_t.gDry ==1 && run_t.gPlasma ==1 && run_t.gUltransonic==0){
			 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 else if(run_t.gDry ==0 && run_t.gPlasma ==1 && run_t.gUltransonic==0){
			 	TM1723_Write_Display_Data(0xC2,((0X01+KILL_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 else if(run_t.gDry ==1 && run_t.gPlasma ==0 && run_t.gUltransonic==0){
			 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 else if(run_t.gDry ==1 && run_t.gPlasma ==0 && run_t.gUltransonic==1){
			 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 
				//TM1723_Write_Display_Data(0xC2,(((0X01+DRY_Symbol+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0F));
	           // display address 0xC3
	           if(run_t.gModel ==1)
			     TM1723_Write_Display_Data(0xC3,((lcdNumber1_Low[lcd_t.number1_low]+AI_Symbol+lcdNumber2_High[lcd_t.number2_high])) & 0x1);
	           else
			   	 TM1723_Write_Display_Data(0xC3,((lcdNumber1_Low[lcd_t.number1_low]+AI_NO_Symbol+lcdNumber2_High[lcd_t.number2_high])) & 0x0);
	           //display address 0xC4
				TM1723_Write_Display_Data(0xC4,(0x01+lcdNumber2_Low[lcd_t.number2_low]+lcdNumber3_High[lcd_t.number3_high])&0xF1);//display "t,c"
        }
		else {
             run_t.gTimer_numbers_one_two_blink =0;
			 number_blink_times++;
		     if(number_blink_times > 3){
                 number_blink_times =0;
				 run_t.setup_temperature_value =0;
			     run_t.panel_key_setup_timer_flag=0;
				 run_t.temperature_set_flag = 1;
				
				
				 
			 }

		}
        TIM1723_Write_Cmd(LUM_VALUE);//(0x9B);
	 }
	 else{ //digital "1,2" don't blink LED
	    //display address 0xC2 ->
	    Display_Kill_Dry_Ster_Icon();
		
		//display address 0xC3
		if(run_t.gModel==1){
	     TM1723_Write_Display_Data(0xC3,lcdNumber1_Low[lcd_t.number1_low]+AI_Symbol+lcdNumber2_High[lcd_t.number2_high]);//display  "AI icon"
		}
	   else { 
	 	TM1723_Write_Display_Data(0xC3,(lcdNumber1_Low[lcd_t.number1_low])+lcdNumber2_High[lcd_t.number2_high]);//don't display "AI icon"
		
	    }
	   
	   //display address 0xC4
        TM1723_Write_Display_Data(0xC4,(0x01+lcdNumber2_Low[lcd_t.number2_low]+lcdNumber3_High[lcd_t.number3_high])&0xff);
	 }
	 
	 /**********************************end temperature*****************************************/
     //T2->"WIFI" icon -> 0xC5
     if(run_t.wifi_connect_flag ==0){   // && run_t.gPower_On==1){ //hasn't wifi run_t.wifi_connect_flag
         if(run_t.wifi_led_fast_blink_flag==0){
			 if(lcd_t.gTimer_wifi_500ms >99 && lcd_t.gTimer_wifi_500ms<200){
		          
		           TM1723_Write_Display_Data(0xC5,(WIFI_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi
					
			 }
		     else if(lcd_t.gTimer_wifi_500ms <100){
			 	 
		         TM1723_Write_Display_Data(0xC5,(WIFI_NO_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi 
	            
			 }
		     else{
		        lcd_t.gTimer_wifi_500ms =0;
		     }
         }
		 else{ //be detected of wifi signal fast blink wiif icon

             if(lcd_t.gTimer_wifi_500ms >14 && lcd_t.gTimer_wifi_500ms<30){ //400ms
		          
		           TM1723_Write_Display_Data(0xC5,(WIFI_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi
					
			 }
		     else if(lcd_t.gTimer_wifi_500ms <15){
			 	 
		         TM1723_Write_Display_Data(0xC5,(WIFI_NO_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi 
	            
			 }
		     else{
		        lcd_t.gTimer_wifi_500ms =0;
               
			 }
             if(run_t.gTimer_wifi_connect_counter > 120){ //
                  run_t.gTimer_wifi_connect_counter=0;
                  run_t.wifi_led_fast_blink_flag=0;
			      

			 }

		 }
		
     }
	 else if(run_t.wifi_connect_flag ==1){//wifi be connect is OK 
           run_t.wifi_led_fast_blink_flag=0;
           TM1723_Write_Display_Data(0xC5,(WIFI_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi
         //  TIM1723_Write_Cmd(LUM_VALUE);
	 }
	
	 TIM1723_Write_Cmd(LUM_VALUE);//(0x97);//(0x94);//(0x9B);

}

/******************************************************************************
	*
	*Function Name:void disp_numbers_five_eight_and_fan_icon_handler(void)
	*Function: display of icon , "1" -> ON ,"0"-> OFF
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void disp_numbers_five_eight_and_fan_icon_handler(void)
{
  static uint8_t  timer_timg_flag;
  //Humidity Icon "0xC9"-numbers "4-4B,4G,4C","5-5A,5F,5E,5D"
     if(run_t.Timer_mode_flag == 1 && run_t.gPower_On == 1 && run_t.setup_timer_timing_item==timer_time && run_t.ptc_too_hot_flag ==0){ //digital -> 5,6,7,8 blink .

     	 if(run_t.gTimer_digital5678_ms < 3){
             TM1723_Write_Display_Data(0xC9,(0x01+lcdNumber4_Low[lcd_t.number4_low]+lcdNumber5_High[lcd_t.number5_high]) & 0xff);//display digital '4,5'
          	 TM1723_Write_Display_Data(0xCA,(T15+lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]) & 0xff);
          	 TM1723_Write_Display_Data(0xCB,(0x01+lcdNumber6_Low[lcd_t.number6_low]+lcdNumber7_High[lcd_t.number7_high]) & 0xff);
          	 TM1723_Write_Display_Data(0xCC,(lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]) & 0xff);
			 if(run_t.disp_wind_speed_grade >66){
			 	TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+ WIND_SPEED_FULL) & 0xff);
			  }
			  else if(run_t.disp_wind_speed_grade >33 && run_t.disp_wind_speed_grade <67){
			     TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_TWO) & 0xff);

			  }
			  else if(run_t.disp_wind_speed_grade <34){
				 TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_ONE) & 0xff);
			  }
          }
	     else if(run_t.gTimer_digital5678_ms > 2 && run_t.gTimer_digital5678_ms < 5){
	     	 TM1723_Write_Display_Data(0xC9,(0x01+lcdNumber4_Low[lcd_t.number4_low]+lcdNumber5_High[lcd_t.number5_high]) & 0x0f);//display digital '4,5'
          	 TM1723_Write_Display_Data(0xCA,(lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]) & 0x01);
          	 TM1723_Write_Display_Data(0xCB,(0x01+lcdNumber6_Low[lcd_t.number6_low]+lcdNumber7_High[lcd_t.number7_high]) & 0x01);
          	 TM1723_Write_Display_Data(0xCC,(T14+lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]) & 0x01);
			 if(run_t.disp_wind_speed_grade >66){
			    TM1723_Write_Display_Data(0xCE,(lcdNumber8_Low[lcd_t.number8_low]+ WIND_SPEED_FULL) & 0xE1);
			 }
			 else if(run_t.disp_wind_speed_grade >33 && run_t.disp_wind_speed_grade <67){
				 TM1723_Write_Display_Data(0xCE,(lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_TWO) & 0xE1);
			 }
			  else if(run_t.disp_wind_speed_grade <34){
				 TM1723_Write_Display_Data(0xCE,(lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_ONE) & 0xE1);
			 }
		}
	     
       if(run_t.gTimer_digital5678_ms > 3){ //4
            run_t.gTimer_digital5678_ms=0;
             timer_timg_flag++;
		

		  if(run_t.gTimer_key_timing > 3){ //4

		         run_t.Timer_mode_flag = 0;
				run_t.timer_time_minutes = 0;
				
				run_t.gTimer_timing=0;
			if(run_t.timer_time_hours !=0){	 
			      run_t.timer_timing_define_flag = timing_success;
				  run_t.setup_timer_flag = 1;
			}
			  run_t.setup_timer_timing_item=0;


		  }
            
            
	     }
     }
     else{
	 	if( run_t.ptc_too_hot_flag ==0){
     	    TM1723_Write_Display_Data(0xC9,(0x01+lcdNumber4_Low[lcd_t.number4_low]+lcdNumber5_High[lcd_t.number5_high]) & 0xff);//display digital '4,5'
	 	 }
		else{
			 TM1723_Write_Display_Data(0xC9,(0x01+lcdNumber4_Low[lcd_t.number4_low]+lcdNumber5_High_E[0]));//display digital 'E'

		 }
     }
           
    
  	 disp_fan_leaf_run_icon();

	
     //open display
	 TIM1723_Write_Cmd(LUM_VALUE);//(0x97);//(0x94);//(0x9B);

}

/******************************************************************************
	*
	*Function Name:static void Display_Kill_Dry_Ster_Icon(void)
	*Function: display of icon , "1" -> ON ,"0"-> OFF
	*
	*
	*
******************************************************************************/
static void Display_Kill_Dry_Ster_Icon(void)
{

   //number "1,2" -> temperature

	  if(run_t.gDry==1 && run_t.gPlasma==1 && run_t.gUltransonic==1){

	     //display address 0xc2
	     TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high])&0xff);//display digital "temp
	     
		 
	  }
	  else if(run_t.gDry==0 && run_t.gPlasma==1 && run_t.gUltransonic==1){

	      TM1723_Write_Display_Data(0xC2,((0X01+DRY_NO_Symbol+KILL_Symbol+BUG_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digital "temp
	    
	   }
	   else if(run_t.gDry==0 && run_t.gPlasma==0 && run_t.gUltransonic ==1){

	        TM1723_Write_Display_Data(0xC2,((0X01+DRY_NO_Symbol+KILL_NO_Symbol+BUG_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
	        
	   }
	  else if(run_t.gDry==0 && run_t.gPlasma==0 && run_t.gUltransonic ==0){

	        TM1723_Write_Display_Data(0xC2,0X01+DRY_NO_Symbol+KILL_NO_Symbol+BUG_NO_Symbol+lcdNumber1_High[lcd_t.number1_high]);//display digit
	       
	  }
	  else if(run_t.gDry==1 && run_t.gPlasma==0 && run_t.gUltransonic ==1){

	        TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_NO_Symbol+BUG_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
	       
	  }
	  else if(run_t.gDry==1 && run_t.gPlasma==0 && run_t.gUltransonic ==0){

	        TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_NO_Symbol+BUG_NO_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
	       
	  }
	  else if(run_t.gDry==0 && run_t.gPlasma==1 && run_t.gUltransonic ==0){
	   
			  TM1723_Write_Display_Data(0xC2,((0X01+DRY_NO_Symbol+KILL_Symbol+BUG_NO_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
			  
	 }
	  else if(run_t.gDry==1 && run_t.gPlasma==1 && run_t.gUltransonic ==0){
		 
				TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol+BUG_NO_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
			
	   }

  
   TIM1723_Write_Cmd(LUM_VALUE);
   


}



void disp_time_colon_ion_handler(void)
{
   /*********************END T15***********************/
     //address"0xCB" ->numbers .T9":","6->6B,6G,6C","7->7A,7F,7E,7D"
     if(lcd_t.gTimer_colon_ms < 6){
     	 if(run_t.Timer_mode_flag == 0){
		 	if(run_t.ptc_too_hot_flag ==0){
	   			TM1723_Write_Display_Data(0xCB,0x01+lcdNumber6_Low[lcd_t.number6_low]+lcdNumber7_High[lcd_t.number7_high]);//display "6,7"
			}
            else{
			     if(run_t.gTimer_ptc_fan_blink_warning < 6){ //500ms
				   TM1723_Write_Display_Data(0xCB,0x01+lcdNumber6_Low_r[0]+lcdNumber7_High[0]);//display "6,7"
			     }
				 else if(run_t.gTimer_ptc_fan_blink_warning> 5 &&  run_t.gTimer_ptc_fan_blink_warning< 11){
							
				      TM1723_Write_Display_Data(0xCB,0x01+lcdNumber6_Low_r[0]+lcdNumber7_High[10]);//display "6,7"

				  }
				   else if(  run_t.gTimer_ptc_fan_blink_warning > 10){

					   run_t.gTimer_ptc_fan_blink_warning=0;

				  }
				 
			}
		}
     }
     else if(lcd_t.gTimer_colon_ms > 5 && lcd_t.gTimer_colon_ms < 11){
     	 if(run_t.Timer_mode_flag == 0){
		 	if(run_t.ptc_too_hot_flag ==0){
	 			TM1723_Write_Display_Data(0xCB,lcdNumber6_Low[lcd_t.number6_low]+lcdNumber7_High[lcd_t.number7_high]);//display "6,7"
		 	}
			else{
				if(run_t.gTimer_ptc_fan_blink_warning < 6){ //500ms
                		TM1723_Write_Display_Data(0xCB,lcdNumber6_Low_r[0]+lcdNumber7_High[0]);//
				}
				else if(run_t.gTimer_ptc_fan_blink_warning> 5 &&  run_t.gTimer_ptc_fan_blink_warning< 11){
							
				     TM1723_Write_Display_Data(0xCB,lcdNumber6_Low_r[0]+lcdNumber7_High[10]);//

				}
			    else if(  run_t.gTimer_ptc_fan_blink_warning > 10){

					 run_t.gTimer_ptc_fan_blink_warning=0;
				}
				
               }
		  }
	 }
	 else  lcd_t.gTimer_colon_ms = 0;


}


static void disp_fan_speed_level(void)
{

      if(run_t.disp_wind_speed_grade >66){
	 	TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+ WIND_SPEED_FULL) & 0xff);
	  }
	  else if(run_t.disp_wind_speed_grade >33 && run_t.disp_wind_speed_grade <67){
	     TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_TWO) & 0xff);

	  }
	  else if(run_t.disp_wind_speed_grade <34){
		 TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_ONE) & 0xff);
	  }

}


static void donot_disp_T13_icon_fan_speed_level(void)
{
    if(run_t.disp_wind_speed_grade >66){
           TM1723_Write_Display_Data(0xCE,(lcdNumber8_Low[lcd_t.number8_low]+ WIND_SPEED_FULL) & 0xff);
         }
         else if(run_t.disp_wind_speed_grade >33 && run_t.disp_wind_speed_grade <67){
            TM1723_Write_Display_Data(0xCE,(lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_TWO) & 0xff);
    
         }
         else if(run_t.disp_wind_speed_grade <34){
            TM1723_Write_Display_Data(0xCE,(lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_ONE) & 0xff);
         }




}


static void disp_fan_leaf_run_icon(void)
{

  if(lcd_t.gTimer_fan_10ms >39 && lcd_t.gTimer_fan_10ms<80){
			 if(run_t.Timer_mode_flag == 0){

                
			 	if(run_t.ptc_too_hot_flag ==0){
                        TM1723_Write_Display_Data(0xCA,T15+lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]);//display digital '5,6'
		      			TM1723_Write_Display_Data(0xCC,lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]);//display "7,8'
		      			disp_fan_speed_level();
			 	}
				else{

					if(run_t.gTimer_ptc_fan_blink_warning < 6){ //500ms
					if(run_t.ptc_warning ==1 && run_t.gTimer_ptc_fan_warning <1){
					    TM1723_Write_Display_Data(0xCA,T15+lcdNumber5_Low_E[0]+lcdNumber6_High_r[0]);//display 
						TM1723_Write_Display_Data(0xCC,lcdNumber7_Low[0]+lcdNumber8_High[1]);//display "7,8'
						
					}
					else if(run_t.fan_warning ==1 && (run_t.gTimer_ptc_fan_warning >0 && run_t.gTimer_ptc_fan_warning <2)){
					    TM1723_Write_Display_Data(0xCA,T15+lcdNumber5_Low_E[0]+lcdNumber6_High_r[0]);//display 
						TM1723_Write_Display_Data(0xCC,lcdNumber7_Low[0]+lcdNumber8_High[2]);//display "7,8'
					}
					else{
						if(run_t.gTimer_ptc_fan_warning >1 && run_t.fan_warning ==1 && run_t.ptc_warning==1)
						 	   run_t.gTimer_ptc_fan_warning=0; 
							else if(run_t.gTimer_ptc_fan_warning >1 && run_t.ptc_warning ==0 && run_t.fan_warning ==1)
								run_t.gTimer_ptc_fan_warning=1;
							else if(run_t.fan_warning==0)
								run_t.gTimer_ptc_fan_warning=0; 

					}
						
					}
					else if(run_t.gTimer_ptc_fan_blink_warning> 5 &&  run_t.gTimer_ptc_fan_blink_warning< 11){
							
							TM1723_Write_Display_Data(0xCC,lcdNumber7_Low[10]+lcdNumber8_High[10]);//display "7,8'

					}
					 else if(  run_t.gTimer_ptc_fan_blink_warning > 10){

					   run_t.gTimer_ptc_fan_blink_warning=0;

					 }
					

				}
                 TM1723_Write_Display_Data(0xCF,((T11+T16)& 0x05));//
			 }
		 }
		 else if(lcd_t.gTimer_fan_10ms <40){
		 	 if(run_t.Timer_mode_flag == 0){
                
			 	if(run_t.ptc_too_hot_flag ==0){
                    TM1723_Write_Display_Data(0xCA,lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]);//display digit
	  				TM1723_Write_Display_Data(0xCC,T14+lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]);//display "t,c
                    donot_disp_T13_icon_fan_speed_level();
                }
				else{

				  if(run_t.gTimer_ptc_fan_blink_warning < 6){ //500ms
				  if(run_t.ptc_warning ==1 && run_t.gTimer_ptc_fan_warning <1){
					    TM1723_Write_Display_Data(0xCA,lcdNumber5_Low_E[0]+lcdNumber6_High_r[0]);//display digital '5,6'
						TM1723_Write_Display_Data(0xCC,T14+lcdNumber7_Low[0]+lcdNumber8_High[1]);//display "t,c
				  	}
				    else if(run_t.fan_warning ==1 && (run_t.gTimer_ptc_fan_warning >0 && run_t.gTimer_ptc_fan_warning <2)){
					    TM1723_Write_Display_Data(0xCA,lcdNumber5_Low_E[0]+lcdNumber6_High_r[0]);//display digital '5,6'
						TM1723_Write_Display_Data(0xCC,T14+lcdNumber7_Low[0]+lcdNumber8_High[2]);//display "t,c
				  	}
					else{
						if(run_t.gTimer_ptc_fan_warning >1 && run_t.fan_warning ==1 && run_t.ptc_warning==1)
						 	   run_t.gTimer_ptc_fan_warning=0; 
							else if(run_t.gTimer_ptc_fan_warning >1 && run_t.ptc_warning ==0 && run_t.fan_warning ==1)
								run_t.gTimer_ptc_fan_warning=1;
							else if(run_t.fan_warning==0)
								run_t.gTimer_ptc_fan_warning=0; 

					} 
						 
				  	}
				  	else if(run_t.gTimer_ptc_fan_blink_warning> 5 &&  run_t.gTimer_ptc_fan_blink_warning< 11){
                        
						TM1723_Write_Display_Data(0xCC,T14+lcdNumber7_Low[10]+lcdNumber8_High[10]);//display "t,c	

					}
					else if(  run_t.gTimer_ptc_fan_blink_warning > 10){

					   run_t.gTimer_ptc_fan_blink_warning=0;

					 }
                 }
                 TM1723_Write_Display_Data(0xCF,((T16+T12+T10)&0x0B));//
			}
		 }
		 else if(lcd_t.gTimer_fan_10ms > 79){
          lcd_t.gTimer_fan_10ms=0;
	     }


}

