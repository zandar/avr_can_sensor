/**
 Title	:   ATmega16 timer control
 Author:    Michal Vokac
 File:	    timer.c
 Version:   1.0
 Date:      2010/10/13       
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/timer.h"
#include "../include/sja_control.h"

#define timer0_stop() TCCR0 &= ~0x07, timer0 = 0

volatile unsigned int timer0 = 0;

/*
 * Osetreni preruseni od citace zpozdeni
 */
ISR(TIMER0_COMP_vect)
{
  timer0++;
}

/**
  Timer 0 - delay in miliseconds
*/
void timer0_delay_us(unsigned int delay)
{
  TCNT0 = 0;
  OCR0 = 200;
  TIMSK |= 0x02;    /* povoleni preruseni pri OC */
  TCCR0 |= 0x09;     /* CTC mode, delicka 1x */
  
  while (timer0 < delay);
  
  timer0_stop();     /*  stop timer0 */
}

/**
  Timer 0 - delay in microseconds
*/
void timer0_delay_ms(unsigned int delay)
{
  
}

/**
  Timer 1 - delay in miliseconds
*/
void timer1_delay_ms(unsigned int delay)
{
  
}
