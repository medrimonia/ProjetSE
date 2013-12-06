#include <stdlib.h>

#include "failsafe.h"

#include "bit_utils.h"
#include "protocol.h"

struct failsafe * new_failsafe( unsigned int nb_pins ){
  struct failsafe * f = malloc(sizeof(struct failsafe));
  f->pins_used = new_mask(nb_pins);
  f->timeout = 0;
  f->pins_failsafe = malloc(nb_pins * sizeof(struct pin_failsafe));
  return f;
}

uint16_t failsafe_nb_bits( const struct failsafe * f, unsigned int nb_pins )
{
  unsigned int sum = 16 + nb_pins;//timeout_size + mask_size
  unsigned int i;
  for (i = 0; i < nb_pins; i++){
    if (f->pins_used[i] == MASK_PIN_OFF) continue;
    uint8_t val_size = get_type_bits_nb(f->pins_failsafe[i].pin_state);
    sum += val_size + PIN_TYPE_BITS_NB;
  }
  return sum;
}

void write_failsafe( unsigned char         * p,
                     const struct failsafe * f,
                     unsigned int            nb_pins ){
  write_bit_value(p, 0, f->timeout, 16);
  write_mask(p + 2, f->pins_used, nb_pins);
  unsigned int i;
  unsigned int offset = 16 + nb_pins;
  for (i = 0; i < nb_pins; i++){
    if (f->pins_used[i] == MASK_PIN_OFF) continue;
    uint16_t pin_val = f->pins_failsafe[i].pin_value;
    uint8_t pin_type = f->pins_failsafe[i].pin_state;
    uint8_t val_size = get_type_bits_nb(pin_type);
    write_bit_value(p, offset, pin_type, PIN_TYPE_BITS_NB);
    offset += PIN_TYPE_BITS_NB;
    write_bit_value(p, offset, pin_val, val_size);
    offset += val_size;
  }
}

void destroy_failsafe( struct failsafe * f ){
  destroy_mask(f->pins_used);
  free(f->pins_failsafe);
  free(f);
}
