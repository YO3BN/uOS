/*
 * kernel.h
 *
 *  Created on: Mar 8, 2020
 *      Author: yo3bn
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_H_
#define SRC_KERNEL_INCLUDE_KERNEL_H_


typedef enum
{
  KERNEL_EVENT_NONE = 0,
  KERNEL_EVENT_IRQ_SYSTICK,
  KERNEL_EVENT_IRQ_TIMER,
  KERNEL_EVENT_IRQ_IO,
  KERNEL_EVENT_IRQ_WATCHDOG,
  KERNEL_EVENT_IO_SENT,
  KERNEL_EVENT_IO_RCVD,
  KERNEL_EVENT_SEM_TAKEN,
  KERNEL_EVENT_SEM_GIVEN,
  KERNEL_EVENT_IPC_SENT,
  KERNEL_EVENT_IPC_RCVD,
} kernel_event_type_t;


typedef volatile struct
{
  unsigned char type;
  void * data;
} kernel_event_t;


volatile unsigned char *stack_head;


#endif /* SRC_KERNEL_INCLUDE_KERNEL_H_ */
