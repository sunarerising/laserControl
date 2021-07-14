/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "stdio.h"
#include "string.h"
#include "key.h"
#include "control.h"
#include "motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t strBuffer[100];
uint8_t keyValue;
uint16_t countX=1000;
uint16_t countY=100;
uint8_t UART1_Rx_Buf[100] = {0};

uint8_t ringCount;
uint8_t ringDerection;
uint16_t pointX=0;
uint16_t pointY=0;

uint16_t TargetPointX = 250;		//球的目标位置
uint16_t TargetPointY = 250;


float DistanceX_Kp = 0.05, DistanceX_Ki = 0.05, DistanceX_Kd = 0.08;		//距离环控制函�??????(0.1 0.05 0.1)
float DistanceY_Kp = 0.05, DistanceY_Ki = 0.04, DistanceY_Kd = 0.08;		//距离环控制函�??????(0.1 0.04 0.1)

uint16_t AngleX;
uint16_t AngleY;

uint16_t AngleXopen = 1000;
uint16_t AngleYopen = 118;

uint8_t motorRun;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_Clear();
//  HAL_UART_Receive_IT(&huart3,(uint8_t *)UART3_Rx_Buf,4);
  HAL_UART_Receive_DMA(&huart1,(uint8_t *)UART1_Rx_Buf,100);
  servoRotate(1000, 118);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	keyValue = KEY_Scan();
	if(keyValue == 1){
		AngleXopen = 1000;
	}
	else if(keyValue == 2){
		if(AngleXopen < 2000)
			AngleXopen = AngleXopen + 3;
		else AngleXopen = 2000;
	}
	else if(keyValue == 3){
		if(AngleXopen >0 )
			AngleXopen = AngleXopen - 3;
		else AngleXopen = 0;
	}
	else if(keyValue == 4){
		AngleXopen = 1000;
	}

	if(keyValue == 5){
		AngleYopen = 118;
	}
	else if(keyValue == 6){
		if(AngleYopen < 214)
			AngleYopen = AngleYopen + 3;
		else AngleYopen = 214;
	}
	else if(keyValue == 7){
		if(AngleYopen > 100)
			AngleYopen = AngleYopen - 3;
		else AngleYopen = 100;
	}
	else if(keyValue == 8){
		AngleYopen = 118;
	}
//	else if(keyValue == 9){
//		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
//	}
	else if(keyValue == 10){
		motorRun = !motorRun;
	}
	else if(keyValue == 11){
		if(TargetPointX < 500){
			TargetPointX = TargetPointX + 25;
		}
		else{
			TargetPointX = 0;

		}

	}
	else if(keyValue == 12){
//		HAL_TIM_Base_Start_IT(&htim6);
		if(TargetPointY < 500){
			TargetPointY = TargetPointY + 25;
		}
		else{
			TargetPointY = 0;

		}
	}
	else if(keyValue == 14){
		HAL_TIM_Base_Stop_IT(&htim6);
		TargetPointX = 250;
		TargetPointY = 250;
		DistanceX_Kp = 0.05;
		DistanceX_Ki = 0.05;
		DistanceX_Kd = 0.01;		//距离环控制函�??????(0.1 0.05 0)
		DistanceY_Kp = 0.05;
		DistanceY_Ki = 0.04;
		DistanceY_Kd = 0.01;
	}
	else if(keyValue == 13){
		HAL_TIM_Base_Start_IT(&htim6);
		DistanceX_Kp = 0.03;
		DistanceX_Ki = 0.05;
		DistanceX_Kd = 0.01;		//距离环控制函�??????(0.1 0.05 0)
		DistanceY_Kp = 0.03;
		DistanceY_Ki = 0.04;
		DistanceY_Kd = 0.01
				;		//距离环控制函�??????(0.06 0.04 0)
	}


	if(motorRun == 0){
		servoRotate(AngleXopen, AngleYopen);
	}


	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);

//	sprintf((char*)strBuffer,"hello world");
//	OLED_ShowString(0, 0, strBuffer, sizeof(strBuffer));
//	sprintf((char*)strBuffer,"CountX = %4d",AngleXopen);
//	OLED_ShowString(0, 1, strBuffer, sizeof(strBuffer));
//	sprintf((char*)strBuffer,"CountY = %4d",AngleYopen);
//	OLED_ShowString(0, 2, strBuffer, sizeof(strBuffer));
//	sprintf((char*)strBuffer,"pointX = %4d",pointX);
//	OLED_ShowString(0, 3, strBuffer, sizeof(strBuffer));
//	sprintf((char*)strBuffer,"pointY = %4d",pointY);
//	OLED_ShowString(0, 4, strBuffer, sizeof(strBuffer));
//	sprintf((char*)strBuffer,"TargetX = %4d",TargetPointX);
//	OLED_ShowString(0, 5, strBuffer, sizeof(strBuffer));
//	sprintf((char*)strBuffer,"TargetY = %4d",TargetPointY);
//	OLED_ShowString(0, 6, strBuffer, sizeof(strBuffer));

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
