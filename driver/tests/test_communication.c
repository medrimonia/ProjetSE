#include <stdlib.h>
#include <stdio.h>

#include "bit_utils.h"
#include "connection.h"
#include "driver.h"

struct connection * c = NULL;

void check_connection_caps()
{
  printf( "Read number of pins     : %d\n", c->caps.nb_pins );
  printf( "Read type mask          : " );
  display_packet_hexa( c->caps.pins_mask_type, c->caps.nb_pins );
  printf( "\n" );
}

int main(void){
  c = connection_open( DEV2DRIV_FILENAME, DRIV2DEV_FILENAME );
  printf( "Connection initialized, sending getCaps :\n" );
  get_caps( c );
  // Tests
  check_connection_caps();

  // Ending
  connection_close( c );
  return EXIT_SUCCESS;
}
