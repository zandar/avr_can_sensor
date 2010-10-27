#include "../include/lcd.h"
#include "../include/display.h"
#include <stdlib.h>

/* Print string parameter to line specified by line_nr */
void lcd_puts_line(unsigned char line_nr,const char *string)
{
    lcd_clear_line(line_nr);
    lcd_gotoxy(0,line_nr);
    lcd_puts(string);
}

/* Clear line specified by line_nr */
void lcd_clear_line(unsigned char line_nr)
{
  unsigned char i = 0;
  
  lcd_gotoxy(0,line_nr);
  
  for (;i < LCD_DISP_LENGTH; i++) {
    lcd_puts(" ");
  }
}

void debug(unsigned char line_nr,long num)
{
  char string[11];
  
  itoa(num,string,10);
  
  lcd_puts_line(line_nr,string);
}
