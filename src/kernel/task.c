/*
 * task.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */


#define MAX_TASKS 100

#include "task.h"


task_t g_task_array[MAX_TASKS];
task_t *g_task;


int task_create(void (*entry_point)(void*), void *arg)
{
  int i;

  /* Sanity checks. */

  if (!entry_point)
    {
      goto return_error;
    }

  /* Find the first free array element. */

  for (i = 0; i < MAX_TASKS &&
      g_task_array[i].state != TASK_STATE_INVALID &&
      g_task_array[i].state != TASK_STATE_EXITED; i++);

  /* Check if array is full. */

  if (i >= MAX_TASKS)
    {
      goto return_error;
    }

  g_task_array[i].tid = i;
  g_task_array[i].state = TASK_STATE_READY;
  g_task_array[i].entry_point = entry_point;

  return 1;

return_error:
  return -1;
}


int task_resume(const int tid)
{
  //TODO implement
  return 1;
}


int task_pause(int tid)
{
  //TODO re-implement
  int idx;
  task_t *task;

  /* */
  if (!tid)
    {
      tid = task_getid();
    }

  for (idx = 0, task = &g_task_array[idx]; idx < MAX_TASKS; idx++)
    {
      task = &g_task_array[idx];

      if (task->tid == tid)
        {
          task->last_state = task->state;
          task->state = TASK_STATE_PAUSED;
          return 1;
        }
    }

  return 0;
}


int task_destroy(int tid)
{
  //TODO implement
  return 1;
}


int task_list()
{
  //TODO implement
  return 1;
}


int task_sleep(unsigned int tid, const unsigned int ticks)
{
  int idx;
  task_t *task;

  /* */
  if (!tid)
    {
      tid = task_getid();
    }

  for (idx = 0, task = &g_task_array[idx]; idx < MAX_TASKS; idx++)
    {
      task = &g_task_array[idx];

      if (task->tid == tid)
        {
          task->state = TASK_STATE_SLEEP;
          task->sleep = ticks;
          return 1;
        }
    }

  return 0;
}


inline unsigned int task_getid(void)
{
  return g_task->tid;
}


