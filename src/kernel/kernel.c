/*
 * kernel.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */

#define MAX_TASKS 100
#define NULL (void*)0

#include "task.h"


void kernel_init(void)
{
  int i;
  unsigned char *ptr;

  // TODO init kernel globals
  /* TODO Configure SysTick. */
  /* TODO Configure watch dog. */

  /* Clear the task array. */

  ptr = (unsigned char *) task_array;
  for (i = 0; i < sizeof(task_array); i++)
    {
      ptr[i] = 0;
    }
}


void kernel_start(void)
{
  // FIXME dummy code
  // TODO start watch dog
  // TODO Start systick

  while (1)
    {
      /* Priority order. */

      //wd_reset();
      //io();
      //semaphores();
      //queues();
      //ipc();
      scheduler();
    }
}

