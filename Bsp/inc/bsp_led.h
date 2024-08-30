#ifndef __BSP_LED_H_
#define __BSP_LED_H_
#include "main.h"

//extern IWDG_HandleTypeDef IWDG_Handler;

#define POWER_ON_LED()             do{LED_INFO_POWER_GPIO_Port->BSRR |= LED_INFO_POWER_Pin;}while(0)//HAL_GPIO_WritePin(LED_INFO_POWER_GPIO_Port, LED_INFO_POWER_Pin,GPIO_PIN_SET)
#define POWER_OFF_LED()            do{LED_INFO_POWER_GPIO_Port->BRR = LED_INFO_POWER_Pin; }while(0)//HAL_GPIO_WritePin(LED_INFO_POWER_GPIO_Port, LED_INFO_POWER_Pin,GPIO_PIN_RESET)


#define LED_MODEL_ON()				HAL_GPIO_WritePin(LED_INFO_MODE_GPIO_Port , LED_INFO_MODE_Pin,GPIO_PIN_SET)
#define LED_MODEL_OFF()             HAL_GPIO_WritePin(LED_INFO_MODE_GPIO_Port , LED_INFO_MODE_Pin,GPIO_PIN_RESET)


#define POWER_TOGGLE()            HAL_GPIO_TogglePin(LED_INFO_POWER_GPIO_Port, LED_INFO_POWER_Pin)




extern void (*panel_led_fun)(void);


void Breath_Led(void);
void Lcd_PowerOn_Fun(void);

void Lcd_PowerOff_Fun(void);

void Panel_Led_OnOff_RunCmd(void (*panelledHandler)(void));


void Panel_Led_OnOff_Function(void);

void lcd_power_off_donot_fan_Fun(void);

//void IWDG_Init(uint8_t prer,uint16_t rlr);
//void IWDG_Feed(void);


#endif 
