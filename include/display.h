#ifndef DISPLAY_H
#define DISPLAY_H

#define CANMSG(string) lcd_puts_line(1,string)

void lcd_puts_line(unsigned char line_nr,const char *string);
void lcd_clear_line(unsigned char line_nr);
void debug(unsigned char line_nr,unsigned long num);

#endif