#ifndef EMBEDDED
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include "io_utils.h"
#endif
#include <stdlib.h>
#include <stdio.h>

#include "bit_utils.h"
#include "connection.h"

/** BUFF_SIZE should be high enough to read up to 256 pin status (3 bits)
 * and value (up to 16 bits)
 * Thus it should be greater than 1+2+256*19/8+1 = 612 
 * On atm8, only 16 pins are available -> 1 + 2 + 16 * 19 / 8 + 1 = 42*/
#ifndef EMBEDDED
#define BUFF_SIZE 612
#else
#define BUFF_SIZE 42
#endif

/** Returns NULL if connection could not be opened */
#ifndef EMBEDDED
struct connection * connection_open( const char * in_filename,
                                     const char * out_filename )
{
  struct connection * c = malloc( sizeof *c );
  c->protocol_version = 0;
  c->caps.nb_pins = 0;
  c->caps.pins_mask_type = NULL;
  c->state.pins_state    = NULL;
  c->failsafe = NULL;

  c->fd_in = open( in_filename, O_RDONLY );

  if ( c->fd_in == -1 ) {
    perror( "Input file open" );
    free( c );
    return NULL;
  }

  c->fd_out = open( out_filename, O_WRONLY );

  if ( c->fd_out == -1 ) {
    perror( "Output file open" );
    close( c->fd_in );
    free( c );
    return NULL;
  }

  return c;
}
#endif

void connection_init_resources( struct connection * c )
{
  // In order to avoid memory leaks, always relase resources first
  connection_release_resources( c );
  c->caps.pins_mask_type = malloc(c->caps.nb_pins);
  c->state.pins_state = malloc(c->caps.nb_pins * sizeof(struct pin_state));
  c->failsafe = new_failsafe( c->caps.nb_pins );
  unsigned int pin_no;
  for ( pin_no = 0; pin_no < c->caps.nb_pins; pin_no++ ){
    //Unknown state should maybe be initialized
    c->caps.pins_mask_type[pin_no] = 0;// No available states at start
    c->state.pins_state[pin_no].pins_type = PIN_TYPE_DISABLED;
  }
}

void connection_release_resources( struct connection * c )
{
  if ( c->caps.pins_mask_type != NULL) {
    free( c->caps.pins_mask_type );
    c->caps.pins_mask_type = NULL;
  }
  if ( c->state.pins_state != NULL) {
    free( c->state.pins_state );
    c->state.pins_state = NULL;
  }
  if ( c->failsafe != NULL ) {
    destroy_failsafe( c->failsafe );
    c->failsafe = NULL;
  }
}

void connection_close( struct connection * c )
{
  connection_release_resources( c );
#ifndef EMBEDDED
  close( c->fd_in  );
  close( c->fd_out );
#endif
  free( c );
  c = NULL;
}

/** Returns -1 if there was an error writing.
 *  Otherwise the number of bytes written is returned */
int connection_write( struct connection   * c,
                      const struct packet * p )
{
  int buff_size = p->size + 4;
  unsigned char * buffer = malloc( buff_size );
  init_packet( buffer, p->size + 4 );
  packet_write( buffer, p );
  int nb_bytes = write( c->fd_out, buffer, buff_size );
  if ( nb_bytes == -1 ) {
#ifndef EMBEDDED
    perror("write");
#endif
    return -1;
  }
  free( buffer );
  return nb_bytes;
}

/** Returns -1 if there was an error reading.
 *  Otherwise the number of bytes read is returned */
int connection_read( struct connection * c,
                     struct packet     * p )
{
  unsigned char buffer[BUFF_SIZE] = {0};
  int nb_bytes = read( c->fd_in, buffer, BUFF_SIZE );
  if ( nb_bytes < OVERHEAD_SIZE ) {
    return -1;
  }

  packet_read( buffer, p );

  return nb_bytes;
}

int send_packet ( struct connection   * connection,
                  struct packet * p )
{
  p->checksum  = compute_checksum( p );
  return connection_write( connection, p );
}
