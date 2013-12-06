#include <stdlib.h>
#include "firmware_utils.h"
#include "bit_utils.h"

int main(void)
{
  unsigned char * packet;
  int             packet_size;
  struct connection * c = connection_open( "driver_to_device", "device_to_driver" );
  int16_t cmd;

  if ( c == NULL ) {
    return EXIT_FAILURE;
  }

  while ( 1 ) {
    connection_read( c, packet, &packet_size );
    if ( !packet_valid( packet, packet_size ) ) {
      continue;
    }

    cmd = read_cmd( packet );

    switch ( cmd ) {
      case CMD_GET_CAPS:
        break;
      case CMD_RESET:
        break;
      case CMD_PING:
        break;
      case CMD_READ:
        break;
      case CMD_WRITE:
        break;
      case CMD_GET_TYPE:
        break;
      case CMD_SET_TYPE:
        break;
      case CMD_GET_FAILSAFE:
        break;
      case CMD_SET_FAILSAFE:
        break;
      default:
        break;
    }
  }

  connection_close( c );

  return 0;
}
