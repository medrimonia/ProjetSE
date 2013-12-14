#include <stdlib.h>
#include <stdio.h>
#include "bit_utils.h"
#include "protocol.h"
#include "connection.h"
#include "firmware_packet_process.h"
#include "atm8_pins.h"
#include "device_state.h"

void init_device( struct device_caps * dc, struct device_state * ds )
{
  unsigned char pins_mask_type[ATM8_NB_PINS];
  int i;
  for (i = 0; i < ATM8_NB_PINS; i++) {
    pins_mask_type[i] = (1 << PIN_TYPE_DIGITAL)
                      + (1 << PIN_TYPE_ANALOG8)
                      + (1 << PIN_TYPE_ANALOG16)
                      + (1 << PIN_TYPE_PWM8)
                      + (1 << PIN_TYPE_PWM16);
  }

  device_caps_init (dc, ATM8_NB_PINS, pins_mask_type);
  device_state_init(ds, dc);
}

void print_help( char ** argv )
{
  fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
}

int main(int argc, char **argv)
{
  struct packet * p = malloc( sizeof(*p) );

  if ( argc != 3 ) {
    print_help( argv );
    return EXIT_FAILURE;
  }

  struct connection * c = connection_open( argv[1], argv[2] );
  int16_t cmd;

  if ( c == NULL ) {
    return EXIT_FAILURE;
  }

  init_device(&c->caps, &c->state);

  //while ( 1 ) {
    connection_read( c, p );
    if ( !packet_valid(p) ) {
      return 1;
    }

    cmd = read_cmd( &p->header );
    switch ( cmd ) {
      case CMD_GET_CAPS:
        reply_get_caps( c );
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
  //}

  connection_close( c );

  return 0;
}
