/* main.c
 * AVR CAN sensor using AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 13/10/2010
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

/*@{*/

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

/**
 * SJA interrupt service routine
 */
ISR(INT0_vect)
{
  sja1000p_irq_handler(&rx_msg);
}

/**
 * MAIN
 */
int main(void)
{
  timer sensor_time = timer_msec;
  
  sei();  /* global interrupt enable */
  
  /* start timing function */
  timer0_init_1khz();
  
  lcd_init(LCD_DISP_ON);
  
  CANMSG("CAN senzor");
  
  init_fsm(&fsm_sensor,&fsm_sensor_init);
  
  
  while(1) {
    
    if (timer_msec >= (sensor_time + 100)) {
      sensor_time = timer_msec;
      debug(1,timer_msec);
      
      /* run fsm every x ms*/
      run_fsm(&fsm_sensor);
    }
    
    /* if new msg recived, configure sensor according to new parameters */
    if (rx_msg.status == NEW) {
      rx_msg.status = NONE;
      sensor_config(&rx_msg,&fsm_sensor);
    }    
  }
  
  return 0;
}

/*@}*/
