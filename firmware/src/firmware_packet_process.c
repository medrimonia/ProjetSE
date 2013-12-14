#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "bit_utils.h"
#include "protocol.h"
#include "failsafe.h"
#include "device_state.h"
#include "firmware_packet_process.h"

void reply_get_caps( struct connection * c )
{
  struct packet rep;
  set_packet_header(&rep, CMD_GET_CAPS, REP_CODE_SUCCESS, c->caps.nb_pins + 2);
  rep.data = malloc( 1 + 1 + c->caps.nb_pins );
  init_packet( rep.data, c->caps.nb_pins + 2 );
  rep.data[0] = get_reply_id();
  rep.data[1] += c->caps.nb_pins;
  memcpy( rep.data + 2, c->caps.pins_mask_type, c->caps.nb_pins );
  send_packet( c, &rep );
  free( rep.data );
}

void reply_reset( struct connection * c, const struct packet * p )
{
}

void reply_ping( struct connection * c, const struct packet * p )
{
}

void reply_read( struct connection * c, const struct packet * p )
{
}

void treat_write( struct connection * c, const struct packet * p )
{
}

void reply_write( struct connection * c, const struct packet * p )
{
  treat_write( c, p );
  struct packet rep;
  set_packet_header( &rep, CMD_WRITE, REP_CODE_SUCCESS, 1 );
  unsigned char rep_id = get_reply_id();
  rep.data = &rep_id;
  send_packet( c, &rep );
}

void reply_get_type( struct connection * c, const struct packet * p )
{
  bool use_mask = p->header % 1 == USE_MASK;
  struct packet rep;
  if (!use_mask) {
    uint8_t pin_id = read_bit_value( p->data, 0, PINS_NO_BITS_NB );
    uint8_t pin_type = c->state.pins_state[pin_id].pins_type;
    set_packet_header( &rep, CMD_SET_TYPE, REP_CODE_SUCCESS, 2 );
    rep.data = malloc(2);
    init_packet( rep.data, 2 );
    write_bit_value( rep.data, REPLY_ID_BITS_NB, pin_type, PIN_TYPE_BITS_NB );
  }
  else {
    fprintf( stderr, "Get type for mask unimplemented !\n" );
    return;
  }
  rep.data[0] = get_reply_id();
  send_packet( c, &rep );
  free( rep.data );
}

void treat_set_type( struct connection * c, const struct packet * p )
{
  bool use_mask = p->header % 1 == USE_MASK;
  if (!use_mask) {
    uint8_t pin_id = read_bit_value( p->data, 0, PINS_NO_BITS_NB );
    uint8_t type = read_bit_value( p->data, PINS_NO_BITS_NB, PIN_TYPE_BITS_NB );
    c->state.pins_state[pin_id].pins_type = type;
  }
  else {
    fprintf( stderr, "Set type for mask unimplemented !\n" );
  }
}

void reply_set_type( struct connection * c, const struct packet * p )
{
  treat_set_type( c, p );
  struct packet rep;
  set_packet_header( &rep, CMD_SET_TYPE, REP_CODE_SUCCESS, 1 );
  unsigned char rep_id = get_reply_id();
  rep.data = &rep_id;
  send_packet( c, &rep );
}

void reply_get_failsafe( struct connection * c, const struct packet * p )
{
}

void reply_set_failsafe( struct connection * c, const struct packet * p )
{
}

