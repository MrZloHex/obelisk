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
#include "main.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "string.h"
#include "controller.h"
#include "serial.h"
// #include "rreset.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


ResetCause
reset_cause_get(void)
{
    ResetCause reset_cause;

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
    {
        reset_cause = RC_LOW_POWER_RESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
    {
        reset_cause = RC_WINDOW_WATCHDOG_RESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
    {
        reset_cause = RC_INDEPENDENT_WATCHDOG_RESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
    {
        reset_cause = RC_SOFTWARE_RESET; 
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
    {
        reset_cause = RC_POWER_ON_POWER_DOWN_RESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
    {
        reset_cause = RC_EXTERNAL_RESET_PIN_RESET;
    }
    else
    {
        reset_cause = RC_UNKNOWN;
    }

    __HAL_RCC_CLEAR_RESET_FLAGS();

    return reset_cause; 
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#define RAPI_UART huart4
#define OCPP_UART huart1

static Controller controller = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void
HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// OCPP
	if (huart->Instance == controller.ocpp.uart->Instance)
	{
    controller_uart_usart_callback(&controller.ocpp.ctrl_uart);
  }
	// RAPI
	if (huart->Instance == controller.rapi.ctrl_uart.huart->Instance)
  {
    controller_uart_usart_callback(&controller.rapi.ctrl_uart);
  }

}

void
HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == controller.ocpp.tim->Instance)
	{
    controller_uart_timer_callback(&controller.ocpp.ctrl_uart);
	}
	else if (htim->Instance == controller.rapi.tim->Instance)
	{
		controller_uart_timer_callback(&controller.rapi.ctrl_uart);
	}
}

#define DEBUG
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

  ResetCause rc = reset_cause_get();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_IWDG_Init();
  MX_RTC_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

#ifdef DEBUG
  uprintf(&OCPP_UART, 100, 20, "DEBUG START\r");
	// uprintf(&OCPP_UART, 100, 20, "%u %u\n", rc, sizeof(Controller_Result));
#endif

  HAL_IWDG_Refresh(&hiwdg);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);

  // Controller_Result res = controller_initialize
  // (
  // 	&controller,
  // 	&OCPP_UART, &RAPI_UART,
  // 	&htim6, &htim7,
  // 	&hrtc, &hi2c2,
	//   GPIOB, GPIO_PIN_1,
  //   rc
  // );
  // if (res.type != CTRL_OK)
  // {
  //   controller.memory.fatal_err = res;
  //   _controller_memory_store(&(controller.memory));
	//   Error_Handler_with_err("FAILED ON INITIALIZATION");
  // }

  // controller.ocpp.it_error = (Controller_Protocol_Result)HAL_UART_Receive_IT(&OCPP_UART, (uint8_t *)&(controller.ocpp.accumulative_buffer[0]), 1);
  // controller.rapi.it_error = (Controller_Protocol_Result)HAL_UART_Receive_IT(&RAPI_UART, (uint8_t *)&(controller.rapi.accumulative_buffer[0]), 1); // delete

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	HAL_IWDG_Refresh(&hiwdg);
	// res = controller_update(&controller);
	// if (res.type != CTRL_OK)
	// {
  //   controller.memory.fatal_err = res;
  //   _controller_memory_store(&(controller.memory));
	// 	uprintf(DBUG_UART, 1000, 100, "ERR: %u\n", res.type);
	// 	uprintf(DBUG_UART, 1000, 100, "PTCL ERR: %u\n", res.errors.ocpp_err);
	// 	uprintf(DBUG_UART, 1000, 100, "TSET ERR: %u\n", res.errors.tset_err);
  //   // TODO add DataTransfer when these errors occur
	// 	Error_Handler_with_err("FAILED IN LOOP");
	// }
	// res.type = 0;
	// res.errors.ocpp_err = 0;
		// YOU SHOULD HANDLE IT!!
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV128;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void
Error_Handler_with_err(const char * err)
{
  #ifndef NODEBUG
  uprintf(DBUG_UART, 1000, 256, "ERROR: %s\r", err);
  #endif
  Error_Handler();
}

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
	HAL_GPIO_WritePin(ERROR_LED, GPIO_PIN_SET);
	// rapi_deinit(&rapi);
	// ocpp_deinit(&ocpp);
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
