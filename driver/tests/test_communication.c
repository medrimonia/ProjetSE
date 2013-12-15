#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include "bit_utils.h"
#include "connection.h"
#include "driver.h"

#define NB_PINS 16

struct connection * c = NULL;

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
}


int main(void)
{
  c = connection_open( DEV2DRIV_FILENAME, DRIV2DEV_FILENAME );
  if ( c == NULL ) return EXIT_FAILURE;
  printf( "Connection initialized, sending getCaps :\n" );
  get_caps( c );
  // Tests
  check_connection_caps();
  //test_reset();//TODO this should be done later
  digital_full_test();
  analogic_full_test();
  pwm8_full_test();
  pwm16_full_test();

  // Ending
  connection_close( c );
  return EXIT_SUCCESS;
}
