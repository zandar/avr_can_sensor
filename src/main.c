/* main.c
 * AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 13/10/2010
 */

#include "../include/F_CPU.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "../include/display.h"
#include "../include/lcd.h"
#include "../include/avr_sja1000p.h"
#include "../include/sja_control.h"
#include "../include/avr_can.h"
#include <util/delay.h>

#define DEBUG

struct canchip_t chip;

struct canmsg_t tx_msg,rx_msg;

char sja_status = 0;

/*
 * SJA interrupt service routine
 */
ISR(INT0_vect)
{
  sja1000p_irq_handler(&rx_msg);
}

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
  
  
  tx_msg.id = 1;
  tx_msg.length = 8;
  tx_msg.flags = MSG_EXT;
  
  for (;i< tx_msg.length;i++)
    tx_msg.data[i] = i*10;
  
  if(sja1000p_chip_config(&chip))
  {
    CANMSG("Config error!");
    _delay_ms(1000);
    CANMSG("Restarting...");
    _delay_ms(1000);
    while (1); // proved sw reset nejak
  } 
  
  sja1000p_pre_write_config(&tx_msg);
  sja1000p_send_msg();
 
  
  while(1) {
    
  }
  
  return 0;
}
