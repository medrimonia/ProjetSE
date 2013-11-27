#ifndef MASK_H
#define MASK_H

#include <stdbool.h>

#include "atm8_pins.h"

#define MASK_PIN_ON  true
#define MASK_PIN_OFF false

typedef bool[NB_PINS]     mask;

/**
 * Return the number of pins used in a mask
 * \param m The mask to analyze
 */
int nb_pins_used( mask m );

int next_pin_used( mask m, int start );

/**
 * Write the content of a mask in a buffer
 * \param m The mask from which the content is read
 * \param buffer An already allocated buffer which should have at least
 *               ceil( NB_PINS / 8 ) free chars available
 * \return The number of chars written
 */
int write_mask( char * buffer, mask m );

/**
 * Read a mask from a buffer, ceil( NB_PINS /8 ) chars will be read.
 * \param buffer The buffer containing the mask datas
 * \param m The mask in which data will be placed.
 * \return the numbers of chars read
 */
int read_mask( char * buffer, mask m );

#endif//MASK_H
