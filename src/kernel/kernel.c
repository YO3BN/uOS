/*
 * kernel.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */

#define NULL (void*)0
#define CONFIG_EVENT_MAX_QUEUE 32
#define CONFIG_EVENT_MAX_SIZE sizeof(unsigned long)


#include "kernel.h"


/* INFO: Dual queue */
static volatile kernel_event_t g_event_queue1[CONFIG_EVENT_MAX_QUEUE];
static volatile kernel_event_t g_event_queue2[CONFIG_EVENT_MAX_QUEUE];


/* Create a buffer in BSS, in which data from events is stored. */
static volatile unsigned char
g_event_buffer[CONFIG_EVENT_MAX_SIZE][CONFIG_EVENT_MAX_QUEUE];


int kevent_enqueue_critical(kernel_event_type_t type, void *data, int size)
{
  // TODO implement
}


kernel_event_t *kevent_dequeue_critical(kernel_event_t *queue)
{
  // TODO implement
}


int kevent_enqueue(kernel_event_type_t type, void *data, int size)
{
  // TODO implement
  disable_interrupts();
  kevent_enqueue_critical(type, data, size);
  enable_interrupts();
}


int kevent_queue_isempty(kernel_event_t *queue)
{
  // TODO implement
}


kernel_event_t *kevent_dequeue(kernel_event_t *queue)
{
  // TODO implement
  //disable_interrupts();??
  kevent_dequeue_critical(queue);
  //enable_interrupts();??
  //empty? memset(queue, 0 ... ) : event
}


static kernel_event_t *kcheck_events_queue(void)
{
  // TODO implement
  return 1;
}


static void kconsume_events(kernel_event_t *event_queue)
{
  int work_todo = 0;
  kernel_event_t *event = NULL;

  /* For all event in the queue retrieve one event
   * and consume it through all sub-kernel modules.
   */

  while ((event = kevent_dequeue(event_queue)) != NULL)
    {
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
}


static void kclear_events_queue(void)
{
  kmemset(g_event_queue1, 0, sizeof(g_event_queue1));
  kmemset(g_event_queue2, 0, sizeof(g_event_queue2));
}


static void kernel_event_loop(void)
{
  /* Head of event queue to be consumed. */

  kernel_event_t *events = NULL;

  /* Forever processing events. */

  for (;;)
    {
      /* Check event queue. */

      events = kcheck_events_queue();

      /* Check if there are events to consume. */

      if (events)
        {
          /* Now, events are going to be consumed by other system parts,
           * (io waiting modules, semaphores, timers, scheduler, tasks, etc).
           * This is the most cpu intensive and time consuming operation.
           *
           * Expected to return before SysTick timer to tick.
           */

          kconsume_events(events);

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

  kclear_events_queue();

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







