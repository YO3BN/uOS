/*
 * arch.h
 *
 *  Created on: Mar 20, 2020
 *      Author: yo3bn
 */

#ifndef SRC_ARCH_ATMEGA1284_ARCH_H_
#define SRC_ARCH_ATMEGA1284_ARCH_H_


/****************************************************************************
 * Mandatory function prototypes for Kernel.
 ****************************************************************************/

/* CPU */
void arch_enable_interrupts(void);
void arch_disable_interrupts(void);
void arch_go_idle(void);

/* Context-Switch */
#include "context.h"

/* Stack. */
void stack_init(void **stack);

/* Timers. */
void arch_reset_watchdog(void);
void arch_start_watchdog(void);
void arch_configure_watchdog(void);
void arch_stop_watchdog(void);
void arch_start_systick(void);
void arch_configure_systick(void);
void arch_stop_systick(void);


/****************************************************************************
 * Other architecture function prototypes.
 ****************************************************************************/

/* UART */
void arch_uart_init(void);
void arch_uart_byte_send(unsigned char c);
void arch_uart_byte_recv(unsigned char *c);


#endif /* SRC_ARCH_ATMEGA1284_ARCH_H_ */
