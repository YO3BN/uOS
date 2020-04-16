/*
 * stack.c
 *
 *  Created on: Apr 10, 2020
 *      Author: yo3bn
 */


/****************************************************************************
 * Included Files.
 ****************************************************************************/

#include <avr/io.h>
#include "config.h"


/****************************************************************************
 * Public Functions.
 ****************************************************************************/


/****************************************************************************
 * Name: stack_init
 *
 * Description:
 *    Set the Stack Pointer starting address for next tasks..
 *
 * Input Parameters:
 *    Pointer to global pointer.
 *
 * Returned Value:
 *    None
 *
 * Assumptions:
 *    Should be called once from kernel initialization.
 *
 ****************************************************************************/

void stack_init(void **stack)
{
  if (!stack)
    {
      return;
    }

  //TODO maybe implement stack-growth and stack-alignment.

  /* RAMEND is defined for each MPU in AVR headers. */

  *stack = ((unsigned char*) (RAMEND - CONFIG_STACK_DEFAULT_SIZE));
}
