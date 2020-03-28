//26 Mar 2020


#include "cpu.h"
#include "klib.h"
#include "kernel.h"
#include "kernel_api.h"
#include "task.h"
#include "semaphore.h"


static void sem_add_waitingtask(semaphore_t *sem, unsigned tidx)
{
  if (!sem)
    {
      return;
    }

  sem->tasks[tidx] = 1;
}


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


void sem_init(semaphore_t *sem)
{
  if (!sem)
    {
      return;
    }

  sem->resources = 0;
  kmemset((void*) sem->tasks, 0, sizeof(sem->tasks));
}


SEMTAKE_T sem_take(semaphore_t *sem, int wait)
{
  SEMTAKE_T retval = SEM_TAKE_ERROR;
  unsigned tidx = g_running_task->idx;

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
          sem_add_waitingtask(sem, tidx);
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

void sem_giveISR(semaphore_t *sem)
{
  if (!sem)
    {
      return;
    }

  sem->resources++;
  kput_event_crit(KERNEL_EVENT_SEM_GIVEN, (void*) sem);
}


SEMGIVE_T sem_give(semaphore_t *sem)
{
  SEMGIVE_T retval = SEM_GIVE_ERROR;

  if (!sem)
    {
      return SEM_GIVE_ERROR;
    }

  disable_interrupts();
  sem->resources++;
  kput_event_crit(KERNEL_EVENT_SEM_GIVEN, (void*) sem);
  retval = SEM_GIVE_OK;
  enable_interrupts();

  return retval;
}


int semaphores(kernel_event_t *event)
{
  /* TODO: Nothing to process here yet. */

  return 0;
}

