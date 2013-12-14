#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdlib.h>
#include "device_state.h"
#include "failsafe.h"
#include "protocol.h"

#ifndef EMBEDDED
#define DRIV2DEV_FILENAME "/tmp/driver2device"
#define DEV2DRIV_FILENAME "/tmp/device2driver"
#endif

struct connection {
  int fd_in;
  int fd_out;
  struct device_caps caps;
  struct device_state state;
  struct failsafe * failsafe;
};

struct connection * connection_open( const char * in_filename,
                                     const char * out_filename );

/** If called when resources are already allocated, free them first
 * c->caps.nb_pins must be set before ! */
void connection_init_resources( struct connection * c );

// Can be safely called, even if resources haven't been initialized
void connection_release_resources( struct connection * c );

void    connection_close( struct connection * c                          );
ssize_t connection_write( struct connection * c, const struct packet * p );
ssize_t connection_read ( struct connection * c,       struct packet * p );

/**
 * Handle the checksum calculation
 * @return -1 if there was an error, the number of bytes written on success
 */
int send_packet( struct connection * connection,
                 struct packet     * p);

#endif /* CONNECTION_H */
