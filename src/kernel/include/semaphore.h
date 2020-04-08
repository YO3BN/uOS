//26 Mar 2020

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_


/****************************************************************************
 * Included Files.
 ****************************************************************************/

#include "config.h"
#include "klib.h"

/****************************************************************************
 * Defined Types.
 ****************************************************************************/


typedef volatile struct
{
  int resources;                      /* Free resources into the semaphore. */
  queue_t waiting_tasks;              /* Queue object for task IDs array. */
  unsigned task_id[CONFIG_SEM_MAX_TASKS]; /* Array storing task IDs. */
} semaphore_t;


typedef enum
{
  SEM_STATUS_ERROR = 0, /* Sem. function returned error. */
  SEM_STATUS_OK,        /* Sem. function returned success. */
  SEM_STATUS_TOOK,      /* Sem. took, resource is free, can continue. */
  SEM_STATUS_WAIT,      /* Sem. is busy, no resource available, should wait. */
  SEM_STATUS_BUSY,      /* When using no waiting, this indicates sem. busy. */
} SEM_STATUS_T;


typedef enum
{
  SEM_WAIT_NO = 0,  /* No waiting, just try to take the semaphore. */
  SEM_WAIT_FOREVER, /* Waiting forever for the semaphore. */
} SEM_WAIT_T;



/****************************************************************************
 * Public function prototypes.
 ****************************************************************************/


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
 * Name: semaphores
 *
 * Description:
 *  Process events related to semaphores.
 *
 * Input Parameters:
 *  event - Event from kernel.
 *
 * Returned Value:
 *  1 - If there is further work to do by kernel.
 *  0 - If there is no further work to do.
 *
 * Assumptions:
 *  Called from kernel loop only.
 *
 ****************************************************************************/

int semaphores(kernel_event_t *event);


#endif /* SEMAPHORE_H_ */
