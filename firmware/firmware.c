#include <stdio.h>
#include <stdlib.h>

#ifndef EMBEDDED
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#include "uart.h"
#endif

#include "bit_utils.h"
#include "firmware_packet_process.h"
#include "connection.h"

#define NB_PINS 16

void end_communication();

// This variable will contain all the things specific to the device
struct connection * device;

void init_device_content()
{
  device = malloc( sizeof(struct connection) );
  device->protocol_version = 1;
  device->fd_in = -1;
  device->fd_out = -1;
  device->caps.pins_mask_type = NULL;
  device->state.pins_state = NULL;
  device->failsafe = NULL;
  device->caps.nb_pins = NB_PINS;
  connection_init_resources( device );
  uint8_t i;
  for ( i = 0; i < NB_PINS; i++ ){
    device->caps.pins_mask_type[i] = (1 << PIN_TYPE_DIGITAL)
                                   + (1 << PIN_TYPE_ANALOG8)
                                   + (1 << PIN_TYPE_ANALOG16)
                                   + (1 << PIN_TYPE_PWM8)
                                   + (1 << PIN_TYPE_PWM16);
    device->state.pins_state[i].pins_type = PIN_TYPE_DIGITAL;
    device->state.pins_state[i].pins_val  = 0;
    //TODO init failsafe
  }
}

void init_communication()
{
#ifdef EMBEDDED
  DDRB = 0x20;
  uart_init(19200);
  sei();
#else
  // Creating 2 fifos for communication
  if ( mkfifo( DRIV2DEV_FILENAME, S_IRWXU ) < 0 && errno != EEXIST ){
    perror( "Failed to create the driver to device fifo file" );
    exit( EXIT_FAILURE );
  }
  if ( mkfifo( DEV2DRIV_FILENAME, S_IRWXU ) < 0 && errno != EEXIST ){
    perror( "Failed to create the device to driver fifo file" );
    unlink( DRIV2DEV_FILENAME );
    exit( EXIT_FAILURE );    
  }
  // Opening files
  device->fd_out = open( DEV2DRIV_FILENAME, O_WRONLY );
  if ( device->fd_out < 0 ) {
    perror( "Failed to open the device to driver fd" );
    end_communication();
  }
  device->fd_in = open( DRIV2DEV_FILENAME, O_RDONLY );
  if ( device->fd_in < 0 ) {
    perror( "Failed to open the driver to device fd" );
    end_communication();
  }
#endif
}

void end_communication()
{
  connection_close(device);
#ifndef EMBEDDED
  unlink( DRIV2DEV_FILENAME );
  unlink( DEV2DRIV_FILENAME );
#endif
}

void mainloop()
{
  while ( true ) {
    struct packet p;
    if ( connection_read( device, &p ) == -1 ) return;
    if ( !packet_valid(&p) ) {
      return;
    }

    int16_t cmd = read_cmd( &p.header );
    switch ( cmd ) {
      case CMD_GET_CAPS:
        reply_get_caps( device );
        break;
      case CMD_RESET:
        reply_reset( device, &p );
        break;
      case CMD_PING:
        reply_ping( device, &p );
        break;
      case CMD_READ:
        reply_read( device, &p );
        break;
      case CMD_WRITE:
        reply_write( device, &p );
        break;
      case CMD_GET_TYPE:
        reply_get_type( device, &p );
        break;
      case CMD_SET_TYPE:
        reply_set_type( device, &p );
        break;
#ifndef DISABLE_FAILSAFE
      case CMD_GET_FAILSAFE:
        reply_get_failsafe( device, &p );
        break;
      case CMD_SET_FAILSAFE:
        reply_set_failsafe( device, &p );
        break;
#endif
      default:
        break;
    }

    packet_free( &p );
  }
}

#ifdef EMBEDDED
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
#endif

int main(void)
{
  init_device_content();
  init_communication();
  mainloop();
  end_communication();
  return 0;
}
