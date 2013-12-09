#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_utils.h"

#include "bit_utils.h"

void print_separator()
{
  unsigned int i;
  for (i = 0; i < 80; i++) {
    printf("-");
  }
  printf("\n");
}

void print_title( const char * title )
{
  printf("\n%s\n", title);
  unsigned int i;
  for (i = 0; i < strlen(title); i++) {
    printf("-");
  }
  printf("\n\n");
}

void print_packet( struct packet * p )
{
  //TODO avoid malloc for print if we have enough time
  unsigned char * buffer = malloc( p->size + 4 );
  init_packet( buffer, p->size + 4 );

  printf("Packet: ");
  packet_write( buffer, p );
  display_packet_hexa( buffer, p->size + 4 );
  printf("\n");

  free( buffer );
}
