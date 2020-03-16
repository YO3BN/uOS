/*
 * task.h
 *
 *  Created on: Mar 5, 2020
 *      Author: yo3bn
 */

#ifndef TASK_H_
#define TASK_H_
#define NULL (void*)0
#define MAX_TASKS 10
#define CONFIG_TASK_MAX_NAME 10

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


typedef struct
{
  char task_name[CONFIG_TASK_MAX_NAME + 1];
  void *arg;
  void *saved_data;
  void *entry_point;

  unsigned int tid;
  unsigned int idx;
  unsigned int hit;

  unsigned long wakeup_ticks;

  task_state_t state;
  task_state_t last_state;
} task_t;


extern task_t *g_running_task;


int task_resume(int tid);
int task_pause(int tid);
int task_destroy(int tid);


unsigned int task_getid(void);


#endif /* TASK_H_ */
