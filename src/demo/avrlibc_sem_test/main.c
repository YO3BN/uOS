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
#include "semaphore.h"


static void uart_init(void)
{
  arch_uart_init();
}


static int uart_putchar(char c, FILE *f)
{
  arch_uart_byte_send(c);
  return 0;
}


static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);

semaphore_t s;

void first_task(void *arg)
{
  unsigned long time = getsysticks();
  int tid = task_getid();
  char *const tname = task_getname(0);

  printf("Tick: %lu => Task: %d [%s]=> ALIVE.\n", time, tid, tname);

  switch (sem_take(&s, 1))
  {
    case SEM_TAKE_TOOK:
      printf("Tick: %lu => Task: %d [%s]=> WORK TODO.\n", time, tid, tname);
      return;

    case SEM_TAKE_WAIT:
      printf("Tick: %lu => Task: %d [%s]=> SEM_WAIT.\n", time, tid, tname);
      return;

    default:
      printf("plm...\n");
      return;
  }
}


void second_task(void *arg)
{
  static int x = 1;

  unsigned long time = getsysticks();
  int tid = task_getid();
  int tid2 = task_getidby_name("1stTsk");
  char * const tname = task_getname(0);
  int sleep_ticks = 1;

  if (x >= 4)
    {
      x = 1;
      printf("Tick: %lu => Task: %d [%s]=> RESUME Task: %d [1stTsk]\n", time, tid, tname, tid2);

      sem_give(&s);
    }
  else
    {
      x++;
    }

  printf("Tick: %lu => Task: %d [%s]=> SLEEP %d ticks\n", time, tid, tname, sleep_ticks);

  return (void) task_sleep(0, sleep_ticks);
}


void main_task(void *arg)
{
  unsigned long time = getsysticks();
  int tid = task_getid();
  char * const tname = task_getname(0);

  sem_init(&s, 1);

  printf("################## Start ##################\n");

  task_create("1stTsk", first_task, NULL);
  task_create("2ndTsk", second_task, NULL);

  printf("Tick: %lu => Task: %d [%s]=> DESTROY\n", time, tid, tname);

  return (void) task_destroy(0);
}


int main(void)
{
  uart_init();
  stdout = &uart_stdout;

  kernel_init();

  task_create("MainTsk", main_task, NULL);

  kernel_start();
}

