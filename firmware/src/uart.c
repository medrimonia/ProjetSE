// No Sense to compile this module if mode is not embedded
#ifdef EMBEDDED
#include "uart.h"
#include "io_utils.h"

void uart_init( uint32_t baudrate )
{
  // calculation for UBBR register
  uint16_t ubbr = (F_CPU/(16 * baudrate) ) -1;

  // Assign the value to the register
  UBRRH = (ubbr >>8);
  UBRRL = ubbr & 0xff;

  /* Enable Receiver and Transmitter */
  UCSRB = (1<<RXEN)|(1<<TXEN) | (1<<RXCIE);

  /* Set frame format : 8data , 1 stop bit, no parity, Asynchronous */
//  UCSRC = (1 << URSEL);
//  UCSRC |= (1 <<UCSZ0) |(1 << UCSZ1);
}

#endif
