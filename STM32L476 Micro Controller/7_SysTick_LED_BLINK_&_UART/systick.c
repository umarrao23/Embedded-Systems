#include "stm32l4xx.h"

#define		SysTick_LOAD_VAl 4000 //Default clock of MC Is 4 MHZ/ Meaning 4 million cycles per second/to get no of cycles per milli second/
							  //1000 milli second make a single second thus no cycle per milli second is 4000000/1000
#define		CTRL_ENABLE		(1U<<0)
#define		CTRL_CLKSRC		(1U<<2)
#define		CTRL_COUNTFLAG	(1U<<16)

//Step 1) search Cortex m4 Generic user Guide on goole download it/ Search SysTick/Page 249/
//		  Bit 0 is enable/disable / bit 2 is clock source// Bit 16 is count flag

//Step 2) Create a function for systick delay MS
void systickDelayMs(int delay)
{
	//Step 3) Reload Systick with number of cycles per millisecond.
	SysTick->LOAD = SysTick_LOAD_VAl; // inside our structure which is loaded into .h file the reload registers is simply called load
	//double click on systick /right click and open declarion. we will see define SysTick. right click on  systickType and then open declaration. we will see CTRL in comments
	//written as systick control and status register

	//Step 4) Clear Systick current value register.
	SysTick->VAL= 0;

	//Step 5) select clock source and enable systick
	SysTick->CTRL = CTRL_CLKSRC	| CTRL_ENABLE; // the order does not matter we can enable clock source first then enable it

	//Step 5)  For loop to get delay
	for (int i=0;i<delay;i++)
	{
		//wait until COUNTFLAG is set
		//to read a bit we use a bit we want to read as one operand and use the register for which you want to read as 2nd operand and then perform the and operation
		while ((SysTick->CTRL & CTRL_COUNTFLAG)==0){}// if return o it means it is not set yet it is stucked in the loop{}
	}
	SysTick->CTRL = 0; // once get out of the loop systick is disabled
}
