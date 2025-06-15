//------------------------------------------------------------------------------
// File     : main.c
// Author   : Umar Wahid
// Target   : STM32 (Register-level control - STM32F4L476)
// Purpose  : Blink or toggle an LED on GPIO pin PA5 using register-level access
// Version  : 1.18.1 STM32CubeIDE
//------------------------------------------------------------------------------
/*
Description:
------------
This program demonstrates how to toggle an LED connected to PA5 on an STM32
microcontroller using direct register manipulation.

Steps:
1. Enables clock access to GPIOA by setting the corresponding bit in RCC_AHB2ENR.
2. Configures GPIOA pin 5 as a general purpose output (MODER5 = 01).
3. Toggles PA5 ON and OFF inside an infinite loop using bitwise XOR.
4. Simple software delay using an empty loop.

Memory Map Notes:
-----------------
- RCC_AHB2ENR base address: 0x4002104C (derived from AHB1 + RCC offset + reg offset)
- GPIOA base address:       0x48000000 (on AHB2)
- GPIOA MODER offset:       0x00
- GPIOA ODR offset:         0x14

Macros:
-------
- RCC_AHB2EN_R     : RCC register for AHB2 peripheral clock enable
- GPIOA_MODE_R     : GPIOA mode register (MODER)
- GPIOA_OD_R       : GPIOA output data register (ODR)
- LED_PIN (PIN_5)  : Bit mask for PA5

Usage:
------
- LED connected to GPIOA pin 5 (on-board LED for Nucleo boards)
*/


#define PERIPH_BASE 		(0X40000000UL)

#define AHB2_OFFSET 		(0x08000000UL)
#define AHB2_PERIPH_BASE 	(PERIPH_BASE + AHB2_OFFSET)

#define AHB1_OFFSET 		(0x00020000UL)
#define AHB1_PERIPH_BASE 	(PERIPH_BASE + AHB1_OFFSET)

#define RCC_OFFSET 		(0x00001000UL)
#define RCC_BASE		(AHB1_PERIPH_BASE + RCC_OFFSET )

#define GPIOA_OFFSET 		(0x000UL)
#define GPIOA_BASE 		(AHB2_PERIPH_BASE + GPIOA_OFFSET )

#define AHB2EN_R_OFFSET		(0x4C)
#define RCC_AHB2EN_R		(*(volatile unsigned int*)(RCC_BASE + AHB2EN_R_OFFSET)) ///clock enabled

#define MODE_R_OFFSET		(0x00)
#define GPIOA_MODE_R		(*(volatile unsigned int*)(GPIOA_BASE + MODE_R_OFFSET))

#define OD_R_OFFSET		(0x14)
#define GPIOA_OD_R		(*(volatile unsigned int*)(GPIOA_BASE + OD_R_OFFSET))

#define PIN_5			(1<<5)
#define LED_PIN			PIN_5

#define GPIOAEN			(1U<<0) // 0b 0000 0000 0000 0000 0000 0000 0000 0001


int main(void)
{
	// 1) Enable clock access to GPIOA
	RCC_AHB2EN_R |= GPIOAEN;

	// 2) Set PA5 as output pin (MODER5 = 01)
	GPIOA_MODE_R &= ~(1U << 11); // clear bit 11
	GPIOA_MODE_R |=  (1U << 10); // set bit 10

	while (1)
	{
		// 3) Set PA5 high
		GPIOA_OD_R |= LED_PIN;

		// Simple delay
		for (volatile int i = 0; i < 100000; i++);

		// 4) Set PA5 low
		GPIOA_OD_R &= ~LED_PIN;

		// Simple delay
		for (volatile int i = 0; i < 100000; i++);
	}
}


int main(void)
{

	//1)enable clock access to GPIOA
	RCC_AHB2EN_R |= GPIOAEN;

	//2)set PA5 as outpin
	GPIOA_MODE_R |= (1U<<10); //set bit 10 to 1
	GPIOA_MODE_R &=~(1U<<11); //set bit 11 to 0

	while(1)
	{
		//3)set PA5 high
//		GPIOA_OD_R |= LED_PIN;

		//4)toggle led
		GPIOA_OD_R ^= LED_PIN; //this ^ is used to change from 0 to 1 and 1 to 0
		for (int i=0; i<100000;i++)
		{}
	}
}
