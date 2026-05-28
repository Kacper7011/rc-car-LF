/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void set_left(int spd);
static void set_right(int spd);
void motor_forward(uint32_t speed);
void motor_left(uint32_t speed);
void motor_right(uint32_t speed);
void motor_stop(void);
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  GPIO_InitTypeDef ledInit = {0};
  ledInit.Pin   = GPIO_PIN_0;
  ledInit.Mode  = GPIO_MODE_OUTPUT_PP;
  ledInit.Pull  = GPIO_NOPULL;
  ledInit.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &ledInit);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    int s1 = HAL_GPIO_ReadPin(IR_S1_GPIO_Port, IR_S1_Pin) == GPIO_PIN_RESET ? 1 : 0;
    int s2 = HAL_GPIO_ReadPin(IR_S2_GPIO_Port, IR_S2_Pin) == GPIO_PIN_RESET ? 1 : 0;
    int s3 = HAL_GPIO_ReadPin(IR_S3_GPIO_Port, IR_S3_Pin) == GPIO_PIN_RESET ? 1 : 0;
    int s4 = HAL_GPIO_ReadPin(IR_S4_GPIO_Port, IR_S4_Pin) == GPIO_PIN_RESET ? 1 : 0;
    int s5 = HAL_GPIO_ReadPin(IR_S5_GPIO_Port, IR_S5_Pin) == GPIO_PIN_RESET ? 1 : 0;

    /* position: negative = line left, positive = line right */
    int pos = -2*s1 - 1*s2 + 0*s3 + 1*s4 + 2*s5;
    int cnt = s1 + s2 + s3 + s4 + s5;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, cnt ? GPIO_PIN_SET : GPIO_PIN_RESET);

    if (cnt == 0) {
        motor_stop();
    } else if (pos < -1) {
        motor_left(700);
    } else if (pos > 1) {
        motor_right(700);
    } else {
        motor_forward(700);
    }
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static void set_left(int spd)
{
    if (spd >= 0) {
        HAL_GPIO_WritePin(MOT_L_IN1_GPIO_Port, MOT_L_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOT_L_IN2_GPIO_Port, MOT_L_IN2_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(MOT_L_IN1_GPIO_Port, MOT_L_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOT_L_IN2_GPIO_Port, MOT_L_IN2_Pin, GPIO_PIN_RESET);
        spd = -spd;
    }
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (uint32_t)spd);
}

static void set_right(int spd)
{
    if (spd >= 0) {
        HAL_GPIO_WritePin(MOT_R_IN3_GPIO_Port, MOT_R_IN3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOT_R_IN4_GPIO_Port, MOT_R_IN4_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(MOT_R_IN3_GPIO_Port, MOT_R_IN3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOT_R_IN4_GPIO_Port, MOT_R_IN4_Pin, GPIO_PIN_RESET);
        spd = -spd;
    }
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint32_t)spd);
}

void motor_forward(uint32_t speed)
{
    set_left((int)speed);
    set_right((int)speed);
}

void motor_left(uint32_t speed)
{
    set_left((int)(speed / 3));
    set_right((int)speed);
}

void motor_right(uint32_t speed)
{
    set_left((int)speed);
    set_right((int)(speed / 3));
}

void motor_stop(void)
{
    HAL_GPIO_WritePin(MOT_L_IN1_GPIO_Port, MOT_L_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOT_L_IN2_GPIO_Port, MOT_L_IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOT_R_IN3_GPIO_Port, MOT_R_IN3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOT_R_IN4_GPIO_Port, MOT_R_IN4_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
