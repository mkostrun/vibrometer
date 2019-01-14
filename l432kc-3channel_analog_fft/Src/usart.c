/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"

#define SERIAL_WAITTIME_MS  (20)


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart2;
volatile ITStatus UART_TX_Completed=RESET, UART_RX_Available=RESET;

/* USART2 init function */

void UART2_Init(int32_t baud)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = baud;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  __HAL_LOCK(&huart2); /* Process Locked */
  huart2.rx_last = huart2.rx_first = huart2.rx_available = 0;
  huart2.ErrorCode = HAL_UART_ERROR_NONE;
  huart2.RxState = HAL_UART_STATE_BUSY_RX;
  huart2.RxISR = NULL;

  /* Computation of UART mask to apply to RDR register */
  UART_MASK_COMPUTATION(&huart2);

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  SET_BIT(huart2.Instance->CR3, USART_CR3_EIE);

  __HAL_UNLOCK(&huart2);  /* Process Unlocked */

  /* Enable the UART Parity Error interrupt and Data Register Not Empty interrupt */
  SET_BIT(huart2.Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);

  /* USART2 interrupt Init */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);

  HAL_Delay(50);
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */

    /* Enable GPIO TX/RX clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    /** USART2 GPIO Configuration
      * PA2           ------> USART2_TX
      * PA15 (JTDI)   ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA15 (JTDI)     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, VCP_TX_Pin|VCP_RX_Pin);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_Cir_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */
uint8_t UART2_Transmit(uint8_t * msg, uint16_t lenmsg)
{
  UART_TX_Completed = RESET;
  return HAL_UART_Transmit_IT(&huart2, msg, lenmsg);
}

/* USER CODE BEGIN 1 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: trasfer complete*/
  UART_TX_Completed = SET;
}

void print_serial(char * msg)
{
  UART_TX_Completed = RESET;
  HAL_UART_Transmit_IT(&huart2, (uint8_t*) msg, strlen(msg));
  while (UART_TX_Completed==RESET);
}

void serial_set_status(uint8_t status)
{
  if (status)
    UART_TX_Completed = SET;
  else
    UART_TX_Completed = RESET;

  return;
}

void print_serial_immediate(char * msg)
{
  UART2_Transmit((uint8_t*) msg, strlen(msg));
}

void print_serial_waitfirst(char * msg)
{
  while (UART_TX_Completed == RESET);
  UART2_Transmit((uint8_t*) msg, strlen(msg));
}

void print_int_serial(int32_t n)
{
  char msg[16];
  sprintf(msg, "%ld", n);
  UART2_Transmit((uint8_t *)msg, strlen(msg));
  while (UART_TX_Completed == RESET);
}

void print_u16_serial(char *fmt, uint16_t n)
{
  char msg[16];
  sprintf(msg, fmt, n);
  UART2_Transmit((uint8_t *)msg, strlen(msg));
  while (UART_TX_Completed == RESET);
}

uint8_t serial_input_available(void)
{
  uint8_t n = HAL_UART_Cir_Available_IT(&huart2);

  if (!n)
    return 0;

  if (HAL_UART_Cir_Last_Time_IT(&huart2)<SERIAL_WAITTIME_MS)
    return 0;

  return n;
}

uint8_t serial_read (void)
{
  if (!serial_input_available())
    return 0;

  return HAL_UART_Cir_RetrieveReceivedChar_IT(&huart2);
}

uint8_t serial_input (char * buffer, uint8_t len)
{
  uint8_t n = serial_input_available();
  if (!n)
    return 0;

  uint16_t i = 0;
  while (HAL_UART_Cir_Available_IT(&huart2))
  {
    buffer[i] = HAL_UART_Cir_RetrieveReceivedChar_IT(&huart2);
    i++;

    if (i==len)
      break;
  }

  while (buffer[i-1]=='\n')
    i--;
  while (buffer[i-1]=='\r')
    i--;
  buffer[i] = '\0';

  return i;
}

serialArduino_t Serial2 =
{
  .begin = &UART2_Init,
  .prints = &print_serial,
  .printi = &print_int_serial,
  .print_u16 = &print_u16_serial,
  .available = &serial_input_available,
  .read = &serial_read,
};
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
