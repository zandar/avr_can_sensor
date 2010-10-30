/* avr_can.h
 * Linux CAN-bus device driver.
 * Written by Pavel Pisa - OCERA team member
 * email:pisa@cmp.felk.cvut.cz
 * This software is released under the GPL-License.
 * Version lincan-0.3  17 Jun 2004
 */

#ifndef _CANMSG_T_H
#define _CANMSG_T_H

/* Number of data bytes in one CAN message */
#define CAN_MSG_LENGTH 8

/* Definitions to use for canmsg_t and canfilt_t flags */
#define MSG_RTR   (1<<0)
#define MSG_OVR   (1<<1)
#define MSG_EXT   (1<<2)
#define MSG_LOCAL (1<<3)

/* Can message ID mask */
#define MSG_ID_MASK ((1l<<29)-1)

#define NONE 0
#define NEW  1

/**
 * struct canmsg_t - structure representing CAN message
 * @flags:  message flags
 * @id:     ID of CAN message
 * @length: length of used data
 * @data:   data bytes buffer
 */
struct canmsg_t {
	unsigned char flags;
	unsigned long id;
	unsigned char length;
	unsigned char data[CAN_MSG_LENGTH];
  unsigned char status;
};

/**
 * struct canchip_t - CAN chip state and type information
 * @clock: chip base clock frequency in Hz
 * @baudrate: selected chip baudrate in Hz
 * @sja_cdr_reg: SJA specific register -
 * @sja_ocr_reg: SJA specific register -
 */
struct canchip_t {
  unsigned char flags;
  unsigned long clock; /* Chip clock in Hz */
  unsigned long baudrate;
  
  unsigned char sja_cdr_reg; /* sja1000 only! */
  unsigned char sja_ocr_reg; /* sja1000 only! */
};



#endif /*_CANMSG_T_H*/
