/*
 * klib.h
 *
 *  Created on: Mar 19, 2020
 *      Author: yo3bn
 */

#ifndef SRC_KERNEL_INCLUDE_KLIB_H_
#define SRC_KERNEL_INCLUDE_KLIB_H_


#ifndef NULL
#  define NULL ((void*)0)
#endif


void kmemset(void *ptr, unsigned char byte, unsigned int size);
void kmemcpy(void *dst, void *src, unsigned int size);
unsigned int kstrlen(const char *str);
void kstrncpy(char *dst, char *src, unsigned int size);


#endif /* SRC_KERNEL_INCLUDE_KLIB_H_ */
