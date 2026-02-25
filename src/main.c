#include "stm32f4xx.h"
#include <stdint.h>

// Символы из Linker Script
extern uint32_t _estack;
extern uint32_t _sdata, _edata, _etext, _sbss, _ebss;

// Прототипы
void Reset_Handler(void);
int main(void);
void delay(volatile uint32_t count);

// Минимальная таблица векторов
__attribute__((section(".isr_vector")))
uint32_t *vector_table[] = {
    (uint32_t *)&_estack,      // Вершина стека
    (uint32_t *)Reset_Handler, // Вектор сброса
};

void Reset_Handler(void) {
    // 1. Копируем инициализированные данные (.data) из FLASH в RAM
    uint32_t *src = &_etext;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // 2. Обнуляем неинициализированные данные (.bss)
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    // 3. Прыгаем в main
    main();

    while(1); // Защита от выхода из main
}

void delay(volatile uint32_t count) {
    while(count--) {
        __NOP();
    }
}

int main(void) {
    // 1. Включаем тактирование порта GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Сбрасываем и настраиваем PA5 как выход (01 в MODER)
    GPIOA->MODER &= ~(3U << (5 * 2)); 
    GPIOA->MODER |=  (1U << (5 * 2));

    GPIOA->OTYPER &= ~(1U << 5);      // Push-Pull (явно)
    GPIOA->OSPEEDR &= ~(3U << (5 * 2)); // Низкая скорость (по умолчанию)
    GPIOA->PUPDR &= ~(3U << (5 * 2)); // Без подтяжки (по умолчанию)

    while(1) {
        // Переключаем состояние светодиода (инверсия 5-го бита ODR)
        GPIOA->ODR ^= (1U << 5);
        delay(1500000);
    }
}
