#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include "bit_utils.h"

void print_separator()
{
  int i;
  for (i = 0; i < 80; i++) {
    printf("-");
  }
  printf("\n");
}

void print_packet( struct packet * p )
{
  unsigned char * buffer = malloc( p->size + 3 );

  printf("Packet: ");
  packet_write( buffer, p );
  display_packet_hexa( buffer, p->size+3 );
  printf("\n");

  free( buffer );
}

void test_checksum(void)
{
  printf("\nTest checksum computation\n");
  printf("-------------------------\n\n");
  struct packet p;

  p.header = 0x2;
  p.size   = 0x0;
  print_packet( &p );
  printf("Computed checksum: 0x%02X\n", compute_checksum( &p ));
  printf("Expected checksum: 0xFD\n");
  printf("\n");

  p.header  = 0xFF;
  p.size    = 0x01;
  p.data    = malloc( p.size );
  p.data[0] = 0xFE;
  print_packet( &p );
  printf("Computed checksum: 0x%02X\n", compute_checksum( &p ));
  printf("Expected checksum: 0x01\n");
  free(p.data);
  printf("\n");

  p.header  = 0xA3;
  p.size    = 0x03;
  p.data    = malloc( p.size );
  p.data[0] = 0x02; p.data[1] = 0x81; p.data[2] = 0x6E;
  print_packet( &p );
  printf("Computed checksum: 0x%02X\n", compute_checksum( &p ));
  printf("Expected checksum: 0x68\n");
  free(p.data);
  printf("\n");

  //printf("Packet validation\n");

}

int main(void)
{
  print_separator();
  test_checksum();
  print_separator();

  return 0;
}
