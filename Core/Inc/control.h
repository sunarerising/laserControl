/*
 * control.h
 *
 *  Created on: Jul 10, 2021
 *      Author: hp
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "usart.h"

void USER_UART_IRQHandler(UART_HandleTypeDef *huart);

void AngleX_Limited(void);
void AngleY_Limited(void);

int DistanceX_FeedbackControl(int TargetDistance, int CurrentDistance);
int DistanceY_FeedbackControl(int TargetDistance, int CurrentDistance);



#endif /* INC_CONTROL_H_ */
