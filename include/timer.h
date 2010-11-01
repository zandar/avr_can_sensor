/* timer.h
 * AVR ATmega16 timer control
 */

#ifndef TIMER_H
#define TIMER_H

void timer0_init_1khz();
void delay_ms(unsigned int delay);

#endif