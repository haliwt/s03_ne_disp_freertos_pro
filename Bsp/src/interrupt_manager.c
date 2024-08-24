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
    if(htim->Instance==TIM17){  
    tm0++;
	
   
	
    if(tm0>999){ //100 *10ms = 1000ms = 1s
		tm0=0;
		
	 }
    
    }
 }


