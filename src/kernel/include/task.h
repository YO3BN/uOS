/*
 * task.h
 *
 *  Created on: Mar 5, 2020
 *      Author: yo3bn
 */

#ifndef TASK_H_
#define TASK_H_

#include "config.h"


/* A task can enter into the following states. */

typedef enum
{
  TASK_STATE_INVALID = 0,
  TASK_STATE_EXITED,
  TASK_STATE_READY,
  TASK_STATE_RUNNING,
  TASK_STATE_IO_WAIT,
  TASK_STATE_SEM_WAIT,
  TASK_STATE_SLEEP,
  TASK_STATE_PAUSED,
  TASK_STATE_RESUMED,
} task_state_t;


/*
 * This structure will be stored at the beginning of the stack on each task.
 * Also, known as Task Control Block or TCB.
 * Therefore, the nodes on task list will point directly to this structure,
 * implicitly to the beginning of the stack address of the task.
 */

typedef struct task
{
  char name[CONFIG_TASK_MAX_NAME + 1];  /* Task Name String. */
  void *arg;                            /* Pointer to given argument. */
  unsigned char *stack_pointer;         /* Saved Stack Pointer. */
  unsigned int stack_size;              /* Configured Stack Size. */
  unsigned int id;                      /* Task ID. */
  unsigned long wakeup_ticks;           /* Time when the task will be woken. */
  task_state_t state;                   /* Task State (sleeping, waiting). */
  task_state_t last_state;              //TODO to be removed?
  struct task *next;                    /* Pointer to next task. */
} task_t;


/* Pointer to first node the in task list.
 * The first task, or id 0 is the kernel itself.
 */

extern volatile task_t *g_task_list_head;


/* Global pointer to the current running task. */

extern volatile task_t *g_running_task;


/* Global Stack Pointer. */

extern volatile unsigned char *g_stack_pointer;


/* Global stack starting address. */

extern volatile unsigned char *g_stack_head;


/****************************************************************************
 * Name: task_create
 *
 * Description:
 *    Create a task.
 *    Configure and insert it into kernel's task list/array.
 *
 * Input Parameters:
 *    name - Short name of task. See, CONFIG_TASK_MAX_NAME.
 *    func - Pointer to task's main function.
 *    arg - Given argument for the task.
 *    stack_size - Stack size in bytes.
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
                void *arg, int stack_size);


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

int task_getnext(task_t **task);


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

task_t *task_getby_id(int tid);


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

char *const task_getname(int tid);


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

task_t *task_getby_name(const char *name);


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

int task_resume(int tid);


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

int task_pause(int tid);


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

int task_destroy(int tid);


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

int task_sleep(unsigned int tid, const unsigned int ticks);


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

unsigned int task_getid(void);


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

unsigned int task_getidby_name(const char *name);


#endif /* TASK_H_ */
