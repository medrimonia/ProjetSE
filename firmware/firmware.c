#include <stdio.h>
#include <stdlib.h>

#ifndef EMBEDDED
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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
  device->fd_in = -1;
  device->fd_out = -1;
  device->caps.pins_mask_type = NULL;
  device->state.pins_state = NULL;
  device->caps.nb_pins = NB_PINS;
  connection_init_resources( device );
  unsigned int i;
  for ( i = 0; i < NB_PINS; i++ ){
    device->caps.pins_mask_type[i] = 31;//TODO hardcoding is bad but fast
    device->state.pins_state[i].pins_type = PIN_TYPE_DIGITAL;
    device->state.pins_state[i].pins_val  = 0;
    //TODO init failsafe
  }
}

void init_communication()
{
#ifdef EMBEDDED
  printf( "Embedded init communication is not implemented yet !\n" );
  exit( EXIT_FAILURE );
#else
  // Creating 2 fifos for communication
  if ( mkfifo( DRIV2DEV_FILENAME, S_IRWXU ) < 0 ){
    perror( "Failed to create the driver to device fifo file" );
    exit( EXIT_FAILURE );
  }
  if ( mkfifo( DEV2DRIV_FILENAME, S_IRWXU ) < 0 ){
    perror( "Failed to create the device to driver fifo file" );
    unlink( DRIV2DEV_FILENAME );
    exit( EXIT_FAILURE );    
  }
  printf( "Opening fd :\n" );
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
  unlink( DRIV2DEV_FILENAME );
  unlink( DEV2DRIV_FILENAME );
}

void mainloop()
{
  while ( true ) {
    struct packet p;
    connection_read( device, &p );
    if ( !packet_valid(&p) ) {
      return;
    }

    int16_t cmd = read_cmd( &p.header );
    switch ( cmd ) {
      case CMD_GET_CAPS:
        printf( "Treating a getcaps cmd\n" );
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
      case CMD_GET_FAILSAFE:
        reply_get_failsafe( device, &p );
        break;
      case CMD_SET_FAILSAFE:
        reply_get_failsafe( device, &p );
        break;
      default:
        break;
    }

    packet_free( &p );
  }
}

int main(void)
{
  init_device_content();
  init_communication();
  printf( "Communication started\n" );
  mainloop();
  end_communication();
  return EXIT_SUCCESS;
}
