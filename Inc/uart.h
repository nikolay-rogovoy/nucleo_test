#ifndef __UART_H
#define __UART_H

#include "stm32f4xx_hal.h"

void UART_Init(void);
void UART_SendChar(char c);
void UART_SendString(const char *str);
void UART_SendInt(int num);

#endif