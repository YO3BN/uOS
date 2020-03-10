/*
 * timer.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */

volatile unsigned long g_systicks;  //todo maybe long long? or other approach!

void reset_watchdog(void)
{
  // TODO implement
}


void start_systick(void)
{
  // TODO implement
}


void start_watchdog(void)
{
  // TODO implement
}


void configure_systick(void)
{
  g_systicks = 0;

  // TODO implement
}


void configure_watchdog(void)
{
  // TODO implement
}

go_idle(){}
