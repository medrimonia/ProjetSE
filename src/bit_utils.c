#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.h"
#include "bit_utils.h"

void init_packet( unsigned char * p, uint16_t packet_size )
{
  uint16_t i;
  for (i = 0; i < packet_size; i++) p[i] = 0;
}

//dst bits must be 0 where they will be written
//TODO improve
void write_bit_value( unsigned char * dst,
                      int offset,
                      int16_t val,
                      int val_size )
{
  int bit_no = offset / 8;
  offset = offset % 8;
  int bits_remaining = val_size;
  int available_bits = 8 - offset;
  while (bits_remaining > available_bits) {
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

int16_t read_bit_value( const unsigned char * src, int offset, int val_size )
{
  int16_t val = 0;
  int bit_no = offset / 8;
  offset = offset % 8;
  int bits_remaining = val_size;
  int available_bits = 8 - offset;
  while (bits_remaining > available_bits) {
    int16_t read_value = (unsigned char)(src[bit_no] << offset) >> offset;
    val += read_value;
    bits_remaining -= available_bits;
    available_bits = 8;
    offset = 0;
    bit_no++;
    val = val << bits_remaining;
  }
  int shift = 8 - bits_remaining - offset;
  unsigned char read_value  = src[bit_no] >> shift;
  // Removing the unwanted left part
  val += (unsigned char)(read_value << (8 - bits_remaining)) >> (8 - bits_remaining);
  return val;
}

void display_binary( unsigned char v )
{
  int i;
  for(i = 7; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}

void display_packet( const unsigned char * p, uint16_t packet_size )
{
  printf("|");
  uint16_t i;
  for (i = 0; i < packet_size; i++) {
    display_binary(p[i]);
    printf("|");
  }
}

void display_packet_hexa( const unsigned char * p, uint16_t packet_size )
{
  printf("|");
  uint16_t i;
  for (i = 0; i < packet_size; i++) {
    printf("%02x|", p[i]);
  }
}

void write_cmd( unsigned char * p, uint8_t cmd_no )
{
  write_bit_value(p, 0, cmd_no, CMD_BITS_NB);
}

void write_pin_type( unsigned char * p, uint8_t pin_type )
{
  write_bit_value(p, CMD_BITS_NB, pin_type, PIN_TYPE_BITS_NB);
}

void write_mask_p( unsigned char * p, uint8_t mask_p )
{
  write_bit_value(p, CMD_BITS_NB+PIN_TYPE_BITS_NB, mask_p, 1);
}

void write_param( unsigned char * p, uint8_t param_no )
{
  write_bit_value(p, CMD_BITS_NB, param_no, PARAM_BITS_NB);
}

void write_data_size( unsigned char * p, uint16_t data_size )
{
  write_bit_value(p + 1, 0, data_size, DATA_SIZE_BITS_NB);
}

void write_data( unsigned char * p, unsigned char * data, uint16_t data_size )
{
  uint16_t i;
  for (i = 0; i < data_size; i++) {
    write_bit_value(p+3+i, 0, data[i], 8);
  }
}

void write_checksum( unsigned char * p, uint8_t checksum, uint16_t data_size )
{
  write_bit_value(p+3+data_size, 0, checksum, CHECKSUM_BITS_NB);
}

void write_header( unsigned char * p, uint8_t cmd_no,
                   uint8_t pin_type, uint8_t mask_p, uint16_t data_size )
{
  write_cmd(p, cmd_no);
  write_pin_type(p, pin_type);
  write_mask_p(p, mask_p);
  write_data_size(p, data_size);
}

void write_value_list( unsigned char * p,
                       unsigned int offset,
                       const uint16_t * values,
                       unsigned int nb_values,
                       unsigned int value_size )
{
  unsigned int i;
  for (i = 0; i < nb_values; i++) {
    write_bit_value(p, offset, values[i], value_size);
    offset += value_size;
  }
}

uint8_t read_cmd( const unsigned char * p )
{
  return read_bit_value(p, 0, CMD_BITS_NB);
}

uint8_t read_param( const unsigned char * p )
{
  return read_bit_value(p, 4, PARAM_BITS_NB);
}

uint16_t read_data_size( const unsigned char * p )
{
  return read_bit_value(p, 8, DATA_SIZE_BITS_NB);
}

void read_header( const unsigned char * p, unsigned char * dst )
{
  memcpy( dst, p, 1 );
}

void read_data( const unsigned char * p, unsigned char * dst, uint16_t size )
{
  memcpy( dst, p + 3, size );
}

uint8_t read_checksum( const unsigned char * p, uint16_t data_size )
{
  return read_bit_value(p+3+data_size, 0, CHECKSUM_BITS_NB);
}

void read_value_list( const unsigned char * p,
                      unsigned int          offset,
                      uint16_t *            values,
                      unsigned int          nb_values,
                      unsigned int          value_size )
{
  unsigned int i;
  for (i = 0; i < nb_values; i++) {
    values[i] = read_bit_value(p, offset, value_size);
    offset += value_size;
  }
}
