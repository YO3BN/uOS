/*
 * cpu.c
 *
 *  Created on: Mar 20, 2020
 *      Author: yo3bn
 */

#include "arch.h"
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>


void arch_enable_interrupts(void)
{
  sei();
}


void arch_disable_interrupts(void)
{
  cli();
}


void arch_go_idle(void)
{
  //TODO re-implement!

  cli();

  /* Save MCUCR, it will be restored back later */

  uint8_t mcucr = MCUCR;

  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  sei();

  sleep_cpu();

  /* We don't need to use sleep_disable(),
   * just restoring the MCUCR register.
   */

  MCUCR = mcucr;
}
