/*
 * main.c
 *
 *  Created on: Mar 5, 2020
 *      Author: yo3bn
 */

#include <stddef.h>
#include <stdio.h>

#include <avr/io.h>

#define MAX_TASKS 100

#include "task.h"

void first_task(void *arg)
{
  // Set de port.
  DDRB = 0xff;
  PORTB = 0xff;

  // Stop the task.
  task_pause(0);
}


void second_task(void *arg)
{
  PORTB ^= PORTB;

  task_sleep(0, 10);
}


void main(void)
{
  //TODO init stuff();

  kernel_init();

  task_create(first_task, NULL);
  task_create(second_task, NULL);

  kernel_start();
}

