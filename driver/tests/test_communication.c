#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include "bit_utils.h"
#include "connection.h"
#include "driver.h"

#define NB_PINS 16

struct connection * c = NULL;

void check_connection_caps()
{
#ifdef DEBUG
  printf( "Read number of pins     : %d\n", c->caps.nb_pins );
  printf( "Read type mask          : " );
  display_packet_hexa( c->caps.pins_mask_type, c->caps.nb_pins );
  printf( "\n" );
#endif
  assert( c->caps.nb_pins == NB_PINS );
  unsigned int i;
  for ( i = 0; i < NB_PINS; i++ ) {
    assert( c->caps.pins_mask_type[i] == 31 );//TODO hard-coded value
  }
}

int main(void){
  c = connection_open( DEV2DRIV_FILENAME, DRIV2DEV_FILENAME );
  if ( c == NULL ) return EXIT_FAILURE;
  printf( "Connection initialized, sending getCaps :\n" );
  get_caps( c );
  // Tests
  check_connection_caps();

  // Ending
  connection_close( c );
  return EXIT_SUCCESS;
}
