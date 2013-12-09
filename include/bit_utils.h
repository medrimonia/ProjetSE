#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>
#include <stdbool.h>

// Fill the given packet with 0
void init_packet( unsigned char * p, uint16_t packet_size );

/**
 * param offset In bits
 * param val_size In bits
 */
void write_bit_value( unsigned char * dst,
                      int             offset,
                      int16_t         val,
                      int             val_size );

int16_t read_bit_value( const unsigned char * dst, int offset, int val_size );

void display_binary( unsigned char v );

/**
 * param packet_size In bytes
 */
void display_packet     ( const unsigned char * p, uint16_t packet_size );
void display_packet_hexa( const unsigned char * p, uint16_t packet_size );

void write_cmd      ( unsigned char * p, uint8_t  cmd_no     );
void write_pin_type ( unsigned char * p, uint8_t  pin_type   );
void write_mask_p   ( unsigned char * p, uint8_t  mask_p     );
void write_param    ( unsigned char * p, uint8_t  param      );
void write_data_size( unsigned char * p, uint16_t data_size  );
void write_data     ( unsigned char * p, unsigned char * data,
                      uint16_t data_size );
void write_checksum ( unsigned char * p, uint8_t checksum, uint16_t data_size );

void write_value_list ( unsigned char  * p,
                        unsigned int     offset,
                        const uint16_t * values,
                        unsigned int     nb_values,
                        unsigned int     value_size );

uint8_t  read_cmd      ( const unsigned char * p );
uint8_t  read_param    ( const unsigned char * p );
uint16_t read_data_size( const unsigned char * p );
void     read_header   ( const unsigned char * p, unsigned char * dst );
void     read_data     ( const unsigned char * p,
                               unsigned char * dst, uint16_t size );
uint8_t  read_checksum ( const unsigned char * p, uint16_t data_size );

void read_value_list ( const unsigned char  * p,
                       unsigned int           offset,
                       uint16_t             * values,
                       unsigned int           nb_values,
                       unsigned int           value_size );

#endif//BIT_UTILS_H
