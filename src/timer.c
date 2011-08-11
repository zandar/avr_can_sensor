/* timer.c
 * AVR ATmega16 timer control
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version AVR CAN sensor 13/10/2010
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/timer.h"
#include "../include/def.h"

volatile timer timer_msec = 0 , delay_time;

/**
 * Timer 0 interrupt service routine
 */
ISR(TIMER0_COMP_vect)
{
  /* increment msec @1kHz */
    ++timer_msec;
}


/**
 * timer0_init_1khz: - initialization of AVR timer0 with 1kHz freqency interrupt
 */
void timer0_init_1khz()
{
  TCNT0 = 0;
  OCR0 = 124;     /* OC register init */
  TIMSK |= 0x02;  /* interrupt enable when OC */
  TCCR0 |= 0x0B;  /* CTC mode, delicka 64x, normal port op. */
}

/**
 * delay_ms: - delay function
 * @delay: number of miliseconds to wait
 */
void delay_ms(unsigned int delay)
{
  delay_time = timer_msec;
  
  while (timer_msec <= delay_time + delay);
  
}

/*@}*/