/*
 * motor.c
 *
 *  Created on: 2021年7月13日
 *      Author: hp
 */

#include "motor.h"

uint8_t LobotTxBuf[128];  //发送缓存
uint8_t LobotRxBuf[16];   //没用到
uint16_t batteryVolt;	  //没用到

#define GET_LOW_BYTE(A) ((uint8_t)(A))
//宏函数 获得A的低八位
#define GET_HIGH_BYTE(A) ((uint8_t)((A) >> 8))
//宏函数 获得A的高八位

LobotServo servoXY[2] = {
		{MOTORX_ID,1000},
		{MOTORY_ID,118}
};

void moveServosByArray(LobotServo servos[], uint8_t Num, uint16_t Time)
{
	uint8_t index = 7;
	uint8_t i = 0;

	if (Num < 1 || Num > 32 || !(Time > 0)) {
		return;                                          //舵机数不能为零和大与32，时间不能为零
	}
	LobotTxBuf[0] = LobotTxBuf[1] = FRAME_HEADER;      //填充帧头
	LobotTxBuf[2] = Num * 3 + 5;                       //数据长度 = 要控制舵机数*3+5
	LobotTxBuf[3] = CMD_SERVO_MOVE;                    //填充舵机移动指令
	LobotTxBuf[4] = Num;                               //要控制的舵机个数
	LobotTxBuf[5] = GET_LOW_BYTE(Time);                //取得时间的低八位
	LobotTxBuf[6] = GET_HIGH_BYTE(Time);               //取得时间的高八位

	for (i = 0; i < Num; i++) {                        //循环填充舵机ID和对应目标位置
		LobotTxBuf[index++] = servos[i].ID;              //填充舵机ID
		LobotTxBuf[index++] = GET_LOW_BYTE(servos[i].Position); //填充目标位置低八位
		LobotTxBuf[index++] = GET_HIGH_BYTE(servos[i].Position);//填充目标位置高八位
	}

//	uartWriteBuf(LobotTxBuf, LobotTxBuf[2] + 2);             //发送
	HAL_UART_Transmit(&huart3, LobotTxBuf, LobotTxBuf[2] + 2, 0xffff);
}

void servoRotate(uint16_t AngleX,uint16_t AngleY){
	servoXY[0].Position = AngleX+500;
	servoXY[1].Position = AngleY+500;
	moveServosByArray(servoXY, 2, 30);
}



