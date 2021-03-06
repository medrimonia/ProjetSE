#ifndef MASK_H
#define MASK_H

#include <stdbool.h>
#include <stdint.h>

#define MASK_PIN_ON  true
#define MASK_PIN_OFF false

typedef bool * mask;

mask new_mask( unsigned int nb_pins );

/**
 * Return the number of pins used in a mask
 * \param m The mask to analyze
 */
int mask_nb_pins_used( const mask m , uint8_t mask_length);

/**
 * Return -1, if no next pin used can be found
 * start research at m[start]
 */
int mask_next_pin_used( const mask m, uint16_t start, uint8_t mask_length );

/**
 * Write the content of a mask in a buffer
 * \param m The mask from which the content is read
 * \param buffer An already allocated buffer which should have at least
 *               ceil( NB_PINS / 8 ) free chars available
 * \return The number of chars written
 */
int write_mask( unsigned char * buffer, const mask m, uint8_t mask_length );

/**
 * Read a mask from a buffer, ceil( NB_PINS /8 ) chars will be read.
 * \param buffer The buffer containing the mask datas
 * \param m The mask in which data will be placed.
 * \return the numbers of chars read
 */
int read_mask( const unsigned char * buffer,
               uint16_t offset,
               mask m,
               uint8_t mask_length );

void destroy_mask(mask m);

#endif//MASK_H
