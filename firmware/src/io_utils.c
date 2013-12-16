//It makes no sense to compile this module if it's not in embedded mode
#ifdef EMBEDDED
#include "io_utils.h"
#include "uart.h"

extern fifo fifo_receiver;
// From uart.c, some doc might be useful
void uart_write( unsigned char c ){
  UDR = c;
  while(!(UCSRA & (1<< UDRE)));
}

int write( int fd, unsigned char * buffer, unsigned int n )
{
  unsigned int i;
  for ( i = 0; i < n; i++ ) {
    uart_write( buffer[i] );
  }
  return i;
}

int read( int fd, unsigned char * buffer, unsigned int n )
{
  unsigned int i = 0;

  while ( i < n )
  {
    if ( fifo_receiver.last_write != fifo_receiver.last_read )
    {
      buffer[i] = fifo_receiver.array[fifo_receiver.last_read++];
      i++;
    }
    else
    {
      // For the avoidance of the overload
      _delay_ms(40);
    }
  }

  return i;
}
#endif
