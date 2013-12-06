#ifndef PROTOCOLES_H
#define PROTOCOLES_H

#include <stdint.h>

// Return the size of values of the specified type in bits
uint8_t get_type_bits_nb( uint8_t pin_type );

/* TYPE (3 bits) */
#define PIN_TYPE_BITS_NB   3
#define PIN_TYPE_ANALOG8   0
#define PIN_TYPE_ANALOG16  1
#define PIN_TYPE_DIGITAL   2
#define PIN_TYPE_PWM8      3
#define PIN_TYPE_PWM16     4
#define PIN_TYPE_USE_STATE 7
#define PIN_TYPE_DISABLED  7

/* COMMAND (4 bits) */
#define CMD_BITS_NB        4
#define CMD_GET_CAPS       0
#define CMD_RESET          1
#define CMD_PING           2
#define CMD_READ           3
#define CMD_WRITE          4
#define CMD_GET_TYPE       5
#define CMD_SET_TYPE       6
#define CMD_GET_FAILSAFE   7
#define CMD_SET_FAILSAFE   8
// This command number is reserved for the specific purpose of spontaneous
// messages from the device in a future version
#define CMD_RESERVED      15

#define PARAM_BITS_NB      4
#define DATA_SIZE_BITS_NB 16

#endif//PROTOCOLES_H
