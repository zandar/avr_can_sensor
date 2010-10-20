/* main.h -> avr_main.h
 * Header file for the Linux CAN-bus driver.
 * Written by Arnaud Westenberg email:arnaud@wanadoo.nl
 * Rewritten for new CAN queues by Pavel Pisa - OCERA team member
 * email:pisa@cmp.felk.cvut.cz
 * Rewritten for AVR mirocontroler by Michal Vokac
 * This software is released under the GPL-License.
 * Version lincan-0.3  17 Jun 2004
 * Version avrCAN-0.1  16/10/2010
 */

#ifndef AVR_MAIN_H
#define AVR_MAIN_H

// #include "./can.h"
// #include "./constants.h"
// #include "./can_sysdep.h"
// #include "./can_queue.h"

/**
 * struct canchip_t - CAN chip state and type information
 * @clock: chip base clock frequency in Hz
 * @baudrate: selected chip baudrate in Hz
 * @sja_cdr_reg: SJA specific register -
 * @sja_ocr_reg: SJA specific register -
 */
struct canchip_t {
	unsigned long clock; /* Chip clock in Hz */
	unsigned long baudrate;
	
	unsigned char sja_cdr_reg; /* sja1000 only! */
	unsigned char sja_ocr_reg; /* sja1000 only! */
	//can_spinlock_t chip_lock;
};

// #define CAN_USER_MAGIC 0x05402033

#endif