#ifndef CONNECTION_H
#define CONNECTION_H

#include "device_state.h"
#include "protocol.h"

struct connection {
  int fd_in;
  int fd_out;
  struct device_caps caps;
  struct device_state state;
};

struct connection * connection_open( const char * in_filename,
                                     const char * out_filename );

void    connection_close( struct connection * c                          );
ssize_t connection_write( struct connection * c, const struct packet * p );
ssize_t connection_read ( struct connection * c,       struct packet * p );

#endif /* CONNECTION_H */
