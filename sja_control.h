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

/* SJA1000 - AVR connection definitions, Intel mode, edit what necessary */
#define SJA_BUS_PORT  PORTC   /* AVR port for SJA bus (data/address) lines */
#define SJA_BUS_DIR   DDRC    /* AVR data direction register for bus lines*/
#define SJA_BUS_DATA  PINC    /* AVR input register for bus lines */
#define SJA_CTRL_PORT PORTA   /* AVR port for SJA control lines */
#define SJA_CTRL_DIR  DDRA    /* AVR data direction register for control lines */
#define SJA_INT_PORT  PORTB   /* AVR port for SJA interrupt line */

#define SJA_ALE_PIN 7   /* AVR pin for SJA ALE pin */
#define SJA_CS_PIN  6   /* AVR pin for SJA CS pin */
#define SJA_RD_PIN  5   /* AVR pin for SJA RD pin */
#define SJA_WR_PIN  4   /* AVR pin for SJA WR pin */
#define SJA_INT_PIN 2   /* AVR pin for SJA INT pin */
#define SJA_INT_BIT 5   /* Bit in AVR GICR register for interrupt control */ 

/* Macros for low level hw. control */
#define sja_bus_out   SJA_BUS_DIR = 0xff   /* set bus lines in DDR register as output */
#define sja_bus_in    SJA_BUS_DIR = 0x00   /* set bus lines in DDR register as input */
#define sja_ctrl_out  SJA_CTRL_DIR |= (1 << SJA_ALE_PIN)|(1 << SJA_CS_PIN)|(1 << SJA_RD_PIN)|(1 << SJA_WR_PIN)  /* set control lines in DDR register as output */

#define sja_bus_write(data) sja_bus_out,SJA_BUS_PORT = data
#define sja_bus_read        SJA_BUS_DATA 

#define sja_ale_high SJA_CTRL_PORT |= (1 << SJA_ALE_PIN)  /* AVR pin to SJA ALE pin high */
#define sja_ale_low  SJA_CTRL_PORT &= ~(1 << SJA_ALE_PIN) /* AVR pin to SJA ALE pin low */
#define sja_cs_high  SJA_CTRL_PORT |= (1 << SJA_CS_PIN)   /* AVR pin to SJA CS pin high */
#define sja_cs_low   SJA_CTRL_PORT &= ~(1 << SJA_CS_PIN)  /* AVR pin to SJA CS pin low */
#define sja_rd_high  SJA_CTRL_PORT |= (1 << SJA_RD_PIN)   /* AVR pin to SJA RD pin high */
#define sja_rd_low   SJA_CTRL_PORT &= ~(1 << SJA_RD_PIN)  /* AVR pin to SJA RD pin low */
#define sja_wr_high  SJA_CTRL_PORT |= (1 << SJA_WR_PIN)   /* AVR pin to SJA WR pin high */
#define sja_wr_low   SJA_CTRL_PORT &= ~(1 << SJA_WR_PIN)  /* AVR pin to SJA WR pin low */

#define sja_disable_irq GICR &= ~(1 << SJA_INT_BIT) /* AVR interrupt from SJA disabled */
#define sja_enable_irq  GICR |= (1 << SJA_INT_BIT)  /* AVR interrupt from SJA enabled */

unsigned char can_read_reg(unsigned char address);
void can_write_reg(unsigned char data, unsigned char address);
void can_enable_irq(void);
void can_disable_irq(void);
