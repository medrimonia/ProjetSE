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
  rep.checksum = compute_checksum( &rep );
  connection_write( c, &rep );
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

void reply_write( struct connection * c, const struct packet * p )
{
}

void reply_get_type( struct connection * c, const struct packet * p )
{
}

void reply_set_type( struct connection * c, const struct packet * p )
{
}

void reply_get_failsafe( struct connection * c, const struct packet * p )
{
}

void reply_set_failsafe( struct connection * c, const struct packet * p )
{
}

