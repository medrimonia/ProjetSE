// No Sense to compile this module if mode is not embedded
#ifdef EMBEDDED
#include "uart.h"
#include "io_utils.h"

fifo fifo_receiver;
fifo fifo_transmitter;


/* Receiver */
ISR( USART_RXC_vect )
{
  if( UCSRA & _RXC) 
    fifo_receiver.array[fifo_receiver.last_write++] = UDR;

}

void delay( int32_t wait )
{
  while(wait--);
}

void uart_init( uint32_t baudrate )
{
  uint16_t ubbr = (F_CPU/(16 * baudrate) ) -1;

  UBRRH = (ubbr >>8);
  UBRRL = ubbr & 0xff;

  /* Enable Receiver and Transmitter */
  UCSRB = (1<<RXEN)|(1<<TXEN) | (1<<RXCIE);

  /* Set frame format : 8data , 1 stop bit, no parity, Asynchronous */
//  UCSRC = (1 << URSEL);
//  UCSRC |= (1 <<UCSZ0) |(1 << UCSZ1);
}

int main()
{
  DDRB = 0x20;
  uart_init(19200);
  sei();

  while(1)
  {
    PORTB |= 0xFF;
    _delay_ms(400);
    PORTB &= 0x00;
    _delay_ms(400);

  }

  return -1;
}
#endif
