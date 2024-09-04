#include "bsp.h"

process_state gpro_t;


static void  disp_set_timer_timing_value_fun(void);

static void disp_normal_timing_handler(void);




void bsp_init(void)
{
  Beijing_Time_Init();
  bsp_wifi_init();

}


void mode_key_long_fun(void)
{
   
   if(run_t.ptc_warning ==0){

       run_t.gModel=2;
       run_t.setup_timer_timing_item=1;//run_t.gModel =2;
      
       run_t.gTimer_key_timing=0;


   
   }

}

void display_timer_and_beijing_time_handler(void)
{

   switch(run_t.setup_timer_timing_item){

    case 0:
      disp_normal_timing_handler();

    break;

    case 1:

      disp_set_timer_timing_value_fun();

    break;

    }

}

/******************************************************************************
	*
	*Function Name:void disp_normal_timing_handler(void)
	*Function: display of icon , "1" -> ON ,"0"-> OFF
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
static void disp_normal_timing_handler(void)
{

        if( run_t.ptc_too_hot_flag ==0){
     	    TM1723_Write_Display_Data(0xC9,(0x01+lcdNumber4_Low[lcd_t.number4_low]+lcdNumber5_High[lcd_t.number5_high]) & 0xff);//display digital '4,5'
	 	 }
		else{
			 TM1723_Write_Display_Data(0xC9,(0x01+lcdNumber4_Low[lcd_t.number4_low]+lcdNumber5_High_E[0]));//display digital 'E'

		 }
    
           
    
  	 disp_fan_leaf_run_icon();

	
     //open display
	 TIM1723_Write_Cmd(LUM_VALUE);//(0x97);//(0x94);//(0x9B);

}



/**************************************************************************************************
*
*Function Name:void static disp_set_timer_timing_value_fun(void)
*
*
*
*****************************************************************************************************/
static void disp_set_timer_timing_value_fun(void)
{
  
     static uint8_t tim_bit_1_hours,tim_bit_2_hours;

      if(run_t.gTimer_key_timing < 4){

            tim_bit_2_hours = run_t.timer_time_hours /10 ;
			tim_bit_1_hours = run_t.timer_time_hours %10;
           // HAL_Delay(20);
			run_t.timer_time_minutes  =0;

			
            
			lcd_t.number5_low=tim_bit_2_hours;
			lcd_t.number5_high =tim_bit_2_hours;

			lcd_t.number6_low = tim_bit_1_hours;
			lcd_t.number6_high = tim_bit_1_hours;

			lcd_t.number7_low=0;
			lcd_t.number7_high =0;

			lcd_t.number8_low = 0;
			lcd_t.number8_high = 0;

       
    
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

                  TM1723_Write_Display_Data(0xCF,(T16+T11)&0x05);//
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

                 TM1723_Write_Display_Data(0xCF,((T16+T12+T10)&0x0B));//
       }
             
       if(run_t.gTimer_digital5678_ms > 3){ //4
           run_t.gTimer_digital5678_ms=0;
               

        }

       }
       else{ //4
              run_t.setup_timer_timing_item=0;//run_t.gModel =2;
              run_t.timer_time_minutes = 0;
              run_t.gTimer_timing=0;
               if(run_t.timer_time_hours !=0){  
                      run_t.timer_timing_define_flag = timing_success;
                     
                }
         
    
      }
                
                
   

   
      TIM1723_Write_Cmd(LUM_VALUE);//(0x97);//(0x94);//(0x9B);

 }
/**************************************************************************************************
*
*Function Name:void set_temperature_compare_value_fun(void)
*
*
*
*****************************************************************************************************/
void set_temperature_compare_value_fun(void)
{

    static uint8_t first_on_ptc;

    if(gpro_t.temp_key_set_value ==1 && gpro_t.gTimer_set_temp_times > 1){
    
         run_t.setup_temperature_value =1;
          gpro_t.temp_key_set_value =0;
          gpro_t.gTimer_temp_copare_value =0;

    }
    else if(gpro_t.set_temp_value_success == 1 && gpro_t.gTimer_temp_copare_value > 3 && gpro_t.temp_key_set_value ==0){

       gpro_t.gTimer_temp_copare_value =0;

      if(run_t.wifi_set_temperature > gpro_t.temp_real_value){

            run_t.gDry = 1;
            SendData_Set_Command(0x22,0x01); //open ptc 
      }
      else{
           run_t.gDry = 0;
           SendData_Set_Command(0x22,0x00); //close ptc  


      }



    }
    else if(gpro_t.set_temp_value_success == 0 && gpro_t.gTimer_temp_copare_value > 5 && gpro_t.temp_key_set_value ==0){ 
        
        if(gpro_t.temp_real_value > 39){ // must be clouse ptc.
    
               first_on_ptc = 1;
               run_t.gDry = 0;
               SendData_Set_Command(0x22,0x00); //close ptc 
          }
          else if(first_on_ptc == 1){
               
                 
               if(gpro_t.temp_real_value < 38){
                       run_t.gDry = 1;
                       SendData_Set_Command(0x22,0x01); //open ptc  
                }
                   

          }
              
    }

}




