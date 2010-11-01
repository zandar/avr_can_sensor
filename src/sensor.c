/* sensor.c
 * AVR CAN sensor using AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version AVR CAN sensor 13/10/2010
 */

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
#include "../include/def.h"
#include "../include/timer.h"
#include "../include/display.h"

/* extract trshold value from msg ID */
#define treshold(id)              ((id >> SHIFT_TRESHOLD) & 0xff)
/* extract trshold channel from msg ID */
#define treshold_channel(id)      ((id >> SHIFT_TRESHOLD_CHANNEL) & 0x03)
/* extract 3bits value with particular channel settings from msg ID */
#define channel_avrg(id,channel)  ((id >> channel) & 0x07)
/* extract delivery value from msg ID */
#define delivery(id)              ((id >> SHIFT_DELIVERY) & 0x01)

#define adc_start   ADCSRA |= (1 << 6)  /* starts new ADC conversion */
#define adc_on      ADCSRA |= (3 << 6)  /* enable ADC, start conversion  */
#define adc_off     ADCSRA &= ~(1 << 7) /* disable ADC */

#define DEBUG

static struct sensor_cfg sen_cfg, sen_cfg_lock;
static struct sensor_data sen_data;
static struct canmsg_t idn_msg;
static struct canmsg_t tx_msg;
static struct canchip_t chip;

volatile unsigned char adc_data[3];

static void wait_for_cmd(struct fsm *fsm, enum event event);
static void sensor_capture_data(struct fsm *fsm, enum event event);
static void sensor_send_data(struct fsm *fsm, enum event event);
static char sensor_init(struct fsm *fsm);

/* returns bit position of LSB bit for particular channel settings in msg ID */
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

/* returns number of samples from patameter of 3bit length */
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

/* ADC interrupt service routine */
ISR(ADC_vect)
{
  static unsigned char channel = 0;
  
  adc_data[channel] = ADCH;
  
  /* Select next ADC channel */
  if (++channel > 2)
    channel = 0;
  
  ADMUX = (ADC_VREF_TYPE & 0xff) + channel;
  
  adc_start;
}

static char sensor_init(struct fsm *fsm)
{
  unsigned char i = 0;
  
  char *idn = SENSOR_IDN;
  
  sja_init_ports();
  
  /* fill chip structure with config values */
  chip.baudrate = SJA_BAUD;
  chip.clock = SJA_CLOCK;
  chip.filter_mask = ~SENSOR_MASK;
  chip.filter_code = SENSOR_MASK;
  chip.sja_cdr_reg = sjaCDR_CLK_OFF;
  chip.sja_ocr_reg = sjaOCR_MODE_NORMAL|sjaOCR_TX0_LH;
  
  if(sja1000p_chip_config(&chip))
  {
    CANMSG("SJA config error");
    CANMSG("Restarting chip!");
    
    return -1;
  }
  
  /* prepaire message for IDN request */
  idn_msg.flags = MSG_EXT;
  idn_msg.id = SENSOR_ID;
  idn_msg.length = strlen(idn);
  
  /* fill msg data bytes with my sensor ID chars  */
  for (;i < strlen(idn);i++) {
    idn_msg.data[i] = idn[i];
  }
  
  ADCSRA = 0x0F;  /* ADC disabled. interrupt enabled, 128x prescaler ~4kHz */
  ADMUX = 0x60;   /*  Vref = Vcc, ADC output in ADCH */
  
  fsm->measurement_start = false;
  
//#ifdef DEBUG
  /* initialize sen_cfg structure with default values
     channel 0 - 2 enabled, no averaging, continual delivery, no treshold */
  for (i = 0;i < 3; i++) {
    sen_cfg.samples[i] = AVRG_1;
  }
  
  sen_cfg.delivery = 1;
  sen_cfg.treshold = 0;
  sen_cfg.treshold_channel = 0;
  
  fsm->measurement_start = true;
//#endif

  CANMSG("Sensor init OK");

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
    
    /* decode and save number of samples for every channel from msg ID */
    for (;i < 3;i++) {
      sen_cfg.samples[i] = averaging(channel_avrg(rx_msg->id,shift(i)));
    }
    
    /* if at least one sample for one channel will measured, continue in decoding */
    if (sen_cfg.samples[0] || sen_cfg.samples[1] || sen_cfg.samples[2]) {
      
        /* save RX msg ID */
      sen_cfg.rx_msg_id = rx_msg->id;
      
      /* decode and save treshold value from msg ID */
      sen_cfg.treshold = treshold(rx_msg->id);
      
      /* decode and save channel with overflow control from msg ID */
      sen_cfg.treshold_channel = treshold_channel(rx_msg->id);
      
      /* decode and save delivery parameter from msg ID */
      sen_cfg.delivery = delivery(rx_msg->id);
      
      /* start FSM measurement */
      fsm->measurement_start = true;
    }
    
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
  
  for (i = 0; i < 3; i++) {
    /* capture number of samples[i] ADC samples */
    while (counter < sen_cfg_lock.samples[i]) {
      /* save sample every 1ms = 1kHz  */
      if (timer_msec >= adc_time + 1) {
        adc_time = timer_msec;
        average += adc_data[i];
        counter++;
      }
    }
    
    /* counts avarage value from saved samples,
       avoid dividing by zero when none sample saved */
    if (sen_cfg_lock.samples[i])
      sen_data.channel_data[i] = average / sen_cfg_lock.samples[i];
    
    counter = 0;
    average = 0;
  }
  
  adc_off;
  
  /* if treshold is set for any channel,
     check measured data limits, if overflow occured, set apropriate bit */
  if (sen_cfg_lock.treshold_channel
    && (sen_data.channel_data[sen_cfg_lock.treshold_channel - 1] >= sen_cfg_lock.treshold))
      sen_data.overflow = 1;
  else
    sen_data.overflow = 0;
}

static char send_samples()
{
  unsigned char i = 0;
  
  /* set msg ID to rx_msg ID */
  tx_msg.id = sen_cfg_lock.rx_msg_id;
  
  tx_msg.length = 0;
  
  /* fill msg data bytes with captured samples
     only this channels with samples[i] != 0 will be transmited */
  for (i = 0; i < 3; i++) {
    if (sen_cfg_lock.samples[i]) {
      tx_msg.data[tx_msg.length] = sen_data.channel_data[i];
      tx_msg.length++;
    }
  }
  
  /* if overflow occure, set appropriate bit in msg ID */
  if (sen_data.overflow)
    tx_msg.id++;
  
  if (sja1000p_pre_write_config(&tx_msg)) {
#ifdef DEBUG    
    CANMSG("FSM TX data err");
#endif    
    return -1;
  }
  
  sja1000p_send_msg();
  
  return 0;
}

/* FSM initial state */
void fsm_sensor_init(struct fsm *fsm, enum event event)
{
  switch (event) {
  case EVENT_ENTRY:
    if (sensor_init(fsm)) {
      // TODO init ERR - provest restart procesoru nejak
    }
    break;
  case EVENT_DO:
    fsm->current_state = wait_for_cmd;
    break;
  case EVENT_EXIT:
#ifdef DEBUG    
    CANMSG("FSM init OK");
#endif
    break;
  }
}

/* FSM state, waiting for coniguration message recive */
static void wait_for_cmd(struct fsm *fsm, enum event event)
{ 
  switch (event) {
  case EVENT_ENTRY:
#ifdef DEBUG
    CANMSG("FSM wait for cmd");
#endif
    break;
  case EVENT_DO:
    /* waiting to start measurement via rx_msg */
    if (fsm->measurement_start) {
      /* disable AVR interrupt, so the sen_cfg structure is propperly copied
         sen_cfg copy needed to perform proper measurement if new config recived */
      can_disable_irq();
      sen_cfg_lock = sen_cfg;
      can_enable_irq();
      fsm->current_state = sensor_capture_data;
    }
    break;
  case EVENT_EXIT:
    /* sets condition for next FSM start with aspect to delivery setting
        if delivery = shot, FSM rubs only ones
        if delivery = continual, FSM runs continuously untill new setting is recived */
    fsm->measurement_start = sen_cfg_lock.delivery;
    break;
  }
}

/* FSM state for capturing ADC samples */
static void sensor_capture_data(struct fsm *fsm, enum event event)
{
  switch (event) {
  case EVENT_ENTRY:
#ifdef DEBUG
    CANMSG("FSM capture data");
#endif
    adc_on;
    break;
  case EVENT_DO:
    save_samples(fsm);
    fsm->current_state = sensor_send_data;
    break;
  case EVENT_EXIT:
    break;
  }
}

/* FSM state for transmiting captured samples */
static void sensor_send_data(struct fsm *fsm, enum event event)
{
  switch (event) {
  case EVENT_ENTRY:
#ifdef DEBUG
    CANMSG("FSM TX data");
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
