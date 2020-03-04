/*
 * kernel.c
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


extern task_t task_array[MAX_TASKS];

enum {
  TASK_STATE_INVALID = -1,
  TASK_STATE_EXITED = 0,
  TASK_STATE_READY = 1,
  TASK_STATE_RUNNING,
  TASK_STATE_IO_WAIT,
  TASK_STATE_SEM_WAIT,
  TASK_STATE_SLEEP,
} task_state;

#define NULL (void*)0

void kernel_init(void)
{
  int i;

  // TODO init kernel globals
  /* TODO Configure SysTick. */
  /* TODO Configure watch dog. */

  /* Initialize the task array. */

  for (i = 0; i < MAX_TASKS; i++)
    {
      task_array[i].state = TASK_STATE_INVALID;
      task_array[i].entry_point = NULL;
      task_array[i].arg = NULL;
      task_array[i].tid = -1;
      task_array[i].saved_data = NULL;
      task_array[i].counter = 0;
    }
}


void kernel_start(void)
{
  // FIXME dummy code
  // TODO start watch dog
  // TODO Start systick

  while (1)
    {
      //wd_reset();
      //io();
      //semaphores();
      //queues();
      scheduler();
    }
}

