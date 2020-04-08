/*
 * scheduler.c
 *
 *  Created on: Mar 3, 2020
 *      Author: yo3bn
 */


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "config.h"
#include "arch.h"
#include "private.h"
#include "kernel.h"
#include "klib.h"
#include "task.h"
#include "timers.h"
#include "scheduler.h"
#include "semaphore.h"


/****************************************************************************
 * Name: scheduler
 *
 * Description:
 *    Round Robin task scheduler.
 *
 *    Cycle through all tasks in the task array and change their states
 *    accordingly to the received event parameter.
 *
 *    Do all possible task computation until all tasks are
 *    blocked/waiting/sleeping then return.
 *
 * Input Parameters:
 *    event - Received Kernel Event.
 *
 * Returned Value:
 *    Since there is no other work or event generated by the scheduler, the
 *    return value is zero at the moment.
 *
 * Assumptions:
 *
 ****************************************************************************/

int scheduler(kernel_event_t *event)
{
  int work_todo  = 0;

  task_t *task = NULL;

  /* Do all possible work for the received event,
   * most likely until all tasks are blocked/waiting/sleeping.
   */

  do
    {
      work_todo = 0;

      /* Go through all tasks and take decisions based on their statuses. */

      while (task_getnext(&task))
        {
          switch (task->state)
          {
            /* These statements are priority ordered.
             * Do not change the order.
             */

            case TASK_STATE_READY:
               /* Run the task, also mark it as RUNNING. */

               g_running_task = task;
               task->state = TASK_STATE_RUNNING;
               run_task();
               g_running_task = task_list_head;

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

/*
            case TASK_STATE_IO_WAIT:
               if (event->type == KERNEL_EVENT_IO_RCVD)
                 {
                   //TODO check_io_for_this_task();
                   //TODO if yes, then set it ready to run and set work to do.
                 }
               break;*/


            case TASK_STATE_RESUMED:
                task->state = task->last_state;
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
                   if (g_systicks >= task->wakeup_ticks)
                     {
                       task->wakeup_ticks = 0;
                       task->state = TASK_STATE_READY;
                       work_todo = 1;
                     }
                 }
               break;


            case TASK_STATE_EXITED:
               //TODO some cleanup and remove it from task array.
               break;


            default:
              break;
          } /* switch (task->state) */
        } /* while (task_getnext(&task)) */

      /* Reached end of task list. Set task to NULL in order to get
       * first task at next iteration if still more work to do.
       */

      task = NULL;
    }
  while (work_todo);

  /* Always return 0, since scheduler never generate
   * events or other work, at least for the moment.
   */

  return 0;
}

