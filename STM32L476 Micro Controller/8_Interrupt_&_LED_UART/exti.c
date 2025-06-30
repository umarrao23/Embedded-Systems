#include "stm32l4xx.h"

#define 	GPIOCEN 	(1U<<2)
#define 	SYSCFGEN 	(1U<<0)

void pc13_exti_init(void)
{
	//Step 1) Enable clock access to GPIOC/ GPIOC connected to AHB2 bus/ In reference manual search AHB2ENR
	RCC->AHB2ENR |= GPIOCEN;

	//Step 2) Enable clock access to SYSCFG/ search apb2enr whic is connected to SYSCFG/ it is bit 0
	RCC->APB2ENR |= SYSCFGEN;

	//Step 3) Set PC13 as input pin
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);


	//Step 4) Clear port selection for EXTI13/REFRENCE MANUAL/ search SYSCFG_EXTICR/page 317
	SYSCFG->EXTICR[3]	&=~(1U<<4); //if in it is [1] it means EXTI control register 2 and if it is 3 in the [3] it means  EXTI control register 4
	SYSCFG->EXTICR[3]	&=~(1U<<5);
	SYSCFG->EXTICR[3]	&=~(1U<<6);
	SYSCFG->EXTICR[3]	&=~(1U<<7);

	//Step 5) Select PORT C for EXTI3/ page 321/ to make it port c we need to pass EXTI 0010 from bit 7 to 4
	SYSCFG->EXTICR[3]	|= (1U<<5);

	//Step 6) Unmask EXT13/ search EXTI_IMR1 in ref manual/page 402/ we need to 1 to unmask it
	EXTI->IMR1 |= (1U<<13);
	//

	//step 7) Select Falling edge trigger/search EXTI_FTSR(Falling trigger select register 1)/page 403/
	EXTI->FTSR1	|=(1U<<13);

	//Step 8) Enable EXTI line in NVIC/ we will use the function that exist in cortex.h file
	NVIC_EnableIRQ(EXTI15_10_IRQn); //irq number is 14 but it has a symbolic name (EXTI15_10_IRQn
}
