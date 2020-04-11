/*
 * template.c
 *
 *  Created on: Apr 11, 2020
 *      Author: yo3bn
 */


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "arch.h"
#include "kernel_api.h"


/****************************************************************************
 * Public Functions
 ****************************************************************************/


/****************************************************************************
 * Name: main
 *
 * Description:
 *    Main function.
 *    Do initializations, set up task arguments, starting kernel.
 *
 * Input Parameters:
 *    none
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    Should never return.
 *
 ****************************************************************************/

int main(void)
{
  /* Variables. */

  /* Initializations. */

  /* Setup task arguments. */

  /* Kernel initialization. */

  kernel_init();

  /* Creating tasks. */

  /* Starting the never-ending kernel loop. */

  kernel_start();

  /* Should not reach here. */

  return 0;
}

