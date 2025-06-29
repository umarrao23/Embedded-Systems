//------------------------------------------------------------------
// Title     : ADC Read and UART Transmission on STM32L476
// Author    : Umar Wahid
// Target    : STM32L476 Series Microcontroller
// Purpose   : Reads analog input from PA4 using ADC1 Channel 9
//             and transmits the digital value via UART2 (PA2 TX).
// Date      : June 21, 2025
// Notes     : - System clock is assumed to be 4 MHz
//             - UART2 baudrate: 115200
//             - ADC configured for continuous conversion mode
//             - No external libraries (HAL/Cube) used
// Version   : 1.18.1 STM32CubeIDE
//------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"
#include "uart.h"
#include "adc.h"

int sensor_value;
void pseudo_dly(void);

int main(void)
{
	pa4_adc1_init();
	pseudo_dly();
	start_conversion();
	uart2_tx_init();


    while (1)
    {
    	sensor_value = adc_read();
    	printf("sensor value : %d\n\r",sensor_value);
    }
}

void pseudo_dly(void)
{
	int x;
	for (int i=0; i<8000;i++){
		x++;
	}
}



