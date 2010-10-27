#include "../include/sensor.h"
#include "../include/avr_sja1000p.h"
#include <avr/interrupt.h>

void ISR(ADC_vect)
{
  
}


char sensor_config(struct sensor *cfg)
{
  // nakonfigurovat nastaveni ADC ap. podle pozadavku z CAN msg.
}
