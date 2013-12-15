#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "protocol.h"

#ifndef EMBEDDED
void print_ok( const char * message );

void print_separator();

// title must be terminated by a \0
void print_title( const char * title );
#endif

#endif//TEST_UTILS_H
