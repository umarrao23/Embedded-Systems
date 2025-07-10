

//Step 1) Find I2C modules connected / select two of them/ configure onw as master and other as slave/
		  // open data sheet / page 17 / APB1 is connected with the I@C1,2 and 3. I2C1 wll be configured
		  // as slave and i2c as master.

//Step 2) find pins associated with I2C 1 SCL and SDA/data sheet search  alternate function /page 88/ bits
		// connected can be seen to i2c and AF4. PB8(SCL) and PB9(SDA)

#include 	"stm32l4xx.h"
#include	"i2c.h"

#define		GPIOBEN		(1U<<1)
#define 	I2C1EN		(1U<<21)
#define		GPIOCEN		(1U<<2)

#define		I2C3EN		(1U<<23)

#define 	I2C_TIMING	(0x00000E14) //TIMING VALUE NO 39 TIME 9:00


static void i2c1_setOwnAddress1(uint32_t OwnAddress1, uint32_t OwnAddSize);


void i2c1_slave_config(void)

{

	//step 3) Enable clock access to GPIOB
	RCC->AHB2ENR |= GPIOBEN;

				//SCL Line
	//step 4) Set PB8 (SCL Line) mode to alternate function
	GPIOB->MODER &=~ (1U<<16);
	GPIOB->MODER |= (1U<<17);

	//step 5) Set alternate function type to I2C1 which is AF4/open ref manual/page 308/ set according to PB8 pin
	GPIOB->AFR[1] &=~(1U<<0);	//1 for AFR high register and 0 for AFR low register/ set bit 0 to 3 to 0100 for AF4
	GPIOB->AFR[1] &=~(1U<<1);
	GPIOB->AFR[1] |= (1U<<2);
	GPIOB->AFR[1] &=~(1U<<3);

	//step 6) Set as open open drain/reference manual/ page / search GPIOx_Otyper/ We will configure OT8
	GPIOB->OTYPER |= (1U<<8);

	//step 8) Enable Pull-/ open reference manual search PUPDR/ configure 0 1
	GPIOB->PUPDR |= (1U<<16);
	GPIOB->PUPDR &= ~(1U<<17);

				//SDA Line
	//step 9) Set PB9 (SDA Line) mode to alternate function type to I2C1(AF4)
	GPIOB->MODER &=~ (1U<<18);
	GPIOB->MODER |= (1U<<19);

	//step 10) Set alternate function type
	GPIOB->AFR[1] &=~(1U<<4);	//1 for AFR high register and 0 for AFR low register/ set bit 0 to 3 to 0100 for AF4
	GPIOB->AFR[1] &=~(1U<<5);
	GPIOB->AFR[1] |= (1U<<6);
	GPIOB->AFR[1] &=~(1U<<7);

	//step 12) PB9 as open drain
	GPIOB->OTYPER |= (1U<<9);

	//step 13) Enable pull up
	GPIOB->PUPDR |= (1<<18);
	GPIOB->PUPDR &= ~(1<<19);

					//Configure I2C parameters
	//step 9)  Enable clock access to I2C module/ in reference manual type APB1ENR/ bit 21 is the enable
	RCC->APB1ENR1 |= I2C1EN;


	//step 10) Enable I2C interrupt in NVIC/ to enable it we will use  NVIC enable IRQ function
	NVIC_EnableIRQ(I2C1_EV_IRQn); //I2C1_EV_IRQn this is taken as argument


	//step 11) Disable I2C1/ go to I2C_CR1 control register 1/ bit 0
	I2C1->CR1 &=~(1U<<0);

	//step 12) Set device address/ To set device address we need to go to own address register 1/ we can type I2C_OAR1
	//it is 10 bit / we can choose 10 or 7 bit  addressing mode
	//Create a function// there will be two arguments that is the address we want to set the address of the device
	//and the 2nd argument will be address size eg 7 bit or 10 bit. In short this function will address 0 t 9 bit and bit 10
	// which is the bit to configure 7 or 10 bit mode
	//function is wriiten at the end

	i2c1_setOwnAddress1(SLAVE_OWN_ADDRESS,0);

	//step 13) Enable own address 1 /type I2C_OAR1/ make bit 15 to 1
	I2C1->OAR1 |= (1U<<15);


	//step 14) Enable I2C1
	I2C1->CR1 |= (1U<<0);

	//step 15) Enable ADDR interrupt/ i2c_cr1 in reference manual/ bit 3
	I2C1->CR1 |= (1U<<3);

	//step 16) Enable NACK interrupt// i2c_cr1 in reference manual/bit 4
	I2C1->CR1 |= (1U<<4);

	//step 18) Enable stop interrup/// i2c_cr1 in reference manual/ bit 5
	I2C1->CR1 |= (1U<<5);
}

	//master device implementation
void i2c3_master_config(void)
{//Part i) Now use i2c module 3/ open data sheet/write alternate function mode/ page 90/ PC0(SCL) and PC1(SDA).

	//Enable clock access to GPIOC
	RCC->AHB2ENR |=GPIOCEN;

	//Set PC0 (SCL Line) mode to alternate function mode
	GPIOC->MODER &=~ (1U<<0);
	GPIOC->MODER |=  (1U<<1);

	//Set alternate function type to I2C3 which is AF4
	GPIOC->AFR[0] &=~(1U<<0);
	GPIOC->AFR[0] &=~(1U<<1);
	GPIOC->AFR[0] |= (1U<<2);//---------
	GPIOC->AFR[0] &=~(1U<<3);

	//Set PC0 as open open drain
	GPIOC->OTYPER |= (1U<<0);

	//Enable pull up on pc0
	GPIOC->PUPDR |= (1<<0);
	GPIOC->PUPDR &= ~(1<<1);

			//Enable SDA line
	//set PC1 (SDL Line) mode to alternate function
	GPIOC->MODER &=~ (1U<<2);
	GPIOC->MODER |=  (1U<<3);

	//set alternate function type to I2C3 (AF4)
	GPIOC->AFR[0] &=~(1U<<4);
	GPIOC->AFR[0] &=~(1U<<5);
	GPIOC->AFR[0] |= (1U<<6);//-------------
	GPIOC->AFR[0] &=~(1U<<7);

	//set PC1 as open drain
	GPIOC->OTYPER |= (1U<<1);

	//enable pull up
	GPIOC->PUPDR |= (1<<2);
	GPIOC->PUPDR &= ~(1<<3);


			//Configure I2C3 parameters
	//Enable clock access to I2C module
	RCC->APB1ENR1 |= I2C3EN;

	////Disable I2C3
	I2C3->CR1 &=~ (1U<<0);

	//Configure I2C timing
	I2C3->TIMINGR =I2C_TIMING;

	//Enable I2C3
	I2C3->CR1 |=(1U<<0);

}

////////////////////////////////2ndargument helps to choose between mode 7 0r 10///
static void i2c1_setOwnAddress1(uint32_t OwnAddress1, uint32_t OwnAddSize)// this is step 12 / this function is provided by st. if we open declation
																		// of MODIFY_REG. we see a file will open mentioning stm32l4xx
//in the open declation line 212 there is a SET_BIT function that takes two arguments/there is also a clear bit function write 0 to entire register
//Read register function  returns the content of register
// write register assigns new value to entire register rather than specific bit
//modify register function takes two arguments clearmask and setmask. clearmask clears the bit you want to modify(eg we want to clear bit 2 and 3 we will have to give hex value)
//set register will put the values
{
    MODIFY_REG(I2C1->OAR1, I2C_OAR1_OA1 | I2C_OAR1_OA1MODE, OwnAddress1 | OwnAddSize);
}

uint32_t i2c1_get_address_matchcode(void)//it is going to read the ADD code / the register is on page 1314 /its position start from 17
{
	return (uint32_t) (READ_BIT(I2C1->ISR,I2C_ISR_ADDCODE) >> I2C_ISR_ADDCODE_Pos <<1);
}
