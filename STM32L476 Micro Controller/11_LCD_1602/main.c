//-------------------------------------------------------------------------------------------
// Title     : LCD Driver and Scrolling Text Display
// Author    : Umar Wahid
// Target    : STM32L476 Series Microcontroller
// Purpose   : Bare-metal 8-bit parallel interface driver for 16x2 character LCD with
//             support for formatted text (LCD_printf) and smooth left-scrolling display.
// Version   : 1.18.1 (STM32CubeIDE)
// Date      : June 21, 2025
// Notes     :
//   - Uses GPIOB for control lines:
//       - RS  = PB5
//       - RW  = PB6
//       - EN  = PB7
//   - Uses GPIOC PC0–PC7 for 8-bit data bus (D0–D7).
//   - Designed for HD44780-compatible LCD modules.
//   - Functions include initialization, command/data write, cursor positioning,
//     formatted string printing, and scrollable long-text display.
//-------------------------------------------------------------------------------------------


#include "stm32l4xx.h"
#include "lcd.h"

int main(void)
{
    LCD_Init();

    while (1)
    {
        LCD_command(0x01);
        LCD_setCursor(0, 0);
        LCD_printf("UMAR WAHID");
        LCD_scrollText(1, "SONOMA STATE UNIVERSITY", 50);
    }
}


