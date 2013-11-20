#ifndef ATM8_VALUE_LIST_H
#define ATM8_VALUE_LIST_H

#include "atm8_mask.h"

typedef char[ATM8_NB_PINS]     atm8_val_list2;
typedef char[ATM8_NB_PINS]     atm8_val_list8;
typedef char[ATM8_NB_PINS * 2] atm8_val_list16;

/**
 * Write the content of a value list to a buffer
 * \param buffer An already allocated buffer which should have at least
 *               ceil(NB_PINS / 4) free chars available
 * \param l A list of the values which will be written to the buffer
 *          l[i] will be written only if m[i] = ATM8_MASK_PIN_ON
 * \param m The mask indicating which values should be written to the buffer
 * \return The number of chars written
 */
int
write_val_list2(char * buffer, atm8_val_list2 l, atm8_mask m);

/**
 * Write the content of a value list to a buffer
 * \param buffer An already allocated buffer which should have at least
 *               NB_PINS free chars available
 * \param l A list of the values which will be written to the buffer
 *          l[i] will be written only if m[i] = ATM8_MASK_PIN_ON
 * \param m The mask indicating which values should be written to the buffer
 * \return The number of chars written
 */
int
write_val_list8(char * buffer, atm8_val_list8 l, atm8_mask m);

/**
 * Write the content of a value list to a buffer
 * \param buffer An already allocated buffer which should have at least
 *               2 * NB_PINS free chars available
 * \param l A list of the values which will be written to the buffer
 *          l[i] will be written only if m[i] = ATM8_MASK_PIN_ON
 * \param m The mask indicating which values should be written to the buffer
 * \return The number of chars written
 */
int
write_val_list16(char * buffer, atm8_val_list16 l, atm8_mask m);

/**
 * Read the content of a value list from a buffer, according to a mask, and
 * place the result in a value list.
 * \param buffer The buffer containing the values
 * \param l The list in which values will be stored
 *          l[i] will be overwritten only if m[i] = ATM8_MASK_PIN_ON
 * \param m The mask indicating which values are used in the buffer
 * \return the number of chars read.
 */
int
read_val_list2(char * buffer, atm8_val_list2 l, atm8_mask m);

/**
 * Read the content of a value list from a buffer, according to a mask, and
 * place the result in a value list.
 * \param buffer The buffer containing the values
 * \param l The list in which values will be stored
 *          l[i] will be overwritten only if m[i] = ATM8_MASK_PIN_ON
 * \param m The mask indicating which values are used in the buffer
 * \return the number of chars read.
 */
int
read_val_list8(char * buffer, atm8_val_list8 l, atm8_mask m);

/**
 * Read the content of a value list from a buffer, according to a mask, and
 * place the result in a value list.
 * \param buffer The buffer containing the values
 * \param l The list in which values will be stored
 *          l[i] will be overwritten only if m[i] = ATM8_MASK_PIN_ON
 * \param m The mask indicating which values are used in the buffer
 * \return the number of chars read.
 */
int
read_val_list16(char * buffer, atm8_val_list16 l, atm8_mask m);

#endif//ATM8_VALUE_LIST_H
