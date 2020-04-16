/*
 * uart.c
 *
 *  Created on: Mar 20, 2020
 *      Author: yo3bn
 */

#include "arch.h"
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>


////////////////TODO re-implement by interrupts? /////////////////////////////


/*
 * The calculation of baudrate prescaler is done by the C
 * preprocessor inside <util/setbaud.h> which is part
 * of AVR C Library.
 *
 * BAUD and F_CPU macros have to be already defined
 * at this point.
 */

#define BAUD  9600
#include <util/setbaud.h>


void arch_uart_init(void)
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

  /* Enable Interrupts. */
  UCSR0B |= _BV(RXCIE0) | _BV(TXCIE0);
}



void arch_uart_byte_send(uint8_t c)
{
  /* wait for empty transmit buffer */

 // while (!(UCSR0A & (1 << UDRE0)));

  /* put data into buffer, sends data */

  UDR0 = c;
}


void arch_uart_byte_recv(uint8_t *c)
{
  while (!(UCSR0A & _BV(RXC0)));

  *c = UDR0;
}
