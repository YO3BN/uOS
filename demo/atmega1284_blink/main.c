/*
 * main.c
 *
 *  Created on: Mar 5, 2020
 *      Author: yo3bn
 */

#include <stddef.h>
#include <stdio.h>


#include <util/delay.h>
#include <avr/io.h>



void first_task(void * arg)
{
  // do something
  DDRB = 0xff;
  PORTB = 0xff;
  _delay_ms(10000);
}


void second_task(void * arg)
{
  DDRB = 0xAA;
  PORTB = 0xAA;
  // do something
  //printf("I am SECOND task\n");
  //sleep(3);
}


void main(void)
{
  //TODO init stuff();

  kernel_init();

  task_create(first_task, NULL);
  task_create(second_task, NULL);

  kernel_start();
}

