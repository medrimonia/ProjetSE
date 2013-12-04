#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "protocol.h"

//dst bits must be 0 where they will be written
//TODO improve
void write_bit_value( unsigned char * dst, int offset, int16_t val, int val_size ){
  int bit_no = offset / 8;
  offset = offset % 8;
  int bits_remaining = val_size;
  int available_bits = 8 - offset;
  while (bits_remaining > available_bits){
    int shift = bits_remaining - available_bits;
    int16_t to_write = val >> shift;
    val -= (to_write << shift);
    dst[bit_no] |= (unsigned char)to_write;
    bits_remaining -= available_bits;
    available_bits = 8;
    offset = 0;
    bit_no++;
  }
  int shift = 8 - bits_remaining - offset;
  int16_t to_write = val << shift;
  dst[bit_no] |= (unsigned char)to_write;
}

//void read_bit_value( unsigned char * src, int offset

void display_binary(unsigned char v){
  int i;
  for(i = 7; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}

void display_packet(unsigned char * p, int packet_size){
  printf("|");
  int i;
  for (i = 0; i < packet_size; i++){
    display_binary(p[i]);
    printf("|");
  }
}

void display_packet_hexa(unsigned char * p, int packet_size){
  printf("|");
  int i;
  for (i = 0; i < packet_size; i++){
    printf("%02x|", p[i]);
  }
}

void write_cmd(unsigned char * p, int cmd_no){
  write_bit_value(p, 0, cmd_no, CMD_BITS_NB);
}

void write_param(unsigned char * p, int param_no){
  write_bit_value(p, 4, param_no, PARAM_BITS_NB);
}

void write_data_size(unsigned char * p, int16_t data_size){
  write_bit_value(p + 1, 0, data_size, DATA_SIZE_BITS_NB);
}
