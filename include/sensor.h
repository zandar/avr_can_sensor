/* sensor.h
 * Header file for AVR CAN sensor using AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version AVR CAN sensor 13/10/2010
 */

/*
 * This file is part of avr_can_sensor.
 *
 * Avr_can_sensor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Avr_can_sensor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with avr_can_sensor.  If not, see <http://www.gnu.org/licenses/>.
 */

/*@{*/

/**
 @defgroup can_sensor CAN sensor

 @brief Defines and structures for setting sensor measurement parameters.

 @author Michal Vokac vokac.m@gmail.com http:destruktor.borec.cz

*/

#ifndef _SENSOR_H
#define _SENSOR_H

#include "../include/avr_can.h"
#include "../include/fsm.h"

/**
  @name Defines of ADC setting
*/
#define ADC_PORT      PORTA /* define port register with ADC input */
#define ADC_DIRECTION DDRA  /* define port direction register with ADC input */

#define ADC_CHANNEL_0 0 /* define ADC input number */
#define ADC_CHANNEL_1 1 /* define ADC input number */
#define ADC_CHANNEL_2 2 /* define ADC input number */

#define ADC_VREF_TYPE 0x60  /* define ADC voltage reference type */

/**
  @name Defines of sensor setting
*/
#define MY_ID 1   /* my sensor ID, possible values are 2^N, N <0,8>*/
#define IDN_RQ 0xff /* IDN request value */
#define SENSOR_IDN  "vokacmic"  /* my identification string */
#define MEASUREMENT_PERIOD 1000 /* define min. time between two measurements in ms. */


/**
 @brief Enum with bit positions of particular parameters in recived msg. identificator
*/
enum sensor_id_cfg {
  SHIFT_ID = 21,
  SHIFT_TRESHOLD = 13,
  SHIFT_TRESHOLD_CHANNEL = 11,
  SHIFT_CHANNEL_0 = 8,
  SHIFT_CHANNEL_1 = 5,
  SHIFT_CHANNEL_2 = 2,
  SHIFT_DELIVERY = 1,
  SHIFT_OVF = 0
};

/**
 @brief Enum with channel averaging values
*/
enum averaging {
  AVRG_0 = 0,
  AVRG_1 = 1,
  AVRG_4 = 4,
  AVRG_8 = 8,
  AVRG_16 = 16,
  AVRG_32 = 32,
  AVRG_64 = 64,
  AVRG_128 = 128
};

/**
 @brief Structure with sensor configuration parameters extracted from recived msg. identificator
*/
struct sensor_cfg {
  unsigned char delivery; /* defines delivery on measured channels, shot = 0, continual = 1 */
  unsigned char samples[3]; /* defines number of samples to be measured on particular chanel before averaging, 0 = no measurement, 1 = measurement,no avrg., else = avrg. samples number */
  unsigned char treshold; /* deifnes treshold value */
  unsigned char treshold_channel; /* defines channel number for overflow signalization */
  unsigned char rx_msg_id[4];  /* field for storing ID of Rx message */
};

/**
 @brief Structure with sensor captured data
*/
struct sensor_data {
  unsigned char channel_data[3];  /* field with measured/averaged samples on particular channels */
  unsigned char overflow; /* overflow signalization if required by rx config. */
};

/**
 @name Functions
*/

/**
 @brief Function for setting sensor measurement parameters
 @param rx_msg new message with sensor configuration
 @param fsm structure with FSM parameters
 @return Negative value reports error

 If new message is recived, sensor_config is executed.

 If Rx message ID is IDN request, MY_ID and SENSOR_IDN is transmited.

 If Rx message ID is MY_ID, decode sensor configuration from message.

*/
char sensor_config(struct canmsg_t *rx_msg, struct fsm *fsm);

/**
 @brief Entry point to sensor FSM
*/
void fsm_sensor_init(struct fsm *fsm, enum event event);

#endif

/*@}*/