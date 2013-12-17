#ifndef FAILSAFE_H
#define FAILSAFE_H

#include <stdint.h>

#include "mask.h"

struct pin_failsafe {
  uint8_t pin_state;
  /** Value is on 16 bits to allow storing diverse types of values. If the
   * length of the associated value is n, value will be read as val16 % 2^n.
   */
  uint16_t pin_value;
};

struct failsafe{
  uint16_t               timeout;
  struct  pin_failsafe * pins_failsafe;
};

struct failsafe * new_failsafe( unsigned int nb_pins );

struct failsafe * sub_failsafe( const struct failsafe * f, mask m,
                                uint8_t mask_length );

uint16_t failsafe_nb_bits( const struct failsafe * f, unsigned int nb_pins );

//TODO refactoring needed, consistency with read_failsafe
void write_failsafe( unsigned char         * p,
                     uint16_t                offset,
                     const struct failsafe * f,
                     uint8_t                 nb_pins);

/**
 * Read only the stream corresponding to pins status and values,
 * offset must be specified in bits
 */
void read_failsafe(const unsigned char * buffer,
                   uint16_t              offset,
                   struct failsafe     * f,
                   uint8_t               nb_pins);

void destroy_failsafe( struct failsafe * f);

#endif//FAILSAFE_H
