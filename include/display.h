/* display.h
 * Header file for LCD dislpay "hi-level" function
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * 13/10/2010
 */

/*
 * This file is part of avr_can_sensor.
 *
 * Avr_can_sensor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Avr_can_sensor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with avr_can_sensor.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

/* TODO dont use canmsg for print in interrupt, delay_ms is not reentrant!!! */
#define CANMSG(string) lcd_puts_line(0,string);delay_ms(1000)

void lcd_puts_line(unsigned char line_nr,const char *string);
void lcd_clear_line(unsigned char line_nr);
void debug(unsigned char line_nr,unsigned long num);

#endif