/* main.c
 * AVR CAN sensor using AVR CAN-bus driver.
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
#include "../include/avr_can.h"
#include "../include/sensor.h"
#include "../include/def.h"
#include "../include/timer.h"
#include "../include/fsm.h"


struct canmsg_t rx_msg;

struct fsm fsm_sensor;

/* SJA interrupt service routine */
ISR(INT0_vect)
{
  sja1000p_irq_handler(&rx_msg);
}

/* MAIN */
int main(void)
{
  timer sensor_time = timer_msec;
  
  sei();  /* global interrupt enable */
  
  timer0_init_1khz();
  
  lcd_init(LCD_DISP_ON);
  
  CANMSG("CAN senzor");
  
  init_fsm(&fsm_sensor,&fsm_sensor_init);
  
  
  while(1) {
    
    if (timer_msec >= (sensor_time + 10)) {
      sensor_time = timer_msec;
      debug(1,timer_msec);
      
      /* run fsm every x ms*/
      run_fsm(&fsm_sensor);
    }
    
    if (rx_msg.status == NEW) {
      rx_msg.status = NONE;
      sensor_config(&rx_msg,&fsm_sensor);
    }
    
    
  }
  
  return 0;
}
