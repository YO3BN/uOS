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
  KERNEL_EVENT_SEM_LOCKED,
  KERNEL_EVENT_SEM_UNLOCKED,
  KERNEL_EVENT_IPC_SENT,
  KERNEL_EVENT_IPC_RCVD,
} kernel_event_type_t;


typedef struct
{
  unsigned char type;
  unsigned char data;
} kernel_event_t;


/****************************************************************************
 * Private functions.
 ****************************************************************************/


/****************************************************************************
 * Name: kget_event_from_buffer
 *
 * Description:
 *    Retrieve next event available in the circular buffer.
 *
 * Output Parameters:
 *    event - Retrieved event from buffer.
 *
 * Returned Value:
 *    1 - if the event is valid.
 *    0 - if the event is invalid.
 *
 * Assumptions:
 *    This should be called from kernel context.
 *
 ****************************************************************************/

static int kget_event_from_buffer(kernel_event_t *event);


/****************************************************************************
 * Name: kconsume_events
 *
 * Description:
 *  - Dispatch the event to all system modules, tasks in order to be consumed.
 *  - This function is most cpu intensive and time consuming since it has to
 *    go through all possible finite states of all modules including tasks.
 *  - Also, expected to return before SysTick to tick.
 *
 * Input Parameters:
 *  event - Event retrieved from event buffer.
 *
 * Returned Value:
 *  none
 *
 * Assumptions:
 *
 ****************************************************************************/

static void kconsume_events(kernel_event_t *event);


/****************************************************************************
 * Name: kernel_event_loop
 *
 * Description:
 *  Kernel forever loop.
 *  Pop events from buffer, consume them, reset watch dog, then go idle.
 *
 * Input Parameters:
 *  none
 *
 * Returned Value:
 *  none
 *
 * Assumptions:
 *
 ****************************************************************************/

static void kernel_event_loop(void);


/****************************************************************************
 * Public functions.
 ****************************************************************************/

/****************************************************************************
 * Name: kput_event_in_buffer
 *
 * Description:
 *    Insert new event in the circular buffer.
 *
 * Input Parameters:
 *    type - Event type.
 *    data - Optional data.
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    This should be called only from ISR context.
 *
 ****************************************************************************/

void kput_event_in_buffer(unsigned char type, unsigned char data);


/****************************************************************************
 * Name: kput_event_in_buffer_critical
 *
 * Description:
 *    Insert new event in the circular buffer (critical).
 *
 * Input Parameters:
 *    type - Event type.
 *    data - Optional data.
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    This should NOT be called from ISR context.
 *
 ****************************************************************************/

void kput_event_in_buffer_critical(unsigned char type, unsigned char data);


/****************************************************************************
 * Name: kernel_init
 *
 * Description:
 *    Kernel initialization.
 *
 * Input Parameters:
 *    None
 *
 * Returned Value:
 *    None
 *
 * Assumptions:
 *    This should be called at the beginning of main();
 *
 ****************************************************************************/

void kernel_init(void);


/****************************************************************************
 * Name: kernel_start
 *
 * Description:
 *    Run the kernel.
 *
 * Input Parameters:
 *    None
 *
 * Returned Value:
 *    None
 *
 * Assumptions:
 *    This function will never return.
 *    It should be called at the end of main();
 *
 ****************************************************************************/

void kernel_start(void);


#endif /* SRC_KERNEL_INCLUDE_KERNEL_H_ */
