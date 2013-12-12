#ifndef PROTOCOLES_H
#define PROTOCOLES_H

#include <stdint.h>
#include <stdbool.h>

#define OVERHEAD_SIZE      4

/* TYPE (3 bits) */
#define PIN_TYPE_BITS_NB   3
#define PIN_TYPE_DIGITAL   0
#define PIN_TYPE_ANALOG8   1
#define PIN_TYPE_ANALOG16  2
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

/* REPLY CODES (4 bits) */
#define REP_CODE_SUCCESS          0
#define REP_CODE_PARTIAL_SUCCESS  1
#define REP_CODE_FAILURE          7


#define REPLY_ID_BITS_NB          8

//TODO : to change on arduino
#define REPLY_SIZE     1000

#define PARAM_BITS_NB      4
#define DATA_SIZE_BITS_NB 16
#define CHECKSUM_BITS_NB   8

#define VERSION_BYTES_NB   1
#define PINS_NO_BITS_NB    8
#define PINS_NO_BYTES_NB   1

#define TIMEOUT_BITS_NB   16

/* TYPES ASSOCIATED BITS NB */
// Possible values for the mask_p bit
#define USE_PIN_ID         0
#define USE_MASK           1

struct packet {
  unsigned char   header;   // (   1 byte )
  uint16_t        size;     // (   2 bytes)
  unsigned char * data;     // (size bytes)
  uint8_t         checksum; // (   1 byte )
};

uint8_t compute_checksum( const struct packet * p );
/** Computes the checksum and compare it with the one received */

int packet_bytes_nb( const struct packet * p );

bool packet_valid( struct packet * p );

void set_packet_header( struct packet * p,
                        uint8_t cmd_no,
                        uint8_t param,
                        uint16_t size );

void packet_write( unsigned char * buffer, const struct packet * p );
/** Memory for data is allocated here to match data size */
void packet_read ( const unsigned char * buffer, struct packet * p );
/** This function must be called after read_packet to free data memory */
void packet_free ( struct packet * p );

// Return the size of values of the specified type in bits
uint8_t get_type_bits_nb( uint8_t pin_type );

void packet_print( struct packet * p );

#endif//PROTOCOLES_H
