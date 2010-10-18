/* main.c
 * AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 13/10/2010
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include "display.h"
#include "lcd.h"
#include "avr_sja1000.h"
#include "avr_main.h"
#include "sja_control.h"

#define DEBUG

struct canchip_t chip;


/*
 *  MAIN
 */
int main(void)
{
  init_ports();
  
  sei();      // globalni povoleni preruseni
  
  lcd_init(LCD_DISP_ON);
  
  lcd_puts_line(0,"CAN senzor");
  
  chip.baudrate = SJA_BAUD;
  chip.clock = SJA_CLOCK;
  chip.sja_cdr_reg = SJA_CDR_REG;
  chip.sja_ocr_reg = SJA_OCR_REG;
  
  sja1000_chip_config(&chip);
  
  while(1) {
    
  }
  
  return 0;
}
