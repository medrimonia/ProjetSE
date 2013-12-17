#include <stdlib.h>

#include "failsafe.h"
#include "bit_utils.h"
#include "protocol.h"

struct failsafe * new_failsafe( unsigned int nb_pins )
{
  struct failsafe * f = malloc(sizeof(struct failsafe));
  f->timeout       = 0;
  f->pins_failsafe = malloc(nb_pins * sizeof(struct pin_failsafe));
  unsigned int i;
  for (i = 0; i < nb_pins; i++){
    f->pins_failsafe[i].pin_state = PIN_TYPE_DISABLED;
    f->pins_failsafe[i].pin_value = 0;
  }
  return f;
}

struct failsafe * sub_failsafe( const struct failsafe * f, mask m,
                                uint8_t mask_length )
{
  struct failsafe * sub = new_failsafe(mask_nb_pins_used( m, mask_length ) );
  int pin_index = 0;
  int sub_index = 0;
  sub->timeout = f->timeout;
  do {
    pin_index = mask_next_pin_used( m, pin_index, mask_length );
    if ( pin_index  == -1 ) break;
    sub->pins_failsafe[sub_index].pin_state = f->pins_failsafe[pin_index].pin_state;
    sub->pins_failsafe[sub_index].pin_value = f->pins_failsafe[pin_index].pin_value;
    sub_index++;
    pin_index++;
  } while( true );
  return sub;
}

uint16_t failsafe_nb_bits( const struct failsafe * f, unsigned int nb_pins)
{
  unsigned int sum = 16 + nb_pins;//timeout_size + mask_size
  unsigned int i;
  for (i = 0; i < nb_pins; i++) {
    if (f->pins_failsafe[i].pin_state == PIN_TYPE_DISABLED) continue;
    uint8_t val_size = get_type_bits_nb(f->pins_failsafe[i].pin_state);
    sum += val_size + PIN_TYPE_BITS_NB;
  }
  return sum;
}

void write_failsafe( unsigned char         * p,
                     uint16_t                offset,
                     const struct failsafe * f,
                     uint8_t                 nb_pins )
{
  unsigned int i;
  for (i = 0; i < nb_pins; i++) {
    if (f->pins_failsafe[i].pin_state == PIN_TYPE_DISABLED) continue;
    uint16_t pin_val = f->pins_failsafe[i].pin_value;
    uint8_t pin_type = f->pins_failsafe[i].pin_state;
    uint8_t val_size = get_type_bits_nb(pin_type);
    write_bit_value(p, offset, pin_type, PIN_TYPE_BITS_NB);
    offset += PIN_TYPE_BITS_NB;
    write_bit_value(p, offset, pin_val, val_size);
    offset += val_size;
  }
}

void read_failsafe(const unsigned char * buffer,
                   uint16_t              offset,
                   struct failsafe     * f,
                   uint8_t               nb_pins)
{
  unsigned int i;
  for (i = 0; i < nb_pins; i++)
  {
    uint8_t pin_type = read_bit_value( buffer, offset, PIN_TYPE_BITS_NB );
    offset += PIN_TYPE_BITS_NB;
    uint8_t val_size = get_type_bits_nb( pin_type );
    uint16_t val = read_bit_value( buffer, offset, val_size );
    offset += val_size;
    f->pins_failsafe[i].pin_state = pin_type;
    f->pins_failsafe[i].pin_value = val;
  }
}

void destroy_failsafe( struct failsafe * f )
{
  free(f->pins_failsafe);
  free(f);
}
