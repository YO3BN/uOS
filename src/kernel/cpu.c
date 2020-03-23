/*
 * cpu.c
 *
 *  Created on: Mar 21, 2020
 *      Author: yo3bn
 */


#include "arch.h"
#include "cpu.h"


void enable_interrupts(void)
{
  arch_enable_interrupts();
}


void disable_interrupts(void)
{
  arch_disable_interrupts();
}


void go_idle(void)
{
  arch_go_idle();
}
