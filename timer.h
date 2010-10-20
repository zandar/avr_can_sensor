/*
 Title	:   ATmega16 timer control
 Author:    Michal Vokac
 File:	    timer.h
 Version:   1.0
 Date:      2010/10/13
*/

#ifndef TIMER_H
#define TIMER_H

void timer0_delay_ms(unsigned int delay);
void timer0_delay_us(unsigned int delay);
void timer1_delay_ms(unsigned int delay);

#endif