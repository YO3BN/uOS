/*
 * memory.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */


void kmemset(void *ptr, unsigned char byte, unsigned int size)
{
  int i = 0;

  for (i = 0; i < size; i++)
    {
      *(((unsigned char*) ptr) + i) = byte;
    }
}
