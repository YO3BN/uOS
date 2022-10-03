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
#include "context.h"


/****************************************************************************
 * Public functions.
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
 *  Should not be called before kernel initialization.
 *
 ****************************************************************************/

void sem_init(semaphore_t *sem)
{
  if (!sem)
    {
      return;
    }

  sem->resources = 0;
  sem->waiting_task = 0;
}


/****************************************************************************
 * Name: sem_take
 *
 * Description:
 *  Block the current task till the specified semaphore or resource is given.
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
  task_t *task = (task_t*) g_running_task;

  if (!sem || !task)
    {
      return retval;
    }

again:
  disable_interrupts();
  if (sem->resources > 0)
    {
      sem->resources = 0;
      retval = SEM_STATUS_TOOK;
    }
  else
    {
      if (wait)
        {
          sem->waiting_task = task->id;
          task->state = TASK_STATE_SEM_WAIT;
          retval = SEM_STATUS_WAIT;
        }
      else
        {
          retval = SEM_STATUS_BUSY;
        }
    }
  enable_interrupts();

  /* FIXME Simulate blocking function here.
   * TODO Implement blocking functions.
   */

  if (retval == SEM_STATUS_WAIT)
    {
      context_switch_to_kernel();
      goto again;
    }

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

  sem->resources = 1;
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
  if (!sem)
    {
      return SEM_STATUS_ERROR;
    }

  disable_interrupts();
  sem->resources = 1;
  kput_event_crit(KERNEL_EVENT_SEM_GIVEN, (void*) sem);
  enable_interrupts();

  return SEM_STATUS_SUCCESS;
}


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

int semaphores(kernel_event_t *event)
{
  semaphore_t *sem;
  task_t *task = NULL;
  unsigned id = 0;
  int retval = 0;

  if (!event)
    {
      return 0;
    }

  if (event->type != KERNEL_EVENT_SEM_GIVEN)
    {
      return 0;
    }

  sem = (semaphore_t *) event->data;
  if (!sem)
    {
      return 0;
    }

  /* Get the waiting task for this semaphore. */

  disable_interrupts();
  id = sem->waiting_task;
  enable_interrupts();

  /* If there is nothing in the waiting queue, then some module gave the
   * semaphore and none is waiting for it.
   */

  if (id)
    {
      task = task_getby_id(id);

      /* Check if task is still alive. */

      if (task)
        {
          task->state = TASK_STATE_READY;
          retval = 1;
        }
    }

  /* Clear this event, even if the task was found or not.
   * This will prevent the kernel to enter into a never-ending loop by
   * processing it over and over again.
   *
   * TODO: Not sure if this is the correct solution. Please, find a better
   * approach if possible. Maybe mark it as consumed?
   */

  kmemset((void*) event, 0, sizeof(kernel_event_t));

  return retval;
}
