/*
 * task.h
 *
 *  Created on: Mar 5, 2020
 *      Author: yo3bn
 */

#ifndef TASK_H_
#define TASK_H_


typedef struct task_tag
{
  int tid;
  int state;
  int hit;
  int delay;
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
} task_state;

extern task_t task_array[MAX_TASKS];

#endif /* TASK_H_ */
