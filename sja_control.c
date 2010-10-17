/* sja_control.c
 * AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version avrCAN-0.1 16/10/2010
 */

#include <avr/io.h>
#include <util/delay.h>

#ifndef _SJA_CONTROL_H
#define _SJA_CONTROL_H
#include "sja_control.h"
#endif

// #include "../include/can.h"
// #include "../include/can_sysdep.h"
// #include "../include/main.h"
// #include "../include/sja1000.h"

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

static unsigned char sja_read_register(unsigned char address)
{
  unsigned char data;
  
  sja_write_address(address);
  sja_cs_low;
  sja_rd_low;
  sja_bus_read;
  _delay_us(1);
  data = SJA_BUS_DATA;
  sja_rd_high;
  sja_cs_high;
  
  return data;
}

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

unsigned char can_read_reg(unsigned char address)
{ 
  return sja_read_register(address);
}

void can_write_reg(unsigned char data,unsigned char address)
{
  sja_write_register(data,address);
}

void can_enable_irq()
{
  sja_enable_irq;
}

void can_disable_irq()
{
  sja_disable_irq;
}


