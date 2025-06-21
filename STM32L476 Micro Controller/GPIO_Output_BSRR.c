//------------------------------------------------------------------
// Title     : LED Blinking with STM32L4 using GPIOA Pin 5 (PA5)
// Author    : Umar Wahid
// Target    : STM32L4 Series Microcontroller
// Purpose   : This program toggles an LED connected to PA5 using
//              register-level programming. It uses the BSRR register
//              to set and reset the pin state.
// Version   : 1.18.1 STM32CubeIDE
//------------------------------------------------------------------

#include "stm32l4xx.h"   // Include device-specific header for STM32L4 series

#define PIN_5               (1U << 5)     // Define bit 5 (PA5)
#define LED_PIN             PIN_5        // Alias for PA5
#define GPIOAEN             (1U << 0)     // Bitmask to enable GPIOA clock

int main(void)
{
    // 1) Enable clock access to GPIOA
    RCC->AHB2ENR |= GPIOAEN;
    // This enables the clock for GPIO port A.
    // Without enabling the clock, GPIOA peripheral registers won't work.

    // 2) Set PA5 as output pin (MODER5 = 01)
    GPIOA->MODER |=  (1U << 10); // Set MODER bit 10 (part of PA5 mode configuration)
    GPIOA->MODER &= ~(1U << 11); // Clear MODER bit 11 to complete setting PA5 as output
    // Each GPIO pin is configured using 2 bits in MODER.
    // For PA5, bits 11:10 control the mode.
    // 00 = Input, 01 = Output, 10 = Alternate Function, 11 = Analog

    while (1)
    {
        // 3) Toggle PA5

        GPIOA->BSRR |= (1U << 5);  // Set PA5 (turn ON LED)
        // BSRR bits 0–15 are used to set GPIO pins.
        // Writing 1 to bit 5 sets PA5 high.

        for (int i = 0; i < 100000; i++) {} // Software delay

        GPIOA->BSRR |= (1U << 21); // Reset PA5 (turn OFF LED)
        // BSRR bits 16–31 are used to reset GPIO pins.
        // Writing 1 to bit (pin# + 16) clears that pin (PA5 = bit 21)

        for(int i = 0; i < 100000; i++) {} // Software delay
    }
}
