/* sensor.c
 * AVR CAN sensor using AVR CAN-bus driver.
 * Written by Michal Vokac
 * email: vokac.m@gmail.com
 * This software is released under the GPL-License.
 * Version AVR CAN sensor 13/10/2010
 */

/*
 * This file is part of avr_can_sensor.
 *
 * Avr_can_sensor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Avr_can_sensor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with avr_can_sensor.  If not, see <http://www.gnu.org/licenses/>.
 */

/*@{*/

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

#define adc_start   ADCSRA |= (1 << 6)  /* start new ADC conversion */
#define adc_on      ADCSRA |= (3 << 6)  /* enable ADC, start conversion  */
#define adc_off     ADCSRA &= ~(1 << 7) /* disable ADC */

//#define DEBUG

/**
 * Global variables
*/
static struct sensor_cfg sen_cfg; /* recived sensor configuration */
static struct sensor_cfg sen_cfg_lock;  /* copy of sen_cfg, can not be rewriten during usage*/
static struct sensor_data sen_data; /* captured data*/
static struct canmsg_t idn_msg; /* prepaired identicication answere message */
static struct canmsg_t tx_msg;  /*  */
static struct canchip_t chip; /* sja1000 chip configuration parameters, baudrate atc. */

/* array with captured data from three ADC channels, updated by ~4kHz */
volatile unsigned char adc_data[3];

static void wait_for_cmd(struct fsm *fsm, enum event event);
static void sensor_capture_data(struct fsm *fsm, enum event event);
static void sensor_send_data(struct fsm *fsm, enum event event);
static char sensor_init(struct fsm *fsm);

/**
 * averaging: - decode number of samples to be averaged
 * @avrg 3bit value
 * 
 * Function gets 3bit value of particular channel averaging configuration
 * and returns number of samples to be averaged for this channel.
 * 
 * Return value: number of samples
 */
static unsigned char averaging(unsigned char avrg)
{
  switch (avrg) {
    case 0:
      /* apropriate channel OFF */
      return AVRG_0;
      
    case 1:
      /* averaging per one sample */
      return AVRG_1;
      
    case 2:
      /* averaging per x sample */
      return AVRG_4;
      
    case 3:
      /* averaging per x sample */
      return AVRG_8;
    
    case 4:
      /* averaging per x sample */
      return AVRG_16;
      
    case 5:
      /* averaging per x sample */
      return AVRG_32;
      
    case 6:
      /* averaging per x sample */
      return AVRG_64;
      
    case 7:
      /* averaging per x sample */
      return AVRG_128;
      
    default:
      return 0;
  }
}

/**
 * ADC interrupt service routine
 */
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

/**
 * sensor_init: - initialize sensor parametes
 * @fsm: pointer to FSM structure with measurement and can config. parameters
 *
 * Return value: negative value reports error
 */
static char sensor_init(struct fsm *fsm)
{
  unsigned char i = 0;
  
  char *idn = SENSOR_IDN;
  
  sja_init_ports();
  
  /* fill chip structure with config values */
  chip.baudrate = SJA_BAUD;
  chip.clock = SJA_CLOCK;
  chip.filter_mask = 0xffffffff;//~SENSOR_MASK;
  chip.filter_code = 0xffffffff;//SENSOR_MASK;
  chip.sja_cdr_reg = sjaCDR_CLK_OFF;
  chip.sja_ocr_reg = sjaOCR_MODE_NORMAL|sjaOCR_TX0_LH;
  
  /* configure the SJA chip*/
  if(sja1000p_chip_config(&chip))
  {
    CANMSG("SJA config error");
    CANMSG("Restarting chip!");
    
    return -1;
  }
  
  /* prepaire message for IDN request */
  idn_msg.flags = MSG_EXT;
  idn_msg.id[0] = MY_ID;
  idn_msg.id[1] = 0;
  idn_msg.id[2] = 0;
  idn_msg.id[3] = 0;
  
  idn_msg.length = strlen(idn);
  
  /* fill msg data bytes with my sensor ID chars  */
  for (;i < strlen(idn);i++) {
    idn_msg.data[i] = idn[i];
  }
  
  ADCSRA = 0x0F;  /* ADC disabled. interrupt enabled, 128x prescaler = ~4kHz */
  ADMUX = 0x60;   /* Vref = Vcc, ADC output in ADCH */
  
  fsm->measurement_start = false;
  
  CANMSG("Sensor init OK");

  return 0;
}

/**
 * sensor_config: - setting sensor measurement parameters
 * @rx_msg: pointer to recive message structure
 */
char sensor_config(struct canmsg_t *rx_msg, struct fsm *fsm)
{
  unsigned char i = 0;
  
  /* if recived message is IDN request, send idn_msg */
  if (rx_msg->id[0] == IDN_RQ) {  
    if (sja1000p_pre_write_config(&idn_msg))
      return -1;
    
    sja1000p_send_msg();
    
#ifdef DEBUG
    CANMSG("IDN? answer OK");
#endif
  } else if (rx_msg->id[0] == MY_ID) {
    /* if recived message is MY_ID = ADC configuration */
    
    /* decode and save number of samples for every channel from msg ID */
      sen_cfg.samples[0] = averaging((rx_msg->id[2] >> 3) & 7);
      sen_cfg.samples[1] = averaging(rx_msg->id[2] & 7);
      sen_cfg.samples[2] = averaging((rx_msg->id[3] >> 2) & 7);
    
    /* if at least one sample for one channel will be measured, continue in decoding */
    if (sen_cfg.samples[0] || sen_cfg.samples[1] || sen_cfg.samples[2]) {
      
      /* save RX msg ID */
      for (i = 0; i < 4; i++) {
        sen_cfg.rx_msg_id[i] = rx_msg->id[i];
      }
      
      /* decode and save treshold value from msg ID */
      sen_cfg.treshold = rx_msg->id[1];
      
      /* decode and save channel with overflow control from msg ID */
      sen_cfg.treshold_channel = (rx_msg->id[2] >> 6) & 0x03;
      
      /* decode and save delivery parameter from msg ID */
      sen_cfg.delivery = (rx_msg->id[3] >> 1) & 0x01;
      
      /* start FSM measurement */
      fsm->measurement_start = true;
      
#ifdef DEBUG
      CANMSG("Sensor config OK");
#endif
    }
  }
  
  return 0;
}

/**
 * save_samples: save samples from ADC
 * 
 * Number of samples for averaging is stored in global varilable 'sen_cfg_lock'.
 * Samples are captured with 1kHz frequency.
 * Average value is saved to global variable 'sen_data'
 */
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

/**
 * send_samples: - transmit captured samples on CAN bus
 * 
 * Puts measured data from 'sensor_data' to can message object tx_msg
 *
 * Return value: negative value roports error.
 */
static char send_samples()
{
  unsigned char i = 0;
  
  /* set msg ID to rx_msg ID, so the listener knows what the data in the tx_msg
  stands for */
  for (i = 0; i < 4; i++) {
    tx_msg.id[i] = sen_cfg_lock.rx_msg_id[i];
  }
  
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
    tx_msg.id[3]++;
  
  if (sja1000p_pre_write_config(&tx_msg)) {
#ifdef DEBUG    
    CANMSG("FSM TX data err");
#endif    
    return -1;
  }
  
  sja1000p_send_msg();
  
  return 0;
}

/**
 * FSM initial state
 */
void fsm_sensor_init(struct fsm *fsm, enum event event)
{
  switch (event) {
  case EVENT_ENTRY:
    if (sensor_init(fsm)) {
      // TODO sensor configuration error - execute restart somehow
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

/**
 * FSM state, waiting for configuration message reception
 */
static void wait_for_cmd(struct fsm *fsm, enum event event)
{
  static timer measurement_time = 0, display_time = 0;
  
  switch (event) {
  case EVENT_ENTRY:
#ifdef DEBUG
    CANMSG("FSM wait for cmd");
#endif
    break;
  case EVENT_DO:
    /* waiting to start measurement via rx_msg, measurement performed periodicaly with  */
    if (fsm->measurement_start && (timer_msec >= measurement_time + MEASUREMENT_PERIOD)) {
      measurement_time = timer_msec;
      /* disable AVR interrupt, so the sen_cfg structure is propperly copied,
         sen_cfg copy needed to perform proper measurement if new config recived */
      can_disable_irq();
      sen_cfg_lock = sen_cfg;
      can_enable_irq();
      fsm->current_state = sensor_capture_data;
    }
#ifdef DEBUG
    /* in idle state display wait message */
    if (timer_msec >= display_time + 1000) {
      display_time = timer_msec;
      lcd_puts_line(0,"FSM wait for cmd");
    }
#endif
    break;
  case EVENT_EXIT:
    /* sets condition for next FSM start with respect to delivery setting
        if delivery = shot, FSM runs only ones
        if delivery = continual, FSM runs continuously untill new setting
        is recived */
    fsm->measurement_start = sen_cfg_lock.delivery;
    break;
  }
}

/**
 * FSM state for capturing ADC samples
 */
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

/**
 * FSM state for transmiting captured samples
 */
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

/*@}*/