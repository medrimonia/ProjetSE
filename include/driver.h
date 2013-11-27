/**
 * This header has the purpose of offering an API allowing the user to
 * communicate with an ATMega 8 using our protocol.
 */
#ifndef DRIVER_H
#define DRIVER_H

#include "failsafe.h"

/**
 * An atm8 connection is a pair of file descriptor, one is used to send
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
 * Initialize a connection to an ATM8 board
 *
 * \param path The path to the io file which will be used to talk to the
 *             ATM8 (dev/...)
 * \param mode Specify if the mode to use should be synchronous or
 *             asynchronous
 * \param failsafe_state the state to take when connection fails, if null
 *                       is used, failsafe won't be used
 * \param connection Thi
 * \return 0 if everything worked well, an error number in case of an error
 */
int
init_connection( char * path,
                      int mode,
                      struct failsafe *  failsafe_state,
                      struct connection * connection );

/**
 * All the connection allocated resources are freed and connection is closed
 * \param connection The connection to close
 */
void
close_connection( struct connection * connection );

/**
 * Send a reset signal to the atm8, after it, the connection is reinitialized
 * \param connection The connection to reset
 */
void
reset( struct connection * connection );

/**
 * Set the failsafe state of the atm8 connected by the specified connection
 * \param connection The connection concerned
 * \param failsafe_state The state that will be taken when connection fails
 * \return 0 if everything worked well, an error number in case of an error
 */
int
set_failsafe( struct connection * connection,
                   struct failsafe * failsafe_state);

/**
 * Change the mode of the connection to the specified mode
 * \param connection  The connection concerned
 * \param mode The mode to be taken
 * \return 0 if everything worked well, an error number in case of an error
 */
int
set_mode( struct connection * connection,
               int mode );

/**
 * Set the heartbeat to be used for the connection, informing the atm8 and
 * ensuring that the value will be sent frequently enough to avoid falling
 * into fail state.
 * \param connection The connection concerned
 * \param frequency 0 -> disable the frequency
 *                  1-7 -> set frequency to 2^n
 * \return 0 if everything worked well, an error number in case of an error
 */
int
set_heartbeat( struct connection * connection,
                    char frequency );

/**
 * Set the state of the given pin
 * \param connection The connection concerned
 * \param pin_no @see atm8_pins.h
 * \param state @see states.h
 * \return 0 if everything worked well, an error number in case of an error
 */
int
set_state( struct connection * connection,
                int pin_no,
                char state );

/**
 * Set several state in one packet
 * \param connection The connection concerned
 * \param mask The mask of the values used @see mask.h
 * \param states The values to use @see val_list2
 * \return 0 if everything worked well, an error number in case of an error
 */
int
set_state_mask( struct connection * connection,
                     struct mask * mask,
                     struct val_list2 states );

/**
 * Get the state of the specified pin
 * \param connection The connection concerned
 * \param pin_no @see atm8_pins.h
 * \param state State of the pin will be set here
 * \return 0 if everything worked well, an error number in case of an error
 */
int
get_state( struct connection * connection,
                int pin_no,
                int8_t * state );

/**
 * Get the states of all the pins used in the given mask
 * \param connection The connection concerned
 * \param mask The pins used @see mask.h
 * \param states The values of the states will be placed here
 * \return 0 if everything worked well, an error number in case of an error
 */
int
get_state_mask( struct connection * connection,
                     struct mask * mask,
                     struct val_list2 * states);

/**
 * Read the value of the pin in it's current state, value will be placed in
 * an int in order to be able to stock any type of value.
 * \param connection The connection concerned
 * \param pin_no The number of the pin where you have to read
 * \param value The value of the pin will be stored in it.
 * \return 0 if everything worked well, an error number in case of an error
 */
int
read_value( struct connection * connection,
                 int pin_no,
                 int * value);

/**
 * Read a digital value on a pin, setting it's state to digital before if
 * needed
 * \param connection The connection concerned
 * \param mask The mask of the pins to read @see mask
 * \param states The mask of the states in which the values have to be read
 *               @see val_list2
 * \param values The values will be stored in this list @see val_list16
 * \return 0 if everything worked well, an error number in case of an error
 */
int
read_value_mask( struct connection * connection,
                      struct mask * mask,
                      struct val_list2 * states,
                      struct val_list16 * values );

/**
 * Read a digital value on a pin, setting it's state to digital before if
 * needed
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param value The value of the pin will be stored in it.
 * \return 0 if everything worked well, an error number in case of an error
 */
int
digital_read( struct connection * connection,
                   int pin_no,
                   bool * value );

/**
 * Read an analogic value on a pin, setting it's state to digital before if
 * needed
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param value The value of the pin will be stored in it.
 * \return 0 if everything worked well, an error number in case of an error
 */
int
analogic_read( struct connection * connection,
                    int pin_no,
                    int16_t * value );

/**
 * Read a pwm8 value on a pin, setting it's state to digital before if
 * needed
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param value The value of the pin will be stored in it.
 * \return 0 if everything worked well, an error number in case of an error
 */
int
pwm8_read( struct connection * connection,
                int pin_no,
                int16_t * value );

/**
 * Read a pwm16 value on a pin, setting it's state to digital before if
 * needed
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param value The value of the pin will be stored in it.
 * \return 0 if everything worked well, an error number in case of an error
 */
int
pwm16_read( struct connection * connection,
                 int pin_no,
                 int16_t * value );

/**
 * Write a digital value to a pin, setting it to this state before if needed
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param value The value to write
 * \return 0 if everything worked well, an error number in case of an error
 */
int
digital_write( struct connection * connection,
                    int pin_no,
                    bool value );

/**
 * Write an analogic value to a pin, setting it to this state before if needed
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param value The value to write
 * \return 0 if everything worked well, an error number in case of an error
 */
int
analogic_write( struct connection * connection,
                     int pin_no,
                     bool value );

/**
 * Write a pwm8 value to a pin, setting it to this state before if needed
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param value The value to write
 * \return 0 if everything worked well, an error number in case of an error
 */
int
pwm8_write( struct connection * connection,
                 int pin_no,
                 bool value );

/**
 * Write a pwm16 value to a pin, setting it to this state before if needed
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param value The value to write
 * \return 0 if everything worked well, an error number in case of an error
 */
int
pwm16_write( struct connection * connection,
                  int pin_no,
                  bool value );

/**
 * Write a mask of values, setting pins types before if needed.
 * \param connection The connection concerned
 * \param mask The mask of the pins used @see mask
 * \param states The mask of the states in which the values have to be written
 *               @see val_list2
 * \param values The values to write on pins @see val_list16
 * \return 0 if everything worked well, an error number in case of an error
 */
int
write_value_mask( struct connection * connection,
                       struct mask * mask,
                       struct val_list2 * states,
                       struct val_list16 * values );

/**
 * Launch a monitoring read on the atm8, concerning the specified pin, the
 * atm8 will then send the values at a given frequency.
 * Rhis feature is not currently supported by the driver
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param frequency The update frequency, in Hertz
 * \return 0 if everything worked well, an error number in case of an error
 */
int
monitor_read( struct connection * connection,
                   int8_t pin_no,
                   int8_t frequency);

/**
 * Launch a monitoring read on the atm8, concerning several pins.
 * This feature is not currently supported by the driver.
 * \param connection The connection concerned
 * \param mask The mask of the pins to be monitored
 * \param frequency The update frequency, in Hertz
 * \return 0 if everything worked well, an error number in case of an error
 */
int
monitor_read_mask( struct connection * connection,
                        struct mask * mask,
                        int8_t frequency);



#endif//DRIVER_H
