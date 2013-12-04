/**
 * This header has the purpose of offering an API allowing the user to
 * communicate with any device with at most 256 pins using our protocol.
 *
 * The return value of each function is used to for error handling purpose.
 * 0 means no error, an error number is given otherwise.
 */
#ifndef DRIVER_H
#define DRIVER_H

#include "failsafe.h"

/**
 * A connection is a pair of file descriptor, one is used to send
 * informations to the other side and the other is used to receive
 * informations
 */
struct connection{
  int fd_in;
  int fd_out;
  mask unknown_pins;// Pins which are not up to date
  val_list2 current_states;
  val_list16 last_values;
  // maybe add time stamp instead of unknown pins
};

/**
 * Initialize a connection to a device
 *
 * \param path The path to the io file which will be used to talk to the
 *             device (dev/...)
 * \param mode Specify if the mode to use should be synchronous or
 *             asynchronous
 * \param failsafe_state the state to take when connection fails, if null
 *                       is used, failsafe won't be used
 * \param connection Thi
 */
int init_connection( char * path,
                     int mode,
                     struct failsafe *  failsafe_state,
                     struct connection * connection );

/**
 * All the connection allocated resources are freed and connection is closed
 */
void close_connection( struct connection * connection );

/**
 * Send a reset signal to the device, after it, the connection is reinitialized
 */
void reset( struct connection * connection );

/**
 * Set the failsafe state of the device connected by the specified connection
 * \param failsafe_state The state that will be taken when connection fails
 */
int set_failsafe( struct connection * connection,
                  struct failsafe * failsafe_state);

/**
 * Change the mode of the connection to the specified mode
 */
int set_mode( struct connection * connection, int mode );

/**
 * Set the heartbeat to be used for the connection, informing the device and
 * ensuring that the value will be sent frequently enough to avoid falling
 * into fail state.
 * \param frequency 0 -> disable the frequency
 *                  1-7 -> set frequency to 2^n
 */
int set_heartbeat( struct connection * connection, char frequency );

/**
 * Set the type of the given pin
 * \param type @see protocol.h
 */
int set_type( struct connection * connection, int pin_id, char type );

/**
 * Set several type in one packet
 * \param mask The mask of the values used @see mask.h
 * \param types The values to use @see val_list2
 */
int
set_type_mask( struct connection * connection,
                     struct mask * mask,
                     struct val_list2 types );

/**
 * Get the type of the specified pin
 * \param type Type of the pin will be set here
 */
int get_type( struct connection * connection, int pin_id, int8_t * type );

/**
 * Get the types of all the pins used in the given mask
 * \param mask The pins used @see mask.h
 * \param types The values of the types will be placed here
 */
int get_type_mask( struct connection * connection,
                   struct mask * mask,
                   struct val_list2 * types);

/**
 * Read the value of the pin in it's current state, value will be placed in
 * an int in order to be able to stock any type of value.
 * \param value The value of the pin will be stored in it.
 */
int read_value( struct connection * connection, int pin_id, int * value);

/**
 * Read a digital value on a pin, setting it's type to digital before if
 * needed
 * \param mask The mask of the pins to read @see mask
 * \param types The mask of the types in which the values have to be read
 *               @see val_list2
 * \param values The values will be stored in this list @see val_list16
 */
int read_value_mask( struct connection * connection,
                     struct mask * mask,
                     struct val_list2 * types,
                     struct val_list16 * values );

/**
 * Read a digital value on a pin, setting it's type to digital before if
 * needed
 * \param value The value of the pin will be stored in it.
 */
int digital_read( struct connection * connection, int pin_id, bool * value );

/**
 * Read an analogic value on a pin, setting it's type to digital before if
 * needed
 * \param value The value of the pin will be stored in it.
 */
int analogic_read( struct connection * connection,
                   int pin_id, int16_t * value );

/**
 * Read a pwm8 value on a pin, setting it's type to digital before if
 * needed
 * \param value The value of the pin will be stored in it.
 */
int pwm8_read( struct connection * connection, int pin_id, int16_t * value );

/**
 * Read a pwm16 value on a pin, setting it's type to digital before if
 * needed
 * \param value The value of the pin will be stored in it.
 */
int pwm16_read( struct connection * connection, int pin_id, int16_t * value );

/**
 * Write a digital value to a pin, setting it to this type before if needed
 */
int digital_write( struct connection * connection, int pin_id, bool value );

/**
 * Write an analogic value to a pin, setting it to this type before if needed
 */
int analogic_write( struct connection * connection, int pin_id, bool value );

/**
 * Write a pwm8 value to a pin, setting it to this type before if needed
 */
int pwm8_write( struct connection * connection, int pin_id, bool value );

/**
 * Write a pwm16 value to a pin, setting it to this type before if needed
 */
int pwm16_write( struct connection * connection, int pin_id, bool value );

/**
 * Write a mask of values, setting pins types before if needed.
 * \param mask The mask of the pins used @see mask
 * \param types The mask of the types in which the values have to be written
 *               @see val_list2
 * \param values The values to write on pins @see val_list16
 */
int write_value_mask( struct connection * connection,
                      struct mask * mask,
                      struct val_list2 * types,
                      struct val_list16 * values );

#endif//DRIVER_H
