/* main.c
 * AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 13/10/2010
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.h"
#include "lcd.h"

#define _AVR_SJA1000_H
#define _AVR_MAIN_H
#define _SJA_CONTROL_H

#include "avr_sja1000.h"
#include "avr_main.h"

struct canchip_t chip;

/*
 *  MAIN
 */
int main(void)
{
  sei();      // globalni povoleni preruseni
  
  lcd_init(LCD_DISP_ON);
  
  lcd_puts_line(0,"CAN senzor");
  
  chip.baudrate = 1000000;
  chip.clock = 16000000;
  chip.sja_cdr_reg = 0;
  chip.sja_ocr_reg = 0;
  
  sja1000_chip_config(&chip);
  
  while(1) {
    
  }
  
  return 0;
}
