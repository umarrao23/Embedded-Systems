//------------------------------------------------------------------
// Title     : LED Control with Push Button
// Author    : Umar Wahid
// Target    : STM32L4 Series Microcontroller
// Purpose   : Turns ON PA5 LED when button (PC13) is not pressed
//             and turns OFF when pressed
// Version   : 1.18.1 STM32CubeIDE
//------------------------------------------------------------------

#include "stm32l4xx.h"   // Include device-specific header for STM32L4 series

#define PIN_5               (1U << 5)     // Define bit 5 (PA5)
#define LED_PIN             PIN_5        // Alias for PA5

#define PIN_13				(1U<<13)
#define BTN_PIN				PIN_13

#define GPIOAEN             (1U<<0)     // Bitmask to enable GPIOA clock
#define GPIOCEN				(1U<<2)

int main(void)
{
    RCC->AHB2ENR |= GPIOAEN;

    RCC->AHB2ENR |= GPIOCEN;

    GPIOA->MODER |=  (1U << 10); // Set MODER bit 10 (part of PA5 mode configuration)
    GPIOA->MODER &= ~(1U << 11); // Clear MODER bit 11 to complete setting PA5 as output

    GPIOC->MODER &= ~(1U << 26); // Clear MODER bit 26
    GPIOC->MODER &= ~(1U << 27); // Clear MODER bit 27

    while (1)
    {

    	if(GPIOC->IDR & BTN_PIN){


        GPIOA->BSRR |= (1U << 5);

    	}
    	else
    	{

        GPIOA->BSRR |= (1U << 21);
    	}
    }
}
