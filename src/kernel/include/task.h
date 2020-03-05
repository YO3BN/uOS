/*
 * task.h
 *
 *  Created on: Mar 5, 2020
 *      Author: yo3bn
 */

#ifndef TASK_H_
#define TASK_H_


typedef struct
{
  unsigned int tid;
  unsigned int idx;
  unsigned int hit;
  unsigned int delay;
  unsigned int state;
  unsigned int last_state;
  void *entry_point;
  void *arg;
  void *saved_data;
} task_t;


enum {
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

extern task_t task_array[MAX_TASKS];

int task_resume(const int tid);
int task_pause(const int tid);
int task_destroy(const int tid);
int task_list(void);

#endif /* TASK_H_ */
