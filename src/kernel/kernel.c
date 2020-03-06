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

  g_task = NULL;

  /* Clear the task array. */

  for (i = 0; i < sizeof(g_task_array); i++)
    {
      *(((unsigned char*) g_task_array) + i) = 0;
    }

  /* TODO Configure SysTick. */

  /* TODO Configure watch dog. */

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

