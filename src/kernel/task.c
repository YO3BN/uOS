/*
 * task.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */


#define MAX_TASKS 100

#include "task.h"

task_t task_array[MAX_TASKS];


int task_create(void (*entry_point)(void*), void *arg)
{
  int i;

  /* Sanity checks. */

  if (!entry_point)
    {
      goto return_error;
    }

  /* Go to first empty array element. */

  for (i = 0; i < MAX_TASKS; i++)
    {
      if (task_array[i].state == TASK_STATE_INVALID)
        {
          break;
        }
    }

  /* Check if array is full. */

  if (i >= MAX_TASKS)
    {
      goto return_error;
    }

  task_array[i].tid = i;
  task_array[i].state = TASK_STATE_READY;
  task_array[i].entry_point = entry_point;

  return 1;

return_error:
  return -1;
}


int task_start(const int tid)
{
  return 1;
}


int task_stop(const int tid)
{
  return 1;
}


int task_kill(const int tid)
{
  return 1;
}

