/*
 * i2c.h
 *
 *  Created on: Jul 2, 2025
 *      Author: umar_
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32l4xx.h"

#define SLAVE_OWN_ADDRESS 		0x5A

void i2c1_slave_config(void);
void i2c3_master_config(void);
uint32_t i2c1_get_address_matchcode(void);

#endif /* I2C_H_ */
