/*
 * timer.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */

#include "arch.h"
#include "kernel_api.h"


//TODO maybe long long? or other approach!
//TODO make this static and use helper functions to set, reset, etc.
volatile unsigned long g_systicks;


void reset_watchdog(void)
{
  arch_reset_watchdog();
}


void start_systick(void)
{
  arch_start_systick();
}


void start_watchdog(void)
{
  arch_start_watchdog();
}


void configure_systick(void)
{
  arch_configure_systick();
}


void configure_watchdog(void)
{
  arch_configure_watchdog();
}


void systick(void)
{
  g_systicks++;
  kput_event_in_buffer(KERNEL_EVENT_IRQ_SYSTICK, 0);
}


unsigned long getsysticks(void)
{
  return g_systicks;
}
