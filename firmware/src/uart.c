#include "uart.h"

fifo fifo_receiver;
fifo fifo_transmitter;

/* Receiver */
ISR(UART_RXC_vect_num)
{
  char sREG;

  sREG = SREG;

  _CLI();
    fifo_receiver.tableau[fifo_receiver.indice_ecriture] = UDR;
    fifo_receiver.indice_ecriture = (fifo_receiver.indice_ecriture +1)%TAILLE_FIFO;

  _SEI();
}

/* Transmitter */
ISR(UART_TXC_vect_num)
{
  char sREG;

  sREG = SREG;

  _CLI();

    UDR =  fifo_transmitter.tableau[fifo_transmitter.indice_lecture];
    fifo_transmitter.indice_lecture = (fifo_transmitter.indice_lecture + 1)%TAILLE_FIFO;
  _SEI(sREG);
}
void
uart_init(unsigned int baudrate)
{

  unsigned int ubbr = (F_CPU/(16 * baudrate) ) -1;

  UBRRH = (unsigned char)(ubbr >>8);
  UBRRL = (unsigned char)(ubbr);

  /* Enable Receiver and Transmitter */
  UCSRB = (1<<RXEN)|(1<<TXEN) | (1<<RXCIE) | (1 << TXCIE);

  /* Set frame format : 8data , 1 stop bit, no parity, Asynchronous */
  UCSRC = (1 << URSEL) | (3 <<UCSZ0);

}


void
uart_write(char c)
{
  while(!(UCSRA | (1<< UDRE)));

  UDR = c;
}


