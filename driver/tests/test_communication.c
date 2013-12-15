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
  print_ok( "Failsafe" );
}

/* Faut pas mettre l'achat de la rue avant l'émeu
void test_mask_failsafe()
{
  uint8_t pin_used [3] = {1, 5, 9};
  uint16_t wished_types[3] = { PIN_TYPE_DIGITAL,
                               PIN_TYPE_ANALOG16,
                               PIN_TYPE_PWM8};
  uint16_t wished_values[3] = { DIGITAL_ON, 515, 42 };
}
*/

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
  test_failsafe();

  print_separator();
  // Ending
  connection_close( c );
  return EXIT_SUCCESS;
}
