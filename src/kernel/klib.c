/*
 * klib.c
 *
 *  Created on: Mar 19, 2020
 *      Author: yo3bn
 */

#include "klib.h"


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

void kmemset(void *ptr, unsigned char byte, unsigned int size)
{
  unsigned int i = 0;

  if (!ptr)
    {
      return;
    }

  for (i = 0; i < size; i++)
    {
      *(((unsigned char*) ptr) + i) = byte;
    }
}


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

void kmemcpy(void *dst, void *src, unsigned int size)
{
  unsigned int i = 0;

  if (!dst || !src)
    {
      return;
    }

  for (i = 0; i < size; i++)
    {
      *(((unsigned char*) dst) + i) = *(((unsigned char*) src) + i);
    }
}


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

unsigned int kstrlen(const char *str)
{
  const char *orig;

  if (!str)
    {
      return 0;
    }

  orig = str;
  while (*(++str));

  return str - orig;
}


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

void kstrncpy(char *dst, const char *src, unsigned int size)
{
  unsigned int i = 0;
  int len;

  if (!dst || !src)
    {
      return;
    }

  len = kstrlen(src);

  for (i = 0; (i < size - 1) && (i < len) && *(src + i); i++)
    {
      *(dst + i) = *(src + i);
    }

  *(dst + i) = '\0';
}


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

int kstreq(const char *str1, const char *str2)
{
  if (!str1 || !str2)
    {
      return 0;
    }

  if (!(*str1) || !(*str2))
    {
      return 0;
    }

  while (*str1 && *str2)
    {
      if (*str1 != *str2)
        {
          return 0;
        }

      str1++;
      str2++;
    }

  return 1;
}


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
    (queue_t *queue, void *array, int array_size, int element_size)
{
  if (!queue || !array || !array_size || !element_size)
    {
      return 0;
    }

  queue->array_ptr    = array;
  queue->read_ptr     = array;
  queue->write_ptr    = array;
  queue->array_size   = array_size;
  queue->element_size = element_size;
  queue->used_size    = 0;

  return 1;
}


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

int kenqueue(queue_t *queue, void *indata)
{
  if (!queue || !indata)
    {
      return 0;
    }

  /* Check if the array is full. */

  if (queue->used_size >= queue->array_size)
    {
      return 0;
    }

  /* Copy data to the array. */

  kmemcpy(queue->write_ptr, indata, queue->element_size);

  /* Increase the write pointer to the next element in array. */

  queue->write_ptr += queue->element_size;

  /* Since new data was inserted, the used size is increased. */

  queue->used_size++;

  /* Check for array boundary and reset the pointer to the beginning
   * of the array.
   *
   * TODO: Verify if modulo % operator is more efficient here.
   */

  if (queue->write_ptr >= (queue->array_ptr + queue->array_size))
    {
      queue->write_ptr = queue->array_ptr;
    }

  return 1;
}


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

int kdequeue(queue_t *queue, void *outdata)
{
  if (!queue || !outdata)
    {
      return 0;
    }

  /* Check if array have data. */

  if (queue->used_size > 0)
    {
      /* Copy data to variable pointed by outdata.
       *
       * NOTE: The data is not removed from array. It will be overwritten.
       */

      kmemcpy(outdata, queue->read_ptr, queue->element_size);

      /* Increase read pointer to the next element in array. */

      queue->read_ptr += queue->element_size;

      /* Since data was retrieved from array, decrease usage, too. */

      queue->used_size--;

      /* Check for array boundary and reset the pointer to the beginning
       * of the array.
       *
       * TODO: Verify if modulo % operator is more efficient here.
       */

      if (queue->read_ptr >= (queue->array_ptr + queue->array_size))
        {
          queue->read_ptr = queue->array_ptr;
        }
      return 1;
    }
  return 0;
}


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

int kqueue_get_usedsize(queue_t *queue)
{
  if (!queue)
    {
      return -1;
    }

  return queue->used_size;
}


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

int kqueue_get_freesize(queue_t *queue)
{
    if (!queue)
    {
      return -1;
    }

    return (queue->array_size - queue->used_size);
}


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

void kqueue_destroy(queue_t *queue)
{
  if (queue)
    {
      queue->array_ptr    = NULL;
      queue->read_ptr     = NULL;
      queue->write_ptr    = NULL;
      queue->array_size   = 0;
      queue->element_size = 0;
      queue->used_size    = 0;
    }
}

