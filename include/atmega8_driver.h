/**
 * This header has the purpose of offering an API allowing the user to
 * communicate with an ATMega 8 using our protocol.
 */
#ifndef ATM8_DRIVER_H
#define ATM8_DRIVER_H

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
