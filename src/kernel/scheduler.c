/*
 * scheduler.c
 *
 *  Created on: Mar 3, 2020
 *      Author: yo3bn
 */

#define NULL (void*)0

#include "task.h"
#include "kernel.h"


int scheduler(kernel_event_t *event)
{
  int idx   = 0;
  int ret   = 1; //TODO ret true or false; true only when generate an event from here
  int work_todo  = 0;

  task_t *task = NULL;
  void (*task_main)(void*);

  /* Do all possible work for the received event,
   * most likely until all tasks are blocked/waiting/sleeping.
   */

  do
    {
      /* Clear previous work. */
      work_todo = 0;

      /* Go through all tasks. */

      for (idx = 0; g_task_array[idx].state != TASK_STATE_INVALID &&
            idx < MAX_TASKS; idx++)
        {

          task = &g_task_array[idx];

          switch (task->state)
          {
            /* These statements are priority ordered. */

             case TASK_STATE_READY:
               /* Run the task, also mark it as RUNNING. */

               task->hit++;
               task->state = TASK_STATE_RUNNING;
               task_main = (void(*)(void*)) task->entry_point;
               task_main(task->arg);

               /* Re-mark it as READY if there was no request
                * to change the state.
                */

               if (task->state == TASK_STATE_RUNNING)
                 {
                   task->state = TASK_STATE_READY;
                   work_todo = 1;
                 }
               break;


             case TASK_STATE_PAUSED:
               /* Just skip it, but note that delay is not
                * decremented anymore.
                */

               break;


             case TASK_STATE_SLEEP:
               /* It the received event is SysTick, then decrement
                * one sleep tick, otherwise just skip it.
                */

               if (event->type == KERNEL_EVENT_IRQ_SYSTICK)
                 {
                   /* If the task is sleeping, just decrease its sleep tick,
                    * no matter if it is waiting for something else.
                    *
                    * Schedule it to run on next tick when sleep ticks
                    * were consumed.
                    */

                   if (task->sleep <= 1)
                     {
                       task->sleep = 0;
                       task->state = TASK_STATE_READY;
                       work_todo = 1;
                     }
                   else
                     {
                       task->sleep--;
                     }
                 }
               break;


             case TASK_STATE_IO_WAIT:
               // TODO implement
               break;


             case TASK_STATE_SEM_WAIT:
               //TODO implement
               break;

            default:
              break;
          }
        }
    }
  while (work_todo);

  return ret;
}





