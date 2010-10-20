/* sja_control.h
 * Library for AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 16/10/2010
 */

#ifndef SJA_CONTROL_H
#define SJA_CONTROL_H

/* SJA1000 - AVR connection definitions, Intel mode, edit what necessary */
#define SJA_BUS_PORT  PORTC   /* AVR port for SJA bus (data/address) lines */
#define SJA_BUS_DIR   DDRC    /* AVR data direction register for bus lines*/
#define SJA_BUS_DATA  PINC    /* AVR input register for bus lines */
#define SJA_CTRL_PORT PORTA   /* AVR port for SJA control lines */
#define SJA_CTRL_DIR  DDRA    /* AVR data direction register for control lines */
#define SJA_INT_PORT  PORTB   /* AVR port for SJA interrupt line */
#define SJA_INT_DIR   DDRB    /* AVR data direction register for SJA interrupt line */

#define SJA_ALE_PIN 7   /* AVR pin for SJA ALE pin */
#define SJA_CS_PIN  6   /* AVR pin for SJA CS pin */
#define SJA_RD_PIN  5   /* AVR pin for SJA RD pin */
#define SJA_WR_PIN  4   /* AVR pin for SJA WR pin */
#define SJA_INT_PIN 2   /* AVR pin for SJA INT pin */
#define SJA_INT_BIT 5   /* Bit in AVR GICR register for interrupt control */

#define SJA_CLOCK   16000000  /* SJA clock frequency in Hz */
#define SJA_BAUD    1000000  /* SJA required baudrate in Hz*/
#define SJA_CDR_REG 0
#define SJA_OCR_REG 0

#define canobj_write_reg can_write_reg
#define canobj_read_reg can_read_reg

unsigned char can_read_reg(unsigned char address);
void can_write_reg(unsigned char data, unsigned char address);
void can_enable_irq(void);
void can_disable_irq(void);
void init_ports(void);

#endif
