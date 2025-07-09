#include "stm32l4xx.h"


#include "lcd.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define RS (1U << 5)   // PB5
#define RW (1U << 6)   // PB6
#define EN (1U << 7)   // PB7

static void delayMs(int delay);
static void GPIO_Init(void);

void LCD_Init(void)
{
    GPIO_Init();

    delayMs(30);
    LCD_command(0x30);
    delayMs(10);
    LCD_command(0x30);
    delayMs(1);
    LCD_command(0x30);

    LCD_command(0x38); // 8-bit, 2-line, 5x7 font
    LCD_command(0x06); // Entry mode
    LCD_command(0x01); // Clear screen
    delayMs(2);
    LCD_command(0x0F); // Display ON, Cursor ON, Blink ON
}

void LCD_command(unsigned char command)
{
    GPIOB->BSRR = (RS | RW) << 16;
    GPIOC->ODR = command;
    GPIOB->BSRR = EN;
    delayMs(1);
    GPIOB->BSRR = EN << 16;
}

void LCD_data(char data)
{
    GPIOB->BSRR = RS;
    GPIOB->BSRR = RW << 16;
    GPIOC->ODR = data;
    GPIOB->BSRR = EN;
    delayMs(1);
    GPIOB->BSRR = EN << 16;
}

void LCD_printf(const char *format, ...)
{
    char buffer[32];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        LCD_data(buffer[i]);
    }
}

void LCD_setCursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? col : (0x40 + col);
    LCD_command(0x80 | address);
}

void LCD_scrollText(uint8_t row, const char *text, int delay)
{
    int len = strlen(text);
    if (len <= 16)
    {
        LCD_setCursor(row, 0);
        LCD_printf(text);
        return;
    }

    for (int i = 0; i <= len - 16; i++)
    {
        LCD_setCursor(row, 0);
        for (int j = 0; j < 16; j++)
        {
            LCD_data(text[i + j]);
        }
        delayMs(delay);
    }
}

static void GPIO_Init(void)
{
    RCC->AHB2ENR |= (1U << 1) | (1U << 2);
    GPIOB->MODER &= ~(0x3 << 10 | 0x3 << 12 | 0x3 << 14);
    GPIOB->MODER |=  (0x1 << 10 | 0x1 << 12 | 0x1 << 14);
    GPIOC->MODER &= ~0x0000FFFF;
    GPIOC->MODER |=  0x00005555;
    GPIOB->BSRR = (RW | EN) << 16;
}

static void delayMs(int delay)
{
    for (; delay > 0; delay--)
    {
        for (volatile int i = 0; i < 3195; i++) {}
    }
}
