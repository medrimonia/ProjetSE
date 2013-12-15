#include <stdlib.h>

#include "mask.h"
#include "bit_utils.h"

mask new_mask( unsigned int nb_pins )
{
  mask m = malloc(nb_pins * sizeof(bool));
  unsigned int i;
  for (i = 0; i < nb_pins; i++) {
    m[i] = false;
  }
  return m;
}

int mask_next_pin_used( const mask m, int start, int mask_length ){
  while ( start < mask_length){
    if (m[start]) return start;
    start++;
  }
  return -1;
}

int mask_nb_pins_used( const mask m , unsigned int mask_length)
{
  unsigned int i;
  unsigned int pins_used = 0;
  for (i = 0; i < mask_length; i++){
    if (m[i] == MASK_PIN_ON){
      pins_used ++;
    }
  }
  return pins_used;
}

int write_mask( unsigned char * buffer,
                const mask m,
                unsigned int mask_length )
{
  unsigned int i;
  for (i = 0; i < mask_length; i++) {
    write_bit_value(buffer, i, m[i], 1);
  }
  return i;
}

int read_mask( const unsigned char * buffer,
               unsigned int offset,
               mask m,
               unsigned int mask_length )
{
  unsigned int i;
  for ( i = 0; i < mask_length; i++ ) {
    m[i] = read_bit_value( buffer, offset + i, 1 );
  }
}

void destroy_mask( mask m )
{
  free(m);
}
