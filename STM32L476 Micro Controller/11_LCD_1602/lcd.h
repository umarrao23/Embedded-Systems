/*
 * lcd.h
 *
 *  Created on: Jul 9, 2025
 *      Author: umar_
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

void LCD_Init(void);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_printf(const char *format, ...);
void LCD_setCursor(uint8_t row, uint8_t col);
void LCD_scrollText(uint8_t row, const char *text, int delay);


#endif /* LCD_H_ */
