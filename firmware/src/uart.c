// No Sense to compile this module if mode is not embedded
#ifdef EMBEDDED
#include "uart.h"
#include "io_utils.h"

fifo fifo_receiver;
fifo fifo_transmitter;


/* Receiver
 * Interupt function about RX vector 
 * */
ISR( USART_RXC_vect )
{
  if( UCSRA & _RXC) 
    fifo_receiver.array[fifo_receiver.last_write++] = UDR;

}
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

int main()
{

  // Assign the pin B5 like out pin
  DDRB = 0x20;
  // Set the baudrate to 19200
  uart_init(19200);
  // Enable the interruption
  sei();

  while(1)
  {
    // Assign every pin on port B to 1
    PORTB |= 0xFF;
    _delay_ms(400);
    // Assign every pin on port B to 2 
    PORTB &= 0x00;
    _delay_ms(400);

  }

  return -1;
}
#endif
