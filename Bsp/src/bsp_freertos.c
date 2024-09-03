#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


#define POWER_KEY_BIT_0	        (1 << 0)
#define MODE_KEY_1	        (1 << 1)
#define DEC_KEY_2           (1 << 2)
#define ADD_KEY_3           (1 << 3)

#define POWER_OFF_BIT_4        (1 << 4)
#define POWER_ON_BIT_5         (1<< 5)



#define MODE_LONG_KEY_8         (1 << 8)
#define DECODER_BIT_9          (1<< 9)


/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
static void vTaskRunPro(void *pvParameters);
static void vTaskDecoderPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);



/* 创建任务通信机制 */
static void AppObjCreate(void);


/***********************************************************************************************************
											变量声明
***********************************************************************************************************/
static TaskHandle_t xHandleTaskRunPro = NULL;
static TaskHandle_t xHandleTaskDecoderPro= NULL;
static TaskHandle_t xHandleTaskStart = NULL;

//static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;
//static QueueHandle_t xQueue3 = NULL;



///#define QUEUE_LENGTH            1                   /* 队列支持的消息个敄1�7 */
///#define QUEUE_ITEM_SIZE         sizeof(uint32_t)    /* 队列中每条消息的大小 */
//#define SEMAPHORE_BINARY_LENGTH 1                   /* 二��信号量的有效长庄1�7 */
//#define QUEUESET_LENGTH         ((2 * QUEUE_LENGTH) + SEMAPHORE_BINARY_LENGTH)  /* 队列集支持的消息个数 */



typedef struct Msg
{
	uint8_t  ucMessageID;
	uint8_t  usData[12];
    uint8_t  long_key_mode_counter;
    uint8_t  key_long_mode_flag;
    //
    uint32_t  long_key_power_counter;
    uint8_t  key_long_power_flag;
	
}MSG_T;

MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */

uint32_t mode_key_long_conter;

uint8_t  rx_end_counter,ulid,rx_data_counter,rx_end_flag;

uint8_t check_code;

uint8_t bcc_check_code;






/**********************************************************************************************************
*	凄1�71ￄ1�77 敄1�71ￄ1�77 各1�71ￄ1�77: vTaskTaskUserIF
*	功能说明: 接口消息处理〄1�71ￄ1�77
*	彄1�71ￄ1�77    叄1�71ￄ1�77: pvParameters 是在创建该任务时传��的形参
*	迄1�71ￄ1�77 囄1�71ￄ1�77 倄1�71ￄ1�77: 旄1�71ￄ1�77
*   伄1�71ￄ1�77 兄1�71ￄ1�77 纄1�71ￄ1�77: 1  (数��越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
     /* 创建任务 */
	  AppTaskCreate();
	  
	  /* 创建任务通信机制 */
	 //  AppObjCreate();
	  
	  /* 启动调度，开始执行任劄1�71ￄ1�77 */
	   vTaskStartScheduler();


}
/**********************************************************************************************************
*   FunctionName: static void vTaskRunPro(void *pvParameters)
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发��的事件标志位设罄1�7
*	彄1�7    叄1�7: pvParameters 是在创建该任务时传��的形参
*	迄1�7 囄1�7 倄1�7: 旄1�7
*   伄1�7 兄1�7 纄1�7: 1  

*   伄1�71ￄ1�77 兄1�71ￄ1�77 纄1�71ￄ1�77: 1  (数��越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskRunPro(void *pvParameters)
{
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(20); /* 设置朢�大等待时间为30ms */
	uint32_t ulValue;
    
    static volatile uint8_t power_on_off_flag,fan_on_off_flag ;
    static uint8_t dry_on_off_flag,plasma_on_off_flag, ai_on_off_flag ;
    static uint8_t key_add_flag,key_dec_flag,key_mode_flag;
    static uint8_t dc_power_on_flag,app_power_on_flag,app_power_off_flag;
    while(1)
    {
		/*
			第一个参敄1�7 ulBitsToClearOnEntry的作用（函数执行前）＄1�7
		          ulNotifiedValue &= ~ulBitsToClearOnEntry
		          箢�单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零��1�7

		          这里ulBitsToClearOnEntry = 0x00000000就是函数执行前保留所有位〄1�7
		
		    第二个参敄1�7 ulBitsToClearOnExit的作用（函数逢�出前）：			
				  ulNotifiedValue &= ~ulBitsToClearOnExit
		          箢�单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零��1�7

				  这里ulBitsToClearOnExi = 0xFFFFFFFF就是函数逢�出前清楚扢�有位〄1�7
		
		    注：ulNotifiedValue表示任务vTaskMsgPro的任务控制块里面的变量��1�7		
		*/
		
		xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue丄1�7 */
						          xMaxBlockTime);  /* 朢�大允许延迟时闄1�7 */
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，棢�测那个位被按丄1�7 */




            if((ulValue & DECODER_BIT_9) != 0)
			{
        	 
              
                gpro_t.disp_rx_cmd_done_flag = 0;

             check_code =  bcc_check(gl_tMsg.usData,ulid);

             if(check_code == bcc_check_code ){
           
                receive_data_fromm_mainboard(gl_tMsg.usData);
                }



            }


            
             
			if((ulValue & POWER_KEY_BIT_0) != 0)
			{
        	    if( gl_tMsg.key_long_power_flag !=1 &&  gl_tMsg.key_long_power_flag !=2){
                   power_on_off_flag = 1;
             
                 }
                 gl_tMsg.long_key_power_counter=0;
              
            }
            else if((ulValue & POWER_ON_BIT_5) != 0){

                    app_power_on_flag=1;

            }
             else if((ulValue & POWER_OFF_BIT_4) != 0){

                    app_power_off_flag =1;

            }
            else if((ulValue & MODE_KEY_1) != 0){
                gl_tMsg.long_key_power_counter =0;
            if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
                if(run_t.gPower_On == power_on)
                 key_mode_flag = 1;
              
               }

            }
            else if((ulValue & DEC_KEY_2) != 0){
                gl_tMsg.long_key_mode_counter=0;
                gl_tMsg.long_key_power_counter =0;
                if(run_t.gPower_On == power_on)
                  key_dec_flag = 1;
              

            }
            else if((ulValue & ADD_KEY_3) != 0){
                gl_tMsg.long_key_mode_counter=0;
                gl_tMsg.long_key_power_counter =0;
                if(run_t.gPower_On == power_on)
                  key_add_flag = 1;
              
           
            }
           
          
          

        
    }
    else{

     
        if(power_on_off_flag == 1 && gl_tMsg.key_long_power_flag !=1){
             power_on_off_flag++;
             gl_tMsg.key_long_power_flag=0;
             if(run_t.gPower_On == power_off){

                run_t.gPower_On = power_on;
                gl_tMsg.long_key_power_counter =0;
                SendData_PowerOnOff(1);
                power_key_short_fun();

              }
              else{

                 run_t.gPower_On = power_off;

              }

            }
            else if(app_power_on_flag ==1){
                app_power_on_flag++;
                 run_t.gPower_On = power_on;
                gl_tMsg.long_key_power_counter =0;
                
                power_key_short_fun();

            }
            else if(app_power_off_flag ==1){
                app_power_off_flag++;

                run_t.gPower_On = power_off;
            }
            else if(gl_tMsg.key_long_power_flag ==1){
                   SendData_Buzzer();
                   gl_tMsg.key_long_power_flag++;
                   power_key_long_fun();
                   SendData_Set_Command(0x05,0x01); // link wifi of command .
                   gpro_t.gTimer_mode_key_long=0;

            }
            else if( key_mode_flag == 1 &&  gl_tMsg.key_long_mode_flag !=1){
                 key_mode_flag ++;
                 gl_tMsg.long_key_mode_counter=0;
                 SendData_Buzzer();
                 mode_key_short_fun();
             }
             else if(gl_tMsg.key_long_mode_flag ==1 ){
                 key_mode_flag =0;
                 mode_key_long_fun();
                
                

            }
            else if(key_add_flag ==1 || key_dec_flag ==1 || key_mode_flag == 1){
                

              if(key_add_flag == 1){

                  key_add_flag ++;
                  add_key_fun();

              }
              else if(key_dec_flag == 1){
                 key_dec_flag ++;
                 dec_key_fun();

              } 
              
        }
          
          
      if(run_t.gPower_On == power_on){

            if( gpro_t.gTimer_mode_key_long > 1 && (gl_tMsg.key_long_mode_flag  ==1 ||gl_tMsg.key_long_power_flag ==2)){
              gpro_t.gTimer_mode_key_long =0;

                gl_tMsg.long_key_power_counter=0;
                if(gl_tMsg.key_long_power_flag ==2){

                    gl_tMsg.key_long_power_flag++;
                }

            }
       set_temperature_compare_value_fun();
            
       disp_temp_humidity_wifi_icon_handler();

       display_timer_and_beijing_time_handler();

       disp_time_colon_ion_handler();

       Timing_Handler();
       
       }
       else if(run_t.gPower_On == power_off){

          power_off_handler();

       }
      //USART1_Cmd_Error_Handler();

    }

   }
}
/**********************************************************************************************************
*	凄1�71ￄ1�77 敄1�71ￄ1�77 各1�71ￄ1�77: vTaskStart
*	功能说明: 消息处理，使用函数comGetChar获取串口命令，使用函数comSendBuf发��串口消恄1�71ￄ1�77
*	彄1�71ￄ1�77    叄1�71ￄ1�77: pvParameters 是在创建该任务时传��的形参
*	迄1�71ￄ1�77 囄1�71ￄ1�77 倄1�71ￄ1�77: 旄1�71ￄ1�77
*   伄1�71ￄ1�77 兄1�71ￄ1�77 纄1�71ￄ1�77: 3  (数��越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
	while(1)
    {
      
    if(KEY_POWER_GetValue()  ==KEY_DOWN &&  gl_tMsg.long_key_power_counter < 2965500){

        while(KEY_POWER_GetValue()   == KEY_DOWN && gl_tMsg.long_key_power_counter < 2965500){
         
            gl_tMsg.long_key_power_counter++;
            if(gl_tMsg.long_key_power_counter > 2950000){
                gl_tMsg.long_key_power_counter = 2965900;
            if(run_t.gPower_On == power_on){  //WT.DEDIT 2024.08.26
                gl_tMsg.key_long_power_flag =1;
            }

            }
         }

        if(gl_tMsg.long_key_power_counter <  2950000 ){
          xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                        POWER_KEY_BIT_0,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位��1�7*/
         }
             

     }
     else if(KEY_MODE_GetValue() ==KEY_DOWN){



          gl_tMsg.long_key_mode_counter ++ ;

          if(gl_tMsg.long_key_mode_counter > 90  && run_t.gPower_On == power_on){
             gl_tMsg.long_key_mode_counter=0;   
               gl_tMsg.key_long_mode_flag =1;
               gpro_t.gTimer_mode_key_long = 0;
            
                SendData_Buzzer();
               


          }


          

          xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                         MODE_KEY_1,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位��1�7*/



     }
     else if(KEY_DEC_GetValue() == KEY_DOWN){

           xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                            DEC_KEY_2,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位��1�7*/

     }
     else if(KEY_ADD_GetValue() ==KEY_DOWN){

          xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                            ADD_KEY_3,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位��1�7*/

    }
    

    

     vTaskDelay(20);
     
    }

}
/**********************************************************************************************************
*	凄1�71ￄ1�77 敄1�71ￄ1�77 各1�71ￄ1�77: AppTaskCreate
*	功能说明: 创建应用任务
*	彄1�71ￄ1�77    参：旄1�71ￄ1�77
*	迄1�71ￄ1�77 囄1�71ￄ1�77 倄1�71ￄ1�77: 旄1�71ￄ1�77
**********************************************************************************************************/
void AppTaskCreate (void)
{

	xTaskCreate( vTaskRunPro,    		/* 任务函数  */
                 "vTaskRunPro",  		/* 任务各1�71ￄ1�77    */
                 256,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 1,           		/* 任务优先纄1�71ￄ1�77 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskRunPro); /* 任务句柄  */

  
    #if 0
    xTaskCreate( vTaskDecoderPro,     		/* 任务函数  */
                 "vTaskDecoderPro",   		/* 任务各1�71ￄ1�77    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,               		/* 任务优先纄1�71ￄ1�77 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskDecoderPro );  /* 任务句柄  */
   
	#endif 
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务各1�71ￄ1�77    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* 任务优先纄1�71ￄ1�77 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskStart );   /* 任务句柄  */
}



/*
*********************************************************************************************************
*	凄1�71ￄ1�77 敄1�71ￄ1�77 各1�71ￄ1�77: AppObjCreate
*	功能说明: 创建任务通信机制
*	彄1�71ￄ1�77    叄1�71ￄ1�77: 旄1�71ￄ1�77
*	迄1�71ￄ1�77 囄1�71ￄ1�77 倄1�71ￄ1�77: 旄1�71ￄ1�77
*********************************************************************************************************
*/
# if 0
void AppObjCreate (void)
{
    #if 1

//   /* 创建10个uint8_t型消息队刄1�71ￄ1�77 */
//	xQueue1 = xQueueCreate(4, sizeof(uint8_t));
//    if( xQueue1 == 0 )
//    {
//        /* 没有创建成功，用户可以在这里加入创建失败的处理机刄1�71ￄ1�77 */
//    }
	
	/* 创建10个存储指针变量的消息队列，由于CM3/CM4内核昄1�71ￄ1�7732位机，一个指针变量占甄1�71ￄ1�774个字芄1�71ￄ1�77 */
	xQueue2 = xQueueCreate(10, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机刄1�71ￄ1�77 */
    }

	

	#endif 

    #if 0

	 /* 创建队列雄1�71ￄ1�77 */
    xQueueSet = xQueueCreateSet(QUEUESET_LENGTH);
    /* 创建队列*/
    xQueue1 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
    xQueue2 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
	
    /* 创建二��信号量 */
    xSemaphore = xSemaphoreCreateBinary();
	
    /* 将队列和二��信号量添加到队列集丄1�71ￄ1�77 */
    xQueueAddToSet(xQueue1, xQueueSet);
    xQueueAddToSet(xQueue2, xQueueSet);
    xQueueAddToSet(xSemaphore, xQueueSet);
    #endif 
}

#endif 
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
     static uint8_t state, rx_mb_data_tag;
     BaseType_t xHigherPriorityTaskWoken = pdFALSE;
     MSG_T *ptMsg;


    if(huart==&huart1) // Motor Board receive data (filter)
	{

    //   DISABLE_INT();
       switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 0x5A){  // 0x5A --main board singla
               rx_data_counter=0;
               gl_tMsg.usData[rx_data_counter] = inputBuf[0];
				state=1; //=1

             }
            else
                state=0;
		break;

       
		case 1: //#1

            if(gpro_t.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               rx_data_counter++;
		     
	          gl_tMsg.usData[rx_data_counter] = inputBuf[0];
              

              if(rx_end_flag == 1){

                state = 0;
            
                ulid = rx_data_counter;
                rx_end_flag=0;

                rx_data_counter =0;

                gpro_t.disp_rx_cmd_done_flag = 1 ;

         

                bcc_check_code=inputBuf[0];

              
                xTaskNotifyFromISR(xHandleTaskRunPro,  /* 目标任务 */
                DECODER_BIT_9,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                  
              }

              }

              if(gl_tMsg.usData[rx_data_counter] ==0xFE && rx_end_flag == 0 &&  rx_data_counter > 4){
                     
                          rx_end_flag = 1 ;
                          
                        
               }

        break;


			
		}
     //   ENABLE_INT();
       __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
	}
}

#if 0
void USART1_Cmd_Error_Handler(void)
{
   uint32_t temp;
   if(run_t.gTimer_usart_error >4){
	  	run_t.gTimer_usart_error=0;
	    
         
        __HAL_UART_CLEAR_OREFLAG(&huart1);
        temp = USART1->RDR;
		UART_Start_Receive_IT(&huart1,inputBuf,1);
       
		}
      
    
  
 }

#endif 


/****************************************************************************
 * 
 * Function Name: void App_PowerOnOff_Handler(void)
 * Function:
 * Input Ref: NO
 * Return Ref: NO
 * 
*****************************************************************************/
void App_PowerOff_Handler(void)
{
     
     xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
	 POWER_OFF_BIT_4 ,            /* 设置目标任务事件标志位bit0  */
	 eSetBits);             /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
     

}

void App_PowerOn_Handler(void)
{
     
     xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
	 POWER_KEY_BIT_0 ,            /* 设置目标任务事件标志位bit0  */
	 eSetBits);             /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
     

}


















