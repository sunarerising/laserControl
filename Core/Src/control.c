/*
 * control.h
 *
 *  Created on: Jul 10, 2021
 *      Author: hp
 */

#include "control.h"
#include "main.h"
#include "tim.h"
#include "motor.h"


extern uint16_t pointX;
extern uint16_t pointY;
extern uint8_t UART1_Rx_Buf[100];

extern float DistanceX_Kp, DistanceX_Ki, DistanceX_Kd;		//距离环控制函数
extern float DistanceY_Kp, DistanceY_Ki, DistanceY_Kd;		//距离环控制函数

extern uint16_t TargetPointX;		//球的目标位置
extern uint16_t TargetPointY;

extern uint16_t AngleX;
extern uint16_t AngleY;

extern uint16_t AngleXopen;
extern uint16_t AngleYopen;

extern uint8_t motorRun;

float DistanceIntegral_biasX;
float DistanceIntegral_biasY;


uint16_t CircleX[25] = {400,395,380,356,325,289,250,211,175,144,120,105,100,105,120,144,175,211,250,289,325,356,380,395};
uint16_t CircleY[25] = {250,289,325,356,380,395,400,395,380,356,325,289,250,211,175,144,120,105,100,105,120,144,175,211};


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	static int CircleCount;
	if(htim->Instance == TIM6){
		if(CircleCount <= 24){
			TargetPointX = CircleX[CircleCount];
			TargetPointY = CircleY[CircleCount];
//			DistanceIntegral_biasX = 0;
//			DistanceIntegral_biasY = 0;
			CircleCount++;
		}
		else{
			CircleCount= 0;
			HAL_TIM_Base_Stop_IT(&htim6);
			TargetPointX = 250;
			TargetPointY = 250;
			DistanceX_Kp = 0.1;
			DistanceX_Ki = 0.05;
			DistanceX_Kd = 0.01;		//距离环控制函�??????(0.1 0.05 0)
			DistanceY_Kp = 0.1;
			DistanceY_Ki = 0.04;
			DistanceY_Kd = 0.01;
		}
	}
}
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//	if(huart->Instance==USART3){
//		if(UART3_Rx_Buf[0] == 0xAA && UART3_Rx_Buf[1] == 0xAE){
//			pointX = UART3_Rx_Buf[2];
//			pointY = UART3_Rx_Buf[3];
//		}
//		HAL_UART_Receive_IT(&huart3,(uint8_t *)UART3_Rx_Buf,4);
//	}
//}

void USER_UART_IRQHandler(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){				//判断是否是串口1（！此处应写(huart->Instance == USART1)
		if(RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)){    //判断是否是空闲中断
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);                     //清楚空闲中断标志（否则会一直不断进入中断）
			HAL_UART_DMAStop(&huart1);
//			rx_len =  100 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
			if(UART1_Rx_Buf[0] == 0xA1)
			{
				pointX = UART1_Rx_Buf[2]<<8 | UART1_Rx_Buf[1];
				pointY = UART1_Rx_Buf[4]<<8 | UART1_Rx_Buf[3];
			}
			if(motorRun == 1){
				AngleX = -DistanceX_FeedbackControl(TargetPointX , pointX)+1000;
				AngleY = -DistanceY_FeedbackControl(TargetPointY , pointY)+118;
				AngleX_Limited();
				AngleY_Limited();
				servoRotate(AngleX, AngleY);
			}
			else{
				DistanceIntegral_biasX = 0;
				DistanceIntegral_biasY = 0;
			}

		}
	}
	HAL_UART_Receive_DMA(&huart1,(uint8_t *)UART1_Rx_Buf,100);
}


int DistanceX_FeedbackControl(int TargetDistance, int CurrentDistance){
	static float DistanceBiasX, PWMX, DistanceLast_BiasX;
	DistanceBiasX = TargetDistance - CurrentDistance;
	if(DistanceBiasX <-10 || DistanceBiasX >10 ){
		DistanceIntegral_biasX += DistanceLast_BiasX;
	}
	else{
		DistanceBiasX = 0;
	}

	DistanceIntegral_biasX=DistanceIntegral_biasX>1500?1500:(DistanceIntegral_biasX<(-1500)?(-1500):DistanceIntegral_biasX);

	PWMX = DistanceX_Kp * DistanceBiasX + DistanceX_Ki * DistanceIntegral_biasX + DistanceX_Kd * ( DistanceBiasX - DistanceLast_BiasX );
	DistanceLast_BiasX = DistanceBiasX;
	return PWMX;
}

int DistanceY_FeedbackControl(int TargetDistance, int CurrentDistance){
	static float DistanceBiasY, PWMY, DistanceLast_BiasY;
	DistanceBiasY = TargetDistance - CurrentDistance;
	if(DistanceBiasY <-10 || DistanceBiasY >10 ){
		DistanceIntegral_biasY += DistanceLast_BiasY;
	}
	else{
		DistanceBiasY = 0;
	}


	DistanceIntegral_biasY=DistanceIntegral_biasY>1500?1500:(DistanceIntegral_biasY<(-1500)?(-1500):DistanceIntegral_biasY);

	PWMY = DistanceY_Kp * DistanceBiasY + DistanceY_Ki * DistanceIntegral_biasY + DistanceY_Kd * ( DistanceBiasY - DistanceLast_BiasY );
	DistanceLast_BiasY = DistanceBiasY;
	return PWMY;
}

void AngleX_Limited(void)
{
	if(AngleX>2000)
		AngleX = 2000;
	if(AngleX<0)
		AngleX = 0;
}

void AngleY_Limited(void)
{
	if(AngleY>214)
		AngleY = 214;
	if(AngleY<118)
		AngleY = 118;

}

