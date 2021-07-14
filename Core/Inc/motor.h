/*
 * motor.h
 *
 *  Created on: 2021年7月13日
 *      Author: hp
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "usart.h"

#define FRAME_HEADER 0x55             //帧头
#define CMD_SERVO_MOVE 0x03           //舵机移动指令
#define CMD_ACTION_GROUP_RUN 0x06     //运行动作组指令
#define CMD_ACTION_GROUP_STOP 0x07    //停止动作组指令
#define CMD_ACTION_GROUP_SPEED 0x0B   //设置动作组运行速度
#define CMD_GET_BATTERY_VOLTAGE 0x0F  //获取电池电压指令

#define MOTORX_ID 0x09 //x轴舵机
#define MOTORY_ID 0x02 //y轴舵机

typedef struct _lobot_servo_ {  //舵机ID,舵机目标位置
	uint8_t ID;
	uint16_t Position;
} LobotServo;

void moveServosByArray(LobotServo servos[], uint8_t Num, uint16_t Time); //position 0-2000
void servoRotate(uint16_t AngleX,uint16_t AngleY);


#endif /* INC_MOTOR_H_ */
