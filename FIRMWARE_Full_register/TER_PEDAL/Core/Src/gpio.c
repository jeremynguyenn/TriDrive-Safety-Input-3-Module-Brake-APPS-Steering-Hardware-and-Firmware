/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"  // Include the GPIO header file for definitions
#include "stm32f1xx.h"  // Include CMSIS header for register definitions

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{
  // GPIO Ports Clock Enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;  // Enable clocks for GPIO ports D, A, and B

  // Configure GPIO pin Output Level
  GPIOA->ODR &= ~(IMP_LED_Pin | DBG_LED_Pin);  // Set the output data register to low (RESET) for IMP_LED and DBG_LED pins

  // Configure GPIO pins: IMP_LED_Pin and DBG_LED_Pin as Output PP, Low Speed, No Pull
  GPIOA->CRL &= ~(GPIO_CRL_MODE3_Msk | GPIO_CRL_CNF3_Msk | GPIO_CRL_MODE4_Msk | GPIO_CRL_CNF4_Msk);  // Clear the mode and configuration bits for pins 3 and 4 (assuming IMP_LED is pin 3, DBG_LED is pin 4)
  GPIOA->CRL |= (GPIO_CRL_MODE3_0 | GPIO_CRL_MODE4_0);  // Set mode to output with max speed 10 MHz (low speed) for pins 3 and 4

  // Thêm cấu hình cho các pin khác nếu cần (dựa trên mã gốc)
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */