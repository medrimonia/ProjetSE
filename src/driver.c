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

// Read the next packet, place it in reply and verify the reply id.
int read_reply( struct connection * c, struct packet * reply )
{
  int n;
  n = connection_read( c, reply );
  if (n == -1){
    perror("Failed to read a packet");
    exit(EXIT_FAILURE);
  }
  uint8_t reply_code = reply->header % 16;
  if (reply_code != REP_CODE_SUCCESS){
    printf("Invalid reply code received : %d\n", reply_code);
  }
  uint8_t reply_id = read_bit_value( reply->data, 0, REPLY_ID_BITS_NB );
  /* TODO
  if (reply_id != expected){
    printf("Invalid reply id received : %d\n", reply_id);
  }
  */
  if (!packet_valid( reply )){
    printf( "Invalid checksum!\n" );
    printf( "\tExpected : %d\n", reply->checksum );
    printf( "\tReceived : %d\n", compute_checksum( reply ) );
  }
  return n;
}

void get_caps( struct connection * connection )
{
  struct packet p;
  set_packet_header(&p, CMD_GET_CAPS, 0, 0);
  send_packet(connection, &p);
  struct packet reply;
  read_reply( connection, &reply );
  //TODO here something should be done about init_resources followed by rd_caps
  connection->caps.nb_pins = read_bit_value( reply.data,
                                             REPLY_ID_BITS_NB,
                                             PINS_NO_BITS_NB );
  connection_init_resources( connection );
  read_device_caps( &(connection->caps), &reply );
  packet_free( &reply );
}

void reset( struct connection * connection )
{
  struct packet p;
  set_packet_header(&p, CMD_RESET, 0, 0);
  send_packet(connection, &p);
  struct packet reply;
  read_reply( connection, &reply );
  packet_free( &reply );
  // No reading action needed
}

void ping( struct connection * connection,
           char protocol_version,
           uint8_t * device_protocol_version)
{
  struct packet p;
  set_packet_header(&p, CMD_PING, 0, 1);
  unsigned char buffer[VERSION_BYTES_NB];
  init_packet(buffer, VERSION_BYTES_NB);
  write_bit_value(buffer, 0, protocol_version, 8);
  p.data = buffer;
  send_packet(connection, &p);
  struct packet reply;
  read_reply( connection, &reply );
  *device_protocol_version = read_bit_value( reply.data + 1, 0,
                                             VERSION_BITS_NB );
  packet_free( &reply );
}

int16_t generic_read( struct connection * c,
                      uint8_t pin_id,
                      uint8_t pin_mode,
                      uint16_t * val)
{
  struct packet p;
  set_packet_header(&p, CMD_READ, (pin_mode << 1) + USE_PIN_ID,
                    PINS_NO_BYTES_NB);
  unsigned char buffer[PINS_NO_BYTES_NB];
  init_packet(buffer, PINS_NO_BYTES_NB);
  write_bit_value(buffer, 0, pin_id, 8);
  p.data = buffer;
  send_packet(c, &p);
  // Read answer and store result
  struct packet reply;
  read_reply( c, &reply );
  unsigned int type_bits_nb = get_type_bits_nb(pin_mode);
  *val = read_bit_value( reply.data, REPLY_ID_BITS_NB, type_bits_nb );
  c->state.pins_state[pin_id].pins_val = *val;
  packet_free( &reply );
  return EXIT_SUCCESS;
}

int digital_read( struct connection * c, uint8_t pin_id, bool * val )
{
  uint16_t tmp_val;
  generic_read( c, pin_id, PIN_TYPE_DIGITAL, &tmp_val );//TODO test return
  *val = tmp_val;
  return EXIT_SUCCESS;
}

int analogic_read( struct connection * c, uint8_t pin_id, uint16_t * val )
{
  generic_read( c, pin_id, PIN_TYPE_ANALOG16, val );//TODO test return
  return EXIT_SUCCESS;
}

int pwm8_read( struct connection * c, uint8_t pin_id, uint8_t * val )
{
  uint16_t tmp_val;
  generic_read( c, pin_id, PIN_TYPE_PWM8, &tmp_val );//TODO test return
  *val = tmp_val;
  return EXIT_SUCCESS;
}

int pwm16_read( struct connection * c, uint8_t pin_id, uint16_t * val)
{
  generic_read( c, pin_id, PIN_TYPE_PWM16, val );//TODO test return
  return EXIT_SUCCESS;
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
  // Reading :
  struct packet reply;
  read_reply( c, &reply );
  packet_free( &reply );
  // Applying :
  c->state.pins_state[pin_id].pins_val = val;
  return EXIT_SUCCESS;
}

int digital_write ( struct connection * c, uint8_t pin_id, bool    val )
{
  generic_write(c, pin_id, PIN_TYPE_DIGITAL, val, 1);
  return EXIT_SUCCESS;
}

int analogic_write( struct connection * c, uint8_t pin_id, int16_t val )
{
  generic_write(c, pin_id, PIN_TYPE_ANALOG16, val, 16);
  return EXIT_SUCCESS;
}

int pwm8_write    ( struct connection * c, uint8_t pin_id, int16_t val )
{
  generic_write(c, pin_id, PIN_TYPE_PWM8, val, 8);
  return EXIT_SUCCESS;
}

int pwm16_write   ( struct connection * c, uint8_t pin_id, int16_t val )
{
  generic_write(c, pin_id, PIN_TYPE_PWM16, val, 16);
  return EXIT_SUCCESS;
}

/*
int write_value_mask( const struct connection * c,
                      const mask                mask,
                      val_list2                 types,
                      val_list16              * vals )
{
  //TODO
  return EXIT_FAILURE;
}
*/

int get_type( struct connection * c, uint8_t pin_id, uint8_t * type )
{
  struct packet p;
  set_packet_header(&p, CMD_GET_TYPE, USE_PIN_ID, PINS_NO_BYTES_NB);
  unsigned char buffer[PINS_NO_BYTES_NB];
  init_packet(buffer, PINS_NO_BYTES_NB);
  write_bit_value(buffer, 0, pin_id, PINS_NO_BITS_NB);
  p.data = buffer;
  send_packet(c, &p);
  // Read value
  struct packet reply;
  read_reply( c, &reply );
  *type = read_bit_value( reply.data, REPLY_ID_BITS_NB, PIN_TYPE_BITS_NB );
  c->state.pins_state[pin_id].pins_type = *type;
  packet_free( &reply );
  return EXIT_SUCCESS;
}

int get_type_mask( struct connection * c,
                   const mask          mask,
                   uint16_t          * types )
{
  struct packet p;
  unsigned int data_bits = c->caps.nb_pins-1;
  unsigned int data_bytes = BITS2BYTES(data_bits);
  set_packet_header(&p, CMD_GET_TYPE, USE_MASK, data_bytes);
  unsigned char * buffer = malloc(data_bytes);
  init_packet(buffer, data_bytes);
  write_mask(buffer, mask, data_bits);
  p.data = buffer;
  send_packet(c, &p);
  free(buffer);
  // Parsing reply
  struct packet reply;
  unsigned int nb_pins_used = mask_nb_pins_used( mask, c->caps.nb_pins);
  read_reply( c, &reply );//TODO check reply
  read_value_list( reply.data, REPLY_ID_BITS_NB, types,
                   nb_pins_used, PIN_TYPE_BITS_NB );
  int pin_id = 0;
  int i = 0;
  do{
    pin_id = mask_next_pin_used( mask, pin_id, c->caps.nb_pins );
    if (pin_id == -1) break;
    c->state.pins_state[pin_id].pins_type = types[i];
    i++;
    pin_id++;
  }while(true);
  packet_free( &reply );
  return EXIT_FAILURE;//TODO wait and parse answer
}

int set_type( struct connection * c, uint8_t pin_id, uint8_t type )
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
  //Verifying reply
  struct packet reply;
  read_reply( c, &reply );//TODO check return code  
  packet_free( &reply );
  // apply change
  c->state.pins_state[pin_id].pins_type = type;
  return EXIT_FAILURE;
}


int set_type_mask( struct connection * c,
                   const mask          mask,
                   const uint16_t    * values )
{
  int nb_values = mask_nb_pins_used(mask, c->caps.nb_pins);
  struct packet p;
  int data_bits_nb = c->caps.nb_pins + PIN_TYPE_BITS_NB * nb_values;
  unsigned int data_bytes = BITS2BYTES(data_bits_nb);
  set_packet_header(&p, CMD_SET_TYPE, USE_MASK, data_bytes);
  p.data = malloc(data_bytes);
  init_packet(p.data, data_bytes);
  write_mask(p.data, mask, c->caps.nb_pins);
  write_value_list(p.data, c->caps.nb_pins, values, nb_values, PIN_TYPE_BITS_NB);
  send_packet(c, &p);
  free(p.data);
  //Verifying reply
  struct packet reply;
  read_reply( c, &reply );//TODO check return code
  packet_free( &reply );
  return EXIT_FAILURE;
}

int get_failsafe( struct connection   * c,
                  uint8_t               pin_id,
                  struct pin_failsafe * failsafe )
{
  struct packet p;
  set_packet_header(&p, CMD_GET_FAILSAFE, USE_PIN_ID, PINS_NO_BYTES_NB);
  unsigned char buffer[PINS_NO_BYTES_NB];
  init_packet(buffer, PINS_NO_BYTES_NB);
  write_bit_value(buffer, 0, pin_id, 8);
  p.data = buffer;
  send_packet(c, &p);
  // Handling reply
  struct packet reply;
  read_reply( c, &reply );//TODO treat return code
  //TODO treat timeout
  unsigned int offset = REPLY_ID_BITS_NB + TIMEOUT_BITS_NB;
  failsafe->pin_state = read_bit_value( reply.data, offset, PIN_TYPE_BITS_NB );
  offset += PIN_TYPE_BITS_NB;
  failsafe->pin_value = read_bit_value( reply.data, offset,
                                        get_type_bits_nb(failsafe->pin_state) );
  packet_free( &reply );
  return EXIT_FAILURE;
}

int get_failsafe_mask( struct connection       * c,
                       const mask                mask,
                       struct failsafe         * f )
{
  struct packet p;
  unsigned int nb_pins_used = mask_nb_pins_used(mask, c->caps.nb_pins);
  unsigned int data_bits = nb_pins_used * PIN_TYPE_BITS_NB;
  unsigned int data_bytes = BITS2BYTES(data_bits);
  set_packet_header(&p, CMD_GET_FAILSAFE, USE_MASK, data_bytes);
  p.data = malloc(data_bytes);
  init_packet(p.data, data_bytes);
  write_mask(p.data, mask, c->caps.nb_pins);
  send_packet(c, &p);
  free(p.data);
  // Handling reply
  struct packet reply;
  read_reply( c, &reply );//TODO treat return code
  unsigned int offset = REPLY_ID_BITS_NB + TIMEOUT_BITS_NB;
  read_failsafe( reply.data, offset, f, nb_pins_used );
  int pin_id = 0;
  int i = 0;
  do{
    pin_id = mask_next_pin_used( mask, pin_id, c->caps.nb_pins );
    if (pin_id == -1) break;
    c->failsafe->pins_failsafe[pin_id].pin_state = f->pins_failsafe[i].pin_state;
    c->failsafe->pins_failsafe[pin_id].pin_value = f->pins_failsafe[i].pin_value;
    i++;
    pin_id++;
  }while(true);
  packet_free( &reply );
  return EXIT_FAILURE;
}

int set_failsafe( struct connection         * c,
                  uint8_t                     pin_no,
                  uint16_t                    timeout,
                  const struct pin_failsafe * failsafe_state )
{
  struct packet p;
  uint8_t type = failsafe_state->pin_state;
  uint8_t type_bits = get_type_bits_nb(type);
  unsigned int data_bits = TIMEOUT_BITS_NB + PINS_NO_BYTES_NB + type_bits;
  unsigned int data_bytes = BITS2BYTES(data_bits);
  set_packet_header(&p, CMD_SET_FAILSAFE, (type << 1) + USE_PIN_ID, data_bytes);
  p.data = malloc(data_bytes);
  init_packet(p.data, data_bytes);
  unsigned int offset = 0;
  write_bit_value(p.data, offset, timeout, TIMEOUT_BITS_NB);
  offset += TIMEOUT_BITS_NB;
  write_bit_value(p.data, offset, pin_no, PINS_NO_BITS_NB);
  offset += PINS_NO_BITS_NB;
  write_bit_value(p.data, offset, failsafe_state->pin_value, type_bits);
  send_packet(c, &p);
  free(p.data);
  //TODO check answer
  return EXIT_FAILURE;
}

/* Multiple changes, needs to be reworked
int set_failsafe_mask( struct connection       * c,
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
