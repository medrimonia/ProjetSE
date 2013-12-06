#ifndef FIRMWARE_UTILS_H
#define FIRMWARE_UTILS_H

struct connection {
    int fd_in;
    int fd_out;
};

struct connection * connection_open( const char * in_filename,
                                     const char * out_filename );

void connection_close( struct connection * c );

void connection_write( struct connection * c, const unsigned char * packet, int   size );
void connection_read(  struct connection * c,       unsigned char * packet, int * size );

#endif /* FIRMWARE_UTILS_H */
