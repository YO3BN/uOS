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
  //TODO TODO
  static int task_iterator = 0;
  int i;

  /* Check for the end of task array. */

  if (task_iterator >= MAX_TASKS)
    {
      task_iterator = 0;
    }

  /* Check if the task is valid. */

  if (!task_array[task_iterator].entry_point ||
      task_array[task_iterator].state == TASK_STATE_INVALID)
    {
      /* Reached end of task list, reseting position to
       * beginning of array.
       */

      task_iterator = 0;
    }

  //TODO if no task in array => sleep/shutdown/panic
  // TODO check if the task is ready

  i = task_iterator;
  task_iterator++;

  // Run task ;TODO prepare it for running by context switch
  void (*func)(void*) = (void (*)(void*)) task_array[i].entry_point;
  func(task_array[i].arg);
}

