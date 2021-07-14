#ifndef __KEY_H
#define __KEY_H	 


#include "gpio.h"


#define sm0(a) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,a) //sao_miao
#define sm1(a) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,a)
#define sm2(a) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,a)
#define sm3(a) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,a)
#define KEY3  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)   //????0
#define KEY2  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)   //????1
#define KEY1  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)   //????2 
#define KEY0  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5)   //????




#define KEY0_PRES 	  1  	 
#define KEY1_PRES	  2
#define KEY2_PRES	  3
#define KEY3_PRES  	  4
#define KEY4_PRES  	  5 
#define KEY5_PRES     6
#define KEY6_PRES     7	 
#define KEY7_PRES     8
#define KEY8_PRES     9 
#define KEY9_PRES    10 
#define KEY10_PRES   11 
#define KEY11_PRES   12 
#define KEY12_PRES   13 
#define KEY13_PRES   14 
#define KEY14_PRES   15
#define KEY15_PRES   16

void KEY_Init(void);
uint8_t KEY_Scan(void);

#endif

