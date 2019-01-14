/*
 * tim1.h
 *
 *  Created on: Jan 7, 2019
 *      Author: mk
 */

#ifndef TIM1_H_
#define TIM1_H_
#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM1_Init(uint32_t sampling_period_s);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
 }
#endif


#endif /* TIM1_H_ */
