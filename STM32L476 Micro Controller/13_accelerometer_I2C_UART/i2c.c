

#include 	"stm32l4xx.h"
#include	"i2c.h"



#define		GPIOCEN		(1U<<2)

#define		I2C3EN		(1U<<23)

#define 	I2C_TIMING	(0x00000E14) //TIMING VALUE NO 39 TIME 9:00

#define 	ISR_BUSY	(1U<<15)
#define 	CR2_START	(1U<<13)

//PC0 ---SCl
//PC1 ---SDA

	//master device implementation
void i2c3_master_config(void)
{//Part i) Now use i2c module 3/ open data sheet/write alternate function mode/ page 90/ PC0(SCL) and PC1(SDA).

	//Enable clock access to GPIOC
	RCC->AHB2ENR |=GPIOCEN;

	//Set PC0 (SCL Line) mode to alternate function mode
	GPIOC->MODER &=~ (1U<<0);
	GPIOC->MODER |=  (1U<<1);
	//set PC1 (SDL Line) mode to alternate function
	GPIOC->MODER &=~ (1U<<2);
	GPIOC->MODER |=  (1U<<3);


	//Set PC0 as open open drain
	GPIOC->OTYPER |= (1U<<0);
	//set PC1 as open drain
	GPIOC->OTYPER |= (1U<<1);


	//Enable pull up on pc0
	GPIOC->PUPDR |= (1<<0);
	GPIOC->PUPDR &= ~(1<<1);
	//enable pull up ON PC1
	GPIOC->PUPDR |= (1<<2);
	GPIOC->PUPDR &= ~(1<<3);




	//Set alternate function type to I2C3 which is AF4(PC)
	GPIOC->AFR[0] &=~(1U<<0);
	GPIOC->AFR[0] &=~(1U<<1);
	GPIOC->AFR[0] |= (1U<<2);//---------
	GPIOC->AFR[0] &=~(1U<<3);

	//set alternate function type to I2C3 (AF4)(PC1)
	GPIOC->AFR[0] &=~(1U<<4);
	GPIOC->AFR[0] &=~(1U<<5);
	GPIOC->AFR[0] |= (1U<<6);//-------------
	GPIOC->AFR[0] &=~(1U<<7);



			//Configure I2C3 parameters
	//Enable clock access to I2C module
	RCC->APB1ENR1 |= I2C3EN;

	////Disable I2C3
	I2C3->CR1 &=~ (1U<<0);

	//ANALOG ENABLE
	I2C3->CR1 &= ~(1U<<12);

	for (volatile int i = 0; i < 10; ++i); //DELAY

	//DIGITAL DISABLE
	I2C3->CR1 &= ~(1U<<11);
	I2C3->CR1 &= ~(1U<<10);
	I2C3->CR1 &= ~(1U<<9);
	I2C3->CR1 &= ~(1U<<8);

	//Configure I2C timing
	I2C3->TIMINGR =I2C_TIMING;

	//Enable I2C3
	I2C3->CR1 |=(1U<<0);

	for (volatile int i = 0; i < 10; ++i); //DELAY
}

void i2c3_write_byte(uint8_t saddr, uint8_t reg, uint8_t data)
{
	//open reference manual/ type i2c_isr/ page 1314/
    while (I2C3->ISR & I2C_ISR_BUSY); // checking when the bus gets free

    // Send both bytes (reg + data) in one transaction
    I2C3->CR2 = 0;
    I2C3->CR2 |= (saddr << 1);     // 7-bit address
    I2C3->CR2 |= (2U << 16);       // NBYTES = 2
    I2C3->CR2 &= ~(1U << 10);      // Write direction
    I2C3->CR2 |= I2C_CR2_START;    // Start condition

    while (!(I2C3->ISR & I2C_ISR_TXIS));  // Wait to send reg
    I2C3->TXDR = reg;

    while (!(I2C3->ISR & I2C_ISR_TXIS));  // Wait to send data
    I2C3->TXDR = data;

    while (!(I2C3->ISR & I2C_ISR_TC));    // Wait for transfer complete
    I2C3->CR2 |= I2C_CR2_STOP;            // Send stop condition
}




uint8_t i2c3_read_byte(uint8_t saddr, uint8_t reg) {
    while (I2C3->ISR & ISR_BUSY);  // Wait if bus is busy

    // Step 1: Write register address to read from
    I2C3->CR2 = (saddr << 1) | (1U << 16);  // NBYTES = 1, write mode
    I2C3->CR2 |= CR2_START;
    while (!(I2C3->ISR & I2C_ISR_TXIS));
    I2C3->TXDR = reg;
    while (!(I2C3->ISR & I2C_ISR_TC));

    // Step 2: Read 1 byte
    I2C3->CR2 = (saddr << 1) | (1U << 16) | (1U << 10);  // NBYTES = 1, read mode
    I2C3->CR2 |= CR2_START;
    while (!(I2C3->ISR & I2C_ISR_RXNE));
    uint8_t data = I2C3->RXDR;

    // Stop condition
    I2C3->CR2 |= I2C_CR2_STOP;

    return data;
}







