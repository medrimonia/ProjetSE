#include "bit_utils.h"
#include "protocol.h"
#include "failsafe.h"
#include "firmware_packet_process.h"
#include "firmware_state.h"

void reply_get_caps( struct connection * c, const struct packet * p )
{
  struct packet rep;
  set_packet_header(&rep, CMD_GET_CAPS, REP_CODE_SUCCESS, 0/* device_caps.nb_pins*/);
  // TODO: Transform state to data
  connection_write(c, &rep);
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

void reply_get_failsafe( struct connection * c, const struct packet * p )
{
}

