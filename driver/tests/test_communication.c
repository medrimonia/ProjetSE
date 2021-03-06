#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include "bit_utils.h"
#include "connection.h"
#include "driver.h"
#include "test_utils.h"

#define NB_PINS 16

struct connection * c = NULL;

void test_ping()
{
  uint8_t device_protocol_version;
  ping( c, &device_protocol_version );
  assert( device_protocol_version == 1 );
  print_ok( "Ping" );
}

void check_connection_caps()
{
#ifdef DEBUG
  printf( "Read number of pins     : %d\n", c->caps.nb_pins );
  printf( "Read type mask          : " );
  display_packet_hexa( c->caps.pins_mask_type, c->caps.nb_pins );
  printf( "\n" );
#endif
  assert( c->caps.nb_pins == NB_PINS );
  unsigned int i;
  for ( i = 0; i < NB_PINS; i++ ) {
    assert( c->caps.pins_mask_type[i] == 31 );//TODO hard-coded value
  }
  print_ok( "Get Caps" );
}

void digital_full_test()
{
  uint8_t pin_id = 3;
  // Writing type
  set_type( c, pin_id, PIN_TYPE_DIGITAL );
  assert( c->state.pins_state[pin_id].pins_type == PIN_TYPE_DIGITAL );
  // Verifying type
  uint8_t verif_type;
  get_type( c, pin_id, &verif_type );
  assert( verif_type == PIN_TYPE_DIGITAL );
  // Writing value
  uint16_t wished_val  = DIGITAL_ON;
  digital_write( c, pin_id, wished_val );
  assert( c->state.pins_state[pin_id].pins_val == wished_val );
  // Verifying value
  bool verif_val;
  digital_read( c, pin_id, &verif_val );
  assert( verif_val == wished_val );
  print_ok( "Digital full" );
}

void analogic_full_test()
{
  uint8_t pin_id = 6;
  // Writing type
  set_type( c, pin_id, PIN_TYPE_ANALOG16 );
  assert( c->state.pins_state[pin_id].pins_type == PIN_TYPE_ANALOG16 );
  // Verifying type
  uint8_t verif_type;
  get_type( c, pin_id, &verif_type );
  assert( verif_type == PIN_TYPE_ANALOG16 );
  // Writing value
  uint16_t wished_val  = 537;
  analogic_write( c, pin_id, wished_val );
  assert( c->state.pins_state[pin_id].pins_val == wished_val );
  // Verifying value
  uint16_t verif_val;
  analogic_read( c, pin_id, &verif_val );
  assert( verif_val == wished_val );
  print_ok( "Analogic full" );
}

void pwm8_full_test()
{
  uint8_t pin_id = 0;
  // Writing type
  set_type( c, pin_id, PIN_TYPE_PWM8 );
  assert( c->state.pins_state[pin_id].pins_type == PIN_TYPE_PWM8 );
  // Verifying type
  uint8_t verif_type;
  get_type( c, pin_id, &verif_type );
  assert( verif_type == PIN_TYPE_PWM8 );
  // Writing value
  uint16_t wished_val  = 42;
  pwm8_write( c, pin_id, wished_val );
  assert( c->state.pins_state[pin_id].pins_val == wished_val );
  // Verifying value
  uint8_t verif_val;
  pwm8_read( c, pin_id, &verif_val );
  assert( verif_val == wished_val );
  print_ok( "PWM8 full" );
}

void pwm16_full_test()
{
  uint8_t pin_id = 13;
  // Writing type
  set_type( c, pin_id, PIN_TYPE_PWM16 );
  assert( c->state.pins_state[pin_id].pins_type == PIN_TYPE_PWM16 );
  // Verifying type
  uint8_t verif_type;
  get_type( c, pin_id, &verif_type );
  assert( verif_type == PIN_TYPE_PWM16 );
  // Writing value
  uint16_t wished_val  = 515;
  pwm16_write( c, pin_id, wished_val );
  assert( c->state.pins_state[pin_id].pins_val == wished_val );
  // Verifying value
  uint16_t verif_val;
  pwm16_read( c, pin_id, &verif_val );
  assert( verif_val == wished_val );
  print_ok( "PWM16 full" );
}

#ifndef DISABLE_FAILSAFE
void test_failsafe()
{
  uint8_t pin_id = 5;
  struct pin_failsafe wished_state;
  uint8_t pin_type = PIN_TYPE_PWM8;
  uint8_t pin_value = 142;
  wished_state.pin_state = pin_type;
  wished_state.pin_value = pin_value;
  uint16_t wished_timeout = 52345;
  set_failsafe( c, pin_id, wished_timeout, &wished_state );
  //After a successful set, all stored values should be adapted
  assert( c->failsafe->timeout == wished_timeout );
  assert( c->failsafe->pins_failsafe[pin_id].pin_state == pin_type  );
  assert( c->failsafe->pins_failsafe[pin_id].pin_value == pin_value );
  struct pin_failsafe read_state;
  get_failsafe( c, pin_id, &read_state );
  assert( read_state.pin_state == pin_type  );
  assert( read_state.pin_value == pin_value );
  print_ok( "Failsafe" );
}
#endif

#define MASK_TEST_NB_PINS 4
void full_mask_test()
{
  // Choosen data
  uint8_t pins_used[MASK_TEST_NB_PINS] = {1, 5, 9, 11};
  // using uint16_t for compatibility
  uint16_t wished_types[MASK_TEST_NB_PINS] = {PIN_TYPE_ANALOG16,
                                              PIN_TYPE_PWM8,
                                              PIN_TYPE_DIGITAL,
                                              PIN_TYPE_PWM8};
  uint16_t wished_values[MASK_TEST_NB_PINS] = {515, 42, DIGITAL_ON, 65};

  // Building needed objects
  mask m = new_mask( c->caps.nb_pins );
  unsigned int i;
  for ( i = 0; i < MASK_TEST_NB_PINS; i++ )
    m[ pins_used[i] ] = true;
  // Types, Set + get
  set_type_mask( c, m, wished_types );
  for ( i = 0; i < MASK_TEST_NB_PINS; i++ ) {
    uint8_t pin_id = pins_used[i];
    assert( c->state.pins_state[pin_id].pins_type == wished_types[i] );
  }
  uint16_t read_types[MASK_TEST_NB_PINS];
  get_type_mask( c, m, read_types );
  for ( i = 0; i < MASK_TEST_NB_PINS; i++ ) {
    assert( read_types[i] == wished_types[i] );
  }
  // Write mask
  mask write_mask = new_mask( c->caps.nb_pins );
  uint8_t write_pins[MASK_TEST_NB_PINS];
  uint16_t write_values[MASK_TEST_NB_PINS];
  unsigned int nb_values = 0;
  for ( i = 0; i < MASK_TEST_NB_PINS; i++ ) {
    if ( wished_types[i] == PIN_TYPE_PWM8 ) {
      unsigned int pin_id = pins_used[i];
      write_mask[pin_id]      = true;
      write_pins[nb_values]   = pin_id;
      write_values[nb_values] = wished_values[i];
      nb_values++;
    }
  }
  write_value_mask( c, write_mask, PIN_TYPE_PWM8, write_values );
  for ( i = 0; i < nb_values; i++ ) {
    uint16_t stored_val = c->state.pins_state[write_pins[i]].pins_val;
    assert( stored_val == write_values[i] );
  }
  // Read mask
  uint16_t read_values[MASK_TEST_NB_PINS];
  read_value_mask( c, write_mask, PIN_TYPE_PWM8, read_values );
  for ( i = 0; i < nb_values; i++ ) {
    assert( read_values[i] == write_values[i] );
  }
  
  destroy_mask( m );
  destroy_mask( write_mask );
  print_ok( "Full mask" );
}

#ifndef DISABLE_FAILSAFE
#define FS_TEST_NB_PINS 5
void test_mask_failsafe()
{
  uint8_t pins_used [FS_TEST_NB_PINS] = {1, 3, 5, 9, 12};
  uint16_t wished_types[FS_TEST_NB_PINS]  = { PIN_TYPE_DIGITAL,
                                              PIN_TYPE_ANALOG16,
                                              PIN_TYPE_DIGITAL,
                                              PIN_TYPE_PWM8,
                                              PIN_TYPE_PWM8};
  uint16_t wished_values[FS_TEST_NB_PINS] = { DIGITAL_ON,
                                              515,
                                              DIGITAL_OFF,
                                              42,
                                              251};
  uint16_t wished_timeout = 513;
  // Setting the failsafe mask for each type
  uint8_t used_types[3] = {PIN_TYPE_DIGITAL,
                           PIN_TYPE_ANALOG16,
                           PIN_TYPE_PWM8};
  int nb_types_used = 3;
  int type_no;
  for ( type_no = 0; type_no < nb_types_used; type_no++ ) {
    uint8_t type = used_types[type_no];
    // Creating appropriated mask, pin_no list and value list
    mask m = new_mask( c->caps.nb_pins );
    uint8_t pins[FS_TEST_NB_PINS];
    uint16_t pins_values[FS_TEST_NB_PINS];
    int i;
    int nb_pins = 0;
    for ( i = 0; i < FS_TEST_NB_PINS; i++ ) {
      if ( wished_types[i] == type ){
        pins[nb_pins]        = pins_used[i];
        pins_values[nb_pins] = wished_values[i];
        m[ pins[nb_pins] ]   = true;
        nb_pins++;
      }
    }
    // Sending Values
    set_failsafe_mask( c, m, type, wished_timeout, pins_values );
    // Verifying that failsafe has been applied internally
    assert( c->failsafe->timeout == wished_timeout );
    for ( i = 0; i < nb_pins; i++ ) {
      uint8_t pin_id = pins[i];
      uint8_t  stored_type  = c->failsafe->pins_failsafe[pin_id].pin_state;
      uint16_t stored_value = c->failsafe->pins_failsafe[pin_id].pin_value;
      assert( type  == stored_type );
      assert( pins_values[i] == stored_value );
    }
    // Liberating resources
    destroy_mask( m ); 
  }
  // Reading the whole of it to check that everything has been received well
  // Computing reading_mask
  mask reading_mask = new_mask( c->caps.nb_pins );
  int i;
  for ( i = 0; i < FS_TEST_NB_PINS; i++ ) {
    reading_mask[ pins_used[i] ] = true;
  }
  struct failsafe * read_failsafe = new_failsafe( FS_TEST_NB_PINS );
  get_failsafe_mask( c, reading_mask, read_failsafe );
  // Asserting
  assert( read_failsafe->timeout == wished_timeout );
  for ( i = 0; i < FS_TEST_NB_PINS; i++ ) {
    uint8_t read_type = read_failsafe->pins_failsafe[i].pin_state;
    uint16_t read_value = read_failsafe->pins_failsafe[i].pin_value;
    assert( read_type == wished_types[i] );
    assert( read_value == wished_values[i] );
  }
  destroy_failsafe( read_failsafe );
  destroy_mask( reading_mask );
  print_ok( "Failsafe mask" );
}
#endif

int main(void)
{
  print_separator();
  c = connection_open( DEV2DRIV_FILENAME, DRIV2DEV_FILENAME );
  if ( c == NULL ) return EXIT_FAILURE;
  c->protocol_version = 1;
  test_ping();
  get_caps( c );
  // Tests
  check_connection_caps();
  //test_reset();//TODO this should be done later
  digital_full_test();
  analogic_full_test();
  pwm8_full_test();
  pwm16_full_test();
  full_mask_test();
#ifndef DISABLE_FAILSAFE
  test_failsafe();
  test_mask_failsafe();
#endif
  
  print_separator();
  // Ending
  connection_close( c );
  return EXIT_SUCCESS;
}
