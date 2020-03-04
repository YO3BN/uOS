/*
 * task.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */


#define MAX_TASKS 100

typedef struct task_tag
{
  int tid;
  int state;
  int counter;
  void *entry_point;
  void *arg;
  void *saved_data;
} task_t;


task_t task_array[MAX_TASKS];


enum {
  TASK_STATE_INVALID = -1,
  TASK_STATE_EXITED = 0,
  TASK_STATE_READY = 1,
  TASK_STATE_RUNNING,
  TASK_STATE_IO_WAIT,
  TASK_STATE_SEM_WAIT,
  TASK_STATE_SLEEP,
} task_state;


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
