/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
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
#include "dma.h"  // Include DMA header
#include "stm32f1xx.h"  // Include CMSIS header

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;  // Enable DMA1 clock

  DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;  // Set peripheral address to ADC data register
  DMA1_Channel1->CMAR = (uint32_t)adcReadings;  // Set memory address to ADC readings array
  DMA1_Channel1->CNDTR = 4;  // Set number of data transfers to 4
  DMA1_Channel1->CCR = DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC | DMA_CCR_PL_0;  // Configure DMA: memory increment, 32-bit size, circular mode, low priority
  DMA1_Channel1->CCR |= DMA_CCR_EN;  // Enable DMA channel

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
