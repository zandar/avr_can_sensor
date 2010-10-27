#ifndef SENSOR_H
#define SENSOR_H

#define adc_set_channel
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2

#define SAMPLES_MAX 100
#define SAMPLES_MIN 1

#define TRESHOLD_MAX 255
#define TRESHOLD_MIN 0

#define ADC_CONTINUAL 1000 /* delay in ms between sending messages with samples */

struct sensor {
  unsigned char channnel; /* */
  unsigned char delivery; /* odmer */
  unsigned char averaging[3];
  unsigned char samples[3];
};

#endif

