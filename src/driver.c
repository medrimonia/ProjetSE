#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "driver.h"

#include "bit_utils.h"
#include "protocol.h"
#include "failsafe.h"

//TODO cmd_size should depend on connection
#define CMD_SIZE 15

struct connection * new_connection()
{
  struct connection * c = malloc(sizeof(struct connection));
  c->fd_in = -1;
  c->fd_out = -1;
  c->nb_pins = 0;
  return c;
}

void destroy_connection( struct connection * c )
{
  free(c);
}

void get_caps( struct connection * connection )
{
  struct packet p;
  set_packet_header(&p, CMD_GET_CAPS, 0, 0);
  send_packet(connection, &p);
}

void reset( struct connection * connection )
{
  struct packet p;
  set_packet_header(&p, CMD_RESET, 0, 0);
  send_packet(connection, &p);
}

void ping( struct connection * connection,
           char protocol_version )
{
  struct packet p;
  set_packet_header(&p, CMD_PING, 0, 1);
  unsigned char buffer[VERSION_BYTES_NB];
  init_packet(buffer, VERSION_BYTES_NB);
  write_bit_value(buffer, 0, protocol_version, 8);
  p.data = buffer;
  send_packet(connection, &p);
}

int16_t generic_read( struct connection * c,
                      uint8_t pin_id, uint8_t pin_mode )
{
  struct packet p;
  set_packet_header(&p, CMD_READ, (pin_mode << 1) + USE_PIN_ID,
                    PINS_NO_BYTES_NB);
  unsigned char buffer[PINS_NO_BYTES_NB];
  init_packet(buffer, PINS_NO_BYTES_NB);
  write_bit_value(buffer, 0, pin_id, 8);
  p.data = buffer;
  send_packet(c, &p);
  return EXIT_FAILURE;//TODO wait and parse answer
}

int digital_read( struct connection * c, uint8_t pin_id, bool * val )
{
  generic_read( c, pin_id, PIN_TYPE_DIGITAL);
  //TODO read value
  return EXIT_FAILURE;//TODO wait and parse answer
}

int analogic_read( struct connection * c, uint8_t pin_id, int16_t * val )
{
  generic_read( c, pin_id, PIN_TYPE_ANALOG16);
  //TODO read value
  return EXIT_FAILURE;//TODO wait and parse answer
}

int pwm8_read( struct connection * c, uint8_t pin_id, int8_t * val )
{
  generic_read( c, pin_id, PIN_TYPE_PWM8);
  //TODO read value
  return EXIT_FAILURE;//TODO wait and parse answer
}

int pwm16_read( struct connection * c, uint8_t pin_id, int16_t * val)
{
  generic_read( c, pin_id, PIN_TYPE_PWM16);
  //TODO read value
  return EXIT_FAILURE;//TODO wait and parse answer
}

int generic_write( struct connection * c,
                   uint8_t pin_id, int pin_mode, int16_t val, int val_size )
{
  struct packet p;
  int data_bytes = BITS2BYTES(PINS_NO_BITS_NB + val_size);
  set_packet_header(&p, CMD_WRITE, (pin_mode << 1) + USE_PIN_ID, data_bytes);
  unsigned char * buffer = malloc(data_bytes);
  init_packet(buffer, data_bytes);
  write_bit_value(buffer, 0, pin_id, PINS_NO_BITS_NB);
  write_bit_value(buffer, PINS_NO_BITS_NB, val, val_size);
  p.data = buffer;
  send_packet(c, &p);
  free(buffer);
  return EXIT_FAILURE;
}

int digital_write ( struct connection * c, uint8_t pin_id, bool    val )
{
  generic_write(c, pin_id, PIN_TYPE_DIGITAL, val, 1);
  return EXIT_FAILURE;//TODO wait and parse answer
}

int analogic_write( struct connection * c, uint8_t pin_id, int16_t val )
{
  generic_write(c, pin_id, PIN_TYPE_ANALOG16, val, 16);
  return EXIT_FAILURE;//TODO wait and parse answer
}

int pwm8_write    ( struct connection * c, uint8_t pin_id, int16_t val )
{
  generic_write(c, pin_id, PIN_TYPE_PWM8, val, 8);
  return EXIT_FAILURE;//TODO wait and parse answer
}

int pwm16_write   ( struct connection * c, uint8_t pin_id, int16_t val )
{
  generic_write(c, pin_id, PIN_TYPE_PWM16, val, 16);
  return EXIT_FAILURE;//TODO wait and parse answer
}

/*
int write_value_mask( const struct connection * c,
                      const mask              * mask,
                      val_list2               * types,
                      val_list16              * vals )
{
  //TODO
  return EXIT_FAILURE;
}
*/

int get_type( struct connection * c, uint8_t pin_id, int8_t * type )
{
  struct packet p;
  set_packet_header(&p, CMD_GET_TYPE, USE_PIN_ID, PINS_NO_BYTES_NB);
  unsigned char buffer[PINS_NO_BYTES_NB];
  init_packet(buffer, PINS_NO_BYTES_NB);
  write_bit_value(buffer, 0, pin_id, PINS_NO_BITS_NB);
  p.data = buffer;
  send_packet(c, &p);
  //TODO set value at end
  return EXIT_FAILURE;//TODO wait and parse answer
}

int get_type_mask( struct connection * c,
                   const mask        * mask,
                   val_list2         * types )
{
  struct packet p;
  unsigned int data_size = (c->nb_pins-1) / 8 + 1;
  set_packet_header(&p, CMD_GET_TYPE, USE_MASK, data_size);
  unsigned char * buffer = malloc(data_size);
  init_packet(buffer, data_size);
  write_mask(buffer, *mask, c->nb_pins);
  p.data = buffer;
  send_packet(c, &p);
  //TODO read values
  free(buffer);
  return EXIT_FAILURE;//TODO wait and parse answer
}

int set_type( struct connection * c, uint8_t pin_id, char type )
{
  struct packet p;
  unsigned int data_size = BITS2BYTES(PINS_NO_BITS_NB + PIN_TYPE_BITS_NB);
  set_packet_header(&p, CMD_SET_TYPE, USE_PIN_ID, data_size);
  unsigned char * buffer = malloc(data_size);
  init_packet(buffer, data_size);
  write_bit_value(buffer, 0, pin_id, PINS_NO_BITS_NB);
  write_bit_value(buffer, PINS_NO_BITS_NB, type, PIN_TYPE_BITS_NB);
  p.data = buffer;
  send_packet(c, &p);
  free(buffer);
  return EXIT_FAILURE;//TODO wait and parse answer
}

/*
int set_type_mask( struct connection * c,
                   const mask        * mask,
                   const uint16_t    * values,
                   unsigned int        nb_values )
{
  unsigned char p[CMD_SIZE];
  init_packet(p, CMD_SIZE);
  int data_bits_nb = c->nb_pins + PIN_TYPE_BITS_NB * nb_values;
  int packet_size = 3 + (data_bits_nb - 1) / 8 + 1;
  write_header(p, CMD_SET_TYPE, 0, USE_MASK, packet_size - 3);
  write_mask(p + 3, *mask, c->nb_pins);
  write_value_list(p + 3, c->nb_pins, values, nb_values, PIN_TYPE_BITS_NB);
  send_packet(c, p, packet_size);
  //TODO read reply
  return EXIT_FAILURE;
}

int get_failsafe( struct connection   * c,
                  uint8_t               pin_id,
                  struct pin_failsafe * failsafe )
{
  unsigned char p[CMD_SIZE];
  init_packet(p, CMD_SIZE);
  write_header(p, CMD_GET_FAILSAFE, 0, USE_PIN_ID, 1);
  write_bit_value(p + 3, 0, pin_id, 8);
  send_packet(c, p, 4);
  //TODO read reply and parse it
  return EXIT_FAILURE;
}

int get_failsafe_mask( struct connection       * c,
                       const mask              * mask,
                       struct failsafe         * failsafe )
{
  unsigned char p[CMD_SIZE];
  init_packet(p, CMD_SIZE);
  write_header(p, CMD_GET_FAILSAFE, 0, USE_MASK, (c->nb_pins - 1) / 8 + 1);
  write_mask(p + 3, *mask, c->nb_pins);
  send_packet(c, p, 4);
  //TODO read and use values
  return EXIT_FAILURE;
}

int set_failsafe( struct connection       * c,
                  const struct failsafe   * failsafe_state)
{
  unsigned char p[CMD_SIZE];
  init_packet(p, CMD_SIZE);
  unsigned int data_bits_nb = failsafe_nb_bits(failsafe_state, c->nb_pins);
  unsigned int data_bytes_nb = (data_bits_nb - 1) / 8 + 1;
  write_header(p, CMD_SET_FAILSAFE, 0, USE_MASK, data_bytes_nb);
  write_failsafe(p + 3, failsafe_state, c->nb_pins);
  send_packet(c, p, 3 + data_bytes_nb);
  //TODO
  return EXIT_FAILURE;
}
*/

int send_packet ( struct connection   * connection,
                  struct packet * p )
{
  p->checksum  = compute_checksum(p);
  unsigned char * buffer = malloc(packet_bytes_nb(p));
  init_packet(buffer, packet_bytes_nb(p));
  packet_write(buffer, p);
  int n = write(connection->fd_out, buffer, packet_bytes_nb(p));
  if (n != packet_bytes_nb(p)){
    perror("Failed to write");
    exit(EXIT_FAILURE);
  }
  free(buffer);
  //packet_print(p);
  //ignoring connection, just dumping for now
  //TODO calculate checksum
  //TODO send to connection
  return EXIT_FAILURE;
}
