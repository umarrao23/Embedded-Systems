//------------------------------------------------------------------
// Title     :
// Author    : Umar Wahid
// Target    : STM32L476 Series Microcontroller
// Purpose   :
// Version   : 1.18.1 (STM32CubeIDE)
// Date      : June 21, 2025
// Notes     :
//
//
// Connection Pins:

//------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"
#include "uart.h"
#include "systick.h"
#include "adc.h"
#include "exti.h"

#define PIN_5               (1U << 5)
#define LED_PIN             PIN_5
#define GPIOAEN             (1U << 0) // enable GPIOA clock

static void systick_callback(void);

int main(void)
{
    //Enable clock access to GPIOA
    RCC->AHB2ENR |= GPIOAEN;

    //Set PA5 as output pin (MODER5 = 01)
    GPIOA->MODER |=  (1U << 10); // set bit 10
    GPIOA->MODER &= ~(1U << 11); // clear bit 11



    uart2_tx_init();
    systick_1hz_interrupt();


    while (1)
    {

    }
}

static void systick_callback(void)
{
	printf("A second passed from Systick \n\r");
}

// this handles interrupt from line 15 to 10/search EXTI_PR1 in reference manual
void SysTick_Handler(void)
{
	//do something...
	systick_callback();
}

//When the falling edge occurs, EXTI_PR1 bit 13 (13) becomes 1.
//Inside your ISR (interrupt service routine), after handling the event, you clear the interrupt:
// clear a pending interrupt by writing a 1 to the corresponding bit.
//Never write 0; it does nothing.
