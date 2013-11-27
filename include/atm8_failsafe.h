#ifndef ATM8_FAILSAFE_H
#define ATM8_FAILSAFE_H

#include "atm8_mask.h"

struct atm8_failsafe{
  /**
   * Failsafe can set only some pins when connection fails.
   */
  atm8_mask pins_used;
  /**
   * The pins states of the values needs to be defined for the pins used
   */
  atm8_val_list2 pins_states;
  /**
   * The list of values to use when failing, all values are considered
   * as 16 bits, since we might have mixed values. If the length of the
   * associated value is n, value will be read as val16 % 2^n.
   */
  atm8_val_list16 pins_values;
};

#endif//ATM8_FAILSAFE_H
