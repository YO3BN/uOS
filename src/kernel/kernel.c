/*
 * kernel.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */

#define NULL (void*)0
#define CONFIG_MAX_EVENTS 128


#include "kernel.h"


static volatile struct
{
  unsigned char read_idx;
  unsigned char write_idx;
  kernel_event_t event[CONFIG_MAX_EVENTS];
} g_kevent_array;


void kput_event_in_buffer(unsigned char type, unsigned char data)
{
  g_kevent_array.event[g_kevent_array.write_idx].type = type;
  g_kevent_array.event[g_kevent_array.write_idx].data = data;
  g_kevent_array.write_idx++;

  if (g_kevent_array.write_idx >= CONFIG_MAX_EVENTS)
    {
      g_kevent_array.write_idx = 0;
    }
}


int kget_event_from_buffer(kernel_event_t *event)
{
  int cnt = 0;

  if (event == NULL)
    {
      return 0;
    }

  /* Clear previous event. */

  kmemset(event, 0, sizeof(kernel_event_t));

  /* Enter critical and check for available event. */

  disable_interrupts();

  do
    {
      event->type = g_kevent_array.event[g_kevent_array.read_idx].type;
      if (event->type != KERNEL_EVENT_NONE)
        {
          event->data = g_kevent_array.event[g_kevent_array.read_idx].data;

          /* Clear entry. */

          g_kevent_array.event[g_kevent_array.read_idx].type = KERNEL_EVENT_NONE;
          g_kevent_array.event[g_kevent_array.read_idx].data = 0;
        }

      /* Advance read index. Reset it at the beginning of the array. */

      g_kevent_array.read_idx++;
      if (g_kevent_array.read_idx >= CONFIG_MAX_EVENTS)
        {
          g_kevent_array.read_idx = 0;
        }

      cnt++;
    }
  while (event->type == KERNEL_EVENT_NONE && cnt <= CONFIG_MAX_EVENTS);

  /* Exit critical section. */

  enable_interrupts();

  if (event->type != KERNEL_EVENT_NONE)
    {
      return 1;
    }

  return 0;
}


static void kconsume_events(kernel_event_t *event)
{
  int work_todo = 0;

  /* Finish all possible work for this event. */

  do
    {
      work_todo |= io(event);
      work_todo |= semaphore(event);
      work_todo |= ipc(event);
      work_todo |= scheduler(event);
    }
  while (work_todo);
}


static void kernel_event_loop(void)
{
  int ret = 0;
  kernel_event_t event;

  /* Forever processing events. */

  for (;;)
    {
      ret = kget_event_from_buffer(&event);

      /* Check if there are events to consume. */

      if (ret)
        {
          /* Now, events are going to be consumed by other system parts,
           * (io waiting modules, semaphores, timers, scheduler, tasks, etc).
           * This is the most cpu intensive and time consuming operation.
           *
           * Expected to return before SysTick timer to tick.
           */

          kconsume_events(&event);

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


void kernel_init(void)
{
  /* disable_interrupts();?? */

  /* TODO Initialize kernel globals. */

  kmemset(g_kevent_array, 0, sizeof(g_kevent_array));

  /* Configure timers. */

  configure_systick();
  configure_watchdog();
}


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







