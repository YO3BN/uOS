/*
 * main.c
 *
 *  Created on: Mar 5, 2020
 *      Author: yo3bn
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "arch.h"
#include "kernel_api.h"


static void uart_init(void)
{
  arch_uart_init();
}


static int uart_putchar(char c, FILE *f)
{
  arch_uart_byte_send(c);
  return 0;
}

semaphore_t s;
static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);


void worker_task(void *arg)
{
  int tid = task_getid();
  unsigned long time = getsysticks();
  char *const tname = task_getname(0);

  while (sem_take(&s, SEM_WAIT_FOREVER) != SEM_STATUS_WAIT)
    {
      printf("Tick: %lu => Task: %d [%s]=> WORK TODO.\n", time, tid, tname);
    }

    printf("Tick: %lu => Task: %d [%s]=> SEM WAIT.\n", time, tid, tname);
    return;
}


void ticker_task(void *arg)
{
  static int x = 1;
  int sleep_ticks = 1;

  puts("\n");
  if (x >= 4)
    {
      x = 1;
      sem_give(&s);
    }
  else
    {
      x++;
    }

  task_sleep(0, sleep_ticks);
  return;
}


void main_task(void *arg)
{
  int tid = task_getid();
  unsigned long time = getsysticks();
  char * const tname = task_getname(0);

  sem_init(&s);

  printf("################## Start ##################\n");

  task_create("TickerTsk", ticker_task, NULL);
  task_create("WorkerTsk1", worker_task, NULL);
  task_create("WorkerTsk2", worker_task, NULL);
  task_create("WorkerTsk3", worker_task, NULL);

  printf("Tick: %lu => Task: %d [%s]=> DESTROY\n", time, tid, tname);

  task_destroy(0);
  return;
}


int main(void)
{
  uart_init();
  stdout = &uart_stdout;

  kernel_init();

  task_create("MainTsk", main_task, NULL);

  kernel_start();
}

