//------------------------------------------------------------------
// Title     : Button Interrupt with UART Message and LED Toggle
// Author    : Umar Wahid
// Target    : STM32L476 Series Microcontroller
// Purpose   : This program configures GPIOC Pin 13 (user button) to
//             trigger an external interrupt on falling edge. When
//             the button is pressed, it toggles an LED on GPIOA Pin 5
//             and prints a message via UART2.
// Version   : 1.18.1 (STM32CubeIDE)
// Date      : June 21, 2025
// Notes     :
//             - External interrupt configured on PC13 (user button).
//             - LED connected to PA5 toggles on interrupt.
//             - UART2 configured on PA2 for printf() output.
//
// Connection Pins:
//   - User Button: PC13  (Active-Low, falling-edge interrupt)
//   - LED        : PA5   (On-board LED toggles on button press)
//   - UART2_TX   : PA2   (USART2 transmit for serial debugging)
//   - GND        : Connect common ground for UART communication
//------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"
#include "uart.h"
#include "systick.h"
#include "exti.h"

#define PIN_5               (1U << 5)
#define LED_PIN             PIN_5
#define GPIOAEN             (1U << 0) // enable GPIOA clock


int main(void)
{
    //Enable clock access to GPIOA
    RCC->AHB2ENR |= GPIOAEN;

    //Set PA5 as output pin (MODER5 = 01)
    GPIOA->MODER |=  (1U << 10); // set bit 10
    GPIOA->MODER &= ~(1U << 11); // clear bit 11



    pc13_exti_init();
    uart2_tx_init();


    while (1)
    {

    }
}

// this handles interrupt from line 15 to 10/search EXTI_PR1 in reference manual
void EXTI15_10_IRQHandler(void) //take no argument and returns void //if we open file in startup and
								// go to line 185 we will see EXTI15_10_IRQHandler
{

	if ((EXTI->PR1 & PIN13) !=0) //if not equal to 0
	{
		//as soon as the interrupt comes we need to clear it(Clear PR flags)
		EXTI->PR1 |= PIN13;

		//do something
		GPIOA->ODR ^= LED_PIN;
		printf("Umar pressed the BUTTON...\n\r");
	}
}

//When the falling edge occurs, EXTI_PR1 bit 13 (13) becomes 1.
//Inside your ISR (interrupt service routine), after handling the event, you clear the interrupt:
// clear a pending interrupt by writing a 1 to the corresponding bit.
//Never write 0; it does nothing.
