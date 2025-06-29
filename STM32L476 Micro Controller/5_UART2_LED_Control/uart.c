
#include <stdint.h>
#include <uart.h>

#define GPIOAEN             (1U<<0)
#define UART2EN				(1U<<17)

#define SYS_FREQ			4000000
#define APB1_CLK			SYS_FREQ

#define CR1_TE				(1U<<3)
#define CR1_UE				(1U<<0)

#define CR1_RE				(1U<<2)

#define ISR_TXE				(1U<<7)
#define ISR_RNXE			(1U<<5)

static uint16_t compute_uart_div(uint32_t PeriphCLK, uint32_t Baudrate);
static void uart_set_baudrate(USART_TypeDef * USARTx,uint32_t PeriphCLK,uint32_t Baudrate);
static void uart2_write(USART_TypeDef * USARTx,uint8_t ch);

int __io_putchar(int ch)
{
	uart2_write(USART2,ch);
	return ch;

}

void uart2_rxtx_init(void)
{
	//Enable clock access to GPIO for PA2 and PA3 as TX and RX line Respectively.[step 1]
	RCC->AHB2ENR |= GPIOAEN;

	//set PA2 mode as alternative Function mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	//set the alternative function typed to UART
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);

	//set PA3 mode as alternative Function mode[step 2]. open refrence manual page 304. moder 3 is bit 6 and 7(10)
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |=(1U<<7);


	//set the alternative function typed to UART[step 3]. Page 308
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &=~(1U<<15);

	/************Configure the UART Module***********/
	//Enable clock access to UART @
	RCC->APB1ENR1 |= UART2EN;

	//CONFIGURE BAUD RATE
	uart_set_baudrate(USART2,APB1_CLK,115200);

	//Configure Transfer Direction [step 4] NEED TO ENBALE RECEIVER  ||| reference manual TYPE USART_CR1
	USART2->CR1 = CR1_TE | CR1_RE; // we can also use or operator
	//USART2->CR1	=CR1_RE;

	//ENABLE UART MODULE
	USART2->CR1 |= CR1_UE;

}

//Function to read Received data
char uart2_read(USART_TypeDef * USARTx) //it will return what it read so that would be a character
{
	//wait for received data register not empty flag(RXNE). go to refrence manual write RXNE page 1399. it is bit 5. data received this is 1 otherwise 0
	while(!(USARTx->ISR & ISR_TXE)){}

	//read the content of received data register
	return USARTx->RDR;
}

static void uart2_write(USART_TypeDef * USARTx,uint8_t ch)
{

	while(!(USARTx->ISR & ISR_RNXE)){}


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
