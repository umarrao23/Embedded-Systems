//------------------------------------------------------------------
// Title   : LED Blinking Project using STM32L476RG
//------------------------------------------------------------------
// Purpose : This project demonstrates basic GPIO control by blinking
//           an onboard or external LED connected to a GPIO pin.
//           The LED toggles its state with a software delay loop.
//
// Special Features:
//   - GPIO Initialization using STM32 HAL drivers
//   - LED toggling using bitwise operations or HAL functions
//   - Software delay for timing control
//
// Tools    : STM32CubeIDE v1.18.1
// MCU      : STM32L476RG6 (ARM Cortex-M4)
// Author   : Umar Wahid
// Inputs   : None
// Outputs  : LED on PA5 (User LED on Nucleo-L476RG)

//------------------------------------------------------------------

#define PERIPH_BASE 		(0X40000000UL)

#define AHB2_OFFSET 		(0x08000000UL)
#define AHB2_PERIPH_BASE 	(PERIPH_BASE + AHB2_OFFSET)
//0x40000000 + 0x48000000 = 0x4800 0000

#define AHB1_OFFSET 		(0x00020000UL)
#define AHB1_PERIPH_BASE 	(PERIPH_BASE + AHB1_OFFSET)

#define RCC_OFFSET 			(0x00001000UL)
#define RCC_BASE			(AHB1_PERIPH_BASE + RCC_OFFSET )

#define GPIOA_OFFSET 		(0x000UL)
#define GPIOA_BASE 			(AHB2_PERIPH_BASE + GPIOA_OFFSET )

#define AHB2EN_R_OFFSET		(0x4C)
#define RCC_AHB2EN_R		(*(volatile unsigned int*)(RCC_BASE + AHB2EN_R_OFFSET)) ///clock enabled

#define MODE_R_OFFSET		(0x00)
#define GPIOA_MODE_R		(*(volatile unsigned int*)(GPIOA_BASE + MODE_R_OFFSET))

#define OD_R_OFFSET			(0x14)
#define GPIOA_OD_R			(*(volatile unsigned int*)(GPIOA_BASE + OD_R_OFFSET))

#define PIN_5				(1<<5)
#define LED_PIN				PIN_5

#define GPIOAEN				(1U<<0) // 0b 0000 0000 0000 0000 0000 0000 0000 0001


int main(void)
{
	// 1) Enable clock access to GPIOA
	RCC_AHB2EN_R |= GPIOAEN;

	// 2) Set PA5 as output pin
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



