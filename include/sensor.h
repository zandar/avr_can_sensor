/* sensor.h
 * Header file for AVR CAN sensor using AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version AVR CAN sensor 13/10/2010
 */

#ifndef SENSOR_H
#define SENSOR_H

#define ADC_PORT      PORTA
#define ADC_DIRECTION DDRA

#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2

#define ADC_VREF_TYPE 0x60

#define MY_ID 1l /* my sensor id, 2^N possible values, N <0,8>*/
#define SENSOR_ID   (MY_ID << SHIFT_ID)
#define SENSOR_MASK (MY_ID << 24)
#define IDN 0xffl
#define IDN_RQ      (IDN << SHIFT_ID)  /* 0x1FE00000 */
#define SENSOR_IDN  "vokacmic"

#include "../include/avr_can.h"
#include "../include/fsm.h"

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

struct sensor_cfg {
  unsigned char delivery; /* shot = 1, continual = 0 */
  unsigned char samples[3]; /* 0 = no measurement, 1 = measurement,no avrg., else = avrg. samples number */
  unsigned char treshold; /* channel number with treshold signalization */
  unsigned char treshold_channel;
  unsigned long rx_msg_id;
};

struct sensor_data {
  unsigned char channel_data[3];
  unsigned char overflow;
};

char sensor_config(struct canmsg_t *rx_msg, struct fsm *fsm);
void fsm_sensor_init(struct fsm *fsm, enum event event);


#endif

