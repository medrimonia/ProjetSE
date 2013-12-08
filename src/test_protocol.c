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
  unsigned char * buffer = malloc( p->size + 4 );

  printf("Packet: ");
  packet_write( buffer, p );
  display_packet_hexa( buffer, p->size + 4 );
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

void test_packet_conversion(void)
{
  printf("\nTest packet conversion\n");
  printf("-------------------------\n\n");

  struct packet src, dst;
  src.header  = 0x12;
  src.size    = 0x04;
  src.data    = malloc( src.size );
  src.data[0] = 0xDE; src.data[1] = 0xAD; src.data[2] = 0xBE; src.data[3] = 0xEF;

  unsigned char * buffer = malloc( src.size + 4 );
  packet_write( buffer, &src );
  packet_read(  buffer, &dst );

  printf("Src ");
  print_packet( &src );
  printf("Dst ");
  print_packet( &dst );
  printf("\n");

  packet_free( &src );
  packet_free( &dst );
}

int main(void)
{
  print_separator();
  test_checksum();
  print_separator();
  test_packet_conversion();
  print_separator();

  return 0;
}
