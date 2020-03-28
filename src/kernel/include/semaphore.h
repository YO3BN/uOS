//26 Mar 2020

//todo add header flag defines

#include "config.h"

typedef volatile struct
{
  int resources;
  unsigned tasks[CONFIG_MAX_TASKS];
} semaphore_t;


typedef enum
{
  SEM_TAKE_ERROR,
  SEM_TAKE_TOOK,
  SEM_TAKE_BUSY,
  SEM_TAKE_FREE,
  SEM_TAKE_WAIT,
  //todo busy
  //todo success
} SEMTAKE_T;


typedef enum
{
  SEM_GIVE_ERROR,
  SEM_GIVE_OK,
  SEM_GIVE_FULL,
} SEMGIVE_T;

//wait type

//wait timeout


int semaphores(kernel_event_t *event);
SEMGIVE_T sem_give(semaphore_t *sem);
SEMTAKE_T sem_take(semaphore_t *sem, int wait);
void sem_init(semaphore_t *sem);
int sem_pop_waitingtask(semaphore_t *sem, unsigned tidx);
void sem_giveISR(semaphore_t *sem);
