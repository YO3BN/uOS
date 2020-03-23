/*
 * interrups.c
 *
 *  Created on: Mar 20, 2020
 *      Author: yo3bn
 */

#include "arch.h"
#include <avr/interrupt.h>



ISR(TIMER1_OVF_vect)
{
  systick_push();
}
