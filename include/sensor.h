#ifndef SENSOR_H
#define SENSOR_H

#define LAST_ADC  2
#define FIRST_ADC 0
#define ADC_VREF_TYPE 0x60

#define SENSOR_ID   (1 << ID)
#define IDN_RQ      (0xff << ID)
#define SENSOR_IDN  "vokacmic"

#define TRESHOLD_MAX 255
#define TRESHOLD_MIN 0

#include "../include/avr_can.h"
#include "../include/fsm.h"

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
  unsigned char delivery; /* shot = 1, continual = 0 */
  unsigned char channel_avrg[3]; /* 0 = no data, 1 = no avrg. else = avrg. samples number */
  unsigned char treshold; /* channel 0 treshold signalization */
};

struct sensor_data {
  unsigned char data[3];
  unsigned char overflow;
};

char sensor_config(struct canmsg_t *rx_msg, struct fsm *fsm);
char sensor_init();

void fsm_sensor_init(struct fsm *fsm, enum event event);


#endif

