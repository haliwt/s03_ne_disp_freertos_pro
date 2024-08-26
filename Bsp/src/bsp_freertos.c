#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


#define POWER_KEY_0	        (1 << 0)
#define MODE_KEY_1	        (1 << 1)
#define DEC_KEY_2           (1 << 2)
#define ADD_KEY_3           (1 << 3)



#define MODE_LONG_KEY_8         (1 << 8)



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
	uint8_t usData[12];
	//uint8_t ulData[1];
}MSG_T;

MSG_T   g_tMsg; /* 定义丢�个结构体用于消息队列 */

volatile uint8_t decoder_flag;

volatile uint8_t ulid,uldata,usdata;


uint32_t mode_key_long_conter;

uint8_t rxcmd[1];

volatile uint8_t rx_data_counter;

uint8_t rx_end_flag  ;
uint8_t rx_end_counter, rx_end_counter_compare;



/*
*********************************************************************************************************
*	凄1�71ￄ1�77 敄1�71ￄ1�77 各1�71ￄ1�77: vTaskTaskUserIF
*	功能说明: 接口消息处理〄1�71ￄ1�77
*	彄1�71ￄ1�77    叄1�71ￄ1�77: pvParameters 是在创建该任务时传��的形参
*	迄1�71ￄ1�77 囄1�71ￄ1�77 倄1�71ￄ1�77: 旄1�71ￄ1�77
*   伄1�71ￄ1�77 兄1�71ￄ1�77 纄1�71ￄ1�77: 1  (数��越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
void freeRTOS_Handler(void)
{
     /* 创建任务 */
	  AppTaskCreate();
	  
	  /* 创建任务通信机制 */
	   AppObjCreate();
	  
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
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(10); /* 设置朢�大等待时间为30ms */
	uint32_t ulValue;
    
    static volatile uint8_t power_on_off_flag,fan_on_off_flag ;
    static uint8_t dry_on_off_flag,plasma_on_off_flag, ai_on_off_flag ;
    static uint8_t key_add_flag,key_dec_flag,key_mode_flag;
    
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
             
			if((ulValue & POWER_KEY_0) != 0)
			{
        	  power_on_off_flag = 1;
              
            }
            else if((ulValue & MODE_KEY_1) != 0){
            if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
                if(run_t.gPower_On == power_on)
                 key_mode_flag = 1;
              
               }

            }
            else if((ulValue & DEC_KEY_2) != 0){
                if(run_t.gPower_On == power_on)
                  key_dec_flag = 1;
              

            }
            else if((ulValue & ADD_KEY_3) != 0){
                if(run_t.gPower_On == power_on)
                  key_add_flag = 1;
              
           
            }
          
          

        
    }
    else{

     
        if(power_on_off_flag == 1){
             power_on_off_flag++;
             if(run_t.gPower_On == power_off){
                power_key_short_fun();

              }
              else{

                 run_t.gPower_On = power_off;

              }

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
              else if(key_mode_flag == 1){
                 key_mode_flag++;
                 SendData_Buzzer();
                 mode_key_short_fun();

                }
        }
          
          
      if(run_t.gPower_On == power_on){

       disp_temp_humidity_wifi_icon_handler();
      
       disp_numbers_five_eight_and_fan_icon_handler();

       disp_time_colon_ion_handler();

       Timing_Handler();
       
       }
       else if(run_t.gPower_On == power_off){

          power_off_handler();

       }
     // USART1_Cmd_Error_Handler();

    }

   }
}
/**********************************************************************************************************
*	凄1�71ￄ1�77 敄1�71ￄ1�77 各1�71ￄ1�77: vTaskStart
*	功能说明: 启动任务，也就是朢�高优先级任务，这里用作按键扫描�ￄ1�71ￄ1�77
*	彄1�71ￄ1�77    叄1�71ￄ1�77: pvParameters 是在创建该任务时传��的形参
*	迄1�71ￄ1�77 囄1�71ￄ1�77 倄1�71ￄ1�77: 旄1�71ￄ1�77
*   伄1�71ￄ1�77 兄1�71ￄ1�77 纄1�71ￄ1�77: 4  (数��越小优先级越低，这个跟uCOS相反)
**********************************************************************************************************/
static void vTaskDecoderPro(void *pvParameters)
{
    MSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* 设置朢�大等待时间为30ms */
	//uint8_t uldata,usdata;
	
    while(1)
    {
		
       xResult = xQueueReceive(xQueue2,                   /* 消息队列句柄 */
		                        (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
		                        (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */
		
		if(xResult == pdPASS){
            
          ulid = ptMsg ->ucMessageID;

        //  usdata = ptMsg->usData[0];

         // uldata = ptMsg->ulData[0];
          
           receive_data_fromm_mainboard(ptMsg->usData,ulid);
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
      
    if(KEY_POWER_GetValue()  ==KEY_DOWN){

           
           xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                        POWER_KEY_0,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位��1�7*/

             

     }
     else if(KEY_MODE_GetValue() ==KEY_DOWN){

        
        // while(MODEL_KEY_VALUE() == KEY_DOWN && mode_key_long_conter < 2965500){

//               mode_key_long_conter++;
//               if(mode_key_long_conter > 2965000){
//                   mode_key_long_conter = 2965900;
//               
//               xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
//                         MODE_LONG_KEY_8,            /* 设置目标任务事件标志位bit0  */
//                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位��1�7*/
//
//                }
//
//
//         }
         
         //if(mode_key_long_conter < 2965000 ){
            
           xTaskNotify(xHandleTaskRunPro, /* 目标任务 */
                         MODE_KEY_1,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位��1�7*/

         // }


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
                 128,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 1,           		/* 任务优先纄1�71ￄ1�77 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskRunPro); /* 任务句柄  */
	
	xTaskCreate( vTaskDecoderPro,     		/* 任务函数  */
                 "vTaskDecoderPro",   		/* 任务各1�71ￄ1�77    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,               		/* 任务优先纄1�71ￄ1�77 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskDecoderPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务各1�71ￄ1�77    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,              		/* 任务优先纄1�71ￄ1�77 数��越小优先级越低，这个跟uCOS相反 */
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
# if 1 
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

       DISABLE_INT();
       switch(state)
		{
		case 0:  //#0
			if(inputBuf[0]==0x5A)  {//hex :4D - "M" -fixed mainboard
               
             
				state=1; 
		    }
			else{
			    state=0; //=1
              

			}
			break;
		case 1: //#1
		       
                   ptMsg = &g_tMsg;
                   ptMsg->usData[rx_data_counter] =  inputBuf[0];
                   rx_data_counter++;

                   if(inputBuf[rx_data_counter]==0x0A && rx_end_flag == 0){
                     
                          rx_end_flag = 1 ;
                          rx_end_counter = rx_data_counter;
                          
                    }
                    else if(rx_end_flag == 1){

                      rx_end_counter_compare = rx_data_counter - rx_end_counter;
                      if(rx_end_counter_compare == 1){
                   
                          if(inputBuf[rx_data_counter]==0x0D){

                              rx_end_counter_compare ++ ;


                          }

                        }
                        else{
                           rx_end_flag = 0;
                         


                        }


                    }
                    
                   

               
               if(rx_end_counter_compare == 2){

                      rx_end_counter_compare=0;
                      rx_end_flag=0;
                      rx_data_counter=0;
                   /* 向消息队列发数据 */
                     /* 初始化结构体指针 */
                     ptMsg = &g_tMsg;
                     ptMsg->ucMessageID=rx_data_counter;
                     
                	xQueueSendFromISR(xQueue2,
                				      (void *)&ptMsg,
                				      &xHigherPriorityTaskWoken);

                	/* 如果xHigherPriorityTaskWoken = pdTRUE，那么���出中断后切到当前朢�高优先级任务执行 */
                	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			

               }
		      
			break;
       #if 0
		case 2://#2
			if(inputBuf[0]=='D'||inputBuf[0]==0x4B || inputBuf[0]=='E'){
				if(inputBuf[0]=='D'){
                    rx_mb_data_tag=PANEL_DATA;
                    state = 3;

                }
                else if(inputBuf[0]=='E'){
                  rx_mb_data_tag=ORDER_DATA;
                  state=3;
                }
                else if(inputBuf[0]==0x4B){
                    state = 5;

                }
				
			}
            else{
				state=0;
				


				}
			break;
            
        case 3:

            if(rx_mb_data_tag==PANEL_DATA){
            
                // run_t.gReal_humtemp[0]=inputBuf[0]; //Humidity value 
                  rxcmd[0]=inputBuf[0];
                  state = 4; 
                
             }
			else if(rx_mb_data_tag==ORDER_DATA){

                rxcmd[0]=  inputBuf[0];  
                /* 初始化结构体指针 */
                ptMsg = &g_tMsg;
                ptMsg->ucMessageID=ORDER_DATA;
    		    ptMsg->ulData[0] = inputBuf[0]; //rxcmd[0];
                ptMsg ->usData[0] = 0;
               
               

                 /* 向消息队列发数据 */
            	xQueueSendFromISR(xQueue2,
            				      (void *)&ptMsg,
            				      &xHigherPriorityTaskWoken);

            	/* 如果xHigherPriorityTaskWoken = pdTRUE，那么���出中断后切到当前朢�高优先级任务执行 */
            	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

             
           

                state=0;

			}
            else {
               state = 0; 
          
            }
	    break;
        
		case 4: //

		 if(rx_mb_data_tag==PANEL_DATA){
              //run_t.gReal_humtemp[1]=inputBuf[0]; //temperature value
              
               /* 初始化结构体指针 */
                ptMsg = &g_tMsg;
                ptMsg->ucMessageID = PANEL_DATA;
    		    ptMsg->ulData[0] =  rxcmd[0];
                ptMsg ->usData[0] = inputBuf[0];
               
               

                 /* 向消息队列发数据 */
            	xQueueSendFromISR(xQueue2,
            				      (void *)&ptMsg,
            				      &xHigherPriorityTaskWoken);

            	/* 如果xHigherPriorityTaskWoken = pdTRUE，那么���出中断后切到当前朢�高优先级任务执行 */
            	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			
		     state=0;
		    
		
              
          }
         else {

		
			state =0;

         }
		
		 
		 break;

         case 5:

           rxcmd[0]=inputBuf[0];

           state = 6;


         break;


		 case 6:

            /* 初始化结构体指针 */
            ptMsg = &g_tMsg;
            ptMsg->ucMessageID=ANSWER_DATA;
		    ptMsg->ulData[0] = rxcmd[0];
            ptMsg ->usData[0] =  inputBuf[0];
           
           

             /* 向消息队列发数据 */
        	xQueueSendFromISR(xQueue2,
        				      (void *)&ptMsg,
        				      &xHigherPriorityTaskWoken);

        	/* 如果xHigherPriorityTaskWoken = pdTRUE，那么���出中断后切到当前朢�高优先级任务执行 */
        	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	       
		 
             state=0;
        break;
           
       
		
		
		default:
			
		break;
		}
      #endif 

          ENABLE_INT();
       __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
	}
}
}
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

















