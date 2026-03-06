#include "uart.h"
#include <string.h>
#include <stdio.h>

UART_HandleTypeDef huart2;

void UART_Init(void)
{
    // 1. Включаем тактирование GPIOA и USART2
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    
    // 2. Настраиваем PA2 (TX) и PA3 (RX)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;  // Альтернативная функция 7
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 3. Настраиваем USART2 (115200 8N1)
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}

void UART_SendChar(char c)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)&c, 1, 10);
}

void UART_SendString(const char *str)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 100);
}

void UART_SendInt(int num)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", num);
    UART_SendString(buf);
}