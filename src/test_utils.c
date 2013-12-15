#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_utils.h"

#include "bit_utils.h"

void print_ok( const char * message )
{
  unsigned int message_max_length = 15;
  printf( "%s", message );
  unsigned int i;
  for (i = 0; i < message_max_length - strlen(message); i++) {
    printf(" ");
  }
  printf( ": OK\n" );
}

void print_separator()
{
  unsigned int i;
  for (i = 0; i < 80; i++) {
    printf("-");
  }
  printf("\n");
}

void print_title( const char * title )
{
  printf("\n%s\n", title);
  unsigned int i;
  for (i = 0; i < strlen(title); i++) {
    printf("-");
  }
  printf("\n\n");
}
