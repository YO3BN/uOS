//26 Mar 2020

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "config.h"


typedef volatile struct
{
  int resources;
  unsigned tasks[CONFIG_MAX_TASKS];
} semaphore_t;


typedef enum
{
  SEM_STATUS_ERROR = 0,
  SEM_STATUS_OK,
  SEM_STATUS_BUSY,
  SEM_STATUS_WAIT,
  SEM_STATUS_TOOK,
} SEM_STATUS_T;

typedef enum
{
  SEM_WAIT_NO = 0,
  SEM_WAIT_FOREVER,
} SEM_WAIT_T;



/****************************************************************************
 * Public function prototypes.
 ****************************************************************************/


/****************************************************************************
 * Name: sem_pop_waitingtask
 *
 * Description:
 *  Check and remove if a task is waiting for this semaphore.
 *
 * Input Parameters:
 *  sem - Semaphore pointer.
 *  tidx - Task idx. Used to map the task to semaphore waiting queue in O(1).
 *
 * Returned Value:
 *  1 - If the task is waiting for this semaphore.
 *  0 - Otherwise.
 *
 * Assumptions:
 *  Called from Scheduler.
 *
 ****************************************************************************/

int sem_pop_waitingtask(semaphore_t *sem, unsigned tidx);


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

#endif /* SEMAPHORE_H_ */
