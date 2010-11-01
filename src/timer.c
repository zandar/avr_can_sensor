/* timer.c
 * AVR ATmega16 timer control
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/timer.h"
#include "../include/def.h"

volatile timer timer_msec = 0 , delay_time;

/* Timer 0 interrupt service routine */
ISR(TIMER0_COMP_vect)
{
  /* increment msec @1kHz */
    ++timer_msec;
}



void timer0_init_1khz()
{
  TCNT0 = 0;
  OCR0 = 124;     /* OC register init */
  TIMSK |= 0x02;  /* interrupt enable when OC */
  TCCR0 |= 0x0B;  /* CTC mode, delicka 64x, normal port op. */
}

void delay_ms(unsigned int delay)
{
  delay_time = timer_msec;
  
  while (timer_msec <= delay_time + delay);
  
}