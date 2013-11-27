#ifndef FAILSAFE_H
#define FAILSAFE_H

#include "mask.h"

struct failsafe{
  /**
   * Failsafe can set only some pins when connection fails.
   */
  mask pins_used;
  /**
   * The pins states of the values needs to be defined for the pins used
   */
  val_list2 pins_states;
  /**
   * The list of values to use when failing, all values are considered
   * as 16 bits, since we might have mixed values. If the length of the
   * associated value is n, value will be read as val16 % 2^n.
   */
  val_list16 pins_values;
};

#endif//FAILSAFE_H
