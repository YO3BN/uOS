/*
 * scheduler.c
 *
 *  Created on: Mar 3, 2020
 *      Author: yo3bn
 */

#define NULL (void*)0
extern volatile unsigned long g_systicks;

#include "task.h"
#include "kernel.h"


int scheduler(kernel_event_t *event)
{
  int idx   = 0;
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

               g_running_task = task;
               task->hit++;
               task->state = TASK_STATE_RUNNING;
               task_main = (void(*)(void*)) task->entry_point;
               task_main(task->arg);
               g_running_task = NULL;

               /* Re-mark it as READY if there was no request
                * to change the state.
                *
                * For example, this happens when the task exited simply
                * using return.
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
               if (event->type == KERNEL_EVENT_IRQ_SYSTICK)
                 {
                   //TODO get g_systicks in critical section
                   if (task->sleep_stamp + task->sleep_ticks >= g_systicks)
                     {
                       task->sleep_stamp = 0;
                       task->sleep_ticks = 0;
                       task->state = TASK_STATE_READY;
                       work_todo = 1;
                     }
                 }
               break;


             case TASK_STATE_IO_WAIT:
               if (event->type == KERNEL_EVENT_IO_RCVD)
                 {
                   //TODO check_io_for_this_task();
                   //TODO if yes, then set it ready to run and set work to do.
                 }
               break;


             case TASK_STATE_SEM_WAIT:
               if (event->type == KERNEL_EVENT_SEM_UNLOCKED)
                 {
                   //TODO check_sem_for_this_task();
                   //TODO if yes, then set it ready to run and set work to do.
                 }
               break;


            default:
              break;
          }
        }
    }
  while (work_todo);

  /* Always return 0, since scheduler never generate
   * any events, at least for the moment.
   */

  return 0;
}





