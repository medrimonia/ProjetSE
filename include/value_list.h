#ifndef VALUE_LIST_H
#define VALUE_LIST_H

#include "mask.h"

typedef char[NB_PINS]     val_list2;
typedef char[NB_PINS]     val_list8;
typedef char[NB_PINS * 2] val_list16;

/**
 * Write the content of a value list to a buffer
 * \param buffer An already allocated buffer which should have at least
 *               ceil(NB_PINS / 4) free chars available
 * \param l A list of the values which will be written to the buffer
 *          l[i] will be written only if m[i] = MASK_PIN_ON
 * \param m The mask indicating which values should be written to the buffer
 * \return The number of chars written
 */
int
write_val_list2(char * buffer, val_list2 l, mask m);

/**
 * Write the content of a value list to a buffer
 * \param buffer An already allocated buffer which should have at least
 *               NB_PINS free chars available
 * \param l A list of the values which will be written to the buffer
 *          l[i] will be written only if m[i] = MASK_PIN_ON
 * \param m The mask indicating which values should be written to the buffer
 * \return The number of chars written
 */
int
write_val_list8(char * buffer, val_list8 l, mask m);

/**
 * Write the content of a value list to a buffer
 * \param buffer An already allocated buffer which should have at least
 *               2 * NB_PINS free chars available
 * \param l A list of the values which will be written to the buffer
 *          l[i] will be written only if m[i] = MASK_PIN_ON
 * \param m The mask indicating which values should be written to the buffer
 * \return The number of chars written
 */
int
write_val_list16(char * buffer, val_list16 l, mask m);

/**
 * Read the content of a value list from a buffer, according to a mask, and
 * place the result in a value list.
 * \param buffer The buffer containing the values
 * \param l The list in which values will be stored
 *          l[i] will be overwritten only if m[i] = MASK_PIN_ON
 * \param m The mask indicating which values are used in the buffer
 * \return the number of chars read.
 */
int
read_val_list2(char * buffer, val_list2 l, mask m);

/**
 * Read the content of a value list from a buffer, according to a mask, and
 * place the result in a value list.
 * \param buffer The buffer containing the values
 * \param l The list in which values will be stored
 *          l[i] will be overwritten only if m[i] = MASK_PIN_ON
 * \param m The mask indicating which values are used in the buffer
 * \return the number of chars read.
 */
int
read_val_list8(char * buffer, val_list8 l, mask m);

/**
 * Read the content of a value list from a buffer, according to a mask, and
 * place the result in a value list.
 * \param buffer The buffer containing the values
 * \param l The list in which values will be stored
 *          l[i] will be overwritten only if m[i] = MASK_PIN_ON
 * \param m The mask indicating which values are used in the buffer
 * \return the number of chars read.
 */
int
read_val_list16(char * buffer, val_list16 l, mask m);

#endif//VALUE_LIST_H
