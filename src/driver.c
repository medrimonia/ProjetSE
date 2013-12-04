#include <stdio.h>
#include <stdlib.h>

#include "driver.h"

#include "bit_utils.h"
#include "protocol.h"

//TODO cmd_size should depend on connection
#define CMD_SIZE 10

void ping( struct connection * connection,
           char protocol_version ){
  unsigned char p[CMD_SIZE];
  init_packet(p, CMD_SIZE);
  write_cmd(p, CMD_PING);
  write_param(p, 0);
  write_data_size(p,1);
  write_bit_value(p + 3, 0, protocol_version, 8);
  send_packet(NULL, p, 4);
}

int send_packet ( struct connection * connection,
                  const unsigned char * p,
                  int packet_size ){
  printf("Sent packet : ");
  display_packet_hexa( p, packet_size );
  printf("\n");
  //ignoring connection, just dumping for now
  //TODO calculate checksum
  //TODO send to connection
}
