#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "protocol.h"
#include "bit_utils.h"

void compute_and_cmp_checksum( struct packet * p, uint8_t expected_checksum )
{
  p->checksum = compute_checksum( p );
  print_packet( p );
  printf("Computed checksum: 0x%02X\n", p->checksum);
  printf("Expected checksum: 0x%02X\n", expected_checksum);
}

void test_checksum(void)
{
  print_title("Test checksum computation");
  struct packet p;

  p.header = 0x2;
  p.size   = 0x0;
  compute_and_cmp_checksum( &p, 0xFD );
  printf("\n");

  p.header  = 0xFF;
  p.size    = 0x01;
  p.data    = malloc( p.size );
  p.data[0] = 0xFE;
  compute_and_cmp_checksum( &p, 0x01 );
  free(p.data);
  printf("\n");

  p.header  = 0xA3;
  p.size    = 0x03;
  p.data    = malloc( p.size );
  p.data[0] = 0x02; p.data[1] = 0x81; p.data[2] = 0x6E;
  compute_and_cmp_checksum( &p, 0x68 );
  free(p.data);
  printf("\n");
}

void test_packet_conversion(void)
{
  print_title("Test packet conversion");

  struct packet src, dst;
  src.header  = 0x12;
  src.size    = 0x04;
  src.data    = malloc( src.size );
  src.data[0] = 0xDE; src.data[1] = 0xAD; src.data[2] = 0xBE; src.data[3] = 0xEF;
  src.checksum = compute_checksum( &src );

  unsigned char * buffer = malloc( src.size + 4 );
  init_packet(  buffer, src.size + 4 );
  packet_write( buffer, &src );
  packet_read(  buffer, &dst );

  printf("Src ");
  print_packet( &src );
  printf("Dst ");
  print_packet( &dst );
  printf("\n");

  free( buffer );
  packet_free( &src );
  packet_free( &dst );
}

void test_packet_validation(void)
{
  print_title("Test packet validation");

  struct packet p;
  p.header  = 0x34;
  p.size    = 0x04;
  p.data    = malloc( p.size );
  p.data[0] = 0xDE; p.data[1] = 0xAD; p.data[2] = 0xBE; p.data[3] = 0xEF;
  p.checksum = compute_checksum( &p );

  print_packet( &p );
  assert( packet_valid( &p ) );
  printf("Checksum checked!\n");
  printf("\n");

  packet_free( &p );
}

int main(void)
{
  print_separator();
  test_checksum();
  print_separator();
  test_packet_conversion();
  print_separator();
  test_packet_validation();
  print_separator();

  return 0;
}
