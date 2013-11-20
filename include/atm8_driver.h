/**
 * This header has the purpose of offering an API allowing the user to
 * communicate with an ATMega 8 using our protocol.
 */
#ifndef ATM8_DRIVER_H
#define ATM8_DRIVER_H

#define ATM8_SYNC  0
#define ATM8_ASYNC 1

/**
 * Initialize a connection to an ATM8 board
 *
 * \param path The path to the io file which will be used to talk to the
 *             ATM8 (dev/...)
 * \param failsafe_state the state to take when 
 * \return 0 if everything work properly, an error code if something bad
 *         occured
 */
int
init_connection( char * path, struct atm8_failsafe *  failsafe_state);





#endif//ATM8_DRIVER_H
