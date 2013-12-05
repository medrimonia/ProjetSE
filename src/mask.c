#include <stdlib.h>

#include "mask.h"

#include "bit_utils.h"

mask new_mask( unsigned int nb_pins ){
  mask m = malloc(nb_pins * sizeof(bool));
  int i;
  for (i = 0; i < nb_pins; i++){
    m[i] = false;
  }
  return m;
}

int write_mask( unsigned char * buffer, const mask m, int mask_length){
  int i;
  for (i = 0; i < mask_length; i++){
    write_bit_value(buffer, i, m[i], 1);
  }
  return i;
}
