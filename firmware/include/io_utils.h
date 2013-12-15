#ifndef IO_UTILS_H
#define IO_UTILS_H

// fd is used in order to have the same prototype as unistd write and read, but
// it isn't used at all

int write( int fd, unsigned char * buffer, unsigned int n );

int read ( int fd, unsigned char * buffer, unsigned int n );

#endif//IO_UTILS_H
