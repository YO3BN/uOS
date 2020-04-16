
//upper half of the uart driver

#include "arch.h"
#include "semaphore.h"

#include "klib.h"

#include "drivers.h"
#include "uart.h"


static semaphore_t drv_mtx;   /* Driver Mutex. Used to exclude other access. */
static semaphore_t rx_irq;    /* When a byte is received this sem. is given. */
static semaphore_t tx_irq;    /* When a byte is transmitted, this is given. */


static volatile struct
{
  int mode;
  int init;
} drv_context =
    {
        .mode = 0,
        .init = 0,
    };


static volatile queue_t rx_queue;
static volatile queue_t tx_queue;


void drv_uart_tx_irq(void)
{
  sem_giveISR(&tx_irq);
}


void drv_uart_rx_irq(unsigned char byte)
{
  unsigned char null;

  /* First, put the byte in to the receiving buffer. */

  if (!kenqueue((queue_t*) &rx_queue, &byte))
    {
      /* Something went wrong, maybe the receiving buffer was not
       * allocated or is full.
       *
       * Nothing more to do in this case, just drop the byte.
       */
      return;
    }

  /* Handle ENTER key for TEXT mode. */

  if (drv_context.mode == DRVCTRL_UART_MODE_TXT)
    {
      /* In TEXT mode, the buffer need to have the null terminator
       * after the '\n', therefore the buffer need to have at least
       * one free byte after the '\n', were the null terminator
       * will be appended.
       *
       * Otherwise, if the buffer is full and no '\n' was received,
       * append the null terminator and give the event.
       */

      if (byte == '\n' || byte == '\r' ||
          kqueue_get_freesize((queue_t*) &rx_queue) == 1)
        {
          null = '\0';
          kenqueue((queue_t*) &rx_queue, &null);
          sem_giveISR(&rx_irq);
          return;
        }
    }
  else
    {
      /* Here is the BINARY mode.
       * Check if the buffer is full and give the event.
       */

      if (kqueue_get_freesize((queue_t*) &rx_queue) == 0)
        {
          sem_giveISR(&rx_irq);
          return;
        }
    }
}


int drv_init_uart(void)
{
  /* Guard against multiple initialization. */

  if (drv_context.init)
    {
      /* The driver was already initiated. */

      return DRV_STATUS_ERROR;
    }

  /* Lower-half init. function. */

  arch_uart_init();

  sem_init(&drv_mtx);
  sem_init(&rx_irq);
  sem_init(&tx_irq);

  /* Configure default settings. */

  drv_context.mode = DRVCTRL_UART_MODE_TXT;

  /* Since there is no resource free first time,
   * give it here, at init. time.
   */

  sem_give(&drv_mtx);

  /* Marking the driver as initialized. */

  drv_context.init = 1;

  return DRV_STATUS_SUCCESS;
}



int drv_open_uart(void)
{
  /* Check if the driver is already used, by trying to take
   * the mutex semaphore.
   * Note: Using no blocking here, only test if the mutex is used.
   */

  switch (sem_take(&drv_mtx, SEM_WAIT_NO))
    {
      case SEM_STATUS_TOOK:
        return DRV_STATUS_SUCCESS;

      case SEM_STATUS_BUSY:
        return DRV_STATUS_BUSY;

      default:
        return DRV_STATUS_ERROR;
    }

  return DRV_STATUS_ERROR;
}


void drv_close_uart(void)
{
  //TODO: Rx Tx operations in pending ??

  /* Release the uart resource. */

  sem_give(&drv_mtx);

  return;
}


int drv_read_uart(void *data, unsigned int size)
{
  int retval = DRV_STATUS_ERROR;

  /* Check if data pointer is not NULL and the driver
   * was initialized before.
   */

  if (!data || !size || !drv_context.init)
    {
      return retval;
    }

  /* Create a queue over memory location pointed by 'data' with size of
   * one element of 1 byte.
   *
   * The ISR will receive one byte at a time and will write it
   * directly into location pointed by data.
   */

  kqueue_init((queue_t*) &rx_queue, data, size, 1);

  /* Waiting for ISR / lower-half to give the semaphore.
   * This will happens when the 'buffer' is full, or when ENTER (CR/LF)
   * key was received, depending on driver configuration.
   */

  if (sem_take(&rx_irq, SEM_WAIT_FOREVER) == SEM_STATUS_ERROR)
    {
      kqueue_destroy((queue_t*) &rx_queue);
      return retval;
    }


  retval = kqueue_get_usedsize((queue_t*) &rx_queue);
  if (retval < 0)
    {
      retval = DRV_STATUS_ERROR;
    }

  return retval;
}


int drv_write_uart(void *data, unsigned int size)
{
  unsigned char byte;
  int retlen = 0;

  /* Check if data pointer is not NULL and the driver
   * was initialized before.
   */

  if (!data || !drv_context.init)
    {
      return DRV_STATUS_ERROR;
    }

  /* Create a non-empty queue over data. */

  kqueue_init2((queue_t*) &tx_queue, data, size, 1, size);

  /* For each byte in the queue, send it to lower-half and wait for
   * transmission to complete.
   */

  while (kdequeue((queue_t*) &tx_queue, (unsigned char*) &byte))
    {
      /* Send the byte to lower-half driver. */

      arch_uart_byte_send(byte);

      /* Wait for lower-half and peripheral transmission to complete. */

      if (sem_take(&tx_irq, SEM_WAIT_FOREVER) == SEM_STATUS_ERROR)
        {
          return DRV_STATUS_ERROR;
        }

      retlen++;
    }

  return retlen;
}


int drv_ctrl_uart(int operation, void *arg)
{
  int retval = DRV_STATUS_ERROR;
  int *mode = arg;

  if (!arg)
    {
      return retval;
    }

  switch (operation)
  {
    case DRVCTRL_GET:
      *mode = drv_context.mode;
      retval = DRV_STATUS_SUCCESS;
      break;

    case DRVCTRL_SET:
      drv_context.mode = *mode;
      retval = DRV_STATUS_SUCCESS;
      break;

    default:
      retval = DRV_STATUS_ERROR;
      break;
  }

  return retval;
}


