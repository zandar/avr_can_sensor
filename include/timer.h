/* timer.h
 * AVR ATmega16 timer control
 */

#ifndef TIMER_H
#define TIMER_H

void timer0_init_1khz();

/* TODO dont use delay_ms in interrupt,fce is not reentrant!!! */
void delay_ms(unsigned int delay);

#endif