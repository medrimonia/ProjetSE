#ifndef FIRMWARE_PACKET_PROCESS_H
#define FIRMWARE_PACKET_PROCESS_H

#include "connection.h"

void reply_get_caps    ( struct connection * c );
void reply_reset       ( struct connection * c, const struct packet * p );
void reply_ping        ( struct connection * c, const struct packet * p );
void reply_read        ( struct connection * c, const struct packet * p );
void reply_write       ( struct connection * c, const struct packet * p );
void reply_get_type    ( struct connection * c, const struct packet * p );
void reply_set_type    ( struct connection * c, const struct packet * p );
void reply_get_failsafe( struct connection * c, const struct packet * p );
void reply_set_failsafe( struct connection * c, const struct packet * p );

#endif /*FIRMWARE_PACKET_PROCESS_H*/
