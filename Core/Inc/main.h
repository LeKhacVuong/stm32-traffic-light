/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPS_RST_Pin GPIO_PIN_1
#define GPS_RST_GPIO_Port GPIOA
#define GPS_TX_Pin GPIO_PIN_2
#define GPS_TX_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_3
#define GPS_RX_GPIO_Port GPIOA
#define SPI1_SS_Pin GPIO_PIN_4
#define SPI1_SS_GPIO_Port GPIOA
#define ADC1_IN8_PIN_Pin GPIO_PIN_0
#define ADC1_IN8_PIN_GPIO_Port GPIOB
#define CTRL_LED_WRN_Pin GPIO_PIN_1
#define CTRL_LED_WRN_GPIO_Port GPIOB
#define LTE_TX_Pin GPIO_PIN_10
#define LTE_TX_GPIO_Port GPIOB
#define LTE_RX_Pin GPIO_PIN_11
#define LTE_RX_GPIO_Port GPIOB
#define EPS_RST_Pin GPIO_PIN_12
#define EPS_RST_GPIO_Port GPIOB
#define ESP_BOOT_Pin GPIO_PIN_13
#define ESP_BOOT_GPIO_Port GPIOB
#define LORA_RST_Pin GPIO_PIN_14
#define LORA_RST_GPIO_Port GPIOB
#define LORA_BOOT_Pin GPIO_PIN_15
#define LORA_BOOT_GPIO_Port GPIOB
#define EPS_TX_Pin GPIO_PIN_9
#define EPS_TX_GPIO_Port GPIOA
#define ESP_RX_Pin GPIO_PIN_10
#define ESP_RX_GPIO_Port GPIOA
#define CTRL_LED_RED_Pin GPIO_PIN_3
#define CTRL_LED_RED_GPIO_Port GPIOB
#define CTRL_LED_YELLOW_Pin GPIO_PIN_4
#define CTRL_LED_YELLOW_GPIO_Port GPIOB
#define CTRL_LED_GREEN_Pin GPIO_PIN_5
#define CTRL_LED_GREEN_GPIO_Port GPIOB
#define LTE_RST_Pin GPIO_PIN_9
#define LTE_RST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern ADC_HandleTypeDef hadc1;

extern I2C_HandleTypeDef hi2c1;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern PCD_HandleTypeDef hpcd_USB_FS;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
