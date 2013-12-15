/**
 * This header has the purpose of offering an API allowing the user to
 * communicate with any device with at most 256 pins using our protocol.
 *
 * The return value of each function is used to for error handling purpose.
 * 0 means no error, an error number is given otherwise.
 *
 * When a function is used to request a value, this value is return through a
 * parameter.
 *
 * For possible pin types @see protocol.h
 */
#ifndef DRIVER_H
#define DRIVER_H

#include "connection.h"
#include "device_state.h"
#include "failsafe.h"
#include "protocol.h"
#include "value_list.h"

struct connection * new_connection();

/** Initialize a connection to a device
 *
 * \param path The path to the io file which will be used to talk to the
 *             device (dev/...)
 * \param failsafe_state the state to take when connection fails, if null
 *                       is used, failsafe won't be used
 */
int init_connection( const char            * path,
                     const struct failsafe * failsafe_state,
                     struct connection     * connection );

/** All the connection allocated resources are freed and connection is closed */
void destroy_connection( struct connection * c );

/** Ask the device the capacities of all its pins (number of pins, pins type) */
// TODO define a caps struct to store the return value
void get_caps( struct connection * c );

/** Send a reset signal to the device, after it, the connection is reinitialized */
void reset( struct connection * c );

/** Send a ping request to check if the board is alive */
void ping( struct connection * c, uint8_t * device_protocol_version );

/** Get the device-stored type of one pin */
int get_type( struct connection * c, uint8_t pin_id, uint8_t * type );

/** Get the device-stored type of a set of pins
 * types[k] = type(pin i, i-th bits used in the mask)*/
int get_type_mask( struct connection * c,
                   const mask          mask,
                   uint16_t           * types );

/** Set and store the type of one pin on the device. */
int set_type( struct connection * c, uint8_t pin_id, uint8_t type );

/** Set and store the type of a set of pins on the device. */
int set_type_mask( struct connection * c,
                   const mask          mask,
                   const uint16_t    * values );

/** Get the failsafe state of the device connected by the specified connection */
int get_failsafe( struct connection   * c,
                  uint8_t               pin_id,
                  struct pin_failsafe * failsafe );

int get_failsafe_mask( struct connection * c,
                       const mask          mask,
                       struct failsafe   * failsafe );

/** Set the failsafe state of the device connected by the specified connection */
int set_failsafe( struct connection         * c,
                  uint8_t                     pin_no,
                  uint16_t                    timeout,
                  const struct pin_failsafe * failsafe_state );

int set_failsafe_mask( struct connection     * c,
                       const struct failsafe * failsafe_state );

/** Read the value of the pin in it's current state, value will be placed in
 * an int in order to be able to stock any type of value. */
int read_value( const struct connection * c, uint8_t pin_id, const int * val );

/** The following read function set the pin to the right type before if needed.
 *  Pins values are return through the value parameter */
int digital_read ( struct connection * c, uint8_t pin_id, bool    * val );
int analogic_read( struct connection * c, uint8_t pin_id, uint16_t * val );
int pwm8_read    ( struct connection * c, uint8_t pin_id, uint8_t  * val );
int pwm16_read   ( struct connection * c, uint8_t pin_id, uint16_t * val );

/** Read several pins values using a mask.
 * \param types  The mask of the types in which the values have to be read
 *               (@see val_list2) */
int read_value_mask( const struct connection * c,
                     const mask                mask,
                     val_list2               * types,
                     val_list16              * vals );

/** The following write function set the pin to the right type before if needed */
int digital_write ( struct connection * c, uint8_t pin_id, bool    val );
int analogic_write( struct connection * c, uint8_t pin_id, int16_t val );
int pwm8_write    ( struct connection * c, uint8_t pin_id, int16_t val );
int pwm16_write   ( struct connection * c, uint8_t pin_id, int16_t val );

/**
 * Write several pins values using a mask.
 * \param types  The mask of the types in which the values have to be written
 *               (@see val_list2) */
int write_value_mask( const struct connection * c,
                      const mask                mask,
                      val_list2               * types,
                      val_list16              * vals );
//TODO it seems that write_value_mask should write for only one type

#endif//DRIVER_H