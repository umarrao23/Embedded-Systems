//------------------------------------------------------------------
// Title     : UART2 Transmission - STM32L4
// Author    : Umar Wahid
// Target    : STM32L4 Series Microcontroller
// Purpose   : To configure and transmit data using USART2 on PA2
//             (TX only) with polling method at 115200 baud rate.
// Version   : 1.18.1 STM32CubeIDE
//------------------------------------------------------------------
#include <stdint.h>    //we are using uint
#include "stm32l4xx.h"   // Include device-specific header for STM32L4 series


#define GPIOAEN             (1U<<0)     // Bitmask to enable GPIOA clock
#define UART2EN				(1U<<17)	//page 253 refrence manual
#define SYS_FREQ			4000000	// it is the frequency at which a MC is runnning at .it is taken and distributed to various parts of MC. Like AHB bus etc
										// It is the default frequency is 4MHz
#define APB1_CLK			SYS_FREQ //clock for ABP1 is PCLK1/APB1_CLK---Clock for APB2 is PCLK2/APB2_CLK
#define CR1_TE				(1U<<3)
#define CR1_UE				(1U<<0)
#define ISR_TXE				(1U<<7)


static uint16_t compute_uart_div(uint32_t PeriphCLK, uint32_t Baudrate);
static void uart_set_baudrate(USART_TypeDef * USARTx,uint32_t PeriphCLK,uint32_t Baudrate);
static void uart2_write(USART_TypeDef * USARTx,uint8_t ch);
void uart2_tx_init(void);

int main(void)
{

	uart2_tx_init();

    while (1)
    {
    	uart2_write(USART2,'Y');
    	for (volatile int i = 0; i < 100000; ++i); // delay

}
}

void uart2_tx_init(void)
{
	/****************Configure UART Pin*************/
	// Step 1----/*enable clock access to GPIOA for PA2 as TX line */
	RCC->AHB2ENR |= GPIOAEN;

	// Step 2----/*set PA2 as alternate function mode */go tp mode register(reference manual page 304)
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);



	// Step 3----/*set Alternate  function type as UART*/ to do this we need to go to alternate function register [refrence manual page 308)
	// We will configure AF7 since page 88 of data sheet figure shows PA2 needs to configure AF7.
	//in header we dont have AFRL or AFRH. The first index is IS for low and 2nd index is for high//0 for low and 1 for high
	//to set AF7 we need 0111 at PA2
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);


	/****************Configure uart Module*************/
	// Step 4----/*enable clock access to UART2*/
	//Go to reference manual type APB1ENR. Page 253 reference manual. USART is  pin 17. enable it
	RCC->APB1ENR1 |= UART2EN;

	// Step 5----/*Configure Baud Rate*/
	uart_set_baudrate(USART2,APB1_CLK,115200); //115200 is the baud rate argument

	// Step 6----/*Configure transfer direction*/
	// select it is RX or TX. We open reference Manual/// page 1366. Bit 3 TE(Transmitter Enable ) and Bit 2 RE(Receiver Enable) ///Enable 1 and Disable 0
	USART2->CR1 = CR1_TE; // or operation not here performed

	// Step 7----/*enable UART module*/
	//page 1366 reference manual// bit 1 enable is USART
	USART2->CR1 |= CR1_UE;	//or operation is performed over here because we have already configured CR1 and just = sign will alter the previous value

}

//Creating a function for values to be transmitted using UART
static void uart2_write(USART_TypeDef * USARTx,uint8_t ch)
{
	//Step 9
	//GO to reference manual . USART Interrupt and Status register page 1381
	/*wait for transmit data register to be empty*/
	while(!(USARTx->ISR & ISR_TXE)){}

	//Step 8
	//reference manual search TRD(8 bit register) page 1387// Contains data characters to be transmitted//
	//This is transmitted value
	//TXE bit 7. we want transmit data register to be empty before we put something new into transmit data register. if its empty its 1 if not its 0
	USARTx->TDR = (ch & 0xFF); //And operation is performed because we want to make sure 8 bit is transmitted
}


//creating a function to set baudrate
static void uart_set_baudrate(USART_TypeDef * USARTx,uint32_t PeriphCLK,uint32_t Baudrate)
{
	USARTx->BRR = compute_uart_div(PeriphCLK,Baudrate);
	//uart_set_baudrate(USART2,uint32 PeriphCLK,uint32_t Baudrate);
}

//Function
static uint16_t compute_uart_div(uint32_t PeriphCLK, uint32_t Baudrate)// pass the frequency of APB1 bus as the first argument and baud rate as 2nd argument
{
	return ((PeriphCLK + (Baudrate/2))/ Baudrate);
}
