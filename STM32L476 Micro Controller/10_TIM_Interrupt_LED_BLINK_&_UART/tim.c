#include "stm32l4xx.h"

#define 	TIM1EN		(1U<<11)
#define		TIM2EN		(1U<<0)

#define		CR1_CEN		(1U<<0)

#define 	DIER_UIE		(1U<<0)

void tim1_1hz_init(void)
{
	RCC->APB2ENR |= TIM1EN;
	TIM1->PSC= 400-1;
	TIM1->ARR=10000-1;
	TIM1->CNT=0;

	TIM1->CR1 = CR1_CEN;

}



void tim1_1hz_interrupt_init(void)
{
	RCC->APB2ENR |= TIM1EN; //enabling clock access to the timer module
	TIM1->PSC= 400-1;			//set pre scaler in the auto relaod values
	TIM1->ARR=10000-1;
	TIM1->CNT=0;				//we clear counter and we enable the timer

	TIM1->CR1 = CR1_CEN;

	//step 1 we need to enable interrupt/ open reference manual/ search tim_dier(interrupt enable register)/page 963/first bit is UIE
	TIM1->DIER	|= DIER_UIE;


	//enable interrupt in NVIC
	NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
					// THIS TIM1_UP_TIM16_IQRn IMPLIES THAT IT IS USED FROM TIMER 1 TO 16
}



void tim2_output_compare(void)
{
	RCC->APB1ENR1 |= TIM2EN;
	TIM2->PSC = 400-1;
	TIM2->ARR = 10000-1;

	//SET MATCH VALUE
	TIM2->CCMR1 |=(1U<<4);
	TIM2->CCMR1 |=(1U<<5);

	//SET MATCH VALUE
	TIM2->CCR1 = 0;

	//ENABLE CH1 COMPARE MODE
	TIM2->CCER |= (1U<<0);

	//CLEAR COUNTER
	TIM2->CNT = 0;

	//ENABLE TIM2
	TIM2->CR1 = (1U<<0);
}
