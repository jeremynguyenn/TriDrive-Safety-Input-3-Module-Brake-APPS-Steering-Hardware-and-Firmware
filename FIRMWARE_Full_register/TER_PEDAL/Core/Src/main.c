/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"  // Include main header
#include "adc.h"  // Include ADC header
#include "can.h"  // Include CAN header
#include "dma.h"  // Include DMA header
#include "tim.h"  // Include TIM header
#include "gpio.h"  // Include GPIO header
#include "stm32f1xx.h"  // Include CMSIS header for core registers

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TeR_CAN.h" //Master libray  // Include TeR_CAN library
#include "pedal.h" //Board functions Library  // Include pedal library
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

//Mensajes

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);  // Declare system clock configuration function
static void MX_NVIC_Init(void);  // Declare NVIC initialization function
/* USER CODE BEGIN PFP */
void readSensors(void); //Lectura de todos los sensores  // Declare function to read sensors
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

  // Thay HAL_Init() bằng bare-metal init
  // Systick init (tương tự HAL_InitTicks)
  SysTick->LOAD = SystemCoreClock / 1000 - 1;  // Set SysTick reload value for 1ms tick
  SysTick->VAL = 0;  // Clear current value register
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;  // Enable SysTick with processor clock, interrupt, and counter

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();  // Call system clock configuration

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();  // Initialize GPIO
  MX_DMA_Init();  // Initialize DMA
  MX_ADC1_Init();  // Initialize ADC1
  MX_CAN_Init();  // Initialize CAN
  MX_TIM2_Init();  // Initialize TIM2

  /* Initialize interrupts */
  MX_NVIC_Init();  // Initialize NVIC
  /* USER CODE BEGIN 2 */
	initPedal(); //Carga los offsets en la placa y arranca enganchada al adc1  // Initialize pedal module
	initCAN(); //Arranca el can enganchado a el timer 2  // Initialize CAN module
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {  // Start infinite loop
		; //Good code depends just on interruptions : )  // Empty statement; rely on interrupts
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  // Enable HSE
  RCC->CR |= RCC_CR_HSEON;  // Enable High-Speed External oscillator
  while (!(RCC->CR & RCC_CR_HSERDY)) {}  // Wait for HSE ready

  // PLL config
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);  // Clear PLL source, prediv, and multiplier bits
  RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL4;  // Set PLL source to HSE and multiplier to 4

  RCC->CR |= RCC_CR_PLLON;  // Enable PLL
  while (!(RCC->CR & RCC_CR_PLLRDY)) {}  // Wait for PLL ready

  // Select PLL as system clock
  RCC->CFGR &= ~RCC_CFGR_SW;  // Clear system clock switch bits
  RCC->CFGR |= RCC_CFGR_SW_PLL;  // Select PLL as system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}  // Wait for switch status to confirm PLL

  // AHB, APB1, APB2 dividers
  RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);  // Clear prescaler bits
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;  // Set APB2 prescaler to divide by 2

  // ADC clock div4
  RCC->CFGR &= ~RCC_CFGR_ADCPRE;  // Clear ADC prescaler bits
  RCC->CFGR |= RCC_CFGR_ADCPRE_DIV4;  // Set ADC clock divider to 4

  // Flash latency
  FLASH->ACR &= ~FLASH_ACR_LATENCY;  // Clear latency bits
  FLASH->ACR |= FLASH_ACR_LATENCY_1;  // Set latency to 1 wait state
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  // USB_LP_CAN1_RX0_IRQn interrupt configuration
  NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0);  // Set priority for CAN RX interrupt
  NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);  // Enable CAN RX interrupt

  // TIM2_IRQn
  NVIC_SetPriority(TIM2_IRQn, 0);  // Set priority for TIM2 interrupt
  NVIC_EnableIRQ(TIM2_IRQn);  // Enable TIM2 interrupt
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
	__disable_irq();  // Disable all interrupts
	while (1) {  // Infinite loop on error
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