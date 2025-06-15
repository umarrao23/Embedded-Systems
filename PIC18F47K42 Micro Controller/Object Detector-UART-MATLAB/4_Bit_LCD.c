//------------------------------------------------------------------------------
// File     : lcd.c
// Author   : Umar Wahid
// MCU      : PIC18F47K42
// Compiler : MPLAB X IDE v6.2, XC8
//------------------------------------------------------------------------------
// Description:
//   This file provides LCD interface functions for a 16x2 character display 
//   using **4-bit mode** communication with a PIC18F47K42 microcontroller.
//
//   The LCD is connected as follows:
//     - Data lines: RD4 to RD7
//     - Control lines: RS (RC2), EN (RC3)
//
//   4-bit mode reduces the number of required data lines by sending data 
//   in two 4-bit nibbles (high nibble first, then low nibble).
//
// Notes:
//   - This implementation assumes LATD, LATC, and TRISD/ANSELx are configured
//     correctly before calling LCD_Init().
//   - `NOP()` is used for EN pulse timing (can be adjusted as needed).
//   - Use `LCD_Command(0x0E)` to turn on cursor or `0x0F` for cursor+blink.
//------------------------------------------------------------------------------

#include <xc.h>
#include "lcd.h"

#define RS LATCbits.LATC2
#define EN LATCbits.LATC3

#define LCD_Port TRISD   // PORTD for data lines
#define LCD_Control TRISC

#define ldata LATD       // Using LATD4?LATD7

// Helper to send high or low nibble
void LCD_Send_Nibble(char nibble)
{
    ldata &= 0x0F;              // Clear RD4?RD7
    ldata |= (nibble & 0xF0);   // Place high nibble on RD4?RD7
    EN = 1;
    NOP();
    EN = 0;
}

void LCD_Command(char cmd)
{
    RS = 0;
    LCD_Send_Nibble(cmd);           // Send high nibble
    LCD_Send_Nibble(cmd << 4);      // Send low nibble
    MSdelay(2);
}

void LCD_Char(char dat)
{
    RS = 1;
    LCD_Send_Nibble(dat);           // Send high nibble
    LCD_Send_Nibble(dat << 4);      // Send low nibble
    MSdelay(2);
}

void LCD_Init()
{
    LCD_Port &= 0x0F;     // Set RD4?RD7 as output
    LCD_Control = 0x00;   // RC2, RC3 as output

    MSdelay(20);

    RS = 0;

    // Initialization in 4-bit mode
    LCD_Send_Nibble(0x30); MSdelay(5);
    LCD_Send_Nibble(0x30); MSdelay(1);
    LCD_Send_Nibble(0x30); MSdelay(1);
    LCD_Send_Nibble(0x20); MSdelay(1); // Set 4-bit mode

    LCD_Command(0x28); // 4-bit, 2-line, 5x8 font
    LCD_Command(0x0C); // Display on, cursor off
    LCD_Command(0x06); // Entry mode
    LCD_Command(0x01); // Clear display
    MSdelay(2);
}

void LCD_Clear()
{
    LCD_Command(0x01);
    MSdelay(2);
}

void LCD_String(const char *msg)
{
    while((*msg)!=0)
    {
        LCD_Char(*msg);
        msg++;
    }
}

void LCD_String_xy(char row, char pos, const char *msg)
{
    char location = (row == 1) ? (0x80 | (pos & 0x0F)) : (0xC0 | (pos & 0x0F));
    LCD_Command(location);
    LCD_String(msg);
}

void LCD_Set_Cursor(char row, char column)
{
    char pos = (row == 1) ? (0x80 + column) : (0xC0 + column);
    LCD_Command(pos);
}

void MSdelay(unsigned int val)
{
    unsigned int i, j;
    for(i=0; i<val; i++)
        for(j=0; j<165; j++);
}
