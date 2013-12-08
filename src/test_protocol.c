#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"

void print_separator()
{
  int i;
  for (i = 0; i < 80; i++) {
    printf("-");
  }
  printf("\n");
}

void test_checksum(void)
{
  printf("Test checksum computation\n");
  struct packet p;

  p.header = 0x0;
  p.size   = 0x0;
  printf("Packet: \n");
  printf("Computed checksum: 0x%X\n", compute_checksum(&p));
  printf("Expected checksum: 0xFF\n");

  p.header  = 0xFF;
  p.size    = 0x01;
  p.data    = malloc( p.size );
  p.data[0] = 0xFE;
  printf("Computed checksum: %X\n", compute_checksum(&p));
  printf("Expected checksum: 1\n");
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
