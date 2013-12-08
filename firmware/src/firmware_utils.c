#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "firmware_utils.h"

// Returns NULL if connection could not be opened
struct connection * connection_open( const char * in_filename,
                                     const char * out_filename )
{
  struct connection * c = malloc( sizeof *c );

  c->fd_in = open( "driver_to_device", O_WRONLY );

  if ( c->fd_in == -1 ) {
    perror( "open" );
    free( c );
    return NULL;
  }

  c->fd_out = open( "device_to_driver", O_RDONLY );

  if ( c->fd_out == -1 ) {
    perror( "open" );
    close( c->fd_in );
    free( c );
    return NULL;
  }

  return c;
}

void connection_close( struct connection * c )
{
  close( c->fd_in  );
  close( c->fd_out );

  free( c );
}

void connection_write( struct connection   * c,
                       const struct packet * p )
{
  //TODO
}

void connection_read( struct connection * c,
                      struct packet     * p )
{
  //TODO
}
