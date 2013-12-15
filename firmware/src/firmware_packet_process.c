#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "bit_utils.h"
#include "protocol.h"
#include "failsafe.h"
#include "device_state.h"
#include "firmware_packet_process.h"

void reply_get_caps( struct connection * c )
{
  struct packet rep;
  set_packet_header(&rep, CMD_GET_CAPS, REP_CODE_SUCCESS, c->caps.nb_pins + 2);
  rep.data = malloc( 1 + 1 + c->caps.nb_pins );
  init_packet( rep.data, c->caps.nb_pins + 2 );
  rep.data[0] = get_reply_id();
  rep.data[1] = c->caps.nb_pins;
  memcpy( rep.data + 2, c->caps.pins_mask_type, c->caps.nb_pins );
  send_packet( c, &rep );
  free( rep.data );
}

void reply_reset( struct connection * c, const struct packet * p )
{
}

void reply_ping( struct connection * c, const struct packet * p )
{
  struct packet rep;
  set_packet_header(&rep, CMD_GET_CAPS, REP_CODE_SUCCESS, 2);
  unsigned char buffer[2];
  rep.data = buffer;
  init_packet( rep.data, 2 );
  rep.data[0] = get_reply_id();
  rep.data[1] = c->protocol_version;
  send_packet( c, &rep );
}

void reply_read( struct connection * c, const struct packet * p )
{
  bool use_mask = p->header % 2 == USE_MASK;
  uint8_t pin_type = read_param( &(p->header) ) >> 1;
  uint8_t val_bits = get_type_bits_nb( pin_type );
  unsigned int offset = REPLY_ID_BITS_NB;
  uint16_t data_bytes = 0;
  struct packet rep;
  if (!use_mask) {
    uint8_t pin_id = read_bit_value( p->data, 0, PINS_NO_BITS_NB );
    uint16_t val = c->state.pins_state[pin_id].pins_val;
    data_bytes = BITS2BYTES( offset + val_bits );
    rep.data = malloc( data_bytes );
    init_packet( rep.data, data_bytes );
    write_bit_value( rep.data, offset, val, val_bits );
  }
  else {
    mask m = new_mask( c->caps.nb_pins );
    read_mask( p->data, 0, m, c->caps.nb_pins );
    int nb_pins_used = mask_nb_pins_used( m, c->caps.nb_pins );
    uint16_t data_bits = offset + nb_pins_used * val_bits;
    data_bytes = BITS2BYTES( data_bits );
    rep.data = malloc( data_bytes );
    init_packet( rep.data, data_bytes );
    int pin_index = 0;
    do{
      pin_index = mask_next_pin_used( m, pin_index, c->caps.nb_pins );
      if ( pin_index == -1 ) break;
      uint16_t val = c->state.pins_state[pin_index].pins_val;
      write_bit_value( rep.data, offset, val, val_bits );
      offset += val_bits;
      pin_index++;
    }while( true );
  }
  set_packet_header( &rep, CMD_READ, REP_CODE_SUCCESS, data_bytes );
  rep.data[0] = get_reply_id();
  send_packet( c, &rep );
  free( rep.data );
}

void treat_write( struct connection * c, const struct packet * p )
{
  uint8_t pin_type = read_param( &(p->header) ) >> 1;
  uint8_t val_bits_nb = get_type_bits_nb( pin_type );
  bool use_mask = p->header % 2 == USE_MASK;
  if (!use_mask) {
    uint8_t pin_id = read_bit_value( p->data, 0, PINS_NO_BITS_NB );
    uint16_t val = read_bit_value( p->data, PINS_NO_BITS_NB, val_bits_nb );
    //TODO allow type switch with write or not?
    c->state.pins_state[pin_id].pins_type = pin_type;
    c->state.pins_state[pin_id].pins_val = val;
  }
  else {
    mask m = new_mask( c->caps.nb_pins );
    read_mask( p->data, 0, m, c->caps.nb_pins );
    int pin_index = 0;
    unsigned int offset = c->caps.nb_pins;
    do{
      pin_index = mask_next_pin_used( m, pin_index, c->caps.nb_pins );
      if ( pin_index == -1 ) break;
      uint8_t val = read_bit_value( p->data, offset, val_bits_nb );
      c->state.pins_state[pin_index].pins_val = val;
      offset += val_bits_nb;
      pin_index++;
    }while( true );
  }
}

void reply_write( struct connection * c, const struct packet * p )
{
  treat_write( c, p );
  struct packet rep;
  set_packet_header( &rep, CMD_WRITE, REP_CODE_SUCCESS, 1 );
  unsigned char rep_id = get_reply_id();
  rep.data = &rep_id;
  send_packet( c, &rep );
}

void reply_get_type( struct connection * c, const struct packet * p )
{
  bool use_mask = p->header % 2 == USE_MASK;
  unsigned int data_bytes = 1;
  struct packet rep;
  if (!use_mask) {
    uint8_t pin_id = read_bit_value( p->data, 0, PINS_NO_BITS_NB );
    uint8_t pin_type = c->state.pins_state[pin_id].pins_type;
    rep.data = malloc(2);
    init_packet( rep.data, 2 );
    data_bytes += BITS2BYTES(PIN_TYPE_BITS_NB);
    write_bit_value( rep.data, REPLY_ID_BITS_NB, pin_type, PIN_TYPE_BITS_NB );
  }
  else {
    mask m = new_mask( c->caps.nb_pins );
    read_mask( p->data, 0, m, c->caps.nb_pins );
    unsigned int offset = 8;// Reply id comes first
    unsigned int nb_pins_used = mask_nb_pins_used( m, c->caps.nb_pins );
    unsigned int bits_nb = nb_pins_used * PIN_TYPE_BITS_NB;
    data_bytes += BITS2BYTES( bits_nb );
    rep.data = malloc( data_bytes );
    init_packet( rep.data, data_bytes );
    int pin_index = 0;
    do{
      pin_index = mask_next_pin_used( m, pin_index, c->caps.nb_pins );
      if ( pin_index == -1 ) break;
      uint8_t pin_type = c->state.pins_state[pin_index].pins_type;
      write_bit_value( rep.data, offset, pin_type, PIN_TYPE_BITS_NB );
      offset += PIN_TYPE_BITS_NB;
      pin_index++;
    }while( true );
    destroy_mask( m );
  }
  set_packet_header( &rep, CMD_SET_TYPE, REP_CODE_SUCCESS, data_bytes );
  rep.data[0] = get_reply_id();
  send_packet( c, &rep );
  free( rep.data );
}

void treat_set_type( struct connection * c, const struct packet * p )
{
  bool use_mask = p->header % 2 == USE_MASK;
  if (!use_mask) {
    uint8_t pin_id = read_bit_value( p->data, 0, PINS_NO_BITS_NB );
    uint8_t type = read_bit_value( p->data, PINS_NO_BITS_NB, PIN_TYPE_BITS_NB );
    c->state.pins_state[pin_id].pins_type = type;
  }
  else {
    mask m = new_mask( c->caps.nb_pins );
    read_mask( p->data, 0, m, c->caps.nb_pins );
    unsigned int offset = c->caps.nb_pins;
    int pin_id = 0;
    do{
      pin_id = mask_next_pin_used( m, pin_id, c->caps.nb_pins );
      if ( pin_id == -1 ) break;
      uint8_t pin_type = read_bit_value( p->data, offset, PIN_TYPE_BITS_NB );
      c->state.pins_state[pin_id].pins_type = pin_type;
      offset += PIN_TYPE_BITS_NB;
      pin_id++;
    }while( true );
    destroy_mask( m );
  }
}

void reply_set_type( struct connection * c, const struct packet * p )
{
  treat_set_type( c, p );
  struct packet rep;
  set_packet_header( &rep, CMD_SET_TYPE, REP_CODE_SUCCESS, 1 );
  unsigned char rep_id = get_reply_id();
  rep.data = &rep_id;
  send_packet( c, &rep );
}

void reply_get_failsafe( struct connection * c, const struct packet * p )
{
  bool use_mask = p->header % 2 == USE_MASK;
  struct packet rep;
  uint8_t data_bytes = 3;
  uint16_t timeout = c->failsafe->timeout;
  uint16_t offset = REPLY_ID_BITS_NB + TIMEOUT_BITS_NB;
  if (!use_mask) {
    uint8_t pin_id = read_bit_value( p->data, 0, PINS_NO_BITS_NB );
    uint8_t pin_type = c->failsafe->pins_failsafe[pin_id].pin_state;
    uint8_t val_bits = get_type_bits_nb( pin_type );
    uint16_t value = c->failsafe->pins_failsafe[pin_id].pin_value;
    data_bytes += BITS2BYTES( PIN_TYPE_BITS_NB + val_bits );
    rep.data = malloc( data_bytes );
    init_packet( rep.data, data_bytes );
    write_bit_value( rep.data, offset, pin_type, PIN_TYPE_BITS_NB );
    offset += PIN_TYPE_BITS_NB;
    write_bit_value( rep.data, offset,  value, val_bits );
  }
  else {
    mask m = new_mask( c->caps.nb_pins );
    read_mask( p->data, 0, m, c->caps.nb_pins );
    unsigned int nb_pins_used = mask_nb_pins_used( m, c->caps.nb_pins );
    struct failsafe * masked_failsafe = sub_failsafe( c->failsafe, m, nb_pins_used );
    int additional_bits = failsafe_nb_bits( masked_failsafe, nb_pins_used );
    data_bytes += BITS2BYTES( additional_bits );
    rep.data = malloc(data_bytes);
    init_packet( rep.data, data_bytes );
    write_failsafe( rep.data, offset, masked_failsafe, nb_pins_used );
    destroy_failsafe( masked_failsafe );
    destroy_mask( m );
  }
  set_packet_header( &rep, CMD_SET_TYPE, REP_CODE_SUCCESS, data_bytes );
  unsigned char rep_id = get_reply_id();
  rep.data[0] = rep_id;
  write_bit_value( rep.data, REPLY_ID_BITS_NB, timeout, TIMEOUT_BITS_NB );
  send_packet( c, &rep );
  free( rep.data );
}

void reply_set_failsafe( struct connection * c, const struct packet * p )
{
  bool use_mask = p->header % 2 == USE_MASK;
  uint8_t type = read_param( &p->header ) >> 1;
  uint8_t val_bits = get_type_bits_nb( type );
  unsigned int offset = 0;
  c->failsafe->timeout = read_bit_value( p->data, 0, TIMEOUT_BITS_NB );
  offset += TIMEOUT_BITS_NB;
  if (!use_mask) {
    uint8_t pin_id = read_bit_value( p->data, offset, PINS_NO_BITS_NB );
    offset += PINS_NO_BITS_NB;
    uint16_t val = read_bit_value( p->data, offset, val_bits );
    c->failsafe->pins_failsafe[pin_id].pin_state = type;
    c->failsafe->pins_failsafe[pin_id].pin_value = val;
  }
  else {
    mask m = new_mask( c->caps.nb_pins );
    read_mask( p->data, 0, m, c->caps.nb_pins );
    offset += c->caps.nb_pins;
    int pin_index = 0;
    do{
      pin_index = mask_next_pin_used( m, pin_index, c->caps.nb_pins );
      if ( pin_index == -1 ) break;
      int16_t val = read_bit_value( p->data, offset, val_bits );
      c->failsafe->pins_failsafe[pin_index].pin_state = type;
      c->failsafe->pins_failsafe[pin_index].pin_value = val;
      pin_index++;
    }while( true );
  }
  // Reply
  struct packet rep;
  set_packet_header( &rep, CMD_SET_TYPE, REP_CODE_SUCCESS, 1 );
  unsigned char rep_id = get_reply_id();
  rep.data = &rep_id;
  send_packet( c, &rep );
}

