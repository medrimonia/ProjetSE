/**
 * This header has the purpose of offering an API allowing the user to
 * communicate with an ATMega 8 using our protocol.
 */
#ifndef ATM8_DRIVER_H
#define ATM8_DRIVER_H

#include "atm8_failsafe.h"


/* Analog pins */
#define ATM8_A0   0
#define ATM8_A1   1
#define ATM8_A2   2
#define ATM8_A3   3
#define ATM8_A4   4
#define ATM8_A5   5
#define ATM8_A6   6
#define ATM8_A7   7
/* Digital pins */
#define ATM8_D0   8
#define ATM8_D1   9
#define ATM8_D2  10
#define ATM8_D3  11
#define ATM8_D4  12
#define ATM8_D5  13
#define ATM8_D6  14
#define ATM8_D7  15
#define ATM8_D8  16
#define ATM8_D9  17
#define ATM8_D10 18
#define ATM8_D11 19
#define ATM8_D12 20
#define ATM8_D13 21

#define ATM8_SYNC  0
#define ATM8_ASYNC 1

/**
 * An atm8 connection is a pair of file descriptor, one is used to send
 * informations to the other side and the other is used to receive
 * informations
 */
struct atm8_connection{
  int fd_in;
  int fd_out;
  //TODO memorize current states and last known values, maybe with a timestamp
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
atm8_init_connection( char * path,
                      int mode,
                      struct atm8_failsafe *  failsafe_state,
                      struct atm8_connection * connection );

/**
 * All the connection allocated resources are freed and connection is closed
 * \param connection The connection to close
 */
void
atm8_close_connection( struct atm8_connection * connection );

/**
 * Send a reset signal to the atm8, after it, the connection is reinitialized
 * \param connection The connection to reset
 */
void
atm8_reset( struct atm8_connection * connection );

/**
 * Set the failsafe state of the atm8 connected by the specified connection
 * \param connection The connection concerned
 * \param failsafe_state The state that will be taken when connection fails
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_set_failsafe( struct atm8_connection * connection,
                   struct atm8_failsafe * failsafe_state);

/**
 * Change the mode of the connection to the specified mode
 * \param connection  The connection concerned
 * \param mode The mode to be taken
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_set_mode( struct atm8_connection * connection,
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
atm8_set_heartbeat( struct atm8_connection * connection,
                    char frequency );

/**
 * Set the state of the given pin
 * \param connection The connection concerned
 * \param pin_no @see atm8_pins.h
 * \param state @see atm8_states.h
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_set_state( struct atm8_connection * connection,
                int pin_no,
                char state );

/**
 * Set several state in one packet
 * \param connection The connection concerned
 * \param mask The mask of the values used @see atm8_mask.h
 * \param states The values to use @see atm8_val_list2
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_set_state_mask( struct atm8_connection * connection,
                     struct atm8_mask * mask,
                     struct atm8_val_list2 states );

/**
 * Get the state of the specified pin
 * \param connection The connection concerned
 * \param pin_no @see atm8_pins.h
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_get_state( struct atm8_connection * connection,
                int pin_no );

/**
 * Get the states of all the pins used in the given mask
 * \param connection The connection concerned
 * \param mask The pins used @see atm8_mask.h
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_get_state_mask( struct atm8_connection * connection,
                     struct atm8_mask * mask );

/**
 * Read the value of the pin in it's current state, value will be placed in
 * an int in order to be able to stock any type of value.
 * \param connection The connection concerned
 * \param pin_no The number of the pin where you have to read
 * \param value The value of the pin will be stored in it.
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_read_value( struct atm8_connection * connection,
                 int pin_no,
                 int * value);

/**
 * Read a digital value on a pin, setting it's state to digital before if
 * needed
 * \param connection The connection concerned
 * \param mask The mask of the pins to read @see atm8_mask
 * \param states The mask of the states in which the values have to be read
 *               @see atm8_val_list2
 * \param values The values will be stored in this list @see atm8_val_list16
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_read_value_mask( struct atm8_connection * connection,
                      struct atm8_mask * mask,
                      struct atm8_val_list2 * states,
                      struct atm8_val_list16 * values );

/**
 * Read a digital value on a pin, setting it's state to digital before if
 * needed
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param value The value of the pin will be stored in it.
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_digital_read( struct atm8_connection * connection,
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
atm8_analogic_read( struct atm8_connection * connection,
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
atm8_analogic_pwm8( struct atm8_connection * connection,
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
atm8_analogic_pwm16( struct atm8_connection * connection,
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
atm8_digital_write( struct atm8_connection * connection,
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
atm8_analogic_write( struct atm8_connection * connection,
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
atm8_pwm8_write( struct atm8_connection * connection,
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
atm8_pwm16_write( struct atm8_connection * connection,
                  int pin_no,
                  bool value );

/**
 * Write a mask of values, setting pins types before if needed.
 * \param connection The connection concerned
 * \param mask The mask of the pins used @see atm8_mask
 * \param states The mask of the states in which the values have to be written
 *               @see atm8_val_list2
 * \param values The values to write on pins @see atm8_val_list16
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_write_value_mask( struct atm8_connection * connection,
                       struct atm8_mask * mask,
                       struct atm8_val_list2 * states,
                       struct atm8_val_list16 * values );

/**
 * Launch a monitoring read on the atm8, concerning the specified pin, the
 * atm8 will then send the values at a given frequency.
 * \param connection The connection concerned
 * \param state The state of the pin in which you will read
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param frequency The update frequency, in Hertz
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_monitor_read( struct atm8_connection * connection,
                   int8_t state,
                   int8_t pin_no,
                   int8_t frequency);


//TODO finish

/**
 * Launch a monitoring read on the atm8, concerning several pins. All pins
 * \param connection The connection concerned
 * \param pin_no The number of the pin to use @see atm8_pins.h
 * \param frequency The update frequency, in Hertz
 * \return 0 if everything worked well, an error number in case of an error
 */
int
atm8_monitor_read_mask( struct atm8_connection * connection,
                        int8_t pin_no,
                        int8_t frequency);



#endif//ATM8_DRIVER_H
