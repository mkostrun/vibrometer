/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

#undef SINE_WINDOW
#undef BARTLETT_WINDOW

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_nucleo_32.h"
#include "gpio.h"
#include "usart.h"
#include "tim1.h"
#include "adc.h"
#include "fft.h"
#include "dma.h"

#define FIRMWARE "lgwatcher-v_1_0"

#define BAUDRATE 115200
// #define BAUDRATE 230400

/* USER CODE BEGIN Includes */
extern volatile uint8_t dl_state;
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

__IO uint32_t
    do_fft_now=0, idx_datum=0, vbat, temp, vbat_avg, temp_avg;

__IO uint16_t
    adata[NSAMPLING_CHANNELS * LEN_F1];

uint32_t
    data_buffer[2 + NSAMPLING_CHANNELS], log2_sampling_period_s=0, scale_amplitude=0;

float
    fft_coef[LEN_F1], channel_avg;

char
    serial_buffer[16];

__IO uint8_t
    scale_amp_changed=0;

extern int16_t avg_counter;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t printme=1, print_labels=0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init(log2_sampling_period_s);
  Serial2.begin(BAUDRATE);


  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* USER CODE END 2 */
  if (!printme)
  {
    Serial2.prints("Firmware: " FIRMWARE);
    sprintf(serial_buffer,"%lu\n" ,(uint32_t)(SystemCoreClock));
    Serial2.prints(serial_buffer);
    Serial2.prints("\n");
  }
  else
    serial_set_status(1);

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (printme)
    {
      if (do_fft_now)
      {
#ifdef DEBUG
        serial_print_u16_array(adata, LEN_F1>>1);
#endif

        // copy, find average, offset/renormalize and window the time-series data in the buffer
        channel_avg = window((uint16_t *) &adata[(do_fft_now-1) * LEN_F1], idx_datum, fft_coef, LEN_F1);

#ifdef DEBUG
        serial_print_float_array(fft_coef, LEN_F1);
#endif

        // print channel
        if (print_labels)
        {
          Serial2.prints("CH=");
        }
        sprintf(serial_buffer,"%u,", do_fft_now);
        Serial2.prints(serial_buffer);

        if (do_fft_now == 1)
        {
          // print temperature
          if (print_labels)
          {
            Serial2.prints("TEMP=");
          }
          sprintf(serial_buffer,"%u,", (uint16_t) temp_avg);
          Serial2.prints(serial_buffer);

          // print vbat
          if (print_labels)
          {
            Serial2.prints("VBAT=");
          }
          sprintf(serial_buffer,"%u,", (uint16_t) vbat_avg);
          Serial2.prints(serial_buffer);

          // print log2 of sampling time
          if (print_labels)
          {
            Serial2.prints("log2(TIME)=");
          }
          sprintf(serial_buffer,"%u,", log2_sampling_period_s);
          Serial2.prints(serial_buffer);

          // print -log10 of max amplitude used for scaling:
          if (print_labels)
          {
            Serial2.prints("-log10(SCALE)=");
          }
          sprintf(serial_buffer,"%u,", scale_amplitude);
          Serial2.prints(serial_buffer);
        }

        // print average
        if (print_labels)
        {
          Serial2.prints("AVG=");
        }
        sprintf(serial_buffer,"%u,", (uint16_t) (10.0f * channel_avg));
        Serial2.prints(serial_buffer);

        // if nothing to report just print X on the serial bus
        if (channel_avg == 0.0f)
        {
          do_fft_now = 0;
          Serial2.prints("X\n");
          continue;
        }

        // do the FFT
        realft((float *) &fft_coef[-1], LEN_F1, 1);
        uint32_t last_idx = convert_fft_to_modamps(fft_coef, LEN_F1, scale_amplitude);

#ifdef DEBUG
        if (print_labels)
        {
          Serial2.prints("LAST=");
        }
        sprintf(serial_buffer,"%u,", (uint16_t) last_idx);
        Serial2.prints(serial_buffer);
#endif

        if (print_labels)
        {
          Serial2.prints("FFT=");
        }
        serial_print_modamps(fft_coef, LEN_F1, last_idx);

        do_fft_now = 0;

      } /* if (do_fft_now) */

    } /* if (printme) */

    /* talk to me! */
    if (Serial2.available())
    {
      char c = Serial2.read();
      if (printme)
      {
        switch (c)
        {
          case 'q':
            printme=0;
            break;

          case 's':
            print_labels=0;
            break;
        }
        continue;
      }

      switch (c)
      {
        case '?':
          Serial2.prints(FIRMWARE "\n");
          break;

        case 't':
          break;

        case 'o':
          /* raw readings compensated by electric and thermal offsets */
          break;

        case 'r':
          /* raw readings */
          break;

        case 'x':
          if (print_labels)
            Serial2.prints("adc=");
//           Serial2.print_u16("%d,", h.adc(0));
          if (print_labels)
              Serial2.prints("bias=");
//           Serial2.print_u16("%d,", h.bias(0));
          if (print_labels)
              Serial2.prints("clk=");
//           Serial2.print_u16("%d,", h.clock(0));
          if (print_labels)
              Serial2.prints("bpa=");
//           Serial2.print_u16("%d,", h.bias_pa(0));
          if (print_labels)
              Serial2.prints("pu=");
//           Serial2.print_u16("%d,", h.pull_up(0));
          if (print_labels)
              Serial2.prints("tn=");
//           Serial2.print_u16("%d,", *h.tn);
          if (print_labels)
              Serial2.prints("PixCmin=");
          // pc.printf("%f,", h.PixCmin);
          if (print_labels)
              Serial2.prints("PixCmax=");
          // pc.printf("%f,", h.PixCmax);
          if (print_labels)
              Serial2.prints("epsilon=");
          // pc.printf("%d,", h.epsilon);
          if (print_labels)
              Serial2.prints("gradScale=");
          // pc.printf("%d,", h.gradScale);
          if (print_labels)
              Serial2.prints("PTATgrad=");
          // pc.printf("%f,", h.PTATgrad);
          if (print_labels)
            Serial2.prints("PTAToffs=");
          // pc.printf("%f\n", h.PTAToffs);
          break;

          /**
            * log2 of Sampling period: 1,2,4,8,16,32,64 seconds:
            */
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
          if (log2_sampling_period_s != (c - '0'))
          {
            log2_sampling_period_s = (c - '0');
            MX_TIM1_Init(log2_sampling_period_s);
          }
          break;

          /**
            * scale of the modulation amplitudes:
            *       u(nity) -> 0:1
            *       d(eci)  -> 0:(0.1)
            *       c(enti) -> 0:(0.01)
            *       m(ili)  -> 0:(0.001)
            */
        case 'u':
          if (scale_amplitude != 0)
          {
            scale_amplitude = 0;
            scale_amp_changed = 1;
          }
          break;

        case 'd':
          if (scale_amplitude != 1)
          {
            scale_amplitude = 1;
            scale_amp_changed = 1;
          }
          break;

        case 'c':
          if (scale_amplitude != 2)
          {
            scale_amplitude = 2;
            scale_amp_changed = 1;
          }
          break;

        case 'm':
          if (scale_amplitude != 3)
          {
            scale_amplitude = 3;
            scale_amp_changed = 1;
          }
          break;

          /**
            * labels Chris' style
            */
        case 'l':
          print_labels=1;
          break;

        case 's':
          print_labels=0;
          break;

          /**
            * print anything? if not, just collect data
            */
        case 'p':
          printme=1;
          break;

      } // switch

    }

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
/**
 * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
 *            System Clock source            = PLL (MSI)
 *            SYSCLK(Hz)                     = 80000000
 *            HCLK(Hz)                       = 80000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 1
 *            APB2 Prescaler                 = 1
 *            MSI Frequency(Hz)              = 4000000
 *            PLL_M                          = 1
 *            PLL_N                          = 40
 *            PLL_R                          = 2
 *            PLL_P                          = 7
 *            PLL_Q                          = 4
 *            Flash Latency(WS)              = 4
 * @param  None
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
  clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
 * @brief  Conversion complete callback in non blocking mode
 * @param  AdcHandle : AdcHandle handle
 * @note   This example shows a simple way to report end of conversion, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  /** Transfer DMA data
   */
  vbat += data_buffer[0];
  temp += data_buffer[1];
  for (uint8_t i=0; i<NSAMPLING_CHANNELS; i++)
  {
    adata[LEN_F1 * i + idx_datum] = data_buffer[2 + i];
  }

  /** Stop ADC/DMA conversion
    */
  if (HAL_ADC_Stop_DMA(&hadc1) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }

  idx_datum = (idx_datum + 1) % LEN_F1;
  if ( !(idx_datum%LEN_F1) )
  {
    // calculate VBAT in mV
    vbat_avg = __HAL_ADC_CALC_DATA_TO_VOLTAGE(3300L, (vbat/LEN_F1), ADC_RESOLUTION_12B);
    vbat = 0;

    // calculate average temp
    temp_avg = __HAL_ADC_CALC_TEMPERATURE(3300L, (temp/LEN_F1), ADC_RESOLUTION_12B);
    temp = 0;
  }

  if ( !(idx_datum%(LEN_F1/3)) )
  {
    // tell the world: 1:3
    do_fft_now = 3 * idx_datum / LEN_F1 + 1;
  }

  BSP_LED_Off(LED3);
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */
void serial_print_modamps(float * f, uint32_t flen, uint32_t last_idx)
{
  uint16_t i, coef_fft_1;

  for(i=1; i<last_idx; i++)
  {
    coef_fft_1 = (uint16_t) f[i<<1];

    if (coef_fft_1)
    {
      sprintf(serial_buffer,"%02x", coef_fft_1);
    }
    else
    {
      sprintf(serial_buffer,"_");
    }
    Serial2.prints(serial_buffer);
  }
  Serial2.prints("X\n");
}

/**
  * Debuging functions for printout of different arrays
  *
  */
void serial_print_u16_array(__IO uint16_t * x, uint32_t len)
{
  uint16_t i;
  uint32_t avgx=0, minx=-1, maxx=0;
  uint8_t *x8 = (uint8_t *)x;
  for (i=0; i<len; i++)
  {
    avgx += x[i];
    if (x[i] > maxx)
    {
      maxx = x[i];
      continue;
    }
    if (x[i] < minx)
    {
      minx = x[i];
    }
  }
  avgx *= 10;
  avgx /= len;
  sprintf(serial_buffer,"%lu,", avgx);
  Serial2.prints(serial_buffer);
  sprintf(serial_buffer,"%lu,", minx);
  Serial2.prints(serial_buffer);
  sprintf(serial_buffer,"%lu,", maxx);
  Serial2.prints(serial_buffer);
  for (i=1; i<(len<<1); i+=2)
  {
    sprintf(serial_buffer,"%02x", x8[i]);
    Serial2.prints(serial_buffer);
  }
  Serial2.prints("\n");
}

void serial_print_float_array(float * x, uint32_t len)
{
  uint16_t i;
  for (i=1; i<(len<<1); i+=2)
  {
    int32_t val = 128*x[i] + 128;
    if (val > 255)
      val = 255;
    if (val > 0)
      sprintf(serial_buffer,"%02lx", (uint32_t) val);
    else
      sprintf(serial_buffer,"_");
    Serial2.prints(serial_buffer);
  }
  Serial2.prints("\n");
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
