/*
 * interrups.c
 *
 *  Created on: Mar 20, 2020
 *      Author: yo3bn
 */

#include "arch.h"
#include <avr/interrupt.h>
#include "kernel_api.h"

#include "uart.h"


ISR(TIMER1_OVF_vect)
{
  systick();
}

ISR(USART_RX_vect)
{
  volatile char byte = UDR0;
  drv_uart_rx_irq(byte);
}

ISR(USART_TX_vect)
{
  drv_uart_tx_irq();
}

