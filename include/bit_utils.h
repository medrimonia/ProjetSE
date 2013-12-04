#ifndef BIT_UTILS_H
#define BIT_UTILS_H

/**
 * param offset In bits
 * param val_size In bits
 */
void write_bit_value( unsigned char * dst,
                      int offset,
                      int16_t val,
                      int val_size );


int16_t read_bit_value( unsigned char * dst,
                        int offset,
                        int val_size );

void display_binary( unsigned char v );

/**
 * param packet_size In bytes
 */
void display_packet( unsigned char * p, int packet_size );
void display_packet_hexa( unsigned char * p, int packet_size );

void write_cmd( unsigned char * p, int cmd_no );

void write_param( unsigned char * p, int param );

void write_data_size( unsigned char * p, int16_t data_size );

int16_t read_cmd(unsigned char * p);

int16_t read_param(unsigned char * p);

int16_t read_data_size(unsigned char * p);


#endif//BIT_UTILS_H
