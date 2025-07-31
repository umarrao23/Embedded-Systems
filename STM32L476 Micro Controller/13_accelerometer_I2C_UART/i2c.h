/*
 * i2c.h
 *
 *  Created on: Jul 2, 2025
 *      Author: umar_
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32l4xx.h"
#include "mpu6050.h"


void i2c3_master_config(void);
void i2c3_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
uint8_t i2c3_read_byte(uint8_t dev_addr, uint8_t reg_addr);
void mpu6050_read_accel(int16_t *ax, int16_t *ay, int16_t *az);

#endif /* I2C_H_ */
