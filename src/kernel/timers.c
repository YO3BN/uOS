/*
 * timer.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */

#include "arch.h"


volatile unsigned long g_systicks;  //todo maybe long long? or other approach!

inline void reset_watchdog(void)
{
  arch_reset_watchdog();
}


inline void start_systick(void)
{
  // TODO implement
  timer_start();
}


inline void start_watchdog(void)
{
  arch_start_watchdog();
}


inline void configure_systick(void)
{
  g_systicks = 0;
  tick_init();
  // TODO implement
}


inline void configure_watchdog(void)
{
  arch_configure_watchdog();
}
