

#ifndef ADC_H_
#define ADC_H_
#include "stm32l4xx.h"
#include <stdint.h>

void start_conversion(void);
uint32_t adc_read(void);
void pa4_adc1_init(void);

#endif /* ADC_H_ */
