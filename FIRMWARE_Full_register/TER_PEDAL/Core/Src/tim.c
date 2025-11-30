/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "tim.h"  // Include TIM header
#include "stm32f1xx.h"  // Include CMSIS header

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  // TIM2 clock enable
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable TIM2 clock

  TIM2->PSC = 0;  // Set prescaler to 0
  TIM2->ARR = 63999;  // Set auto-reload register (period)
  TIM2->CR1 = TIM_CR1_URS;  // Set control register: update request source
  TIM2->EGR = TIM_EGR_UG;  // Generate update event

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */