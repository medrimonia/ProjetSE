#ifndef FAILSAFE_H
#define FAILSAFE_H

#include <stdint.h>

#include "mask.h"

struct pin_failsafe {
    uint8_t  pin_state;
    /** Value is on 16 bits to allow storing diverse types of values. If the
     * length of the associated value is n, value will be read as val16 % 2^n.
     */
    uint16_t pin_value;
};

struct failsafe{
  /** Failsafe can set only some pins when connection fails.  */
  mask     pins_used;
  uint16_t timeout;
  struct   pin_failsafe * pins_failsafe;
};

#endif//FAILSAFE_H
