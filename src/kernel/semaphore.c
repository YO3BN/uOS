//26 Mar 2020


#include "cpu.h"
#include "klib.h"
#include "kernel.h"
#include "kernel_api.h"
#include "task.h"
#include "semaphore.h"


void sem_init(semaphore_t *sem)
{
  if (!sem)
    {
      return;
    }

  sem->resources = 0;

  //TODO owner ?
}

// wait()
// atomic
SEMTAKE_T sem_take(semaphore_t *sem, int wait)
{
  SEMTAKE_T retval = SEM_TAKE_ERROR;

  if (!sem)
    {
      return SEM_TAKE_ERROR;
    }

  disable_interrupts();
  if (sem->resources > 0)
    {
      sem->resources--;
      retval = SEM_TAKE_TOOK;
    }
  else
    {
      if (wait)
        {
          //todo add task id into waiting list
          //enqueue(sem->queue, taskid);
          g_running_task->state = TASK_STATE_SEM_WAIT;
          retval = SEM_TAKE_WAIT;
        }
      else
        {
          retval = SEM_TAKE_BUSY;
        }
    }
  enable_interrupts();

  return retval;
}

// sem_give_crit ??
// signal()
// atomic
SEMGIVE_T sem_give(semaphore_t *sem)
{
  SEMGIVE_T retval = SEM_GIVE_ERROR;

  if (!sem/* || sem->owner != task_getid()*/)
    {
      return SEM_GIVE_ERROR;
    }

  disable_interrupts();
  sem->resources++;
  kput_event_crit(KERNEL_EVENT_SEM_GIVEN, (void*) sem);
  //todo need to do something with task id here?? no I think...
  //todo store owner ? only owner can give?
  retval = SEM_GIVE_OK;
  enable_interrupts();

  return retval;
}


int semaphores(kernel_event_t *event)
{
  //todo implement semaphore processing. Processing once ==> changed--; remove tid from queue

  return 0;
}

