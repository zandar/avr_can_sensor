#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <stdbool.h>
#include "../include/F_CPU.h"
#include <util/delay.h>

#include "../include/sensor.h"
#include "../include/avr_sja1000p.h"
#include "../include/sja_control.h"
#include "../include/fsm.h"
#include "../include/timer.h"
#include "../include/display.h"

#define adc_channels_in ADC_DIRECTION |= (1 << ADC_CHANNEL_0)|(1 << ADC_CHANNEL_1)|(1 << ADC_CHANNEL_2)

#define treshold(id)              ((id >> SHIFT_TRESHOLD) & 0xff)
#define treshold_channel(id)      ((id >> SHIFT_TRESHOLD_CHANNEL) & 0x03)
#define channel_avrg(id,channel)  ((id >> channel) & 0x07)
#define delivery(id)              ((id >> SHIFT_DELIVERY) & 0x01)

#define adc_start   ADCSRA |= (1 << 6)
#define adc_stop    ADCSRA &= ~(1 << 6)

#define DEBUG

static struct sensor_cfg sen_cfg, sen_cfg_lock;
static struct sensor_data sen_data;
static struct canmsg_t idn_msg;
static struct canmsg_t tx_msg;

volatile unsigned char adc_data[3];

static void wait_for_cmd(struct fsm *fsm, enum event event);
static void sensor_capture_data(struct fsm *fsm, enum event event);
static void sensor_send_data(struct fsm *fsm, enum event event);

static unsigned char shift(unsigned char channel)
{
  
  switch (channel) {
    case 0:
      return SHIFT_CHANNEL_0;
    case 1:
      return SHIFT_CHANNEL_1;
    case 2:
      return SHIFT_CHANNEL_2;
      
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
  static unsigned char channel = 0;
  
  adc_data[channel] = ADCH;
  
  // Select next ADC input
  if (++channel > (LAST_ADC - FIRST_ADC))
    channel = 0;
  
  ADMUX = (FIRST_ADC | (ADC_VREF_TYPE & 0xff)) + channel;
  
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
    sen_cfg.samples[i] = AVRG_1;
  }
  
  sen_cfg.delivery = 1;
  sen_cfg.treshold = 0;
  sen_cfg.treshold_channel = 0;
  
  ADCSRA = 0xCF;  /* ADC enabled. interrupt enabled, 128x prescaler ~4kHz */
  ADMUX = 0x60;   /*  Vref = Vcc, ADC output in ADCH */
  
  //adc_channels_in;
  adc_start;
  
#ifdef DEBUG
  CANMSG("Sensor init OK");
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
#endif
  }
  else {
    /* if recived message is ADC configuration */
    sen_cfg.treshold = treshold(rx_msg->id);
    sen_cfg.treshold_channel = treshold_channel(rx_msg->id);
    
    for (;i < 3;i++) {
      sen_cfg.samples[i] = averaging(channel_avrg(rx_msg->id,shift(i)));
    }
    
    sen_cfg.delivery = delivery(rx_msg->id);
    
    if (sen_cfg.samples[0] || sen_cfg.samples[1] || sen_cfg.samples[2])
      fsm->measurement_start = true;
    
#ifdef DEBUG
    CANMSG("Sensor config OK");
#endif
  }
  
  return 0;
}


static void save_samples()
{
  timer adc_time = 0;
  unsigned char counter = 0, i = 0;
  unsigned int average = 0;

  for (i = 0; i < (LAST_ADC - FIRST_ADC + 1); i++) {
    
    while (counter < sen_cfg_lock.samples[i]) {
      /* save sample every 1ms = 1kHz  */
      if (timer_msec >= adc_time + 1) {
        adc_time = timer_msec;
        average += adc_data[i];
        counter++;
      }
    }
    
    /* avoid dividing by zero */
    if (sen_cfg_lock.samples[i])
      sen_data.channel_data[i] = average / sen_cfg_lock.samples[i];
    
    counter = 0;
  }
  
  #ifdef DEBUG
    CANMSG("ADC data");
    
    for (i = 0;i<3;i++) {
     debug(1,sen_data.channel_data[i]);
     _delay_ms(500);
    }
#endif
  
  if (sen_cfg_lock.treshold_channel
    && (sen_data.channel_data[sen_cfg_lock.treshold_channel - 1] >= sen_cfg_lock.treshold))
      sen_data.overflow = 1;
  else
    sen_data.overflow = 0;
}

static char send_samples()
{
  unsigned char i = 0;
  
  //TODO v ID nastavit priznak pro treshold a jake kanly posilam
  tx_msg.id = SENSOR_ID;
  
  for (;i < 3;i++) {
    if (sen_cfg_lock.samples[i]) {
      tx_msg.data[tx_msg.length] = sen_data.channel_data[i];
      tx_msg.length++;
    }
  }
  
  if (sja1000p_pre_write_config(&tx_msg)) {
#ifdef DEBUG    
    CANMSG("Send samples err");
#endif    
    return -1;
  }
  
  sja1000p_send_msg();
  
  return 0;
}

void fsm_sensor_init(struct fsm *fsm, enum event event)
{
  switch (event) {
  case EVENT_ENTRY:
    break;
  case EVENT_DO:
    fsm->measurement_start = true;
    fsm->current_state = wait_for_cmd;
    break;
  case EVENT_EXIT:
#ifdef DEBUG    
    CANMSG("FSM init OK");
#endif
    break;
  }
}

static void wait_for_cmd(struct fsm *fsm, enum event event)
{  
  switch (event) {
  case EVENT_ENTRY:
#ifdef DEBUG
    CANMSG("FSM wait for cmd");
#endif
    break;
  case EVENT_DO:
    /* waiting to start adc sampling */
    if (fsm->measurement_start) {
      /* disable AVR interrupt, so the sen_cfg structure is propperly copied */
      can_disable_irq();
      sen_cfg_lock = sen_cfg;
      can_enable_irq();
      fsm->current_state = sensor_capture_data;
    }
    break;
  case EVENT_EXIT:
    fsm->measurement_start = sen_cfg_lock.delivery;
    break;
  }
}

static void sensor_capture_data(struct fsm *fsm, enum event event)
{
  switch (event) {
  case EVENT_ENTRY:
#ifdef DEBUG
    CANMSG("FSM capture data");
#endif
    break;
  case EVENT_DO:
    save_samples(fsm);
    fsm->current_state = sensor_send_data;
    break;
  case EVENT_EXIT:
    break;
  }
}

static void sensor_send_data(struct fsm *fsm, enum event event)
{
  switch (event) {
  case EVENT_ENTRY:
#ifdef DEBUG
    CANMSG("FSM send data");
#endif
    break;
  case EVENT_DO:
    send_samples();
    fsm->current_state = wait_for_cmd;
    break;
  case EVENT_EXIT:
    break;
  }
}
