/*
 * task.c
 *
 *  Created on: Mar 4, 2020
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
#include "task.h"
#include "timers.h"
#include "klib.h"
#include "context.h"


/****************************************************************************
 * Public globals.
 ****************************************************************************/

volatile task_t *g_task_list_head;
volatile task_t *g_running_task;
volatile unsigned char *g_stack_pointer;
volatile unsigned char *g_stack_head;


/****************************************************************************
 * Private globals.
 ****************************************************************************/


/****************************************************************************
 * Public functions.
 ****************************************************************************/


/****************************************************************************
 * Name: task_create
 *
 * Description:
 *    Create a task.
 *    Configure and insert it into kernel's task list/array.
 *
 * Input Parameters:
 *    name - Short name of task. See, CONFIG_TASK_MAX_NAME.
 *    func - Pointer to the main function of the task.
 *    arg - Given argument for the task.
 *    stack_size - Stack size in bytes. If no size was specified, default
 *        stack size value is used. See, CONFIG_STACK_DEFAULT_SIZE.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *    Task name and entry point are mandatory.
 *
 ****************************************************************************/

int task_create(const char *name, void (*func)(void*),
                void *arg, int stack_size)
{
  int i;
  int id;
  int stack_used;
  task_t *task = NULL;
  task_t *prev_task = NULL;

  /* Sanity Checks. */

  if (!func || !name)
    {
      return 0;
    }

  /* If the task creation was requested to have no stack size,
   * the default value of the stack size will be used.
   */

  if (!stack_size)
    {
      stack_size = CONFIG_STACK_DEFAULT_SIZE;
    }

  /* TODO Find first a free stack space which fulfill the space
   * required for the stack size of this task.
   */

  id = 0;
  stack_used = 0;
  task = (task_t*) g_task_list_head;

  while (task)
    {
      stack_used += task->stack_size + sizeof(task_t);
      prev_task = task;
      task = task->next;
      id++;
    }

  /* Setup the task. */

  task = (task_t*) (g_stack_head - stack_used - sizeof(task_t) + 1);
  task->next = NULL;
  task->id = id;
  task->arg = arg;
  task->state = TASK_STATE_READY;
  task->stack_size = stack_size;
  task->stack_pointer = (unsigned char*) (g_stack_head - stack_used - sizeof(task_t));
  kstrncpy(task->name, name, CONFIG_TASK_MAX_NAME + 1);

  /* Make previous task to point to this new one. */

  if (prev_task)
    {
      prev_task->next = task;
    }

  /* If no task were created so far, make the head of task list to
   * point to this task.
   */

  if (!g_task_list_head)
    {
      g_task_list_head = task;
    }

  /* Clear stack for the new task. */

  for (i = 0; i < stack_size; i++)
    {
      *(task->stack_pointer - i) = 0;
    }

  /* Since the tasks are always executed with context-switch and NOT by simply
   * calling the function, here a clean context is created onto the stack
   * and the function pointer set as the return address.
   *
   * Thus, at first run, the registers are popped from the stack,
   * containing zeroes, since they were previously cleared, then the returning
   * address represents actually the function pointer, which in this case
   * enter to function itself for the first run.
   */

  //TODO these are architecture dependent. Move them in arch!!

  /* Put the return address to point to task function pointer. */

  *task->stack_pointer-- = ((unsigned char) ((unsigned int)func));
  *task->stack_pointer-- = (unsigned char) (((unsigned int)func) >> 8);

  /* Simulate, clean registers were pushed already onto the stack. */

  task->stack_pointer -= 32 + 1; // R0-R31 + SREG

  return 1;
}


/****************************************************************************
 * Name: task_getnext
 *
 * Description:
 *    Get next following task from the given one.
 *
 * Input/Output Parameters:
 *    task - As an INPUT, it takes address to a pointer pointing to an initial
 *            task.
 *         - As an OUTPUT, it modifies the given pointer to point to the
 *            next task following the initial one.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *    If the given pointer at address is NULL, then the address of the first
 *    task in the list/array is returned.
 *
 ****************************************************************************/

int task_getnext(task_t **task)
{
  if (!task)
    {
      return 0;
    }

  /* If the *task is given NULL, it means that first task is requested.
   * Also, skip the first task, the kernel.
   */

  if (!(*task))
    {
      *task = (task_t*) g_task_list_head;
    }

  /* Move pointer to next task in the list. */

  *task = (*task)->next;

  /* Check  for return value. */

  if (*task)
    {
      return 1;
    }

  return 0;
}


/****************************************************************************
 * Name: task_getby_id
 *
 * Description:
 *    Get a pointer to a task specifying the task id.
 *
 * Input Parameters:
 *    id - Given task ID.
 *
 * Returned Value:
 *    Valid pointer to task - For success.
 *    NULL - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

task_t *task_getby_id(int id)
{
  task_t *task = NULL;

  if (!id)
    {
      return NULL;
    }

  while (task_getnext(&task))
    {
      if (task->id == id)
        {
          break;
        }
    }

  return task;
}


/****************************************************************************
 * Name: task_getby_name
 *
 * Description:
 *    Get a pointer to a task specifying the task name.
 *
 * Input Parameters:
 *    name - Task name to search for.
 *
 * Returned Value:
 *    Valid pointer to task - For success.
 *    NULL - For errors.
 *
 * Assumptions:
 *    If there are many tasks with the same name, only first one is returned.
 *
 ****************************************************************************/

task_t *task_getby_name(const char *name)
{
  task_t *task = NULL;

  if (!name)
    {
      return NULL;
    }

  while (task_getnext(&task))
    {
      if (kstreq(name, task->name))
        {
          break;
        }
    }

  return task;
}


/****************************************************************************
 * Name: task_getname
 *
 * Description:
 *    Get a pointer to a task name specifying the task id.
 *
 * Input Parameters:
 *    id - Given task ID.
 *
 * Returned Value:
 *    Valid pointer to task name.
 *    NULL - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

char *const task_getname(int id)
{
  task_t *task = NULL;

  if (!id)
    {
      id = task_getid();
    }

  task = task_getby_id(id);

  if (!task)
    {
      return NULL;
    }

  return task->name;
}


/****************************************************************************
 * Name: task_resume
 *
 * Description:
 *    Resume a task specifying the task id.
 *
 * Input Parameters:
 *    id - Given task ID.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

int task_resume(int id)
{
  int ret = 0;
  task_t *task = NULL;

  if (!id)
    {
      id = task_getid();
    }

  task = task_getby_id(id);

  if (task)
    {
      if (task->state == TASK_STATE_PAUSED)
        {
          task->state = TASK_STATE_RESUMED;
          ret = 1;
        }
    }

  return ret;
}


/****************************************************************************
 * Name: task_pause
 *
 * Description:
 *    Pause a task specifying the task id.
 *
 * Input Parameters:
 *    id - Given task ID.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

int task_pause(int id)
{
  //TODO Not supported in actual implementation.
#if 0
  task_t *task = NULL;

  if (!id)
    {
      id = task_getid();
    }

  task = task_getby_id(id);

  if (task)
    {
      task->last_state = task->state;
      task->state = TASK_STATE_PAUSED;
      return 1;
    }
#endif
  return 0;
}


/****************************************************************************
 * Name: task_destroy
 *
 * Description:
 *    Destroy a task specifying the task id.
 *
 * Input Parameters:
 *    id - Given task ID.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

int task_destroy(int id)
{
  // TODO Not supported yet -> Removing from stack.
#if 0
  task_t *task = NULL;

  if (!id)
    {
      id = task_getid();
    }

  task = task_getby_id(id);

  if (task)
    {
      /* Remove task id, task name and set state to EXITED,
       * thus others should not be able to find this task
       * anymore by name or id.
       */

      task->id = 0;
      task->state = TASK_STATE_EXITED;
      kmemset(task->name, 0, CONFIG_TASK_MAX_NAME + 1);
      return 1;
    }
#endif
  return 0;
}


/****************************************************************************
 * Name: task_sleep
 *
 * Description:
 *    Put a task in sleep mode specifying the task id.
 *
 * Input Parameters:
 *    id - Given task ID.
 *    ticks - Number of ticks to sleep.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

int task_sleep(unsigned int id, const unsigned int ticks)
{
  task_t *task;

  if (!id)
    {
      id = task_getid();
    }

  task = task_getby_id(id);

  if (task)
    {
      //TODO only if ready or running
      task->state = TASK_STATE_SLEEP;
      task->wakeup_ticks = ticks + g_systicks; // TODO this in critical section

      /* FIXME Simulate blocking function.
       * TODO Implement blocking functions.
       */

      context_switch_to_kernel();
    }

  return 0;
}


/****************************************************************************
 * Name: task_getid
 *
 * Description:
 *    Get the task ID of the current running task (the caller function).
 *
 * Input Parameters:
 *    none
 *
 * Returned Value:
 *    Task ID.
 *
 * Assumptions:
 *    Called only from a valid running task.
 *
 ****************************************************************************/

unsigned int task_getid(void)
{
  if (!g_running_task)
    {
      return 0;
    }

  return g_running_task->id;
}


/****************************************************************************
 * Name: task_getidby_name
 *
 * Description:
 *    Get the task ID by named by given argument "name".
 *
 * Input Parameters:
 *    name - Task name to search for.
 *
 * Returned Value:
 *    Task ID.
 *
 * Assumptions:
 *    If there are many tasks with the same name, only first one is returned.
 *
 ****************************************************************************/

unsigned int task_getidby_name(const char *name)
{
  unsigned int id = 0;
  task_t *task = NULL;

  if (!name)
    {
      return 0;
    }

  while (task_getnext(&task))
    {
      if (kstreq(name, task->name))
        {
          id = task->id;
          break;
        }
    }

  return id;
}

