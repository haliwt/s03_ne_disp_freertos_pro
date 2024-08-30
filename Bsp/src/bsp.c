#include "bsp.h"

process_state gpro_t;


static void  disp_set_timer_timing_value_fun(void);

static void disp_normal_timing_and_fan_icon_handler(void);




void bsp_init(void)
{
  Beijing_Time_Init();

}


void mode_key_long_fun(void)
{
   
   if(run_t.ptc_warning ==0){

       run_t.gModel=2;
       run_t.setup_timer_timing_item=1;//run_t.gModel =2;
      
       run_t.gTimer_key_timing=0;


   
   }

}

void disp_timer_time_and_fan_icon_handler(void)
{

   switch(run_t.setup_timer_timing_item){

    case 0:
      disp_normal_timing_and_fan_icon_handler();

    break;

    case 1:

      disp_set_timer_timing_value_fun();

    break;

    }

}

/******************************************************************************
	*
	*Function Name:void disp_normal_timing_and_fan_icon_handler(void)
	*Function: display of icon , "1" -> ON ,"0"-> OFF
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
static void disp_normal_timing_and_fan_icon_handler(void)
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
  
       static uint8_t  timer_timg_flag;
      //Humidity Icon "0xC9"-numbers "4-4B,4G,4C","5-5A,5F,5E,5D"
        //digital -> 5,6,7,8 blink .
    
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
                 timer_timg_flag++;
            
    
              if(run_t.gTimer_key_timing > 3){ //4
    
                    
                    run_t.timer_time_minutes = 0;
                    
                    run_t.gTimer_timing=0;
                if(run_t.timer_time_hours !=0){  
                      run_t.timer_timing_define_flag = timing_success;
                      run_t.setup_timer_flag = 1;
                }
                  run_t.setup_timer_timing_item=0;
    
    
              }
                
                
             }
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



