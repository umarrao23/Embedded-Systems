#include "stm32l4xx.h"
#include <stdint.h>

#define GPIOAEN             (1U<<0)
#define UART2EN				(1U<<17)

#define SYS_FREQ			4000000
#define APB1_CLK			SYS_FREQ

#define CR1_TE				(1U<<3)
#define CR1_UE				(1U<<0)

#define ISR_TXE				(1U<<7)

static uint16_t compute_uart_div(uint32_t PeriphCLK, uint32_t Baudrate);
static void uart_set_baudrate(USART_TypeDef * USARTx,uint32_t PeriphCLK,uint32_t Baudrate);
static void uart2_write(USART_TypeDef * USARTx,uint8_t ch);

int __io_putchar(int ch)
{
	uart2_write(USART2,ch);
	return ch;

}

void uart2_tx_init(void)
{

	RCC->AHB2ENR |= GPIOAEN;


	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);


	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);



	RCC->APB1ENR1 |= UART2EN;


	uart_set_baudrate(USART2,APB1_CLK,115200);

	USART2->CR1 = CR1_TE;
	USART2->CR1 |= CR1_UE;

}

static void uart2_write(USART_TypeDef * USARTx,uint8_t ch)
{

	while(!(USARTx->ISR & ISR_TXE)){}


	USARTx->TDR = (ch & 0xFF);
}


static void uart_set_baudrate(USART_TypeDef * USARTx,uint32_t PeriphCLK,uint32_t Baudrate)
{
	USARTx->BRR = compute_uart_div(PeriphCLK,Baudrate);

}


static uint16_t compute_uart_div(uint32_t PeriphCLK, uint32_t Baudrate)
{
	return ((PeriphCLK + (Baudrate/2))/ Baudrate);
}

