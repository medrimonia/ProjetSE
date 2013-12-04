#ifndef PACKET_H
#define PACKET_H

#include "driver.h"

// Return 0 if everything worked
// Checksum is calculated before sending
int send_packet( char * p, int size );

#endif//PACKET_H
