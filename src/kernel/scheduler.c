/*
 * scheduler.c
 *
 *  Created on: Mar 3, 2020
 *      Author: yo3bn
 */


#define MAX_TASKS 100


#include "task.h"


void scheduler(void)
{
  int idx = 0;
  void (*task_main)(void*);

  /* Consume ticks. */

  //g_ticks--;

  /* Go through all tasks. */

  for (idx = 0; g_task_array[idx].state != TASK_STATE_INVALID &&
        idx < MAX_TASKS; idx++)
    {

      g_task = &g_task_array[idx];

      switch (g_task->state)
      {
        /* These statements are priority ordered. */

        case TASK_STATE_READY:
          /* Run the task, also mark it as RUNNING. */

          g_task->hit++;
          g_task->state = TASK_STATE_RUNNING;
          task_main = (void(*)(void*)) g_task->entry_point;
          task_main(g_task->arg);

          /* Re-mark it as READY if there was no request
           * to change the state.
           */

          if (g_task->state == TASK_STATE_RUNNING)
            {
              g_task->state = TASK_STATE_READY;
            }
          break;


        case TASK_STATE_PAUSED:
          /* Just skip it, but note that delay is not decremented anymore. */

          break;


        case TASK_STATE_SLEEP:
          /* If the task is sleeping, just decrease the the tick delay,
           * no matter if it is waiting for something else.
           */

          if (g_task->sleep == 0)
            {
              g_task->state = TASK_STATE_READY;
            }
          else
            {
              g_task->sleep--;
            }
          break;


        default:
          break;
      }
    }
  //TODO if no task in array => sleep/shutdown/panic
  //TODO no work to do then sleep
}
















