/*
 * klib.h
 *
 *  Created on: Mar 19, 2020
 *      Author: yo3bn
 */

#ifndef SRC_KERNEL_INCLUDE_KLIB_H_
#define SRC_KERNEL_INCLUDE_KLIB_H_


/****************************************************************************
 * Public types.
 ****************************************************************************/


/* Since the kernel does not contain any external headers,
 * the NULL should be defined.
 */

#ifndef NULL
#  define NULL ((void*)0)
#endif


/*
 * Working with non-blocking functions:
 *
 * - Since, there are no actually blocking functions, as a simple rule of
 *   thumb, each "blocking" function will return 0 when they are supposed to
 *   wait for an event.
 *
 * - Therefore when a function which is supposed to block the current task, it
 *   will return 0, the caller also need to return 0, no other code
 *   modifications in between.
 *
 * - This condition will create a chain of returning functions from the top
 *   down to the bottom, returning back to the scheduler, which will choose
 *   to run other ready task. Thus, the stack is popped and pushed like a
 *   context switch in a natural way by the compiler, but the task state
 *   must be kept the same in order to enter in the same position again.
 *
 *
 *                ...
 *                ret = sem_take(&rx_irq, SEM_WAIT_FOREVER);
 *                if (ret == 0)
 *                  {
 *                    return 0;
 *                  }
 *                if (ret == SEM_STATUS_ERROR)
 *                  {
 *                    Semaphore function error.
 *                    ...
 *                  }
 *                Do work.
 *                ...
 *
 *
 *  - A MACRO helper was defined in order to ease the coding work, as it
 *    follows.
 *  - Using the BLOCKING MACRO:
 *
 *
 *              ...
 *              BLOCKING(ret, sem_take(&rx_irq, SEM_WAIT_FOREVER));
 *              if (ret == SEM_STATUS_ERROR)
 *                {
 *                  Semaphore function error.
 *                  ...
 *                }
 *              Do work.
 *              ...
 *
 */

#define BLOCKING(ret,...) \
{                         \
  ret = (__VA_ARGS__);    \
  if (!ret)               \
    {                     \
      return 0;           \
    }                     \
}


/* Queue object used to manage the array storage. */

typedef struct
{
  void *array_ptr;
  void *read_ptr;
  void *write_ptr;
  int array_size;
  int used_size;
  int element_size;
} queue_t;


/****************************************************************************
 * Public function prototypes.
 ****************************************************************************/


/****************************************************************************
 * Name: kmemset
 *
 * Description:
 *    Set an entire block of memory to a specified byte.
 *
 * Output Parameters:
 *    ptr - Pointer to memory block.
 *    byte - The block will be filed with this byte.
 *    size - The size of the block.
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

void kmemset(void *ptr, unsigned char byte, unsigned int size);


/****************************************************************************
 * Name: kmemcpy
 *
 * Description:
 *    Copy one memory block to another.
 *
 * Output Parameters:
 *    dst - Pointer to destination memory block to copy to.
 *    src - Pointer to source memory block to copy from.
 *    size - The size of bytes to be copied.
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

void kmemcpy(void *dst, void *src, unsigned int size);


/****************************************************************************
 * Name: kstrlen
 *
 * Description:
 *    Calculate the length of a string.
 *
 * Output Parameters:
 *    str - The given string.
 *
 * Returned Value:
 *    0 - If string is NULL.
 *    Otherwise, number of characters.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

unsigned int kstrlen(const char *str);


/****************************************************************************
 * Name: kstrncpy
 *
 * Description:
 *    Copy a string.
 *
 * Output Parameters:
 *    dst - Pointer to destination string to copy to.
 *    src - Pointer to source string to copy from.
 *    size - The max. size in bytes to be copied.
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

void kstrncpy(char *dst, const char *src, unsigned int size);


/****************************************************************************
 * Name: kstreq
 *
 * Description:
 *    Verify if two strings are equal.
 *
 * Output Parameters:
 *    str1 - Pointer to string one.
 *    str2 - Pointer to string two.
 *
 * Returned Value:
 *    1 - If the two strings contain same characters.
 *    0 - If error.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

int kstreq(const char *str1, const char *str2);


/****************************************************************************
 * Name: kqueue_init
 *
 * Description:
 *    Queue initialization.
 *    This queue store the data into given array (Queue over Array).
 *
 * Parameters:
 *    queue - Pointer to queue object.
 *    array - Pointer to storage array.
 *    array_size - The size of storage array.
 *    element_size - The size of one element in array.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - If error.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

int kqueue_init
    (queue_t *queue, void *array, int array_size, int element_size);


/****************************************************************************
 * Name: kenqueue
 *
 * Description:
 *    Insert one element in storage array.
 *    This queue store the data into given array (Queue over Array).
 *
 * Parameters:
 *    queue - Pointer to queue object.
 *    indata - Pointer to data to insert.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - If error.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

int kenqueue(queue_t *queue, void *indata);


/****************************************************************************
 * Name: kdequeue
 *
 * Description:
 *    Retrieve one element in storage array.
 *    This queue store the data into given array (Queue over Array).
 *
 * Parameters:
 *    queue - Pointer to queue object.
 *    outdata - Pointer to variable where data will be written.
 *
 * Returned Value:
 *    1 - For success.
 *    0 - If error.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

int kdequeue(queue_t *queue, void *outdata);


/****************************************************************************
 * Name: kqueue_get_usedsize
 *
 * Description:
 *    Get the number of elements stored into the queue.
 *    This queue store the data into given array (Queue over Array).
 *
 * Parameters:
 *    queue - Pointer to queue object.
 *
 *
 * Returned Value:
 *    Number of elements stored in the queue are returned.
 *    -1 - If error.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

int kqueue_get_usedsize(queue_t *queue);


/****************************************************************************
 * Name: kqueue_get_freesize
 *
 * Description:
 *    Get the number of unused elements in the queue.
 *    This queue store the data into given array (Queue over Array).
 *
 * Parameters:
 *    queue - Pointer to queue object.
 *
 * Returned Value:
 *    Number of unused elements in the queue are returned.
 *    -1 - If error.
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

int kqueue_get_freesize(queue_t *queue);


/****************************************************************************
 * Name: kqueue_destroy
 *
 * Description:
 *    Queue de-initialization.
 *    Clear the queue parameters, pointers, etc.
 *    This queue store the data into given array (Queue over Array).
 *
 * Parameters:
 *    queue - Pointer to queue object.
 *
 * Returned Value:
 *    none
 *
 * Assumptions:
 *    none
 *
 ****************************************************************************/

void kqueue_destroy(queue_t *queue);


#endif /* SRC_KERNEL_INCLUDE_KLIB_H_ */
