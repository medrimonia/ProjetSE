#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "protocol.h"

void print_separator();

// title must be terminated by a \0
void print_title( const char * title );

void print_packet( struct packet * p );

#endif//TEST_UTILS_H
