/*
 * task.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */

extern volatile unsigned long g_systicks;
#define MAX_TASKS 100

#include "task.h"


task_t *g_running_task;
static task_t g_task_array[MAX_TASKS];


int task_create(const char *task_name, void (*entry_point)(void*), void *arg)
{
  int i;

  if (!entry_point || !task_name)
    {
      return 0;
    }

  /* Find the first free array element. */

  for (i = 0; i < MAX_TASKS &&
      g_task_array[i].state != TASK_STATE_INVALID &&
      g_task_array[i].state != TASK_STATE_EXITED; i++);

  /* Check if array is full. */

  if (i >= MAX_TASKS)
    {
      return 0;
    }

  /* Create/copy task attributes. */

  g_task_array[i].idx = i;
  g_task_array[i].tid = i + 1; // TODO FIXME overflow here
  g_task_array[i].state = TASK_STATE_READY;
  g_task_array[i].entry_point = entry_point;
  strncpy(g_task_array[i].task_name, task_name, CONFIG_TASK_MAX_NAME + 1);

  return 1;
}


int task_getnext(task_t **task)
{
  int idx;

  if (!task)
    {
      return 0;
    }

  /* If the *task is NULL, then go for the first task. TODO recomment this */

  if (*task == NULL)
    {
      idx = 0;
    }
  else
    {
      idx = (*task)->idx + 1;
    }

  /* Check to not exceed array boundary. */

  if (idx >= MAX_TASKS)
    {
      return 0;
    }

  if (g_task_array[idx].state == TASK_STATE_INVALID)
    {
      return 0;
    }

  *task = &g_task_array[idx];

  return 1;
}


task_t *task_getby_id(int tid)
{
  task_t *task = NULL;

  if (!tid)
    {
      return NULL;
    }

  while (task_getnext(&task))
    {
      if (task->tid == tid)
        {
          break;
        }
    }

  return task;
}


char *const task_getname(int tid)
{
  task_t *task = NULL;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (!task)
    {
      return NULL;
    }

  return task->task_name;
}


int task_resume(int tid)
{
  task_t *task = NULL;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (task)
    {
      task->state = task->last_state;
      task->state = TASK_STATE_RESUMED;
      return 1;
    }

  return 0;
}


int task_pause(int tid)
{
  task_t *task = NULL;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (task)
    {
      task->last_state = task->state;
      task->state = TASK_STATE_PAUSED;
      return 1;
    }

  return 0;
}


int task_destroy(int tid)
{
  task_t *task;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (task)
    {
      //TODO implement
      task->state = TASK_STATE_EXITED;
      return 1;
    }

  return 0;
}


int task_sleep(unsigned int tid, const unsigned int ticks)
{
  task_t *task;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (task)
    {
      task->state = TASK_STATE_SLEEP;
      task->wakeup_ticks = ticks + g_systicks; // TODO this in critical section
      return 1;
    }

  return 0;
}


inline unsigned int task_getid(void)
{
  return g_running_task->tid;
}


