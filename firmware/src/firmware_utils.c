#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "firmware_utils.h"

/** BUFF_SIZE should be high enough to read up to 256 pin status (3 bits)
 * and value (up to 16 bits)
 * Thus it should be greater than 1+2+256*19/8+1 = 612 */
#define BUFF_SIZE 612

/** Returns NULL if connection could not be opened */
struct connection * connection_open( const char * in_filename,
                                     const char * out_filename )
{
  struct connection * c = malloc( sizeof *c );

  c->fd_in = open( in_filename, O_WRONLY );

  if ( c->fd_in == -1 ) {
    perror( "open" );
    free( c );
    return NULL;
  }

  c->fd_out = open( out_filename, O_RDONLY );

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

/** Returns -1 if there was an error writing.
 *  Otherwise the number of bytes written is returned */
ssize_t connection_write( struct connection   * c,
                          const struct packet * p )
{
  int buff_size = p->size + 4;
  unsigned char * buffer = malloc( buff_size );
  packet_write( buffer, p );
  ssize_t nb_bytes = write( c->fd_out, buffer, buff_size );
  if ( nb_bytes == -1 ) {
    perror("write");
    return -1;
  }

  return nb_bytes;
}

/** Returns -1 if there was an error reading.
 *  Otherwise the number of bytes read is returned */
ssize_t connection_read( struct connection * c,
                      struct packet     * p )
{
  unsigned char buffer[BUFF_SIZE] = {0};
  ssize_t nb_bytes = read( c->fd_in, buffer, BUFF_SIZE );
  if ( nb_bytes == -1 ) {
    perror("read");
    return -1;
  }

  packet_read( buffer, p );

  return nb_bytes;
}
