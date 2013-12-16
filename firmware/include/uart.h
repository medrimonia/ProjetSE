#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define _RXC (1 << RXC)
#define _TXC (1 << TXC)
#define _UDRE (1 << UDRE)


/**
 *\def FIFO_SIZE Used by the fifo struct to determinate the size of array
 * */

#define FIFO_SIZE 200

/**
 *\struct fifo
 *\brief Objet fifo
 *
 * The structure contains a array of size SIZE_FIFO.
 * It stores characters.
 * Two index are available :
 *  - reading_index : for read a value in the fifo
 *  - writing_index : for write a new value in the fifo
 *
 **/

typedef struct{
  unsigned char array[FIFO_SIZE];
  int last_read;
  int last_write;
}fifo;

/**
 *\fn void uart_init(unsigned int baudrate)
 *\param[in] baudrate Communication Speed
 *
 *  Initialisation of the UART Communication with a baudrate equal to "baudrate"
 *  RX and TX interupt are enabled
 *  Frame Format is 8bit data, 1 Stop bit, no parity , asynchronous
 *
 * */

void uart_init(uint32_t);

/**
 *\fn void uart_write(char c)
 *\param[in] c caractère à envoyer
 *
 * Send a char c using the UART communication
 *
 *\bug If you don't initialise the USART communication, your character will never be send
 *
 * */

void uart_write(unsigned char c);


void delay(int32_t);

#endif
