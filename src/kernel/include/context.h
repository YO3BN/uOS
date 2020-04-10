/*
 * context.h
 *
 *  Created on: Apr 9, 2020
 *      Author: yo3bn
 */

//TODO add defines against multiple inclusion

void __attribute__((naked)) __attribute__((noinline)) yield(void);
void __attribute__((naked)) __attribute__((noinline)) context_switch_to_kernel(void);
void __attribute__((naked)) __attribute__((noinline)) context_switch_to_task(void);
void __attribute__((naked)) __attribute__((noinline)) exec_kernel(void);
