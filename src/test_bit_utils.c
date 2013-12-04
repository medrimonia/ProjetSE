#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bit_utils.h"

#define CMD_SIZE 5

void print_separator(){
  int i;
  for (i = 0; i < 80; i++){
    printf("-");
  }
  printf("\n");
}

int main(int argc, char ** argv){
  // Test 1
  print_separator();
  printf("Test 1\n");
  unsigned char test[CMD_SIZE];
  int i;
  for (i = 0; i < CMD_SIZE; i++) test[i] = 0;
  write_bit_value(test, 0, 2, 4);
  write_bit_value(test, 4, 5, 6);
  printf("Expected packet : |00100001|01000000|\n");
  printf("Created packet  : ");
  display_packet(test, 2);
  printf("\n");
  printf("Expected packet : |21|40|\n");
  printf("Created packet  : ");
  display_packet_hexa(test, 2);
  printf("\n");
  print_separator();
  // Test 2
  printf("Test 2\n");
  unsigned char p[CMD_SIZE];
  for (i = 0; i < CMD_SIZE; i++) p[i] = 0;
  write_cmd(p, 10);
  write_param(p, 3);
  write_data_size(p, 257);
  printf("Expected packet : |10100011|00000001|00000001|\n");
  printf("Created packet  : ");
  display_packet(p, 3);
  printf("\n");
  printf("Expected packet : |a3|01|01|\n");
  printf("Created packet  : ");
  display_packet_hexa(p, 3);
  printf("\n");
  print_separator();
  exit(EXIT_SUCCESS);
}
