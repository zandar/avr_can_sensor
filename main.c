/* main.c
 * AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 13/10/2010
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "avr_sja1000.h"


/*
 *  MAIN
 */
int main(void)
{
  lcd_init(LCD_DISP_ON);
  
  sei();			// globalni povoleni preruseni
  
  lcd_puts_line(0,"CAN senzor");    
  
  while(1) {
  ;
  }
  
  return 0;
}
