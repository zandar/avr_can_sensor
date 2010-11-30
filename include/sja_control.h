/* sja_control.h
 * Library for AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 16/10/2010
 */

/**
 @defgroup sja_control SJA1000 control library
 
 @brief Macros and low-level functions for controling SJA chip.
       
 @author Michal Vokac vokac.m@gmail.com http://destruktor.borec.cz

*/

/*@{*/

#ifndef _SJA_CONTROL_H
#define _SJA_CONTROL_H

/**
  @name AVR port definitions

  SJA1000 <-> AVR connection definitions, Intel mode configuration.
  Edit what specific in your app.
*/
#define SJA_BUS_PORT  PORTC   /**< AVR port for SJA bus (data/address) lines */
#define SJA_BUS_DIR   DDRC    /**< AVR data direction register for bus lines*/
#define SJA_BUS_DATA  PINC    /**< AVR input register for bus lines */
#define SJA_CTRL_PORT PORTA   /**< AVR port for SJA control lines */
#define SJA_CTRL_DIR  DDRA    /**< AVR data direction register for control lines */
#define SJA_INT_PORT  PORTD   /**< AVR port for SJA interrupt line */
#define SJA_INT_DIR   DDRD    /**< AVR data direction register for SJA interrupt line */

#define SJA_ALE_PIN 7   /**< AVR pin for SJA ALE pin */
#define SJA_CS_PIN  6   /**< AVR pin for SJA CS pin */
#define SJA_RD_PIN  5   /**< AVR pin for SJA RD pin */
#define SJA_WR_PIN  4   /**< AVR pin for SJA WR pin */
#define SJA_INT_PIN 2   /**< AVR pin for SJA INT pin */
#define SJA_INT_BIT 6   /**< Bit in AVR GICR register for interrupt control */

#define SJA_INT_ISC_BIT 0   /**< Defines first bit in MCUCR register for interrupt sense control */

#define SJA_INTERRUPT SJA_INT_LOW  /**< Defines type of interrupt */

#define SJA_INT_LOW     0   /**< low level on int. pin generates interrupt */
#define SJA_INT_CHANGE  1   /**< level change on int. pin generates interrupt */
#define SJA_INT_FALL    2   /**< falling edge on int. pin generates interrupt */
#define SJA_INT_RISE    3   /**< rising edge on int. pin generates interrupt */

/**
  @name SJA chip definitions

  SJA1000 chip specific parameters.
*/
#define SJA_CLOCK   16000000  /**< SJA clock frequency in Hz */
#define SJA_BAUD    1000000  /**< SJA required baudrate in Hz*/
#define SJA_CDR_REG 0
#define SJA_OCR_REG 0

/**
  @name Functions
*/

/**
  @brief Read SJA chip register
  @param address Addres of the register to be read.
  @return Returns value from the register.

  Reads value from the specified register of the SJA1000 chip.
*/
unsigned char can_read_reg(unsigned char address);

/**
  @brief Writes SJA chipregister
  @param data Data to be write in the register.
  @param address Addres of the register to be written.

  Writes data in the specified register of the SJA1000 chip.
*/
void can_write_reg(unsigned char data, unsigned char address);

/**
  @name Enable AVR interrupt

  Enables external interrupt from SJA1000 chip.
*/
void can_enable_irq(void);

/**
  @name Disable AVR interrupt

  Disables external interrupt from SJA1000 chip.
*/
void can_disable_irq(void);

/**
  @name Init AVR port

  Initialize AVR ports with specific SJA functions.
*/
void sja_init_ports(void);

#endif
