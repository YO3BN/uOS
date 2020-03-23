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
#include "timers.h"


static void uart_init(void)
{
  arch_uart_init();
}


static int uart_putchar(char c, FILE *tnameeam)
{
  arch_uart_byte_send(c);
  return 0;
}


static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);


void first_task(void *arg)
{
  int time = g_systicks / 125;
  int tid = task_getid();
  char *const tname = task_getname(0);

  printf("Time: %ds => Task: %d [%s]=> PAUSE.\n",time, tid, tname);

  return (void) task_pause(0);
}


void second_task(void *arg)
{
  static int x = 1;

  int time = g_systicks / 125;
  int tid = task_getid();
  //int tid2 = task_getby_name("1stTsk");
  char * const tname = task_getname(0);

  if (x >= 4)
    {
      x = 1;
      printf("Time: %ds => Task: %d [%s]=> RESUME Task 1\n", time, tid, tname);

      return (void) task_resume(2);
    }
  else
    {
      x++;
    }

  printf("Time: %ds => Task: %d [%s]=> SLEEP 2sec\n", time, tid, tname);

  return (void) task_sleep(0, 2 * 125);
}


void main_task(void *arg)
{
  int time = g_systicks / 125;
  int tid = task_getid();
  char * const tname = task_getname(0);

  printf("################## Start ##################\n");

  task_create("1stTsk", first_task, NULL);
  task_create("2ndTsk", second_task, NULL);

  printf("Time: %ds => Task: %d [%s]=> DESTROY\n", time, tid, tname);

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

