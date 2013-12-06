#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bit_utils.h"

#define CMD_SIZE 5

void print_separator()
{
  int i;
  for (i = 0; i < 80; i++) {
    printf("-");
  }
  printf("\n");
}

void test1()
{
  unsigned char p[CMD_SIZE];
  int i;
  for (i = 0; i < CMD_SIZE; i++) p[i] = 0;
  int16_t v1, v2;
  int offset1, offset2, val_size1, val_size2;
  v1 = 2;
  v2 = 5;
  offset1 = 0;
  offset2 = 4;
  val_size1 = 4;
  val_size2 = 6;
  printf("Writing v1 = %d on %d bits with an offset of %d\n",
         v1, val_size1, offset1);
  printf("Writing v2 = %d on %d bits with an offset of %d\n",
         v2, val_size2, offset2);
  write_bit_value(p, 0, 2, 4);
  write_bit_value(p, 4, 5, 6);
  printf("Expected packet : |00100001|01000000|\n");
  printf("Created packet  : ");
  display_packet(p, 2);
  printf("\n");
  printf("Expected packet : |21|40|\n");
  printf("Created packet  : ");
  display_packet_hexa(p, 2);
  printf("\n");
  v1 = read_bit_value(p, offset1, val_size1);
  v2 = read_bit_value(p, offset2, val_size2);
  printf("read v1 : %d\n",v1);
  printf("read v2 : %d\n",v2);
}

void test2()
{
  printf("Test 2\n");
  unsigned char p[CMD_SIZE];
  int i;
  for (i = 0; i < CMD_SIZE; i++) p[i] = 0;
  int16_t cmd, param, data_size;
  cmd = 10;
  param = 3;
  data_size = 257;
  write_cmd(p, cmd);
  write_param(p, param);
  write_data_size(p, data_size);
  printf("Expected packet : |10100011|00000001|00000001|\n");
  printf("Created packet  : ");
  display_packet(p, 3);
  printf("\n");
  printf("Expected packet : |a3|01|01|\n");
  printf("Created packet  : ");
  display_packet_hexa(p, 3);
  printf("\n");
  cmd = read_cmd(p);
  param = read_param(p);
  data_size = read_data_size(p);
  printf("cmd       : %d\n", cmd);
  printf("param     : %d\n", param);
  printf("data_size : %d\n", data_size);
}

int main( void )
{
  print_separator();
  test1();
  print_separator();
  test2();
  print_separator();
  exit(EXIT_SUCCESS);
}
