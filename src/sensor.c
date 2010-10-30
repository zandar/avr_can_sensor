#include "../include/sensor.h"
#include "../include/avr_sja1000p.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

#define treshold(id)              ((id >> TRESHOLD) & 0xff)
#define channel_avrg(id,channel)  ((id >> channel) & 0x07)
#define delivery(id)              ((id >> DELIVERY) & 0x01)

struct sensor_cfg sen_cfg;
struct sensor_data sen_data;

static struct canmsg_t idn_msg;

static unsigned char shift(unsigned char channel)
{
  switch (channel) {
    case 0:
      return CHANNEL_0;
    case 1:
      return CHANNEL_1;
    case 2:
      return CHANNEL_2;
      
    default:
      return 0;
  }
}

static unsigned char averaging(unsigned char avrg)
{
  switch (avrg) {
    case 0:
      return AVRG_0;
      
    case 1:
      return AVRG_1;
      
    case 2:
      return AVRG_4;
      
    case 3:
      return AVRG_8;
    
    case 4:
      return AVRG_16;
      
    case 5:
      return AVRG_32;
      
    case 6:
      return AVRG_64;
      
    case 7:
      return AVRG_128;
      
    default:
      return 0;
  }
}

ISR(ADC_vect)
{
  
}

char sensor_init()
{
  unsigned char i = 0;
  
  char *idn = SENSOR_IDN;
  
  /* prepaire message for IDN request */
  idn_msg.flags = MSG_EXT;
  idn_msg.id = SENSOR_ID;
  idn_msg.length = strlen(idn);
  
  for (;i < strlen(idn);i++) {
    idn_msg.data[i] = idn[i];
  }
  
  //TODO nastavit ADC na defaultni hodnoty
 
 return 0;
  
}

char sensor_config(struct canmsg_t *rx_msg)
{
  /* if recived message is IDN request, send idn_msg */
  if (rx_msg->id == IDN_RQ) {  
    if (sja1000p_pre_write_config(&idn_msg))
      return -1;
    
    sja1000p_send_msg();
  }
  else {
    /* if recived message is ADC configuration */
    decode_msg(rx_msg);
    
    //TODO nastavit adc podle parametru v sensor_cfg 
    
    
  }
  
  return 0;
}

void decode_msg(struct canmsg_t *rx_msg)
{
  unsigned char i = 0;
  
  sen_cfg.treshold = treshold(rx_msg->id);
  
  for (;i < 3;i++) {
    sen_cfg.channel_avrg[i] = channel_avrg(rx_msg->id,shift(i));
  }
  
  sen_cfg.delivery = delivery(rx_msg->id); 
}
