/*
 * main.c
 *
 *  Created on: Mar 5, 2020
 *      Author: yo3bn
 */

#include <stddef.h>
#include <stdio.h>

#include <avr/io.h>

#define MAX_TASKS 100

#include "task.h"

#include <stdint.h>
#include <avr/io.h>



/*
 * The calculation of baudrate prescaler is done by the C
 * preprocessor inside <util/setbaud.h> which is part
 * of AVR C Library.
 *
 * BAUD and F_CPU macros have to be already defined
 * at this point.
 */
#ifndef F_CPU
# error "F_CPU is not defined. It is needed to calculate UART prescaler."
#endif
#define BAUD  9600
#include <util/setbaud.h>

#define MAX_TIME_COUNT (F_CPU>>4) // <- 125ms timeout at 1MHz CPU

volatile uint8_t com_buf[256 + 16];

void com_init(void)
{
  /*
   * Write the baudrate to the USART Baud Rate Register.
   *
   * The macros UBRRH_VALUE and UBRRL_VALUE already contains
   * the required prescaler value and they were calculated
   * by C preprocessor inside <util/setbaud.h> header.
   */
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~(_BV(U2X0));
#endif

  /* CONFIGURE: 8 BIT DATA, 1 STOP BIT, NO PARITY */
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
}


void uart_send_byte(uint8_t c)
{
  /* wait for empty transmit buffer */
  while (!(UCSR0A & (1 << UDRE0)));

  /* put data into buffer, sends data */
  UDR0 = c;
}


int8_t uart_recv_byte(uint8_t *c)
{
  uint32_t count = 0;
  while (!(UCSR0A & _BV(RXC0)))
  {
    count++;
    if (count > MAX_TIME_COUNT)
      return -1;
  }
  *c = UDR0;
  return 0;
}


void com_send(void *pvdata, uint16_t uslen)
{
  uint8_t *p = pvdata;
  while (uslen)
  {
    uart_send_byte(*p);
    uslen--;
    p++;
  }
}


uint16_t com_recv(void)
{
  uint16_t len;

  for (len = 0; len < 256 + 16; len++)
  {
    if (uart_recv_byte((uint8_t*) com_buf + len) == -1)
    {
      break;
    }
  }

  return len;
}





















#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>


static volatile uint8_t tick_overflow;
 extern volatile unsigned long g_systicks;

ISR(TIMER1_OVF_vect)
{
  tick_overflow = (uint8_t) 1;
  g_systicks++;
}


void enable_interrupts()
{
sei();
}


void disable_interrupts()
{
cli();
}




void tick_init(void)
{
  TCCR1A = (uint8_t) \
     (0 << COM1A1) |  /* Compare Output Mode A */
     (0 << COM1A0) |  /* Compare Output Mode A */
     (0 << COM1B1) |  /* Compare Output Mode B */
     (0 << COM1B0) |  /* Compare Output Mode B */
     (0 << FOC1A)  |  /* Force Output Compare A */
     (0 << FOC1B)  |  /* Force Output Compare B */
     (0 << WGM11)  |  /* Waveform Generation Mode */
     (0 << WGM10);    /* Waveform Generation Mode */

  TCCR1B = (uint8_t) \
     (0 << ICNC1) |   /* Input Capture Noise Canceler */
     (0 << ICES1) |   /* Input Capture Edge Select */
     (0 << WGM13) |   /* Waveform Generation Mode */
     (0 << WGM12) |   /* Waveform Generation Mode */
     (0 << CS12)  |   /* Clock Select */
     (0 << CS11)  |   /* Clock Select */
     (0 << CS10);     /* Clock Select */

  TIMSK1 = (uint8_t) \
//    (0 << TICIE1) |   /* Timer/Cnt1 Input Capture Interrupt */
//    (0 << OCIE1A) |   /* Timer/Cnt1 Output Compare Match */
//    (0 << OCIE1B) |   /* Timer/Cnt1 Output Compare Match */
    (1 << TOIE1);     /* Timer/Cnt1 Overflow Interrupt Enable */

  /* Reset the values */
  tick_overflow = (uint8_t) 0;
  TCNT1 = (uint16_t) 0;
}


void timer_start(void)
{
  /* Start Timer/Counter1 by choosing the prescaler */
  TCCR1B |= (uint8_t) \
      (0 << CS12) |
      (0 << CS11) |
      (1 << CS10);

  /* Reset the values */
  tick_overflow = (uint8_t) 0;
  TCNT1 = (uint16_t) 0;
}


void timer_stop(void)
{
  /* Clearing Clock will disable timer/counter1 */
  TCCR1B &= (uint8_t) ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}


uint8_t tick(void)
{
  cli();

  /* Save MCUCR, it will be restored back later */
  uint8_t mcucr = MCUCR;
//tick_overflow = (uint8_t) 0;

  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  sei();
//  timer_start();
  sleep_cpu();
//  timer_stop();

  /*
   * We don't need to use sleep_disable(),
   * just restoring the MCUCR register.
   */
  MCUCR = mcucr;

  /* Return TRUE when waking the CPU up by TIMER1_OVF */
  //return tick_overflow;
  if (tick_overflow) return 1;
  else return 0;
}












void first_task(void *arg)
{
  com_send("first task\n", 11);

  // Stop the task.
  task_pause(0);
}


void second_task(void *arg)
{
  com_send("second task\n", 12);

  //sleep
  task_sleep(0, 1);
}


void main(void)
{
  //TODO init stuff();
  tick_init();


  kernel_init();

  task_create(first_task, NULL);
  task_create(second_task, NULL);

  kernel_start();
}

