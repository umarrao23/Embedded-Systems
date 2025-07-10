//-------------------------------------------------------------------------------------------
// Title     : I2C Master-Slave Communication (STM32L476)
// Author    : Umar Wahid
// Target    : STM32L476 Series Microcontroller
// Purpose   : Demonstrates I2C communication between master (I2C3 on PC0/PC1)
//             and slave (I2C1 on PB8/PB9) using interrupt-driven reception and polling-based transmission.
// Version   : 1.18.1 (STM32CubeIDE)
// Date      : June 21, 2025
// Notes     :
//   - Master (I2C3):
//       - SCL: PC0 (AF4)
//       - SDA: PC1 (AF4)
//   - Slave (I2C1):
//       - SCL: PB8 (AF4)
//       - SDA: PB9 (AF4)
//   - Ground of both boards must be connected for communication.
//   - I2C1 uses interrupts for reception; I2C3 sends data using polling.
//   - UART2 on PA2 is used for printing output (Tx only).
//-------------------------------------------------------------------------------------------



#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"
#include "uart.h"
#include "i2c.h"

const uint8_t msg[]= "HELLO"; // creates an array of Hello

uint8_t* p_tx_buff = (uint8_t*)msg; //it creates a pointer that points the beginning of the msg array
uint8_t tx_size = sizeof(msg); //this stores total number of bytes it transmits. for hello its 6 including \0 (null operator)
volatile uint8_t tx_idx = 0; //keeps track of which byte is being currently transmitted
//volotile tell the compiler that check varibale every time it might change because of interrupt

uint8_t rx_buff[sizeof(msg)]; // buffer to receive data
uint8_t rx_size = sizeof(msg); // indiactes how many bytes to receive
volatile uint8_t rx_idx = 0; // keeps track how many bytes received so far

static void handle_i2c_master(void);
//use a jumper wire to connect SCL to SCL and SDA to SDA
// connect ground of one board to the ground of the other board if we are using two boards
//PB8(SCL)--------------PC0(SDL)
//PB9(SDA)--------------PC1(SDA)
//open user guide to see pins

int main(void)
{
	uart2_tx_init();

	i2c1_slave_config(); //enable slace device
	i2c3_master_config(); //enable master device
	handle_i2c_master();

	while(1)
	{

	}
}

static void handle_i2c_master(void)
{
	//set slave address(i2c_cr2)
	I2C3->CR2 |= SLAVE_OWN_ADDRESS;

	//Set 7 bits address mode
	I2C3->CR2 &=~(1U<<11);

	//set transfer size
	I2C3->CR2 |= (tx_size<<16);

	//set automatic end mode
	I2C3->CR2 |=(1U<<25);

	//generate start condition
	I2C3->CR2 |= (1U<<13);

	//loop until STOP flag is raised/ reference manual/ search I2C interrupt and status register (I2C_ISR)
	//bit 5 is the stop flag// check for this flag so will use while loop
	while(!(I2C3->ISR & (1U<<5)))
	{
		if (I2C3->ISR & (1U<<1))
		{
			//write data in transmit data register
			//this flag is cleared by writing data in TXDR REGISTER
			I2C3->TXDR = (*p_tx_buff++);
		}
	}
	//CLEAR STOP FLAG /SEARCH I2C_ICR/BIT 5
	I2C3-> ICR |= (1U<<5);
}

void slave_reception_callback(void)
{
	//read character in receive data register
	//RXNE flag is cleared by reading data in RXDR register


	//read first 8 bits
	rx_buff[rx_idx++] = (I2C1->RXDR & 0xFF);
}

void slave_complete_callback(void)
{
	printf("Transfer complete!!!\n\r");
}

void I2C1_EV_IRQHandler(void) //function for interrupt service routine for IC1 . it can be found in startup.h file
{
	//check address flag value in ISR register/search  I2c_ISR
	if (I2C1->ISR & (1U<<3))
	{
		//Verify the address match with the OWN Slave address
		if (i2c1_get_address_matchcode() == SLAVE_OWN_ADDRESS)
		{
			//VERIFY DIRECTION, a write direction , slave enters receiver mode
			if ((I2C1->ISR & (1U<<16)) == 0)
			{
				//clear ADDR flag value in ISR register
				I2C1->ICR |= (1U<<3);

				//enable receive interrupt/page 1306
				I2C1 ->CR1 |=(1U<<2);

			}
		}
	}
	//check RXNE flag value in ISR register
	else if (I2C1->ISR & (1U<<2))
	{
		//Do something
		slave_reception_callback();
	}
	//check stop flag value in ISR Register
	else if (I2C1 -> ISR & (1U<<5))
	{
		//if stop flag is 1 we want to end tranmission and that can be done simply by clearing the stop flag
		//clear stop flag/ search i2c_icr / there is a stop flag bit which is bit 5
		I2C1 ->ICR |= (1U<<5);

		// do something when transmission ends
		slave_complete_callback();

	}

	else{

		//implement error routine
	}
}





