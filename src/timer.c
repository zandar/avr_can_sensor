/*
 Title	:   ATmega16 timer control
 Author:    Michal Vokac
 File:	    timer.c
 Version:   1.0
 Date:      2010/10/13       
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/timer.h"
#include "../include/def.h"

volatile timer timer_msec = 0, timer_usec = 0;

/*
 * Osetreni preruseni od citace zpozdeni
 */
ISR(TIMER0_COMP_vect)
{
  /* increment msec @1kHz */
    ++timer_msec;
}



void timer0_init_1khz()
{
  TCNT0 = 0;
  OCR0 = 124;
  TIMSK |= 0x02;    /* interrupt enable when OC */
  TCCR0 |= 0x0B;     /* CTC mode, delicka 64x, normal port op. */
}

// void timer2_init_100khz()
// {
//   TCNT2 = 0;
//   OCR2 = 79;
//   TIMSK |= 0x80;    /* interrupt enable when OC */
//   TCCR2 |= 0x09;    /* CTC mode, delicka 1x, normal port op. */
// }