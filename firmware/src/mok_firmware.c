#include <stdlib.h>
#include "bit_utils.h"
#include "protocol.h"
#include "firmware_utils.h"
#include "firmware_packet_process.h"

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

  device_caps_init (&dc, ATM8_NB_PINS, pins_mask_type);
  device_state_init(&ds, &dc);
}

int main(void)
{
  struct packet * p = malloc( sizeof(*p) );
  struct connection * c = connection_open( "driver_to_device", "device_to_driver" );
  int16_t cmd;

  // TODO: init device caps and state

  if ( c == NULL ) {
    return EXIT_FAILURE;
  }

  init_device(c->caps, c->state);

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
