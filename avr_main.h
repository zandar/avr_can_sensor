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
 * @chip_type: text string describing chip type
 * @chip_idx: index of the chip in candevice_t.chip[] array
 * @chip_irq: chip interrupt number if any
 * @chip_base_addr: chip base address in the CPU IO or virtual memory space
 * @flags: chip flags: %CHIP_CONFIGURED .. chip is configured,
 *	%CHIP_SEGMENTED .. access to the chip is segmented (mainly for i82527 chips)
 * @clock: chip base clock frequency in Hz
 * @baudrate: selected chip baudrate in Hz
 * @write_register: write chip register function copy
 * @read_register: read chip register function copy
 * @chip_data: pointer for optional chip specific data extension
 * @sja_cdr_reg: SJA specific register -
 *	holds hardware specific options for the Clock Divider
 *	register. Options defined in the sja1000.h file:
 *	%CDR_CLKOUT_MASK, %CDR_CLK_OFF, %CDR_RXINPEN, %CDR_CBP, %CDR_PELICAN
 * @sja_ocr_reg: SJA specific register -
 *	hold hardware specific options for the Output Control
 *	register. Options defined in the sja1000.h file:
 *	%OCR_MODE_BIPHASE, %OCR_MODE_TEST, %OCR_MODE_NORMAL, %OCR_MODE_CLOCK,
 *	%OCR_TX0_LH, %OCR_TX1_ZZ.
 * @int_cpu_reg: Intel specific register -
 *	holds hardware specific options for the CPU Interface
 *	register. Options defined in the i82527.h file:
 *	%iCPU_CEN, %iCPU_MUX, %iCPU_SLP, %iCPU_PWD, %iCPU_DMC, %iCPU_DSC, %iCPU_RST.
 * @int_clk_reg: Intel specific register -
 *	holds hardware specific options for the Clock Out
 *	register. Options defined in the i82527.h file:
 *	%iCLK_CD0, %iCLK_CD1, %iCLK_CD2, %iCLK_CD3, %iCLK_SL0, %iCLK_SL1.
 * @int_bus_reg: Intel specific register -
 *	holds hardware specific options for the Bus Configuration
 *	register. Options defined in the i82527.h file:
 *	%iBUS_DR0, %iBUS_DR1, %iBUS_DT1, %iBUS_POL, %iBUS_CBY.
 * @msgobj: array of pointers to individual communication objects
 * @chipspecops: pointer to the set of chip specific object filled by init_chip_data() function
 * @hostdevice: pointer to chip hosting board
 * @max_objects: maximal number of communication objects connected to this chip
 * @chip_lock: reserved for synchronization of the chip supporting routines
 *	(not used in the current driver version)
 * @worker_thread: chip worker thread ID (RT-Linux specific field)
 * @pend_flags: holds information about pending interrupt and tx_wake() operations
 *	(RT-Linux specific field). Masks values:
 *	%MSGOBJ_TX_REQUEST .. some of the message objects requires tx_wake() call, 
 *	%MSGOBJ_IRQ_REQUEST .. chip interrupt processing required
 *	%MSGOBJ_WORKER_WAKE .. marks, that worker thread should be waked
 *		for some of above reasons
 *
 * The fields @write_register and @read_register are copied from
 * corresponding fields from @hwspecops structure
 * (chip->hostdevice->hwspecops->write_register and 
 * chip->hostdevice->hwspecops->read_register)
 * to speedup can_write_reg() and can_read_reg() functions.
 */
struct canchip_t {
	unsigned long clock; /* Chip clock in Hz */
	unsigned long baudrate;
	
	unsigned char sja_cdr_reg; /* sja1000 only! */
	unsigned char sja_ocr_reg; /* sja1000 only! */
/*
	struct msgobj_t *msgobj[MAX_MSGOBJS];

	struct chipspecops_t *chipspecops;

	struct candevice_t *hostdevice;

	can_spinlock_t chip_lock;*/
};

/**
 * struct msgobj_t - structure holding communication object state
 * @obj_base_addr: 
 * @minor: associated device minor number
 * @object: object number in canchip_t structure +1
 * @flags: message object flags
 * @ret: field holding status of the last Tx operation
 * @qends: pointer to message object corresponding ends structure
 * @tx_qedge: edge corresponding to transmitted message
 * @tx_slot: slot holding transmitted message, slot is taken from
 *	canque_test_outslot() call and is freed by canque_free_outslot()
 *	or rescheduled canque_again_outslot()
 * @tx_retry_cnt: transmission attempt counter
 * @tx_timeout: can be used by chip driver to check for the transmission timeout
 * @rx_msg: temporary storage to hold received messages before
 *	calling to canque_filter_msg2edges()
 * @hostchip: pointer to the &canchip_t structure this object belongs to
 * @obj_used: counter of users (associated file structures for Linux
 *	userspace clients) of this object
 * @obj_users: list of user structures of type &canuser_t.
 * @obj_flags: message object specific flags. Masks values:
 *	%MSGOBJ_TX_REQUEST .. the message object requests TX activation
 *	%MSGOBJ_TX_LOCK .. some IRQ routine or callback on some CPU 
 *		is running inside TX activation processing code
 * @rx_preconfig_id: place to store RX message identifier for some chip types
 *		 that reuse same object for TX
 */
// struct msgobj_t {
// 	unsigned long obj_base_addr;
// 	unsigned int minor;	/* associated device minor number  */
// 	unsigned long obj_flags; 
// 	int ret;
// 
// 	struct canque_ends_t *qends;
// 
// 	struct canque_edge_t *tx_qedge;
// 	struct canque_slot_t *tx_slot;
// 	int tx_retry_cnt;
// 	struct timer_list tx_timeout;
// 	
// 	struct canmsg_t rx_msg;
//  
// 	unsigned long rx_preconfig_id;
// };
// 
// #define CAN_USER_MAGIC 0x05402033
// 
// extern int major;
// extern int minor[MAX_TOT_CHIPS];
// extern int extended;
// extern int baudrate[MAX_TOT_CHIPS];
// extern char *hw[MAX_HW_CARDS];
// extern int irq[MAX_IRQ];
// extern unsigned long io[MAX_HW_CARDS];
// extern int processlocal;
// 
// extern struct canhardware_t *hardware_p;
// extern struct canchip_t *chips_p[MAX_TOT_CHIPS];
// extern struct msgobj_t *objects_p[MAX_TOT_MSGOBJS];
// 
// extern struct mem_addr *mem_head;
// 
// 
// /* Inline function to write to the hardware registers. The argument address is 
//  * relative to the memory map of the chip and not the absolute memory address.
//  */
// extern inline void canobj_write_reg(const struct canchip_t *chip, const struct msgobj_t *obj,
// 				unsigned char data, unsigned address)
// {
// 	unsigned long address_to_write;
// 	address_to_write = obj->obj_base_addr+address;
// 	chip->write_register(data, address_to_write);
// }
// 
// extern inline unsigned canobj_read_reg(const struct canchip_t *chip, const struct msgobj_t *obj,
// 				unsigned address)
// {
// 	unsigned long address_to_read;
// 	address_to_read = obj->obj_base_addr+address;
// 	return chip->read_register(address_to_read);
// }
// 
// int can_check_dev_taken(void *anydev);
// 

#endif