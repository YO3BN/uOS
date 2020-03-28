//26 Mar 2020


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "cpu.h"
#include "klib.h"
#include "kernel.h"
#include "kernel_api.h"
#include "task.h"
#include "semaphore.h"


/****************************************************************************
 * Public functions.
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

int sem_pop_waitingtask(semaphore_t *sem, unsigned tidx)
{
  unsigned retval = 0;

  if (!sem)
    {
      return retval;
    }

  retval = sem->tasks[tidx];
  sem->tasks[tidx] = 0;

  return retval;
}


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

void sem_init(semaphore_t *sem)
{
  if (!sem)
    {
      return;
    }

  sem->resources = 0;
  kmemset((void*) sem->tasks, 0, sizeof(sem->tasks));
}


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

SEM_STATUS_T sem_take(semaphore_t *sem, SEM_WAIT_T wait)
{
  SEM_STATUS_T retval = SEM_STATUS_ERROR;
  unsigned tidx = g_running_task->idx;

  if (!sem)
    {
      return retval;
    }

  disable_interrupts();
  if (sem->resources > 0)
    {
      sem->resources--;
      retval = SEM_STATUS_TOOK;
    }
  else
    {
      if (wait)
        {
          /* Map the task to waiting queue in O(1), using task idx. */

          sem->tasks[tidx] = 1;

          g_running_task->state = TASK_STATE_SEM_WAIT;
          retval = SEM_STATUS_WAIT;
        }
      else
        {
          retval = SEM_STATUS_BUSY;
        }
    }
  enable_interrupts();

  return retval;
}


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

void sem_giveISR(semaphore_t *sem)
{
  if (!sem)
    {
      return;
    }

  sem->resources++;
  kput_event_crit(KERNEL_EVENT_SEM_GIVEN, (void*) sem);
}


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

SEM_STATUS_T sem_give(semaphore_t *sem)
{
  SEM_STATUS_T retval = SEM_STATUS_ERROR;

  if (!sem)
    {
      return retval;
    }

  disable_interrupts();
  sem->resources++;
  kput_event_crit(KERNEL_EVENT_SEM_GIVEN, (void*) sem);
  retval = SEM_STATUS_OK;
  enable_interrupts();

  return retval;
}
