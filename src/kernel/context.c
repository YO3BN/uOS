/*
 * context.c
 *
 *  Created on: Apr 9, 2020
 *      Author: yo3bn
 */


/****************************************************************************
 * Included Files.
 ****************************************************************************/

#include "config.h"
#include "arch.h"
#include "cpu.h"
#include "private.h"
#include "kernel.h"
#include "timers.h"
#include "task.h"
#include "scheduler.h"
#include "semaphore.h"
#include "klib.h"
#include "context.h"


/****************************************************************************
 * Public Functions.
 ****************************************************************************/


/****************************************************************************
 * Name: yield
 *
 * Description:
 *    Switch context to kernel/scheduler manually.
 *
 * Input Parameters:
 *    None
 *
 * Returned Value:
 *    None
 *
 * Assumptions:
 *    This function should be called from a task context.
 *    Context saving and restoring is using a global variable to store
 *    the stack pointer.
 *
 ****************************************************************************/

void yield(void)
{
  /* WARNING: NAKED Function.
   *
   * DO NOT ADD ANY CODE HERE !!!
   *
   * The Returning Address was just stored onto the stack at this point.
   * This is used to return back to the next instruction bellow CALL
   * instruction were this function was called.
   * Just proceed to save the context and Stack Pointer.
   */

  SAVE_CONTEXT();

  /* Switch from task to kernel by exchanging the stack pointers. */

  g_running_task->stack_pointer = (unsigned char*) g_stack_pointer;
  g_stack_pointer = g_task_list_head->stack_pointer;

  /* Restore context based on stack pointer. */

  RESTORE_CONTEXT();

  /* Go to the saved Returning Address. */

  RETURN();
}


/****************************************************************************
 * Name: context_switch_to_kernel
 *
 * Description:
 *    Switch to kernel context.
 *
 * Input Parameters:
 *    None
 *
 * Returned Value:
 *    None
 *
 * Assumptions:
 *    Context saving and restoring is using a global variable to store
 *    the stack pointer.
 *
 ****************************************************************************/

void context_switch_to_kernel(void)
{
  /* WARNING: NAKED Function.
   *
   * DO NOT ADD ANY CODE HERE !!!
   *
   * The Returning Address was just stored onto the stack at this point.
   * This is used to return back to the next instruction bellow CALL
   * instruction were this function was called.
   * Just proceed to save the context and Stack Pointer.
   */

  SAVE_CONTEXT();

  /* Switch from task to kernel by exchanging the stack pointers. */

  g_running_task->stack_pointer = (unsigned char*) g_stack_pointer;
  g_stack_pointer = g_task_list_head->stack_pointer;

  /* Restore context based on stack pointer. */

  RESTORE_CONTEXT();

  /* Go to the saved Returning Address. */

  RETURN();
}


/****************************************************************************
 * Name: context_switch_to_task
 *
 * Description:
 *    Switch context to task.
 *
 * Input Parameters:
 *    None
 *
 * Returned Value:
 *    None
 *
 * Assumptions:
 *    Context saving and restoring is using a global variable to store
 *    the stack pointer.
 *
 ****************************************************************************/

void context_switch_to_task(void)
{
  /* WARNING: NAKED Function.
   *
   * DO NOT ADD ANY CODE HERE !!!
   *
   * The Returning Address was just stored onto the stack at this point.
   * This is used to return back to the next instruction bellow CALL
   * instruction were this function was called.
   * Just proceed to save the context and Stack Pointer.
   */

  SAVE_CONTEXT();

  /* Switch from kernel to task by exchanging the stack pointers. */

  g_task_list_head->stack_pointer = (unsigned char*) g_stack_pointer;
  g_stack_pointer = g_running_task->stack_pointer;

  /* Restore context based on stack pointer. */

  RESTORE_CONTEXT();

  /* Go to the saved Returning Address. */

  RETURN();
}


/****************************************************************************
 * Name: exec_kernel
 *
 * Description:
 *    Run the kernel main loop.
 *
 *    Since it was created as a task, the kernel should be executed as if
 *    it has already saved its context onto the stack. In this case, here
 *    only restore its content and do not save context for main() function
 *    because the loop will never return.
 *
 * Input Parameters:
 *    None
 *
 * Returned Value:
 *    None
 *
 * Assumptions:
 *    This function will never return.
 *    It should be called at the end of main();
 *
 ****************************************************************************/

void exec_kernel(void)
{
  /* Set the Stack Pointer to point from the newly created kernel task. */

  g_stack_pointer = g_task_list_head->stack_pointer;

  /* Restore context based on stack pointer. */

  RESTORE_CONTEXT();

  /* Go to the saved Returning Address. */

  RETURN();
}
