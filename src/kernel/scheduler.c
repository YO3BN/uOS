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
  task_t *task;
  void (*task_main)(void*);

  /* Go through all tasks. */

  for (idx = 0; task_array[idx].state != TASK_STATE_INVALID &&
        idx < MAX_TASKS; idx++)
    {

      task = &task_array[idx];

      task->hit++;

      switch (task->state)
      {
        /* These statements are priority ordered. */

        case TASK_STATE_READY:
          /* Run the task with its own structure as an argument.
           * Also mark it as RUNNING.
           */

          task->state = TASK_STATE_RUNNING;
          task_main = (void(*)(void*)) task->entry_point;
          task_main(task);

          /* Re-mark it as READY if there was no request
           * to change the state.
           */

          if (task->state == TASK_STATE_RUNNING)
            {
              task->state = TASK_STATE_READY;
            }
          break;


        case TASK_STATE_PAUSED:
          /* Just skip it, but note that delay is not decremented anymore. */

          break;


        case TASK_STATE_SLEEP:
          /* If the task is sleeping, just decrease the the tick delay,
           * no matter if it is waiting for something else.
           */

          task->delay--;
          if (task->delay == 0)
            {
              task->state = TASK_STATE_READY;
            }
          break;


        case TASK_STATE_IO_WAIT:
          // check IOs for this task (owner/tid)
          // and change state if necessary
          break;


        case TASK_STATE_SEM_WAIT:
          // check Semaphores for this task (owner/tid)
          // and change state if necessary
          break;


        case TASK_STATE_RESUMED:
          break;

        default:
          break;
      }
    }
  //TODO if no task in array => sleep/shutdown/panic
  //TODO no work to do then sleep
}
















