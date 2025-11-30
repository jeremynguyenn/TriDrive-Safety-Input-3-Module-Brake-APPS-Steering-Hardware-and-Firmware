/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"  // Include ADC header
#include "stm32f1xx.h"  // Include CMSIS header

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  // ADC1 clock enable
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  // Enable ADC1 clock

  // GPIO analog: PA6 (CH6), PA7 (CH7), PB0 (CH8), PB1 (CH9)
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;  // Enable GPIOA and GPIOB clocks
  GPIOA->CRL &= ~(GPIO_CRL_MODE6_Msk | GPIO_CRL_CNF6_Msk | GPIO_CRL_MODE7_Msk | GPIO_CRL_CNF7_Msk);  // Set PA6 and PA7 to analog mode
  GPIOB->CRL &= ~(GPIO_CRL_MODE0_Msk | GPIO_CRL_CNF0_Msk | GPIO_CRL_MODE1_Msk | GPIO_CRL_CNF1_Msk);  // Set PB0 and PB1 to analog mode

  // ADC config: Scan enable, Continuous, 4 conversions
  ADC1->CR1 = ADC_CR1_SCAN;  // Enable scan mode
  ADC1->CR2 = ADC_CR2_CONT | ADC_CR2_DMA | ADC_CR2_EXTSEL;  // Enable continuous conversion, DMA, software trigger
  ADC1->SMPR2 = 0;  // Set sampling time to 1.5 cycles for channels

  // Sequence: CH6, CH7, CH8, CH9
  ADC1->SQR1 = (3 << 20);  // Set number of conversions to 4 (L=3)
  ADC1->SQR3 = (6 << 0) | (7 << 5) | (8 << 10) | (9 << 15);  // Set sequence ranks

  // Calibrate
  ADC1->CR2 |= ADC_CR2_RSTCAL;  // Reset calibration
  while (ADC1->CR2 & ADC_CR2_RSTCAL) {}  // Wait for reset complete
  ADC1->CR2 |= ADC_CR2_CAL;  // Start calibration
  while (ADC1->CR2 & ADC_CR2_CAL) {}  // Wait for calibration complete

  // Enable ADC
  ADC1->CR2 |= ADC_CR2_ADON;  // Enable ADC

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */