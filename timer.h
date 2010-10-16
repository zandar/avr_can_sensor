/**
 Title	:   ATmega16 timer control
 Author:    Michal Vokac
 File:	    timer.h
 Version:   1.0
 Date:      2010/10/13
*/

#ifndef F_CPU
#define F_CPU 8000000 /** clock frequency in Hz*/
#endif

void timer0_delay_ms(unsigned int delay);
void timer0_delay_us(unsigned int delay);
void timer1_delay_ms(unsigned int delay);
