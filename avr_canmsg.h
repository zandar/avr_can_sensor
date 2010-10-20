/* canmsg.h - common kernel-space and user-space CAN message structure
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

typedef unsigned int canmsg_id_t;

/**
 * struct canmsg_t - structure representing CAN message
 * @flags:  message flags
 * @id:     ID of CAN message
 * @length: length of used data
 * @data:   data bytes buffer
 *
 * Header: canmsg.h
 */
struct canmsg_t {
	unsigned char flags;
	canmsg_id_t id;
	unsigned char length;
	unsigned char data[CAN_MSG_LENGTH];
};


/* Definitions to use for canmsg_t and canfilt_t flags */
#define MSG_RTR   (1<<0)
#define MSG_OVR   (1<<1)
#define MSG_EXT   (1<<2)
#define MSG_LOCAL (1<<3)

/* Can message ID mask */
#define MSG_ID_MASK ((1l<<29)-1)


#endif /*_CANMSG_T_H*/
