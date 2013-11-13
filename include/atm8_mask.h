#ifndef ATM8_MASK_H
#define ATM8_MASK_H

#include <stdbool.h>

#include "atm8_pins.h"

#define ATM8_MASK_PIN_ON  true
#define ATM8_MASK_PIN_OFF false

typedef mask bool[ATM8_NB_PINS];

/**
 * Return the number of pins used in a mask
 * \param m The mask to analyze
 */
int
nb_pins_used(mask m);

/**
 * Write the content of a mask in a buffer
 * \param m The mask from which the content is read
 * \param buffer An already allocated buffer which should have at least
 *               ceil(NB_PINS / 3) free chars available
 * \return The number of chars written
 */
int
write_mask(mask m, char * buffer);

#endif//ATM8_MASK_H
