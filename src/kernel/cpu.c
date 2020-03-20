/*
 * cpu.c
 *
 *  Created on: Mar 21, 2020
 *      Author: yo3bn
 */


inline void go_idle(void)
{
  //TODO implement arch_go_idle();
  tick();
}

inline void disable_interrupts(void)
{
  arch_disable_interrupts();
}


inline void enable_interrups(void)
{
  arch_enable_interrupts();
}

