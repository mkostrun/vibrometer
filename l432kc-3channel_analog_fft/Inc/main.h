/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/* USER CODE BEGIN Includes */
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_conf.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/**
  * ADC sampling time: the longer the better
  */
// #define ADC_SAMPLETIME_MAIN_H        ADC_SAMPLETIME_6CYCLES_5
// #define ADC_SAMPLETIME_MAIN_H        ADC_SAMPLETIME_24CYCLES_5
// #define ADC_SAMPLETIME_MAIN_H        ADC_SAMPLETIME_47CYCLES_5
// #define ADC_SAMPLETIME_MAIN_H        ADC_SAMPLETIME_92CYCLES_5
// #define ADC_SAMPLETIME_MAIN_H        ADC_SAMPLETIME_247CYCLES_5
#define ADC_SAMPLETIME_MAIN_H         ADC_SAMPLETIME_640CYCLES_5

/** Math: FFT
 */
#define SAMPLING_FREQ         (256L)
#define FFT_N_PRINT_TERMS     (401L)
#define FFTSLOW_N_PRINT_TERMS (96L)
#define LEN_F1                (1024L)
#define LEN_F1_HALF           (LEN_F1>>1)
#define SINE_WINDOW
#define NSAMPLING_CHANNELS     (3L)

#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_3
#define LD3_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the
  *        HAL drivers code
  */
 #define USE_FULL_ASSERT    1U

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

extern __IO uint32_t
    do_fft_now, idx_datum, vbat, temp, vbat_avg, temp_avg;

extern __IO uint16_t
    adata[NSAMPLING_CHANNELS * LEN_F1];

extern uint32_t
    data_buffer[2 + NSAMPLING_CHANNELS], log2_sampling_period_s, scale_amplitude;

extern float
    fft_coef[LEN_F1], channel_avg;

extern char
    serial_buffer[16];

extern __IO uint8_t
    scale_amp_changed;


void serial_print_modamps(float * f, uint32_t flen, uint32_t last_idx);
void serial_print_u16_array(__IO uint16_t * x, uint32_t len);
void serial_print_float_array(float * x, uint32_t len);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
