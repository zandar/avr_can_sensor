#ifndef SENSOR_H
#define SENSOR_H

#define SENSOR_ID   (1 << ID)
#define IDN_RQ      (0xff << ID)
#define SENSOR_IDN  "vokacmic"

#define TRESHOLD_MAX 255
#define TRESHOLD_MIN 0

#define ADC_CONTINUAL 1000 /* delay in ms between sending messages with samples */

#include "../include/avr_can.h"

enum sensor_id_cfg {
  ID = 21,
  TRESHOLD = 13,
  CHANNEL_0 = 10,
  CHANNEL_1 = 7,
  CHANNEL_2 = 2,
  DELIVERY = 6  
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
  unsigned char delivery; /* shot & continual */
  unsigned char channel_avrg[3]; /* */
  unsigned char samples[3];
  unsigned char treshold;
};

struct sensor_data {
  unsigned char data[3];
  unsigned char overflow;
};

char sensor_config(struct canmsg_t *rx_msg);
void decode_msg(struct canmsg_t *rx_msg);
char sensor_init();

#endif

