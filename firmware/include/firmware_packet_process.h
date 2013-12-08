#ifndef FIRMWARE_PACKET_PROCESS_H
#define FIRMWARE_PACKET_PROCESS_H

#include <stdbool.h>

void process_get_caps    ( const struct packet * p );
void process_reset       ( const struct packet * p );
void process_ping        ( const struct packet * p );
void process_read        ( const struct packet * p );
void process_write       ( const struct packet * p );
void process_get_type    ( const struct packet * p );
void process_set_type    ( const struct packet * p );
void process_get_failsafe( const struct packet * p );
void process_get_failsafe( const struct packet * p );

#endif /*FIRMWARE_PACKET_PROCESS_H*/
