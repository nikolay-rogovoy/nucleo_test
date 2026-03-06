#include "stm32f4xx_hal.h"

// Прототипы
void SystemClock_Config(void);
void Error_Handler(void);

int main(void)
{
    // 1. Инициализация HAL
    HAL_Init();
    
    // 2. Настройка тактирования (системный генератор)
    SystemClock_Config();
    
    // 3. Настройка GPIO через HAL
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Включаем тактирование порта A
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // Настраиваем PA5 (LED) как выход
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    while (1)
    {
        // Включаем LED
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        HAL_Delay(500);  // Ждём 500 мс
        
        // Выключаем LED
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_Delay(500);
    }
}

// Обязательная функция инициализации тактов
void SystemClock_Config(void)
{
    // Для начала можно оставить пустой или настроить PLL
    // По умолчанию МК работает на внутреннем HSI (16 МГц)
}

// Обработчик ошибок
void Error_Handler(void)
{
    while(1);
}

// Колбэк SysTick (нужен для HAL_Delay)
void SysTick_Handler(void)
{
    HAL_IncTick();
}


// #include "stm32f4xx_hal.h"
// #include "stm32f4xx.h"
// #include <stdint.h>

// // Символы из Linker Script
// extern uint32_t _estack;
// extern uint32_t _sdata, _edata, _etext, _sbss, _ebss;

// // Прототипы
// void Reset_Handler(void);
// int main(void);
// void delay(volatile uint32_t count);

// // Минимальная таблица векторов
// __attribute__((section(".isr_vector")))
// uint32_t *vector_table[] = {
//     (uint32_t *)&_estack,      // Вершина стека
//     (uint32_t *)Reset_Handler, // Вектор сброса
// };

// void Reset_Handler(void)
// {
//     // 1. Копируем инициализированные данные (.data) из FLASH в RAM
//     uint32_t *src = &_etext;
//     uint32_t *dst = &_sdata;
//     while (dst < &_edata)
//     {
//         *dst++ = *src++;
//     }

//     // 2. Обнуляем неинициализированные данные (.bss)
//     dst = &_sbss;
//     while (dst < &_ebss)
//     {
//         *dst++ = 0;
//     }

//     // 3. Прыгаем в main
//     main();

//     while (1)
//         ; // Защита от выхода из main
// }

// void delay(volatile uint32_t count)
// {
//     while (count--)
//     {
//         __NOP();
//     }
// }

// static int iii;
// static int iiy;

// int main(void)
// {
//     // 1. Включаем тактирование порта GPIOA
//     RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

//     // 2. Сбрасываем и настраиваем PA5 как выход (01 в MODER)
//     GPIOA->MODER &= ~(3U << (5 * 2));
//     GPIOA->MODER |= (1U << (5 * 2));

//     GPIOA->OTYPER &= ~(1U << 5);        // Push-Pull (явно)
//     GPIOA->OSPEEDR &= ~(3U << (5 * 2)); // Низкая скорость (по умолчанию)
//     GPIOA->PUPDR &= ~(3U << (5 * 2));   // Без подтяжки (по умолчанию)

//     while (1)
//     {
//         iii++;
//         iiy++;
//         // Переключаем состояние светодиода (инверсия 5-го бита ODR)
//         GPIOA->ODR ^= (1U << 5);

//         delay(1500000 + iii + iiy);
//     }
// }
