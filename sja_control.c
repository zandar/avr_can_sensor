/* sja_control.c
 * AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 16/10/2010
 */

#include <avr/io.h>
#include "sja_control.h"
#include <util/delay.h>

// #include "../include/can.h"
// #include "../include/can_sysdep.h"
// #include "../include/main.h"
// #include "../include/sja1000.h"

/* Macros low level hw. control */
#define sja_bus_out()     SJA_BUS_DIR = BUSOUT
#define sja_bus_in()      SJA_BUS_DIR = BUSIN
#define sja_control_out() SJA_CTRL_DIR |= 0xF0

#define sja_ale_high() SJA_CTRL_PORT |= (1 << SJA_ALE_PIN)
#define sja_ale_low()  SJA_CTRL_PORT &= ~(1 << SJA_ALE_PIN)
#define sja_cs_high()  SJA_CTRL_PORT |= (1 << SJA_CS_PIN)
#define sja_cs_low()   SJA_CTRL_PORT &= ~(1 << SJA_CS_PIN)
#define sja_rd_high()  SJA_CTRL_PORT |= (1 << SJA_RD_PIN)
#define sja_rd_low()   SJA_CTRL_PORT &= ~(1 << SJA_RD_PIN)
#define sja_wr_high()  SJA_CTRL_PORT |= (1 << SJA_WR_PIN)
#define sja_wr_low()   SJA_CTRL_PORT &= ~(1 << SJA_WR_PIN)

#define can_disable_irq() GICR &= ~(1 << SJA_INT_BIT)
#define can_enable_irq()  GICR |= (1 << SJA_INT_BIT)

unsigned char can_read_reg(unsigned char address)
{
  sja_rd_high();
  sja_wr_high();
  sja_cs_high();
  
  sja_ale_high();
  
  SJA_BUS_DIR = BUS_OUT;
  SJA_BUS_PORT = address;
  _delay_us(1);
  sja_ale_low();
  _delay_us(1);
  
  
  return chip->read_register(address_to_read);
}




