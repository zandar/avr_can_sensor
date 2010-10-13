/*
 * avr_can.c	v.1.0
 *
 * Program pro ovladani radice CAN 
 *
 * Michal Vokac 11.10.2010
 *
 */

#include <avr/io.h>
#define F_CPU 8000000UL  //  8MHz - takt MCU
#include <avr/interrupt.h>
#include "lcd.h"
#include "timer.h"
#include "sja_control.h"



//volatile unsigned char adc_data[LAST_ADC - FIRST_ADC + 1];
//volatile unsigned char int_wait = 0;

/*
 * Osetreni externiho preruseni od IR cidla SFH
 */
ISR(INT0_vect)
{
 
}

/*
 * Osetreni preruseni od citace zpozdeni
 */
ISR(TIMER0_OVF_vect)
{

}

/*
 * Osetreni preruseni od ADC
 */
// ISR(ADC_vect)
// {
//   static unsigned char channel = 0;
//   unsigned int i = 0;
//   
//   adc_data[channel] = ADCH;
//   
//   Select next ADC input
//   if (++channel > (LAST_ADC - FIRST_ADC))
//     channel = 0;
//   
//   ADMUX = (FIRST_ADC | (ADC_VREF_TYPE & 0xff)) + channel;
//   
//   while(i++ < 30000);
//   ADCSRA |= 0x40;
// } 

/*
 * Metoda pro nastaveni citace pro generovani zpozdeni
 */
// void motor_timer_start()
// { 
//   TIMSK |= 0x01;        // preruseni preteceni casovace 0 povoleno
//   TCCR0 = 0x05;        // preddelicka 1024
//   TCNT0 = 0x00;
// }

/*
 * Metoda pro nastaveni citace pro generovani zpozdeni
 */
// void led_pwm_init()
// { 
//   TCCR2 = 0x19;        // CTC mode, delicka 1x
//   OCR2 = 110;           // generuje 36kHz
//   TCNT2 = 0x00;
// }

// void adc_init()
// {
//   ADCSRA = 0xCB;
//   ADMUX = 0x60;   //  Vref = Vcc
// }

void input_output_init()
{
  DDRD = 0xFF;    // LCD display

}

// void int_init()
// {
//   MCUCR |= 0x02;  // preruseni int0 na spadovou hranu
// }

/*
 *  MAIN
 */
int main(void)
{
  input_output_init();
  lcd_init(LCD_DISP_ON);
  sja_init();
  
  sei();			// globalni povoleni preruseni
  
  lcd_puts_line(0,"CAN senzor");    
  
  while(1) {
    
  
  }
  
  return 0;
}
