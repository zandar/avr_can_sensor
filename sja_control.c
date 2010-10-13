/**
 Title  :   SJA1000 with ATmega16 control library
 Author:    Michal Vokac
 File:      sja_control.c
 Version:   1.0
 Date:      2010/10/13       
*/

#include <avr/io.h>
#include "sja_control.h"
#include "timer.h"

#define sja_bus_out() SJA_BUS_DIRECTION = 0xFF
#define sja_bus_in() SJA_BUS_DIRECTION = 0x00
#define sja_control_out() SJA_CONTROL_DIRECTION |= 0xF0

#define sja_ale_high() SJA_CONTROL_PORT |= (1 << SJA_ALE_PIN)
#define sja_ale_low()  SJA_CONTROL_PORT &= ~(1 << SJA_ALE_PIN)
#define sja_cs_high()  SJA_CONTROL_PORT |= (1 << SJA_CS_PIN)
#define sja_cs_low()   SJA_CONTROL_PORT |= (1 << SJA_CS_PIN)
#define sja_rd_high()  SJA_CONTROL_PORT |= (1 << SJA_RD_PIN)
#define sja_rd_low()   SJA_CONTROL_PORT |= (1 << SJA_RD_PIN)
#define sja_wr_high()  SJA_CONTROL_PORT |= (1 << SJA_WR_PIN)
#define sja_wr_low()   SJA_CONTROL_PORT |= (1 << SJA_WR_PIN)

/**
  SJA controler initialization
*/
char sja_init()
{
  sja_control_out();
  
  return 0;

}
