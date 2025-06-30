#include	"adc.h"
#define		GPIOAEN		(1U<<0)
#define 	ASCR_ASC4	(1U<<4)
#define 	ADCEN		(1U<<13)
#define		CFGR_CONT	(1U<<13)
#define 	SEQ_LEN_1	0
#define		ISR_EOC		(1U<<2)

//Step 1) 	Open data sheet/ block diagram page 17/ see bus connected to ADC module/ connected to AHB2 bus/

void pa4_adc1_init(void)
{
										/****main Step 1****/
										/*Configure ADC GPIO pin*/
//Step 2) 	open data sheet to find which GPIO pins are connected with ADC module/ search pin definition/ page 74/ we can see that pin PA4 can
//			be used as ADC 1 and 2 and input channel 9.

//Step 3)	Enable clock access to GPIOA/open reference/search AHB2ENR/page 251
	RCC->AHB2ENR = GPIOAEN;

//Step 4)	Set PA4 to analog mode/ open reference manual/ search MODER/ page 304 /Since using PA4 we will make mode 4 analog/ These are bits 8 and 9
	GPIOA->MODER |= (1U>>8);
	GPIOA->MODER |= (1U>>9);
//Step A)	Connect  GPIO analog switch to ADC input/reference manual/ Search for GPIOx_ascr/help to connect analog switch to ADC input/ asc4 need to set to 1
	GPIOA->ASCR |= ASCR_ASC4;

										/****Configure ADC Module****/
//Step 5)	Enable clock access to ADC module/ open reference manual / type AHB2ENR register/ Page 251/ ADCEN bit 13 change it to 1.
	RCC->AHB2ENR |= ADCEN;

//Step 6)	Set ADC common Clock/ open reference manual/ search ADC registers/  find ADC common control register / page 607/ we will configure CKMODE/
//			BIT 17 and 16 are CK / we will configure HCLK/2 /HCLK is our system frequency it becomes our ADC CLOCK once we set it/ bit 17=1 and bit 16=0.
	ADC123_COMMON->CCR |= (1U<<17);	//ADC 123 are common
	ADC123_COMMON->CCR &= ~(1U<<16);

//Step 7)	Set Continuous Conversion mode/ Open reference manual/ type ADC_CFGR/Page 586/set bit 13 to 1 for continonous
	ADC1->CFGR &= ~(CFGR_CONT);// this is single conversion mode

//Step 8)	Set sequencer length/ Open reference manual/ type ADC_SQR1/ in Sequence length it is the number of conversion/ if we have
//			3 conversions we put in the binary form in bit 0 to 3 / if we have 9 conversions we put in the binary form there/ if it is 1 it is 0000/
	ADC1->SQR1 = SEQ_LEN_1;

//Step 9)	Set ADC channel and rank/ Open reference manual/ type ADC_SQR1/it is the same register/if we want to sample channel 6 first and channel 9 2nd we place channel
//			channel 6 on SQ1 and channel 9 in SQ2. if channel 9 is converted to binary place it in SQ1 from bit 6 to 10.
	ADC1->SQR1 |= (1U<<6);
	ADC1->SQR1 |=(1U<<9);//0b 0 1001 =9

//Step 10)	Disable ADC deep Power down it is enabled by default /Open reference manual/ type ADC_CR (control register)/page 583/bit 29 to 0
	ADC1->CR &= ~(1U<<29);


//Step 11)	Enable ADC internal regulator/ Open reference manual/ type ADC_CR (control register)/page 583/ ADC voltage regulator enable bit 28/
//			ADVREGEN should be set to 1
	ADC1->CR |= (1U<<28);

//Step 12)	Start ADC Calibration// Open reference manual/ type ADC_CR (control register)/page 583/ Bit 31
	ADC1->CR |= (1U<<31);

//Step 13)	Enable ADC/same control register
	ADC1->CR |= (1U<<0);
}

//Step 13)	create Start conversion function/ in ADC control register page 583 / bit 2 is the start of adc regular conversion/
void start_conversion(void)
{
	ADC1->CR |= (1U<<2);// adc conversion register set
}


//Step 14)	function to read ADC Value
uint32_t adc_read(void)
{
//Step 15)	Wait for end of conversion flag/ open reference manual / type ADC_ISR/ bit 2 is EOC (End of conversion flag)/ need to read bit 2
	while (!(ADC1->ISR & ISR_EOC)){} //if statement used // & operation will return a true if bit 2 value is 1.we will use not (!) sign
							//that will say value return is not 1. it will say in the loop using {}

	ADC1->ISR |= ISR_EOC; //once flag is found it needs to be cleared

//step 16)	Read results// return the contents of ADC to data register
	return(ADC1->DR);
}
