/*
 * public.h
 *
 *  Created on: Mar 19, 2020
 *      Author: yo3bn
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_API_H_
#define SRC_KERNEL_INCLUDE_KERNEL_API_H_


/****************************************************************************
 * Included files from kernel.
 ****************************************************************************/

#include "kernel.h"
#include "task.h"
#include "timers.h"


/****************************************************************************
 * Public prototypes.
 ****************************************************************************/

/****************************************************************************
 * Name: kput_event_in_buffer
 *
 * Description:
 *    Insert new event in the circular buffer.
 *
 * Input Parameters:
 *    type - Event type.
 *    data - Optional data.
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    This should be called only from ISR context.
 *
 ****************************************************************************/

void kput_event_in_buffer(unsigned char type, unsigned char data);


/****************************************************************************
 * Name: kput_event_in_buffer_critical
 *
 * Description:
 *    Insert new event in the circular buffer (critical).
 *
 * Input Parameters:
 *    type - Event type.
 *    data - Optional data.
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    This should NOT be called from ISR context.
 *
 ****************************************************************************/

void kput_event_in_buffer_critical(unsigned char type, unsigned char data);


/****************************************************************************
 * Name: kernel_init
 *
 * Description:
 *    Kernel initialization.
 *
 * Input Parameters:
 *    None
 *
 * Returned Value:
 *    None
 *
 * Assumptions:
 *    This should be called at the beginning of main();
 *
 ****************************************************************************/

void kernel_init(void);


/****************************************************************************
 * Name: kernel_start
 *
 * Description:
 *    Run the kernel.
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

void kernel_start(void);


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

int task_create(const char *task_name, void (*entry_point)(void*), void *arg);


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


#endif /* SRC_KERNEL_INCLUDE_KERNEL_API_H_ */
