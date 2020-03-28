/*
 * kernel.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */


/****************************************************************************
 * Included Files.
 ****************************************************************************/

#include "config.h"
#include "arch.h"
#include "cpu.h"
#include "private.h"
#include "kernel.h"
#include "timers.h"
#include "scheduler.h"
#include "semaphore.h"
#include "klib.h"


/****************************************************************************
 * Private data.
 ****************************************************************************/

/* Kernel Event Buffer
 *
 * Events produced by INTERRUPTS, KERNEL are stored temporarily in this buffer
 * and consumed from here by kernel itself, its submodules and finally tasks.
 *
 * Two indexes are used, one for inserting events -write_idx- and one for
 * retrieve events -read_idx- respectively.
 */

static volatile struct
{
  unsigned char read_idx;                   /* Position for retrieving. */
  unsigned char write_idx;                  /* Position for inserting. */
  unsigned char used_size;                  /* Events stored in array. */
  kernel_event_t event[CONFIG_MAX_EVENTS];  /* Events are stored here. */
} g_kevent_buffer;


/****************************************************************************
 * Private functions.
 ****************************************************************************/


/****************************************************************************
 * Name: kget_event
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
 *    This should be called only from kernel context.
 *
 ****************************************************************************/

static int kget_event(kernel_event_t *event)
{
  int ret = 0;

  if (event == NULL)
    {
      return ret;
    }

  /* Enter critical and check for available event. */

  disable_interrupts();
  if (g_kevent_buffer.used_size > 0)
    {
      /* Copy event from kernel buffer in order to be used locally. */

      kmemcpy((void*) event,
              (void*) &g_kevent_buffer.event[g_kevent_buffer.read_idx],
              sizeof(kernel_event_t));

      /* Decrease used slots in buffer.
       *
       * NOTE: The event content is not removed from buffer.
       * FIXME: Not sure if removing its content is really required.
       */

      g_kevent_buffer.used_size--;
      g_kevent_buffer.read_idx++;

      /* Buffer overlapping.
       * TODO: Use modulo % if is more efficient.
       */

      if (g_kevent_buffer.read_idx >= CONFIG_MAX_EVENTS)
        {
            g_kevent_buffer.read_idx = 0;
        }

      ret = 1;
    }

  /* Exit critical section. */

  enable_interrupts();
  return ret;
}


/****************************************************************************
 * Name: kconsume_event
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

static void kconsume_event(kernel_event_t *event)
{
  int work_todo = 0;

  /* Finish all possible work for this event. */

  do
    {
      /* Add other kernel submodules here (io, sem, etc.). */

      work_todo |= semaphores(event);
      work_todo |= scheduler(event);
    }
  while (work_todo);

  /* Clear event. */

  kmemset((void*) event, 0, sizeof(kernel_event_t));
}


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

static void kernel_event_loop(void)
{
  kernel_event_t event;

  /* Forever processing events. */

  for (;;)
    {
      /* Consume all events from buffer. */

      while (kget_event(&event))
        {
          /* Now, events are going to be consumed by other system parts,
           * (io modules, semaphores, timers, scheduler, tasks, etc).
           * This is the most cpu intensive and time consuming operation.
           *
           * Expected to return before SysTick timer to tick.
           */

          kconsume_event(&event);

          /* Here, all events in the queue were consumed.
           * It is time to reset the watch dog timer.
           */

          reset_watchdog();
        }

      /* Kernel reach here only after all the work was done
       * (drivers, semaphores, ipc, scheduler, tasks, etc)
       * and have nothing else to do.
       *
       * FIXME: An interrupt can sneak here leaving it un-consumed,
       * but it is saved into event queue and consumed next tick.
       */

      go_idle();

      /* Just woken up by an interrupt.
       * Continue checking for received events.
       */
    }
}


/****************************************************************************
 * Public functions.
 ****************************************************************************/


/****************************************************************************
 * Name: kput_event_crit
 *
 * Description:
 *    Insert new event in the circular buffer.
 *    This function is used in critical sections and ISR.
 *
 * Input Parameters:
 *    type - Event type.
 *    data - Pointer to optional data.
 *           Or it can be casted to other type smaller or equal than void
 *           pointer. (char, int, void*).
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    This should be called only from critical section or ISR context.
 *
 ****************************************************************************/

void kput_event_crit(unsigned char type, void * data)
{
  if (g_kevent_buffer.used_size >= CONFIG_MAX_EVENTS)
    {
      return;
    }

  g_kevent_buffer.event[g_kevent_buffer.write_idx].type = type;
  g_kevent_buffer.event[g_kevent_buffer.write_idx].data = data;
  g_kevent_buffer.used_size++;
  g_kevent_buffer.write_idx++;

  /* Buffer overlapping.
   * TODO: Use modulo % if is more efficient.
   */

  if (g_kevent_buffer.write_idx >= CONFIG_MAX_EVENTS)
    {
      g_kevent_buffer.write_idx = 0;
    }
}


/****************************************************************************
 * Name: kput_event
 *
 * Description:
 *    Insert new event in the circular buffer (no critical).
 *
 * Input Parameters:
 *    type - Event type.
 *    data - Pointer to optional data.
 *           Or it can be casted to other type smaller or equal than void
 *           pointer. (char, int, void*).
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    This should be called ONLY from kernel context, not from ISR.
 *
 ****************************************************************************/

void kput_event(unsigned char type, void * data)
{
  disable_interrupts();
  kput_event_crit(type, data);
  enable_interrupts();
}


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

void kernel_init(void)
{
  g_systicks = 0;
  kmemset((void*) &g_kevent_buffer, 0, sizeof(g_kevent_buffer));

  /* Configure timers. */

  configure_systick();
  configure_watchdog();
}


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

void kernel_start(void)
{
  /* NOTE: Enabling interrupts could
   * generate some events.
   */

  enable_interrupts();

  start_watchdog();
  start_systick();

  /* Start Event Finite State Machine. */

  kernel_event_loop();
}

