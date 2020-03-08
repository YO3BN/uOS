/*
 * kernel.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */

#define NULL (void*)0
#define CONFIG_MAX_EVENT_QUEUE 10


typedef enum
{
  KERNEL_EVENT_NONE     = 0,
  KERNEL_EVENT_SYSTICK  = 1,
  KERNEL_EVENT_TIMER    = 2,
  KERNEL_EVENT_IO       = 4,
  KERNEL_EVENT_WATCHDOG = 8,
} kernel_event_type_t;


typedef struct
{
  kernel_event_type_t type;
  void *data;
} kernel_event_t;


static volatile kernel_event_t g_event_queue1[CONFIG_MAX_EVENT_QUEUE];
static volatile kernel_event_t g_event_queue2[CONFIG_MAX_EVENT_QUEUE];


static kernel_event_t *kcheck_events_queue(void)
{
  // TODO implement
  return 1;
}


static void kconsume_events_fsm(kernel_event_t *event_queue)
{
  // TODO implement
  scheduler();
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
          /* Events are going to be consumed by other system parts,
           * (io waiting modules, semaphores, timers, scheduler, tasks, etc).
           * This is the most cpu intensive and time consuming operation.
           *
           * Expected to return before SysTick timer to tick.
           */

          kconsume_events_fsm(events);

          /* Here, all events in the queue were consumed.
           * It is time to reset the watch dog timer.
           */

          reset_watchdog();
        }

      /* Kernel reach here only after all the work was done
       * (drivers, semaphores, ipc, scheduler, tasks, etc)
       * and have nothing else to do.
       *
       * NOTE: An interrupt can sneak here leaving it un-consumed,
       * but it is saved into event queue and consumed next tick.
       */

      go_idle();

      /* Just woken up by an interrupt, continue. */
    }
}


void kernel_init(void)
{
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







