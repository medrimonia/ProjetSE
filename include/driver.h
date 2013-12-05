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

#include "failsafe.h"
#include "value_list.h"

/**
 * A connection is a pair of file descriptor, one is used to send
 * informations to the other side and the other is used to receive
 * informations
 */
struct connection{
  int fd_in;
  int fd_out;
  mask unknown_pins;// Pins which are not up to date
  //TODO adding things !!! (nb pins, pins states)
  // maybe add time stamp instead of unknown pins
};

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
void close_connection( struct connection * c );

/** Ask the device the capacities of all its pins (number of pins, pins type) */
// TODO define a caps struct to store the return value
void get_caps( struct connection * c );

/** Send a reset signal to the device, after it, the connection is reinitialized */
void reset( struct connection * c );

/** Send a ping request to check if the board is alive */
void ping( struct connection * c, char protocol_version );

/** Get the device-stored type of one pin */
int get_type( const struct connection * c, uint8_t pin_id, int8_t * type );

/** Get the device-stored type of a set of pins */
int get_type_mask( const struct connection * c,
                   const mask              * mask,
                   val_list2               * types );

/** Set and store the type of one pin on the device. */
int set_type( const struct connection * c, uint8_t pin_id, char type );

/** Set and store the type of a set of pins on the device. */
int set_type_mask( const struct connection * c,
                   const mask              * mask,
                   val_list2                 types );

/** Get the failsafe state of the device connected by the specified connection */
int get_failsafe( const struct connection * c,
                  uint8_t                   pin_id,
                  struct pin_failsafe     * failsafe );

int get_failsafe_mask( const struct connection * c,
                       const mask              * mask,
                       struct failsafe         * failsafe );

/** Set the failsafe state of the device connected by the specified connection */
int set_failsafe( const struct connection * c,
                  const struct failsafe   * failsafe_state,
                  uint16_t                  timeout );

/** Read the value of the pin in it's current state, value will be placed in
 * an int in order to be able to stock any type of value. */
int read_value( const struct connection * c, uint8_t pin_id, const int * val );

/** The following read function set the pin to the right type before if needed.
 *  Pins values are return through the value parameter */
int digital_read ( struct connection * c, uint8_t pin_id, bool    * val );
int analogic_read( struct connection * c, uint8_t pin_id, int16_t * val );
int pwm8_read    ( struct connection * c, uint8_t pin_id, int8_t * val );
int pwm16_read   ( struct connection * c, uint8_t pin_id, int16_t * val );

/** Read several pins values using a mask.
 * \param types  The mask of the types in which the values have to be read
 *               (@see val_list2) */
int read_value_mask( const struct connection * c,
                     const mask              * mask,
                     val_list2               * types,
                     val_list16              * vals );

/** The following write function set the pin to the right type before if needed */
int digital_write ( const struct connection * c, uint8_t pin_id, bool    val );
int analogic_write( const struct connection * c, uint8_t pin_id, int16_t val );
int pwm8_write    ( const struct connection * c, uint8_t pin_id, int16_t val );
int pwm16_write   ( const struct connection * c, uint8_t pin_id, int16_t val );

/**
 * Write several pins values using a mask.
 * \param types  The mask of the types in which the values have to be written
 *               (@see val_list2) */
int write_value_mask( const struct connection * c,
                      const mask              * mask,
                      val_list2               * types,
                      val_list16              * vals );

// Handle the checksum calculation
int send_packet( struct connection * connection,
                 const unsigned char * p,
                 int packet_size);

#endif//DRIVER_H
