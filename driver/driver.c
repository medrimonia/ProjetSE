#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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
    exit(1);
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
           uint8_t * device_protocol_version)
{
  struct packet p;
  set_packet_header(&p, CMD_PING, 0, 1);
  unsigned char buffer[VERSION_BYTES_NB];
  init_packet(buffer, VERSION_BYTES_NB);
  write_bit_value(buffer, 0, connection->protocol_version, 8);
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
  return 0;
}

int digital_read( struct connection * c, uint8_t pin_id, bool * val )
{
  uint16_t tmp_val;
  generic_read( c, pin_id, PIN_TYPE_DIGITAL, &tmp_val );//TODO test return
  *val = tmp_val;
  return 0;
}

int analogic_read( struct connection * c, uint8_t pin_id, uint16_t * val )
{
  generic_read( c, pin_id, PIN_TYPE_ANALOG16, val );//TODO test return
  return 0;
}

int pwm8_read( struct connection * c, uint8_t pin_id, uint8_t * val )
{
  uint16_t tmp_val;
  generic_read( c, pin_id, PIN_TYPE_PWM8, &tmp_val );//TODO test return
  *val = tmp_val;
  return 0;
}

int pwm16_read( struct connection * c, uint8_t pin_id, uint16_t * val)
{
  generic_read( c, pin_id, PIN_TYPE_PWM16, val );//TODO test return
  return 0;
}

int read_value_mask( struct connection * c,
                     const mask          mask,
                     uint8_t             type,
                     uint16_t          * values ){
  struct packet p;
  int data_bytes = BITS2BYTES( c->caps.nb_pins );
  set_packet_header( &p, CMD_READ, (type << 1) + USE_MASK, data_bytes );
  p.data = malloc( data_bytes );
  init_packet( p.data, data_bytes );
  write_mask( p.data, mask, c->caps.nb_pins );
  send_packet( c, &p );
  free( p.data );
  // Reading and applying
  struct packet reply;
  read_reply( c, &reply );
  unsigned int val_bits = get_type_bits_nb( type );
  int pin_index = 0;
  int index = 0;
  unsigned int offset = REPLY_ID_BITS_NB;
  do{
    pin_index = mask_next_pin_used( mask, pin_index, c->caps.nb_pins );
    if ( pin_index == -1 ) break;
    values[index] = read_bit_value( reply.data, offset, val_bits);
    c->state.pins_state[pin_index].pins_val = values[index];
    offset += val_bits;
    index++;
    pin_index++;
  }while( true );
  packet_free( &reply );
  return 0;
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
  return 0;
}

int digital_write ( struct connection * c, uint8_t pin_id, bool    val )
{
  generic_write(c, pin_id, PIN_TYPE_DIGITAL, val, 1);
  return 0;
}

int analogic_write( struct connection * c, uint8_t pin_id, int16_t val )
{
  generic_write(c, pin_id, PIN_TYPE_ANALOG16, val, 16);
  return 0;
}

int pwm8_write    ( struct connection * c, uint8_t pin_id, int16_t val )
{
  generic_write(c, pin_id, PIN_TYPE_PWM8, val, 8);
  return 0;
}

int pwm16_write   ( struct connection * c, uint8_t pin_id, int16_t val )
{
  generic_write(c, pin_id, PIN_TYPE_PWM16, val, 16);
  return 0;
}


int write_value_mask( struct connection * c,
                      const mask          mask,
                      uint8_t             type,
                      uint16_t          * values )
{
  unsigned int nb_pins_used = mask_nb_pins_used( mask, c->caps.nb_pins );
  unsigned int value_bits   = get_type_bits_nb( type );
  unsigned int data_bits    = c->caps.nb_pins + nb_pins_used * value_bits;
  unsigned int data_bytes   = BITS2BYTES(data_bits);
  struct packet p;
  set_packet_header( &p, CMD_WRITE, (type << 1) + USE_MASK, data_bytes );
  p.data = malloc( data_bytes );
  init_packet( p.data, data_bytes );
  write_mask( p.data, mask, c->caps.nb_pins );
  write_value_list( p.data, c->caps.nb_pins, values, nb_pins_used, value_bits );
  send_packet( c, &p );
  free( p.data );
  // Consuming reply
  struct packet reply;
  read_reply( c, &reply );
  packet_free( &reply );
  // Saving datas
  int pin_index = 0;
  unsigned int index = 0;
  do{
    pin_index = mask_next_pin_used( mask, pin_index, c->caps.nb_pins );
    if ( pin_index == -1 ) break;
    c->state.pins_state[pin_index].pins_val = values[index];
    index++;
    pin_index++;
  }while( true );
  return 0;
}

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
  return 0;
}

int get_type_mask( struct connection * c,
                   const mask          mask,
                   uint16_t          * types )
{
  struct packet p;
  unsigned int data_bits = c->caps.nb_pins-1;//TODO check: why -1?
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
  return 1;//TODO wait and parse answer
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
  return 1;
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
  // Verifying reply
  struct packet reply;
  read_reply( c, &reply );//TODO check return code
  packet_free( &reply );
  // Applying datas
  int mask_index = 0;
  int val_index = 0;
  do{
    mask_index = mask_next_pin_used( mask, mask_index, c->caps.nb_pins );
    if ( mask_index == -1 ) break;
    c->state.pins_state[mask_index].pins_type = values[val_index];
    val_index++;
    mask_index++;
  }while(true);
  return 0;
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
  return 1;
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
  return 1;
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
  //Checking reply
  struct packet reply;
  read_reply( c, &reply );//TODO check return code  
  packet_free( &reply );
  //TODO store given value
  c->failsafe->timeout = timeout;
  c->failsafe->pins_failsafe[pin_no].pin_value = failsafe_state->pin_value;
  c->failsafe->pins_failsafe[pin_no].pin_state = failsafe_state->pin_state;
  return 1;
}

int set_failsafe_mask( struct connection     * c,
                       mask                    m,
                       uint8_t                 type,
                       uint16_t                timeout,
                       uint16_t              * values )
{
  uint8_t nb_pins_used = mask_nb_pins_used( m, c->caps.nb_pins );
  int val_bits = get_type_bits_nb( type );
  int data_bits = TIMEOUT_BITS_NB + c->caps.nb_pins + nb_pins_used * val_bits;
  int data_bytes = BITS2BYTES( data_bits );
  struct packet p;
  set_packet_header( &p, CMD_SET_FAILSAFE, (type << 1) + USE_MASK, data_bytes);
  p.data = malloc( data_bytes );
  unsigned int offset = 0;
  write_bit_value( p.data, offset, timeout, TIMEOUT_BITS_NB );
  offset += TIMEOUT_BITS_NB;
  write_mask( p.data + 2, m, c->caps.nb_pins );//TODO offset to write mask
  offset += c->caps.nb_pins;
  write_value_list( p.data, offset, values, nb_pins_used, val_bits );
  send_packet( c, &p );
  free( p.data );
  // read reply
  struct packet reply;
  read_reply( c, &reply );//TODO check return code  
  packet_free( &reply );
  // apply changes on the connection properties
  c->failsafe->timeout = timeout;
  int pin_index = 0;
  unsigned int i = 0;
  do{
    pin_index = mask_next_pin_used( m, pin_index, c->caps.nb_pins );
    if (pin_index == -1) break;
    c->failsafe->pins_failsafe[pin_index].pin_state = type;
    c->failsafe->pins_failsafe[pin_index].pin_value = values[i];
    i++;
    pin_index++;
  }while(true);
  return 0;
}
