/*
 * timers.c
 *
 *  Created on: Mar 20, 2020
 *      Author: yo3bn
 */


#include "arch.h"
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "kernel_api.h"


void arch_reset_watchdog(void)
{
  //TODO
}


void arch_start_watchdog(void)
{
  //TODO
}


void arch_configure_watchdog(void)
{
  //TODO
}


void arch_stop_watchdog(void)
{
  //TODO
}


void arch_start_systick(void)
{
  //TODO re-implement

  /* Start Timer/Counter1 by choosing the prescaler. */

  TCCR1B |= (uint8_t) \
      (1 << CS12) |
      (0 << CS11) |
      (0 << CS10);

  /* Reset the values. */

  TCNT1 = (uint16_t) 0;
}


void arch_configure_systick(void)
{
  //TODO re-implement

  //g_systicks = 0;

  TCCR1A = (uint8_t) \
     (0 << COM1A1) |  /* Compare Output Mode A */
     (0 << COM1A0) |  /* Compare Output Mode A */
     (0 << COM1B1) |  /* Compare Output Mode B */
     (0 << COM1B0) |  /* Compare Output Mode B */
     (0 << FOC1A)  |  /* Force Output Compare A */
     (0 << FOC1B)  |  /* Force Output Compare B */
     (0 << WGM11)  |  /* Waveform Generation Mode */
     (0 << WGM10);    /* Waveform Generation Mode */

  TCCR1B = (uint8_t) \
     (0 << ICNC1) |   /* Input Capture Noise Canceler */
     (0 << ICES1) |   /* Input Capture Edge Select */
     (0 << WGM13) |   /* Waveform Generation Mode */
     (0 << WGM12) |   /* Waveform Generation Mode */
     (0 << CS12)  |   /* Clock Select */
     (0 << CS11)  |   /* Clock Select */
     (0 << CS10);     /* Clock Select */

  TIMSK1 = (uint8_t) \
//    (0 << TICIE1) |   /* Timer/Cnt1 Input Capture Interrupt */
//    (0 << OCIE1A) |   /* Timer/Cnt1 Output Compare Match */
//    (0 << OCIE1B) |   /* Timer/Cnt1 Output Compare Match */
    (1 << TOIE1);     /* Timer/Cnt1 Overflow Interrupt Enable */

  /* Reset the values */

  TCNT1 = (uint16_t) 0;
}


void arch_stop_systick(void)
{
 /* Clearing Clock will disable timer/counter1 */
 TCCR1B &= (uint8_t) ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}
