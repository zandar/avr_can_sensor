/**
 Title	:   ATmega16 timer control
 Author:    Michal Vokac
 File:	    timer.h
 Version:   1.0
 Date:      2010/10/13
*/

#include <avr/io.h>

#define C_CPU 8000000 /** clock frequency in Hz*/

void time0_ms(unsigned int delay);
void time0_us(unsigned int delay);
void time1_ms(unsigned int delay);
