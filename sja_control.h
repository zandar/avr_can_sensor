/**
 Title  :   SJA1000 with ATmega16 control library
 Author:    Michal Vokac
 File:      sja_control.h
 Version:   1.0
 Date:      2010/10/13       
*/

#define SJA_BUS_PORT PORTC
#define SJA_BUS_DIRECTION DDRC
#define SJA_CONTROL_PORT PORTA
#define SJA_CONTROL_DIRECTION DDRA

#define SJA_ALE_PIN 7
#define SJA_CS_PIN 6
#define SJA_RD_PIN 5
#define SJA_WR_PIN 4


char sja_init();