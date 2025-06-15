#ifndef LCD_H
#define LCD_H

void LCD_Init();
void LCD_Clear();
void LCD_Command(char cmd);
void LCD_Char(char dat);
void LCD_String(const char *msg);
void LCD_String_xy(char row, char pos, const char *msg);
void LCD_Set_Cursor(char row, char column);  // <--- Add this line
void MSdelay(unsigned int val);

#endif
