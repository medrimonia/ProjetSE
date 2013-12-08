#include <stdlib.h>
#include "protocol.h"
#include "bit_utils.h"

uint8_t compute_checksum( struct packet * p )
{
  uint8_t sum = p->header;
  sum += (uint8_t)(p->size % (1<<8)); // First  byte of p->size
  sum += (uint8_t)(p->size >> 8);     // Second byte
  int i = 0;
  for (; i < p->size; i++ ) {
    sum += p->data[i];
  }
  sum = ~sum;

  return sum;
}

bool packet_valid( struct packet * p )
{
  return compute_checksum( p ) == p->checksum;
}

void packet_write( unsigned char * buffer, const struct packet * p )
{
  write_cmd      ( buffer, read_cmd  (&p->header) );
  write_param    ( buffer, read_param(&p->header) );
  write_data_size( buffer, p->size               );
  //write_data     ( buffer, p->data,    p->size   );
  //write_checksum ( buffer, p->checksum           );
}

void packet_read( const unsigned char * buffer, struct packet * p )
{
  uint16_t size = read_data_size(buffer);
  p->size = size;
  p->data = malloc(size);
  //p->checksum = read_checksum(buffer);

  write_cmd  ( &p->header, read_cmd  (buffer)       );
  write_param( &p->header, read_param(buffer)       );
  //write_data ( p->data  , read_data (buffer), size );
}

void packet_free( struct packet * p )
{
  free( p->data );
}

uint8_t get_type_bits_nb( uint8_t pin_type )
{
  switch(pin_type) {
    case PIN_TYPE_DIGITAL:
      return 1;
    case PIN_TYPE_ANALOG8:
    case PIN_TYPE_PWM8:
      return 8;
    case PIN_TYPE_ANALOG16:
    case PIN_TYPE_PWM16:
      return 16;
  }
  // Invalid mode
  return 0;
}
