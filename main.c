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
#include "avr_sja1000p.h"
#include "avr_main.h"
#include "sja_control.h"
#include "avr_canmsg.h"

struct canchip_t chip;
//struct canmsg_t msg;


/*
 *  MAIN
 */
int main(void)
{
  unsigned char i = 0;
  
  init_ports();
  
  sei();      // globalni povoleni preruseni
  
  lcd_init(LCD_DISP_ON);
  
  lcd_puts_line(0,"CAN senzor");
  
  chip.baudrate = SJA_BAUD;
  chip.clock = SJA_CLOCK;
  chip.sja_cdr_reg = sjaCDR_CLK_OFF;
  chip.sja_ocr_reg = sjaOCR_MODE_NORMAL|sjaOCR_TX0_LH;
  
  
//   msg.id = 0x00;
//   msg.length = 8;
//   
//   for (;i< CAN_MSG_LENGTH;i++)
//     msg.data[i] = i;
  
  sja1000p_chip_config(&chip);
//   sja1000_pre_write_config(&msg);
//   sja1000_send_msg();
  
  while(1) {
    
  }
  
  return 0;
}
