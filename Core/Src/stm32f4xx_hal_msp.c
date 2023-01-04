/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/* USER CODE BEGIN 1 */
/* code just to follow the MCU2 course, there is no need since
 * low level configuration is already placed in usart.c
 */
	/* void HAL_UART_MspInit(UART_HandleTypeDef *huart){
		// Low level inits
		GPIO_InitTypeDef gpio_uart;

		// 1. Enable the clock for the USART2 peripheral
		__HAL_RCC_USART2_CLK_ENABLE();

		// 2. Do the pin muxing configurations
		gpio_uart.Pin = GPIO_PIN_2;
		gpio_uart.Mode = GPIO_MODE_AD_PP;
		gpio_uart.Pull = PULLUP;
		gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
		gpio_uart.Alternate = GPIO_AF7_USART2; //UART2_TX
		HAL_GPIO_Init(GPIOA,&gpio_uart); // GPIO doesn't have handle struct

		gpio_uart.Pin = GPIO_PIN_3;	//UART2_RX
		HAL_GPIO_Init(GPIOA,&gpio_uart); // GPIO doesn't have handle struct

		// 3. Enable the IRQn
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
	} */
/* USER CODE END 1 */
