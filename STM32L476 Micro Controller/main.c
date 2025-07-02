//-------------------------------------------------------------------------------------------
// Title     : TIM1 Interrupt-Based Blinking (1Hz) with UART Debug
// Author    : Umar Wahid
// Target    : STM32L476 Series Microcontroller
// Purpose   : Configure TIM1 to generate 1Hz periodic interrupts for toggling PA5 (LED).
//             UART is initialized for debug print every second.
// Version   : 1.18.1 (STM32CubeIDE)
// Date      : June 21, 2025
// Notes     : - PA5 is configured as a GPIO output for LED toggling.
//             - TIM1 is set up with a prescaler and auto-reload value to generate 1Hz interrupts.
//             - On every interrupt, an LED is toggled and a message is printed via UART.
//             - No use of HAL libraries; direct register-level programming.
//-------------------------------------------------------------------------------------------


#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"
#include "uart.h"
//#include "systick.h"
//#include "adc.h"
//#include "exti.h"
#include "tim.h"

#define PIN_5               (1U << 5)
#define LED_PIN             PIN_5
#define GPIOAEN             (1U << 0) // enable GPIOA clock

static void tim1_callback(void);


int main(void)
{
    //Enable clock access to GPIOA
    RCC->AHB2ENR |= GPIOAEN;

    //Set PA5 as output pin (MODER5 = 01)
    GPIOA->MODER |=  (1U << 10); // set bit 10
    GPIOA->MODER &= ~(1U << 11); // clear bit 11



    uart2_tx_init();
    tim1_1hz_interrupt_init();

    while (1)
    {

    }
}

static void tim1_callback(void)
{
	printf("A second passed from TIM1 \n\r");
}


void TIM1_UP_TIM16_IRQHandler(void)// THIS FUNCTION NAME IS DEFINE IN STATUP THIS NAME IS DEFINED THERE
{
	//clear update flag
	TIM1->SR &=~SR_UIF;
	//Whenever the interrupt occurs we need to clear the flag as well

	//do something...
	tim1_callback();

	GPIOA->ODR ^= LED_PIN;

}


