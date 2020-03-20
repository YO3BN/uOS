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
#include "cpu.h"
#include "private.h"
#include "kernel.h"
#include "task.h"
#include "timers.h"
#include "klib.h"


/****************************************************************************
 * Public globals.
 ****************************************************************************/

task_t *g_running_task;


/****************************************************************************
 * Private globals.
 ****************************************************************************/

static task_t g_task_array[CONFIG_MAX_TASKS];


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
 *    task_name - Short name of task. See, CONFIG_TASK_MAX_NAME.
 *    entry_point - Pointer to task's main function.
 *    arg - Given argument for the task.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *    Task name and entry point are mandatory.
 *
 ****************************************************************************/

int task_create(const char *task_name, void (*entry_point)(void*), void *arg)
{
  int i;

  if (!entry_point || !task_name)
    {
      return 0;
    }

  /* Find the first free array element. */

  for (i = 0; i < CONFIG_MAX_TASKS &&
      g_task_array[i].state != TASK_STATE_INVALID &&
      g_task_array[i].state != TASK_STATE_EXITED; i++);

  /* Check if array is full. */

  if (i >= CONFIG_MAX_TASKS)
    {
      return 0;
    }

  /* Create/copy task attributes. */

  g_task_array[i].idx = i;
  g_task_array[i].tid = i + 1; // TODO FIXME overflow here
  g_task_array[i].state = TASK_STATE_READY;
  g_task_array[i].entry_point = entry_point;
  kstrncpy(g_task_array[i].task_name, task_name, CONFIG_TASK_MAX_NAME + 1);

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
  int idx;

  if (!task)
    {
      return 0;
    }

  /* If the *task is NULL, then go for the first task. TODO recomment this */

  if (*task == NULL)
    {
      idx = 0;
    }
  else
    {
      idx = (*task)->idx + 1;
    }

  /* Check to not exceed array boundary. */

  if (idx >= CONFIG_MAX_TASKS)
    {
      return 0;
    }

  if (g_task_array[idx].state == TASK_STATE_INVALID)
    {
      return 0;
    }

  *task = &g_task_array[idx];

  return 1;
}


/****************************************************************************
 * Name: task_getby_id
 *
 * Description:
 *    Get a pointer to a task specifying the task id.
 *
 * Input Parameters:
 *    tid - Given task ID.
 *
 * Returned Value:
 *    Valid pointer to task - For success.
 *    NULL - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

task_t *task_getby_id(int tid)
{
  task_t *task = NULL;

  if (!tid)
    {
      return NULL;
    }

  while (task_getnext(&task))
    {
      if (task->tid == tid)
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
 *    tid - Given task ID.
 *
 * Returned Value:
 *    Valid pointer to task name.
 *    NULL - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

char *const task_getname(int tid)
{
  task_t *task = NULL;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (!task)
    {
      return NULL;
    }

  return task->task_name;
}


/****************************************************************************
 * Name: task_resume
 *
 * Description:
 *    Resume a task specifying the task id.
 *
 * Input Parameters:
 *    tid - Given task ID.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

int task_resume(int tid)
{
  task_t *task = NULL;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (task)
    {
      task->state = task->last_state;
      task->state = TASK_STATE_RESUMED;
      return 1;
    }

  return 0;
}


/****************************************************************************
 * Name: task_pause
 *
 * Description:
 *    Pause a task specifying the task id.
 *
 * Input Parameters:
 *    tid - Given task ID.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

int task_pause(int tid)
{
  task_t *task = NULL;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (task)
    {
      task->last_state = task->state;
      task->state = TASK_STATE_PAUSED;
      return 1;
    }

  return 0;
}


/****************************************************************************
 * Name: task_destroy
 *
 * Description:
 *    Destroy a task specifying the task id.
 *
 * Input Parameters:
 *    tid - Given task ID.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

int task_destroy(int tid)
{
  task_t *task;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (task)
    {
      //TODO implement
      task->state = TASK_STATE_EXITED;
      return 1;
    }

  return 0;
}


/****************************************************************************
 * Name: task_sleep
 *
 * Description:
 *    Put a task in sleep mode specifying the task id.
 *
 * Input Parameters:
 *    tid - Given task ID.
 *    ticks - Number of ticks to sleep.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - For errors.
 *
 * Assumptions:
 *
 ****************************************************************************/

int task_sleep(unsigned int tid, const unsigned int ticks)
{
  task_t *task;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (task)
    {
      task->state = TASK_STATE_SLEEP;
      task->wakeup_ticks = ticks + g_systicks; // TODO this in critical section
      return 1;
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

inline unsigned int task_getid(void)
{
  return g_running_task->tid;
}


