#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>

// Fill the given packet with 0
void init_packet( unsigned char * p, int packet_size );

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
void display_packet     ( const unsigned char * p, int packet_size );
void display_packet_hexa( const unsigned char * p, int packet_size );

void write_cmd      ( unsigned char * p, int     cmd_no    );
void write_param    ( unsigned char * p, int     param     );
void write_data_size( unsigned char * p, int16_t data_size );
void write_header   ( unsigned char * p,
                      int cmd_no, int param, int16_t data_size );

void write_value_list ( unsigned char  * p,
                        unsigned int     offset,
                        const uint16_t * values,
                        unsigned int     nb_values,
                        unsigned int     value_size );

int16_t read_cmd      ( const unsigned char * p );
int16_t read_param    ( const unsigned char * p );
int16_t read_data_size( const unsigned char * p );

void read_value_list ( const unsigned char  * p,
                       unsigned int           offset,
                       uint16_t             * values,
                       unsigned int           nb_values,
                       unsigned int           value_size );


#endif//BIT_UTILS_H
