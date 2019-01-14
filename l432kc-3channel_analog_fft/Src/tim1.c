/*
 * tim1.c
 *
 *  Created on: Jan 7, 2019
 *      Author: mk
 */
#include "main.h"
#include "tim1.h"
#include "adc.h"
#include "stm32l4xx_nucleo_32.h"

#define TIM1_PRINCIPAL_CLOCK_FREQ (2071L)
#define TIM1_PERIOD_FOR_SAMPLING_FREQ_1024HZ  (2L)
#define TIM1_PERIOD_FOR_SAMPLING_FREQ_512HZ   (4L)
#define TIM1_PERIOD_FOR_SAMPLING_FREQ_256HZ   (8L)
#define TIM1_PERIOD_FOR_SAMPLING_FREQ_128HZ   (16L)
#define TIM1_PERIOD_FOR_SAMPLING_FREQ_64HZ    (32L)
#define TIM1_PERIOD_FOR_SAMPLING_FREQ_32HZ    (64L)

/* Includes ------------------------------------------------------------------*/
TIM_HandleTypeDef htim1;

/* TIM1 init function */
void MX_TIM1_Init(uint32_t sampling_period_s)
{
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = ((uint32_t)(SystemCoreClock) / TIM1_PRINCIPAL_CLOCK_FREQ) - 1;
  if (sampling_period_s == 1)
  {
    /* 2^1 = 2*/
    htim1.Init.Period = TIM1_PERIOD_FOR_SAMPLING_FREQ_512HZ - 1;
  }
  else if (sampling_period_s == 2)
  {
    /* 2^2 = 4*/
    htim1.Init.Period = TIM1_PERIOD_FOR_SAMPLING_FREQ_256HZ - 1;
  }
  else if (sampling_period_s == 3)
  {
    /* 2^3 = 8*/
    htim1.Init.Period = TIM1_PERIOD_FOR_SAMPLING_FREQ_128HZ - 1;
  }
  else if (sampling_period_s == 4)
  {
    /* 2^4 = 16*/
    htim1.Init.Period = TIM1_PERIOD_FOR_SAMPLING_FREQ_64HZ - 1;
  }
  else if (sampling_period_s == 5)
  {
    /* 2^4 = 16*/
    htim1.Init.Period = TIM1_PERIOD_FOR_SAMPLING_FREQ_32HZ - 1;
  }
  else
  {
    /* In all other cases use the shortest period: 2^0 = 1*/
    htim1.Init.Period = TIM1_PERIOD_FOR_SAMPLING_FREQ_1024HZ - 1;
  }
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if (HAL_TIM_Base_Start_IT(&htim1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (scale_amp_changed)
  {
    scale_amp_changed = 0;
    idx_datum = 0;
    vbat = 0;
    temp = 0;
    for (uint16_t i=0; i<LEN_F1; i++)
    {
      adata[i] = 0;
      adata[LEN_F1 + i] = 0;
      adata[2*LEN_F1 + i] = 0;
    }
  }

  /**
    * Initiate ADC/DMA
    */
  if (HAL_ADC_Start_DMA(&hadc1, data_buffer, 2 + NSAMPLING_CHANNELS) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }

  BSP_LED_On(LED3);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
    /* USER CODE BEGIN TIM1_MspInit 0 */
    /* USER CODE END TIM1_MspInit 0 */

    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* USER CODE BEGIN TIM1_MspInit 1 */

    /* TIM1 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

    /* USER CODE END TIM1_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
    /* USER CODE BEGIN TIM1_MspDeInit 0 */

    /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /* USER CODE BEGIN TIM1_MspDeInit 1 */

    /* TIM1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);

    /* USER CODE BEGIN TIM1_MspInit 1 */

    /* USER CODE END TIM1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
