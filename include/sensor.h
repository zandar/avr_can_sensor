#ifndef SENSOR_H
#define SENSOR_H

#define ADC_PORT      PORTA
#define ADC_DIRECTION DDRA

#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2

#define LAST_ADC  2
#define FIRST_ADC 0
#define ADC_VREF_TYPE 0x60

#define SENSOR_ID   (1 << SHIFT_ID)
#define IDN_RQ      (0xff << SHIFT_ID)
#define SENSOR_IDN  "vokacmic"

#define TRESHOLD_MAX 255
#define TRESHOLD_MIN 0

#define SAMPLING_FREQUENCY 10 /* adc sampling frequency in kHz*/
#define SAMPLING_PERIOD (1000 / SAMPLING_FREQUENCY)/* adc sampling period in us */

#include "../include/avr_can.h"
#include "../include/fsm.h"

enum sensor_id_cfg {
  SHIFT_ID = 21,
  SHIFT_TRESHOLD = 13,
  SHIFT_TRESHOLD_CHANNEL = 11,
  SHIFT_CHANNEL_0 = 8,
  SHIFT_CHANNEL_1 = 5,
  SHIFT_CHANNEL_2 = 2,
  SHIFT_DELIVERY = 1  
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
};

struct sensor_data {
  unsigned char channel_data[3];
  unsigned char overflow;
};

char sensor_config(struct canmsg_t *rx_msg, struct fsm *fsm);
char sensor_init();

void fsm_sensor_init(struct fsm *fsm, enum event event);


#endif

