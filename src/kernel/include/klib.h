/*
 * klib.h
 *
 *  Created on: Mar 19, 2020
 *      Author: yo3bn
 */

#ifndef SRC_KERNEL_INCLUDE_KLIB_H_
#define SRC_KERNEL_INCLUDE_KLIB_H_


/* Since the kernel does not contain any external headers,
 * the NULL should be defined.
 */

#ifndef NULL
#  define NULL ((void*)0)
#endif


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


#endif /* SRC_KERNEL_INCLUDE_KLIB_H_ */
