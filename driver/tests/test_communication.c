#include <stdlib.h>
#include <stdio.h>

#include "connection.h"
#include "driver.h"

struct connection * c = NULL;

int main(void){
  c = connection_open( DEV2DRIV_FILENAME, DRIV2DEV_FILENAME );
  printf( "Connection initialized, sending getCaps :\n" );
  get_caps( c );
  connection_close( c );
  return EXIT_SUCCESS;
}
