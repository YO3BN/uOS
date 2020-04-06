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


/****************************************************************************
 * Public globals.
 ****************************************************************************/

task_t *g_running_task;
int current_running = 1;

/****************************************************************************
 * Private globals.
 ****************************************************************************/

task_t g_task_array[CONFIG_MAX_TASKS];
volatile unsigned char *stack_global;

/****************************************************************************
 * Public functions.
 ****************************************************************************/

void __attribute__((naked)) yield(void)
{
  asm volatile (
      "push r31\n\t"
      "push r30\n\t"
      "push r29\n\t"
      "push r28\n\t"
      "push r27\n\t"
      "push r26\n\t"
      "push r25\n\t"
      "push r24\n\t"
      "push r23\n\t"
      "push r22\n\t"
      "push r21\n\t"
      "push r20\n\t"
      "push r19\n\t"
      "push r18\n\t"
      "push r17\n\t"
      "push r16\n\t"
      "push r15\n\t"
      "push r14\n\t"
      "push r13\n\t"
      "push r12\n\t"
      "push r11\n\t"
      "push r10\n\t"
      "push r9\n\t"
      "push r8\n\t"
      "push r7\n\t"
      "push r6\n\t"
      "push r5\n\t"
      "push r4\n\t"
      "push r3\n\t"
      "push r2\n\t"
      "push r1\n\t"
      "push r0\n\t"
      );

  g_task_array[current_running].sp = (unsigned char*) SP;
  g_task_array[current_running].sreg = SREG;

  //////////////////////////////////////////////////////////////////

  SREG = (unsigned char) g_task_array[0].sreg;
  SP = (unsigned int) g_task_array[0].sp;

  asm volatile (
      "pop r0\n\t"
      "pop r1\n\t"
      "pop r2\n\t"
      "pop r3\n\t"
      "pop r4\n\t"
      "pop r5\n\t"
      "pop r6\n\t"
      "pop r7\n\t"
      "pop r8\n\t"
      "pop r9\n\t"
      "pop r10\n\t"
      "pop r11\n\t"
      "pop r12\n\t"
      "pop r13\n\t"
      "pop r14\n\t"
      "pop r15\n\t"
      "pop r16\n\t"
      "pop r17\n\t"
      "pop r18\n\t"
      "pop r19\n\t"
      "pop r20\n\t"
      "pop r21\n\t"
      "pop r22\n\t"
      "pop r23\n\t"
      "pop r24\n\t"
      "pop r25\n\t"
      "pop r26\n\t"
      "pop r27\n\t"
      "pop r28\n\t"
      "pop r29\n\t"
      "pop r30\n\t"
      "pop r31\n\t"
      "ret\n\t"
      );
}


void __attribute__((naked)) run_task(void)
{
  asm volatile (
      "push r31\n\t"
      "push r30\n\t"
      "push r29\n\t"
      "push r28\n\t"
      "push r27\n\t"
      "push r26\n\t"
      "push r25\n\t"
      "push r24\n\t"
      "push r23\n\t"
      "push r22\n\t"
      "push r21\n\t"
      "push r20\n\t"
      "push r19\n\t"
      "push r18\n\t"
      "push r17\n\t"
      "push r16\n\t"
      "push r15\n\t"
      "push r14\n\t"
      "push r13\n\t"
      "push r12\n\t"
      "push r11\n\t"
      "push r10\n\t"
      "push r9\n\t"
      "push r8\n\t"
      "push r7\n\t"
      "push r6\n\t"
      "push r5\n\t"
      "push r4\n\t"
      "push r3\n\t"
      "push r2\n\t"
      "push r1\n\t"
      "push r0\n\t"
      );

  g_task_array[0].sp = (unsigned char*) SP;
  g_task_array[0].sreg = SREG;

  //////////////////////////////////////////////////////////////////

  SREG = (unsigned char) g_task_array[current_running].sreg;
  SP = (unsigned int) g_task_array[current_running].sp;

  asm volatile (
      "pop r0\n\t"
      "pop r1\n\t"
      "pop r2\n\t"
      "pop r3\n\t"
      "pop r4\n\t"
      "pop r5\n\t"
      "pop r6\n\t"
      "pop r7\n\t"
      "pop r8\n\t"
      "pop r9\n\t"
      "pop r10\n\t"
      "pop r11\n\t"
      "pop r12\n\t"
      "pop r13\n\t"
      "pop r14\n\t"
      "pop r15\n\t"
      "pop r16\n\t"
      "pop r17\n\t"
      "pop r18\n\t"
      "pop r19\n\t"
      "pop r20\n\t"
      "pop r21\n\t"
      "pop r22\n\t"
      "pop r23\n\t"
      "pop r24\n\t"
      "pop r25\n\t"
      "pop r26\n\t"
      "pop r27\n\t"
      "pop r28\n\t"
      "pop r29\n\t"
      "pop r30\n\t"
      "pop r31\n\t"
      "ret\n\t"
      );
}

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
  g_task_array[i].arg = arg;
  kstrncpy(g_task_array[i].name, task_name, CONFIG_TASK_MAX_NAME + 1);


#define stack_size 256
  //task SP
  g_task_array[i].sp = (unsigned char *) stack_global;

  //clear stack
/*  for (int i = 0; i < stack_size; i++)
    {
      *((unsigned char*) g_task_array[i].sp - i) = 0;
    }*/

  //init empty context : Clean Run
  //put return address to point to entry point fun
  *g_task_array[i].sp-- = ((unsigned char) ((unsigned int)entry_point));
  *g_task_array[i].sp-- = (unsigned char) (((unsigned int)entry_point) >> 8);

  //placeholder for R0-R31 registers
  g_task_array[i].sp -= 32;

  //sreg = 0;
  g_task_array[i].sreg = 0;

  // set stack section for next task
  stack_global -= stack_size;

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
      idx = 1;
    }
  else
    {
      idx = (*task)->idx + 1;
    }

  /* Check to not exceed array boundary. */

  if (idx >= CONFIG_MAX_TASKS)
    {
      *task = NULL;
      return 0;
    }

  if (g_task_array[idx].state == TASK_STATE_INVALID)
    {
      *task = NULL;
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

  return task->name;
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
  int ret = 0;
  task_t *task = NULL;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

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
  task_t *task = NULL;

  if (!tid)
    {
      tid = task_getid();
    }

  task = task_getby_id(tid);

  if (task)
    {
      /* Remove task id, task name and set state to EXITED,
       * thus others should not be able to find this task
       * anymore by name or id.
       */

      task->tid = 0;
      task->state = TASK_STATE_EXITED;
      kmemset(task->name, 0, CONFIG_TASK_MAX_NAME + 1);
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
      //TODO only if ready or running
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

unsigned int task_getid(void)
{
  if (!g_running_task)
    {
      return 0;
    }

  return g_running_task->tid;
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
  unsigned int tid = 0;
  task_t *task = NULL;

  if (!name)
    {
      return 0;
    }

  while (task_getnext(&task))
    {
      if (kstreq(name, task->name))
        {
          tid = task->tid;
          break;
        }
    }

  return tid;
}

