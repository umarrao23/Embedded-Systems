//------------------------------------------------------------------
// Title     : UART2 Transmission and LED Control - STM32L4
// Author    : Umar Wahid
// Target    : STM32L476 Series Microcontroller
// Purpose   : This program initializes UART2 on PA2 (TX) and PA3 (RX)
//             using polling. It reads characters sent to the MCU and
//             controls an LED connected to PA5 based on the input.
//             If the received character is '1', the LED turns ON.
//             Otherwise, the LED turns OFF.
//
//             Live Expressions in STM32CubeIDE can be used to
//             monitor variables like 'key'.
//
// Version   : 1.18.1 (STM32CubeIDE)
// Date      : June 21, 2025
// Notes     : Ensure a proper USB-UART bridge is connected to PA2 (TX)
//             and PA3 (RX). Have Used terminal emulator Realterm
//             set at 115200 baud.
//------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"
#include "uart.h"

//for led
#define GPIOAEN		(1U<<0)
#define GPIOA_5		(1U<<5)
#define LED_PIN		GPIOA_5



char key;


int main(void)
{
	//ENable clock access to LED
	RCC->AHB2ENR |= GPIOAEN;

	//SET PA5 OUTPUT PIN
	GPIOA->MODER |=(1U<<10);
	GPIOA->MODER &=~(1U<<11);


	uart2_rxtx_init();

    while (1)
    {
    	key = uart2_read(USART2);
    	if (key =='1')
    	{
    		GPIOA->ODR |= LED_PIN; //output data register//led on
    	}
    	else{
    		GPIOA->ODR &= ~(LED_PIN);
    	}
}
}




