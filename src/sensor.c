#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <stdbool.h>
#include "../include/F_CPU.h"
#include <util/delay.h>

#include "../include/sensor.h"
#include "../include/avr_sja1000p.h"
#include "../include/fsm.h"
#include "../include/timer.h"
#include "../include/display.h"

#define treshold(id)              ((id >> TRESHOLD) & 0xff)
#define channel_avrg(id,channel)  ((id >> channel) & 0x07)
#define delivery(id)              ((id >> DELIVERY) & 0x01)

#define adc_start   ADCSRA |= (1 << 7)
#define adc_stop    ADCSRA &= ~(1 << 7)

#define DEBUG

static struct sensor_cfg sen_cfg;
static struct sensor_data sen_data;
static struct canmsg_t idn_msg;
static struct canmsg_t tx_msg;

volatile unsigned char adc_data[3];

static void wait_for_cmd(struct fsm *fsm, enum event event);
static void sensor_capture_data(struct fsm *fsm, enum event event);
//static void sensor_send_data(struct fsm *fsm, enum event event);

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
  unsigned char channel = 0;
  
  adc_data[channel] = ADCH;
  
  // Select next ADC input
  if (++channel > (LAST_ADC - FIRST_ADC))
    channel = 0;
  
  ADMUX = (FIRST_ADC | (ADC_VREF_TYPE & 0xff)) + channel;
  
  _delay_us(10);
  
  adc_start;
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
  
  /* initialize sen_cfg structure with default values
   * channel 0 - 2 enabled, no averaging, continual delivery
   * no treshold */
  for (i = 0;i < 3; i++) {
    sen_cfg.channel_avrg[i] = 1;
  }
  
  sen_cfg.delivery = 1;
  sen_cfg.treshold = 0;
  
  ADCSRA = 0x88;  /* ADC enabled. interrupt enabled, no prescaler */
  ADMUX = 0x60;   /*  Vref = Vcc, ADC output in ADCH */
  
  adc_start;
  
#ifdef DEBUG
  CANMSG("Sensor init OK");
  _delay_ms(1000);
#endif

  return 0;
}

char sensor_config(struct canmsg_t *rx_msg, struct fsm *fsm)
{
  unsigned char i = 0;
  
  /* if recived message is IDN request, send idn_msg */
  if (rx_msg->id == IDN_RQ) {  
    if (sja1000p_pre_write_config(&idn_msg))
      return -1;
    
    sja1000p_send_msg();
    
#ifdef DEBUG
    CANMSG("IDN? answer OK");
    _delay_ms(1000);
#endif
  }
  else {
    /* if recived message is ADC configuration */
    sen_cfg.treshold = treshold(rx_msg->id);
    
    for (;i < 3;i++) {
      sen_cfg.channel_avrg[i] = channel_avrg(rx_msg->id,shift(i));
    }
    
    sen_cfg.delivery = delivery(rx_msg->id);
    
    fsm->measurement_start = true;
  }
  
#ifdef DEBUG
  CANMSG("Sensor config OK");
  _delay_ms(1000);
#endif
  
  return 0;
}


static void save_samples(struct fsm *fsm)
{
   static timer adc_time = 0;
   static int counter = 0;

  if(timer0_msec >= adc_time + 1000)   // save ADC sample every ADC_PERIOD us
  {
    adc_time = timer0_msec;
    debug(1,counter++);
    
//     if(counter == 0)  // when first sample, time = 0
//       time_base = 0;
//     else
//       time_base = timer_usec - adc_time;  // else time = time betwen samples
//     
//     adc_time = timer_usec;
//     
//     fsm->adc_data[ADC_DATA][counter] = adc_val[0];
//     fsm->adc_data[ADC_TIME][counter] = time_base;
//     counter++;
//     
    if(counter >= 10)
    {
      counter = 0;
      fsm->send_samples = true;
    }
   }
}

void fsm_sensor_init(struct fsm *fsm, enum event event)
{
  switch (event) {
  case EVENT_ENTRY:
    break;
  case EVENT_DO:
    fsm->measurement_start = true;
    fsm->send_samples = false;
    fsm->sensor_ready = true;
    fsm->current_state = wait_for_cmd;
    break;
  case EVENT_EXIT:
#ifdef DEBUG    
    CANMSG("FSM init OK");
    _delay_ms(1000);
#endif
    break;
  }
}

static void wait_for_cmd(struct fsm *fsm, enum event event)
{  
  switch (event) {
  case EVENT_ENTRY:
    break;
  case EVENT_DO:
#ifdef DEBUG
    CANMSG("FSM wait for cmd");
    _delay_ms(1000);
#endif
    /* waiting to start adc sampling */
    if (fsm->measurement_start && (sen_cfg.channel_avrg[0]
      || sen_cfg.channel_avrg[1] || sen_cfg.channel_avrg[2]) )
       fsm->current_state = sensor_capture_data;
    break;
  case EVENT_EXIT:
    fsm->sensor_ready = false;
    fsm->measurement_start = sen_cfg.delivery;
    break;
  }
}

static void sensor_capture_data(struct fsm *fsm, enum event event)
{
  switch (event) {
  case EVENT_ENTRY:
#ifdef DEBUG
    CANMSG("FSM capture data");
    _delay_ms(1000);
#endif
    break;
  case EVENT_DO:
    save_samples(fsm);
    
    if (fsm->send_samples)
      fsm->current_state = wait_for_cmd;
    break;
  case EVENT_EXIT:
    fsm->measurement_start = false;
    break;
  }
}
