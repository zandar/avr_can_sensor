/* main.c
 * AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 13/10/2010
 */

#include "../include/F_CPU.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "../include/display.h"
#include "../include/lcd.h"
#include "../include/avr_sja1000p.h"
#include "../include/sja_control.h"
#include "../include/avr_can.h"
#include "../include/sensor.h"
#include "../include/def.h"
#include "../include/timer.h"
#include "../include/fsm.h"

#define DEBUG

struct canchip_t chip;
struct canmsg_t rx_msg;

struct fsm fsm_sensor;

/*
 * SJA interrupt service routine
 */
ISR(INT0_vect)
{
  sja1000p_irq_handler(&rx_msg);
  
  if (rx_msg.status == NEW) {
    rx_msg.status = NONE;
    sensor_config(&rx_msg,&fsm_sensor);
  }
}

/*
 *  MAIN
 */
int main(void)
{
  timer sensor_time = timer_msec;
  
  sei();      // global interrupt enable
  
  timer0_init_1khz();
  sja_init_ports();
  
  lcd_init(LCD_DISP_ON);
  
  CANMSG("CAN senzor");
  
  chip.baudrate = SJA_BAUD;
  chip.clock = SJA_CLOCK;
  chip.sja_cdr_reg = sjaCDR_CLK_OFF;
  chip.sja_ocr_reg = sjaOCR_MODE_NORMAL|sjaOCR_TX0_LH;
  
  if(sja1000p_chip_config(&chip))
  {
    CANMSG("Config error!");
    CANMSG("Restarting...");
    
    while (1); // TODO execute sw reset
  }

  sensor_init();
  
  init_fsm(&fsm_sensor,&fsm_sensor_init);
  
  while(1) {
    
    if (timer_msec >= (sensor_time + 100)) {
      sensor_time = timer_msec;
      
      /* run fsm every x ms*/
      run_fsm(&fsm_sensor);
    }
  }
  
  return 0;
}
