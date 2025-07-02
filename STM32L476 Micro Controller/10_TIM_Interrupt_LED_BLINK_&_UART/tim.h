/*
 * tim.h
 *
 *  Created on: Jul 2, 2025
 *      Author: umar_
 */

#ifndef TIM_H_
#define TIM_H_

void tim1_1hz_init(void);

//#define SR_UIF (1U<<0)
#define SR_UIF (1U<<0)

void tim2_output_compare(void);
void tim1_1hz_interrupt_init(void);

#endif /* TIM_H_ */
