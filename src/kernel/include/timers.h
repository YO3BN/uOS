/*
 * timers.h
 *
 *  Created on: Mar 19, 2020
 *      Author: yo3bn
 */

#ifndef SRC_KERNEL_INCLUDE_TIMERS_H_
#define SRC_KERNEL_INCLUDE_TIMERS_H_


void reset_watchdog(void);
void start_systick(void);
void start_watchdog(void);
void configure_systick(void);
void configure_watchdog(void);
void go_idle(void);


extern volatile unsigned long g_systicks;

#endif /* SRC_KERNEL_INCLUDE_TIMERS_H_ */
