

#ifndef __UART_H__
#define __UART_H__


/* Diver CTRL commands supported by UART upper half driver.
 *
 * DRVCTRL_UART_MODE_TXT - Operating UART in text mode. Basically, the read and
 *    write functions will return when meets CR\LF\NULL on the line or buffer,
 *    or when the size of the buffers are filled or depleted.
 *    Usually used for terminals.
 *
 * DRVCTRL_UART_MODE_BIN - Operating UART in binary mode. Read and write
 *    functions will return only when size of buffers are filled or depleted,
 *    no character analyzing.
 *    Usually used to transfer files.
 */

enum
{
  DRVCTRL_UART_MODE_TXT = 1,
  DRVCTRL_UART_MODE_BIN = 2,
} DRVCTRL_UART_MODE_T;


/* TODO add descriptions. */

void drv_uart_tx_irq(void);
void drv_uart_rx_irq(unsigned char byte);
int drv_init_uart(void);
int drv_open_uart(void);
void drv_close_uart(void);
int drv_read_uart(void *data, unsigned int size);
int drv_write_uart(void *data, unsigned int size);
int drv_ctrl_uart(int operation, void *arg);

#endif /* __UART_H__ */
