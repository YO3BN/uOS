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
#include "semaphore.h"


/****************************************************************************
 * Public prototypes.
 ****************************************************************************/

/****************************************************************************
 * Name: kput_event_crit
 *
 * Description:
 *    Insert new event in the circular buffer.
 *    This function is used in critical sections and ISR.
 *
 * Input Parameters:
 *    type - Event type.
 *    data - Pointer to optional data.
 *           Or it can be casted to other type smaller or equal than void
 *           pointer. (char, int, void*).
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    This should be called only from critical section or ISR context.
 *
 ****************************************************************************/

void kput_event_crit(unsigned char type, void * data);


/****************************************************************************
 * Name: kput_event
 *
 * Description:
 *    Insert new event in the circular buffer (no critical).
 *
 * Input Parameters:
 *    type - Event type.
 *    data - Pointer to optional data.
 *           Or it can be casted to other type smaller or equal than void
 *           pointer. (char, int, void*).
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    This should be called ONLY from kernel context, not from ISR.
 *
 ****************************************************************************/

void kput_event(unsigned char type, void * data);


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
 *    Call this function at the end of the main function.
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
 * Name: sem_init
 *
 * Description:
 *  Initialize a semaphore.
 *
 * Input Parameters:
 *  sem - Pointer to semaphore.
 *
 * Returned Value:
 *  none
 *
 * Assumptions:
 *
 ****************************************************************************/

void sem_init(semaphore_t *sem);


/****************************************************************************
 * Name: sem_take
 *
 * Description:
 *  Take the semaphore using a waiting type.
 *  This function does not block, but it will return a specific state for
 *  waiting.
 *
 * Input Parameters:
 *  sem - Semaphore pointer.
 *  wait - Wait type. Waiting forever, or no waiting at all.
 *
 * Returned Value:
 *  SEM_STATUS_ERROR - If error encountered.
 *  SEM_STATUS_TOOK - If semaphore was took.
 *  SEM_STATUS_WAIT - If the semaphore is not available.
 *                    The task will not be called after exit, thus until
 *                    semaphore is available.
 *  SEM_STATUS_BUSY - If the waiting type is WAIT_NO, this is returned when
 *                    the semaphore is not available.
 *
 * Assumptions:
 *  Called only from a valid task, not from kernel, nor from ISR.
 *
 ****************************************************************************/

SEM_STATUS_T sem_take(semaphore_t *sem, SEM_WAIT_T wait);


/****************************************************************************
 * Name: sem_giveISR
 *
 * Description:
 *  Give semaphore from ISR.
 *
 * Input Parameters:
 *  sem - Pointer to semaphore.
 *
 * Returned Value:
 *  none
 *
 * Assumptions:
 *  Should be called from ISR ONLY.
 *
 ****************************************************************************/

void sem_giveISR(semaphore_t *sem);


/****************************************************************************
 * Name: sem_give
 *
 * Description:
 *  Give semaphore for a resource.
 *
 * Input Parameters:
 *  sem - Pointer to semaphore.
 *
 * Returned Value:
 *  SEM_STATUS_OK - Success.
 *  SEM_STATUS_ERROR - If error encountered.
 *
 * Assumptions:
 *
 ****************************************************************************/

SEM_STATUS_T sem_give(semaphore_t *sem);


/****************************************************************************
 * Name: kqueue_init
 *
 * Description:
 *    Queue initialization.
 *    This queue store the data into given array (Queue over Array).
 *
 * Parameters:
 *    queue - Pointer to queue object.
 *    array - Pointer to storage array.
 *    array_size - The size of storage array.
 *    element_size - The size of one element in array.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - If error.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

int kqueue_init
    (queue_t *queue, void *array, int array_size, int element_size);


/****************************************************************************
 * Name: kenqueue
 *
 * Description:
 *    Insert one element in storage array.
 *    This queue store the data into given array (Queue over Array).
 *
 * Parameters:
 *    queue - Pointer to queue object.
 *    indata - Pointer to data to insert.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - If error.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

int kenqueue(queue_t *queue, void *indata);


/****************************************************************************
 * Name: kdequeue
 *
 * Description:
 *    Retrieve one element in storage array.
 *    This queue store the data into given array (Queue over Array).
 *
 * Parameters:
 *    queue - Pointer to queue object.
 *    outdata - Pointer to variable where data will be written.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - If error.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

int kdequeue(queue_t *queue, void *outdata);


//TODO document these
void systick(void);
unsigned long getsysticks(void);

#endif /* SRC_KERNEL_INCLUDE_KERNEL_API_H_ */
