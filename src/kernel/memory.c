/*
 * memory.c
 *
 *  Created on: Mar 4, 2020
 *      Author: yo3bn
 */


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


void kstrncpy(char *dst, char *src, unsigned int size)
{
  //TODO finish implementation
  unsigned int i = 0;
  int len;

  if (!dst || !src)
    {
      return;
    }

  len = kstrlen(src);//////////////

  for (i = 0; (i < size - 1) && (i < len); i++)
    {
      *dst = *src;
    }
  //////////////////////////////
}


unsigned int kstrlen(const char *str)
{
  char *orig;

  if (!str)
    {
      return 0;
    }

  orig = str;
  while (*(++str));

  return str - orig;
}

