/*
 * interrups.c
 *
 *  Created on: Mar 20, 2020
 *      Author: yo3bn
 */

#include "arch.h"
#include <avr/interrupt.h>
#include "kernel_api.h"


ISR(TIMER1_OVF_vect)
{
  systick();
}
