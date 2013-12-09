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

  // TODO: init device caps and state

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
        reply_get_caps( c, p );
        break;
      case CMD_RESET:
        reply_reset( c, p );
        break;
      case CMD_PING:
        reply_ping( c, p );
        break;
      case CMD_READ:
        reply_read( c, p );
        break;
      case CMD_WRITE:
        reply_write( c, p );
        break;
      case CMD_GET_TYPE:
        reply_get_type( c, p );
        break;
      case CMD_SET_TYPE:
        reply_set_type( c, p );
        break;
      case CMD_GET_FAILSAFE:
        reply_get_failsafe( c, p );
        break;
      case CMD_SET_FAILSAFE:
        reply_get_failsafe( c, p );
        break;
      default:
        break;
    }

    packet_free( p );
  }

  connection_close( c );

  return 0;
}
