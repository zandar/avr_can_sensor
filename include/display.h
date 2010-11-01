/* display.h
 * Header file for LCD dislpay "hi-level" function
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * 13/10/2010
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#define CANMSG(string) lcd_puts_line(0,string);delay_ms(1000)

void lcd_puts_line(unsigned char line_nr,const char *string);
void lcd_clear_line(unsigned char line_nr);
void debug(unsigned char line_nr,unsigned long num);

#endif