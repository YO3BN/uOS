This is a Cyclic Cooperative Operating System used in small systems and
low power micro-controllers.

Originally developed for ~1MHz, low-power, low electromagnetic noise
systems.

This is architecturized as subsets of finite state machines.

Tasks are cyclic and they should not do busy-waiting loops, or heavy
cpu loading work on a single call.

Thus they have to split the work in little parts,
using switch-case state machine, or other clever technique.


FEATURES:
- Super portable. Basically the only thing required to be implemented is
  the CPU initialization and the lower-half part of the drivers which are
  CPU specific.
- Big Finite State Machine approach.
- Deferred work approach.
- Based mostly on static memory.
- Fairly small footprint.


DRAWBACKS:
- Unusual task flow, relying on state machines approach.
- Short task working sections.
- No preemtion.
- No security.
- Flat binary.
- It could be inefficient on some architectures due function stack frames
  changing.


Meaning of the "Cyclic Task":
- Since this is a cooperative operating system, tasks are not preemted, they
  have to give the control to the scheduler after a little work.
- They don't have to do CPU intensive work, or other busy-waiting polling,
  because they will delay other tasks, or watch dog timout.
- The Cyclic here, means the tasks are called repetitively when they are ready
  to run. They do not need a while loop in order do a specific work, but a
  state machine which changes its state according to work progress.
- Basically, below is a very simplified view of the scheduler:
  scheduler()
  {
    for (;;)
      {
        task_one();
        task_two();
        task_n();
      }
  }


Cyclic Task Example:
- A task may look something like in the example below:
task(void *arg)
  {
    state = TASK_NONE;
    switch (state)
      {
        case TASK_NONE:
          /* First run, do the initialization part. */
          drv_ethernet_open();
          /* Set next task state. */
          state = TASK_WORK;
          /* Do not break here yet, only if you want to yield.
           * It will cause a premature return.
           * break;
           */

        case TASK_WORK:
          /* Cyclic work. */
          ...
          /* If a fatal error encountered, or all work is done,
           * destroy the task.
           */
          ...
          state = TASK_DESTROY;
          break;

       /* Task destroy. */
       default:
          /* Do some cleaunp. */
          ...
          state = TASK_NONE;
          /* Destroy now! */
          task_destroy(0);
          return;
      }
      return;
  }


Working with non-blocking functions:
- Since, there are no actually blocking functions, as a simple rule of thumb,
  each "blocking" function will return 0 when they are supposed to wait for an event.
- Therefore when a functon which is supposed to block the current task, it will
  return 0, the caller also need to return 0, no other code modifications in between.
- This condition will create a chain of returning functions from the top down to
  the bottom, returning back to the scheduler, which will choose to run other
  ready task. Thus, the stack is poped and pushed like a context switch in a natural
  way by the compiler, but the task state must be keeped the same in order to enter
  in the same position again.
- A MACRO helper was defined in order to ease the coding work, as it follows.


Non Blocking Example:
- Normally working with a blocking function can be something like this:
  ...
  ret = sem_take(&rx_irq, SEM_WAIT_FOREVER);
  if (ret == 0)
    {
      return 0;
    }
  if (ret == SEM_STATUS_ERROR)
    {
      /* Semaphore function error. */
      ...
    }
  /* Do work. */
  ...

- Using the BLOCKING MACRO:
  ...
  BLOCKING(ret, sem_take(&rx_irq, SEM_WAIT_FOREVER));
  if (ret == SEM_STATUS_ERROR)
    {
      /* Semaphore function error. */
      ...
    }
  /* Do work. */
  ...


Since the tasks are popping and pushing the stack pointer back and forth, in order
to create the context switch, for tasks with bigger stacks (a lot of local
variables), it will be an overkill to keep those variables onto the stack.
Thus, a better approach is to keep them in a static or dynamic memory structure.


Still developing ...
