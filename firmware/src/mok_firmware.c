#include <stdlib.h>
#include "bit_utils.h"
#include "protocol.h"
#include "firmware_utils.h"
#include "firmware_packet_process.h"

int main(void)
{
  struct packet * p = malloc( sizeof(*p) );
  struct connection * c = connection_open( "driver_to_device", "device_to_driver" );
  int16_t cmd;

  if ( c == NULL ) {
    return EXIT_FAILURE;
  }

  while ( 1 ) {
    connection_read( c, p );
    if ( !packet_valid(p) ) {
      continue;
    }

    cmd = read_cmd( &p->header );
    switch ( cmd ) {
      case CMD_GET_CAPS:
        process_get_caps( p );
        break;
      case CMD_RESET:
        process_reset( p );
        break;
      case CMD_PING:
        process_ping( p );
        break;
      case CMD_READ:
        process_read( p );
        break;
      case CMD_WRITE:
        process_write( p );
        break;
      case CMD_GET_TYPE:
        process_get_type( p );
        break;
      case CMD_SET_TYPE:
        process_set_type( p );
        break;
      case CMD_GET_FAILSAFE:
        process_get_failsafe( p );
        break;
      case CMD_SET_FAILSAFE:
        process_get_failsafe( p );
        break;
      default:
        break;
    }

    packet_free( p );
  }

  connection_close( c );

  return 0;
}
