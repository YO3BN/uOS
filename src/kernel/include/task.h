/*
 * task.h
 *
 *  Created on: Mar 5, 2020
 *      Author: yo3bn
 */

#ifndef TASK_H_
#define TASK_H_

#define MAX_TASKS 100

typedef struct
{
  unsigned int tid;
  unsigned int idx;
  unsigned int hit;
  unsigned int sleep;
  unsigned int state;
  unsigned int last_state;
  void *entry_point;
  void *arg;
  void *saved_data;
} task_t;


enum
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
} task_state;


extern task_t g_task_array[MAX_TASKS];
extern task_t *g_task;


int task_resume(int tid);
int task_pause(int tid);
int task_destroy(int tid);
int task_list(void);


unsigned int task_getid(void);


#endif /* TASK_H_ */
