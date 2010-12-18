/* sja_control.c
 * AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 16/10/2010
 */

/*@{*/

#include <avr/io.h>
#include "../include/F_CPU.h"
#include <util/delay.h>
#include "../include/sja_control.h"

/**
 * Macros for low level hw. control
 */
/* set bus lines in DDR register as output */
#define sja_bus_out   SJA_BUS_DIR = 0xff
/* set bus lines in DDR register as input */
#define sja_bus_in    SJA_BUS_PORT = 0xff,SJA_BUS_DIR = 0x00
/* set control lines in DDR register as output */
#define sja_ctrl_out  SJA_CTRL_DIR |= (1 << SJA_ALE_PIN)|(1 << SJA_CS_PIN)\
                      |(1 << SJA_RD_PIN)|(1 << SJA_WR_PIN)
/* initialize AVR external interrupt from SJA chip */
#define sja_int_init  SJA_INT_DIR &= ~(1 << SJA_INT_PIN)\
                      ,SJA_INT_PORT |= (1 << SJA_INT_PIN)\
                      ,MCUCR |= (SJA_INT_ISC_BIT << SJA_INTERRUPT)
/* writes data value to SJA bus */
#define sja_bus_write(data) sja_bus_out,SJA_BUS_PORT = data
/* returns data value on SJA bus */
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

/**
 * sja_init_ports: - set AVR ports to initial state
 */
void sja_init_ports()
{
  sja_bus_out;
  sja_ctrl_out;
  sja_int_init;
  sja_enable_irq;
}

/**
 * sja_write_address: - sequence for adressing appropriate register in SJA chip
 * @address: address of the sja1000 chip register to be written to the bus
 */
static void sja_write_address(unsigned char address)
{
  sja_ale_low;
  sja_rd_high;
  sja_wr_high;
  sja_cs_high;
  sja_ale_high;
  sja_bus_write(address);
  _delay_us(1);
  sja_ale_low;
  _delay_us(1);
}

/**
 * sja_read_register: - sequence for reading one byte from SJA register
 * @address: address of the sja1000 chip register to be read
 *
 * This function performes address write using sja_write_address and then reads
 * value from the register on data bus.
 *
 * Return value: value from the addressed register
 */
static unsigned char sja_read_register(unsigned char address)
{
  unsigned char data;
  
  sja_write_address(address);
  sja_cs_low;
  sja_rd_low;
  sja_bus_in;
  _delay_us(1);
  data = SJA_BUS_DATA;
  sja_rd_high;
  sja_cs_high;
  
  return data;
}

/**
 * sja_write_register: - sequence for writing one byte to SJA register
 * @data: value to be written to the register
 * @address: address of the register where write the data in
 */
static void sja_write_register(unsigned char data,unsigned char address)
{
  sja_write_address(address);
  sja_cs_low;
  sja_wr_low;
  sja_bus_write(data);
  _delay_us(1);
  sja_wr_high;
  sja_cs_high;
  _delay_us(1);
  
}

/**
 * can_read_reg: - public method for reading from SJA chip
 * @address: address of the sja1000 chip register to be read
 *
 * Return value: value from the addressed register 
 */
unsigned char can_read_reg(unsigned char address)
{ 
  return sja_read_register(address);
}

/**
 * Public method for writing to SJA chip
 * @data: value to be written to the register
 * @address: address of the register where write the data in
 */
void can_write_reg(unsigned char data,unsigned char address)
{
  sja_write_register(data,address);
}

/**
 * can_enable_irq: - enable AVR interrupt from SJA chip
 */
void can_enable_irq()
{
  sja_enable_irq;
}

/**
 * can_disable_irq: - disable AVR interrupt from SJA chip
 */
void can_disable_irq()
{
  sja_disable_irq;
}

/*@}*/
