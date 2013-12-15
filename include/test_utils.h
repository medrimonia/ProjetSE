#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "protocol.h"

struct multiple_assert{
  const char * name;
  int nb_success;
  int nb_tests;
};

void print_separator();

// title must be terminated by a \0
void print_title( const char * title );

#endif//TEST_UTILS_H
