/* sja_control.h
 * Library for AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 16/10/2010
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

/* SJA1000 - AVR connection definitions, Intel mode */
#define SJA_BUS_PORT  PORTC   /* AVR port for SJA data/address lines */
#define SJA_BUS_DIR   DDRC    /* data direction register */
#define SJA_CTRL_PORT PORTA   /* AVR port for SJA control lines */
#define SJA_CTRL_DIR  DDRA    /* data direction register */
#define SJA_INT_PORT  PORTB   /* AVR port for SJA interrupt line */

#define SJA_ALE_PIN 7   /* AVR pin for SJA ALE pin */
#define SJA_CS_PIN  6   /* AVR pin for SJA CS pin */
#define SJA_RD_PIN  5   /* AVR pin for SJA RD pin */
#define SJA_WR_PIN  4   /* AVR pin for SJA WR pin */
#define SJA_INT_PIN 2   /* AVR pin for SJA INT pin */
#define SJA_INT_BIT 5   /* Bit in AVR GICR register to control interrupt */

#define BUS_OUT    0xff   /* AVR pin for SJA ALE pin */
#define BUS_IN     0x00   /* AVR pin for SJA ALE pin */
#define CTRL_OUT  (1 << SJA_ALE_PIN)|(1 << SJA_CS_PIN)\ 
                  |(1 << SJA_RD_PIN)|(1 << SJA_WR_PIN)

/* AVR interrupt from SJA disabled */
void can_disable_irq();
