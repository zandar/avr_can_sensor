/* display.c
 * LCD diaply "hi-level" functions
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * 13/10/2010
 */

/*@{*/

#include "../include/lcd.h"
#include "../include/display.h"
#include <stdlib.h>

/**
 * lcd_puts_line: - print string to display line
 * @line_nr: number of line to print the string in
 * @string: max. 16 characters to print on the display
 */
void lcd_puts_line(unsigned char line_nr,const char *string)
{
    lcd_clear_line(line_nr);
    lcd_gotoxy(0,line_nr);
    lcd_puts(string);
}

/**
 * lcd_clear_line: - clear display line
 * @line_nr: number of line to clear
 */
void lcd_clear_line(unsigned char line_nr)
{
  unsigned char i = 0;
  
  lcd_gotoxy(0,line_nr);
  
  for (;i < LCD_DISP_LENGTH; i++) {
    lcd_puts(" ");
  }
}

/**
 * debug: - print float number to diplay line
 * @line_nr: number of line to print the number in
 * @num: number to print, max. 10 digits
 */
void debug(unsigned char line_nr,unsigned long num)
{
  char string[11];
  
  ultoa(num,string,10);
  
  lcd_puts_line(line_nr,string);
}

/*@}*/
