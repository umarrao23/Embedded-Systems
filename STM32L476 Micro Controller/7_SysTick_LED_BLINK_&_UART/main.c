//------------------------------------------------------------------
// Title     : SysTick-Based LED Blinker with UART Output
// Author    : Umar Wahid
// Target    : STM32L476 Series Microcontroller
// Purpose   : Demonstrates the use of the SysTick timer to generate 
//             precise millisecond delays. A 1-second (1000 ms) delay 
//             is used to toggle an LED on PA5 and send a message 
//             via UART2 every second.
// Version   : 1.18.1 (STM32CubeIDE)
// Date      : June 21, 2025
// Notes     : - PA5 is configured as output to drive the onboard LED.
//             - SysTick generates delays in milliseconds.
//             - uart2_tx_init() sets up UART2 for transmitting text.
//             - systickDelayMs(1000) creates a 1-second delay.
//------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"

#define PIN_5               (1U << 5)
#define LED_PIN             PIN_5
#define GPIOAEN             (1U << 0) // enable GPIOA clock


int main(void)
{
    // 1) Enable clock access to GPIOA
    RCC->AHB2ENR |= GPIOAEN;

    // 2) Set PA5 as output pin (MODER5 = 01)
    GPIOA->MODER |=  (1U << 10); // set bit 10
    GPIOA->MODER &= ~(1U << 11); // clear bit 11

	uart2_tx_init();


    while (1)
    {
    	systickDelayMs(1000);
    	printf("A second just passed!\n\r");
    	GPIOA->ODR^=LED_PIN;
    }
}




