/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32h7xx_hal.h"

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
#define PS2_DI_Pin GPIO_PIN_0
#define PS2_DI_GPIO_Port GPIOF
#define PS2_D0_Pin GPIO_PIN_2
#define PS2_D0_GPIO_Port GPIOF
#define U4_1_Pin GPIO_PIN_4
#define U4_1_GPIO_Port GPIOF
#define PS2_CS_Pin GPIO_PIN_7
#define PS2_CS_GPIO_Port GPIOF
#define U4_2_Pin GPIO_PIN_8
#define U4_2_GPIO_Port GPIOF
#define U3_2_Pin GPIO_PIN_9
#define U3_2_GPIO_Port GPIOF
#define PS1_CLK_Pin GPIO_PIN_0
#define PS1_CLK_GPIO_Port GPIOC
#define U2_PWM_Pin GPIO_PIN_9
#define U2_PWM_GPIO_Port GPIOE
#define U1_PWM_Pin GPIO_PIN_11
#define U1_PWM_GPIO_Port GPIOE
#define U3_PWM_Pin GPIO_PIN_13
#define U3_PWM_GPIO_Port GPIOE
#define U4_PWM_Pin GPIO_PIN_14
#define U4_PWM_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOB
#define U1_1_Pin GPIO_PIN_7
#define U1_1_GPIO_Port GPIOC
#define U2_1_Pin GPIO_PIN_8
#define U2_1_GPIO_Port GPIOC
#define U1_2_Pin GPIO_PIN_9
#define U1_2_GPIO_Port GPIOC
#define U2_2_Pin GPIO_PIN_10
#define U2_2_GPIO_Port GPIOC
#define SDA_Pin GPIO_PIN_11
#define SDA_GPIO_Port GPIOC
#define SCL_Pin GPIO_PIN_12
#define SCL_GPIO_Port GPIOC
#define SYS_LED_Pin GPIO_PIN_2
#define SYS_LED_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
