/*
 Title	:   ATmega16 timer control
 Author:    Michal Vokac
 File:	    timer.h
 Version:   1.0
 Date:      2010/10/13
*/

#ifndef TIMER_H
#define TIMER_H

#include "../include/def.h"

extern volatile timer timer0_msec;

void timer0_init_1khz();

#endif