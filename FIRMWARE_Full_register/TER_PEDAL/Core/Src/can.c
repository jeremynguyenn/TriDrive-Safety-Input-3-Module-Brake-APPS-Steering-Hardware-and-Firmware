/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    can.c
 * @brief   This file provides code for the configuration
 *          of the CAN instances.
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
#include "can.h"  // Include CAN header
#include "stm32f1xx.h"  // Include CMSIS header

/* USER CODE BEGIN 0 */
#include "ter.h"  // Include TER header

/* USER CODE END 0 */

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  // CAN clock enable
  RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;  // Enable CAN1 clock

  // GPIO for CAN (PA11 RX, PA12 TX)
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  // Enable GPIOA clock
  GPIOA->CRH &= ~(GPIO_CRH_MODE11_Msk | GPIO_CRH_CNF11_Msk | GPIO_CRH_MODE12_Msk | GPIO_CRH_CNF12_Msk);  // Clear config for PA11 and PA12
  GPIOA->CRH |= (GPIO_CRH_CNF11_1 | GPIO_CRH_MODE12_0 | GPIO_CRH_CNF12_1);  // Set PA11 as input floating, PA12 as AF PP 10MHz

  // CAN config: Prescaler 4, Mode Normal, SJW 2, BS1 6, BS2 1
  CAN1->MCR = CAN_MCR_INRQ;  // Enter initialization mode
  while (!(CAN1->MSR & CAN_MSR_INAK)) {}  // Wait for initialization mode

  CAN1->BTR = (1 << 30) | (1 << 24) | (5 << 20) | (0 << 16) | (3 << 0);  // Set bit timing register: normal mode, SJW=2, BS1=6, BS2=1, prescaler=4

  CAN1->MCR |= CAN_MCR_ABOM;  // Enable auto bus off management
  CAN1->MCR &= ~CAN_MCR_AWUM;  // Disable auto wakeup
  CAN1->MCR &= ~CAN_MCR_NART;  // Disable no auto retransmission (gốc DISABLE, tức NART=1? nhưng bare-metal set to disable retrans)
  CAN1->MCR &= ~CAN_MCR_RFLM;  // Disable receive FIFO locked
  CAN1->MCR &= ~CAN_MCR_TXFP;  // Disable transmit FIFO priority

  CAN1->MCR &= ~CAN_MCR_INRQ;  // Exit initialization mode
  while (CAN1->MSR & CAN_MSR_INAK) {}  // Wait to exit initialization mode

  // Filter config (for TER_COMMAND_FRAME_ID)
  CAN1->FMR |= CAN_FMR_FINIT;  // Enter filter initialization mode
  CAN1->FA1R |= 1;  // Activate filter 0
  CAN1->FM1R &= ~1;  // Set to ID list mode
  CAN1->FS1R |= 1;  // Set to 32-bit scale
  CAN1->sFilterRegister[0].FR1 = (TER_COMMAND_FRAME_ID << 5);  // Set filter ID high
  CAN1->sFilterRegister[0].FR2 = 0;  // Set filter mask or low ID
  CAN1->FFA1R &= ~1;  // Assign to FIFO 0
  CAN1->FMR &= ~CAN_FMR_FINIT;  // Exit filter initialization mode

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */