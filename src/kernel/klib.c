/*
 * klib.c
 *
 *  Created on: Mar 19, 2020
 *      Author: yo3bn
 */

#include "klib.h"


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


int kstreq(const char *str1, const char *str2)
{
  if (!str1 || !str2)
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
