#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bit_utils.h"
#include "driver.h"
#include "protocol.h"
#include "test_utils.h"

#define NB_PINS 12

void set_input( struct connection * c, const char * path ){
  if (c->fd_in != -1){
    close(c->fd_in);
  }
  c->fd_in = open( path, O_RDONLY );
}


void test_get_caps( struct connection * c )
{
  print_title("Get caps :");
  set_input( c, "driver/tests/test_get_caps_reply" );
  get_caps(c);
  printf( "Read number of pins     : %d\n", c->caps.nb_pins );
  printf( "Expected number of pins : 2\n" );
  printf( "Read type mask          : " );
  display_packet_hexa( c->caps.pins_mask_type, c->caps.nb_pins );
  printf( "\nExpected type mask      : |ff|0d|\n");
}

void test_reset( struct connection * c )
{
  print_title("Reset :");
  set_input( c, "driver/tests/test_reset_reply" );
  reset(c);
}

void test_ping( struct connection * c )
{
  print_title("Ping 3 :");
  set_input( c, "driver/tests/test_ping_reply" );
  uint8_t device_protocol_version;
  ping(c, 3, &device_protocol_version);
  printf( "Expected protocol version : 1\n" );
  printf( "Real protocol version     : %d\n", device_protocol_version );
}

void test_digital_read( struct connection * c )
{
  print_title("Digital read on pin 5  :");
  set_input( c, "driver/tests/test_digital_read_reply" );
  bool val;
  digital_read(c, 5, &val);
  printf("Pin state      : %s\n", DIGPIN2STR(val));
  printf("Expected state : %s\n", DIGPIN2STR(  1));
}

void test_analogic_read( struct connection * c )
{
  print_title("Analogic read on pin 2  :");
  set_input( c, "driver/tests/test_analogic_read_reply" );
  uint16_t val;
  analogic_read(c, 5, &val);
  printf("Pin state      : %u\n", val);
  printf("Expected state : %u\n", 510);
}

void test_pwm8_read( struct connection * c )
{
  print_title("Pwm8 read on pin 7  :");
  pwm8_read(c, 7, NULL);
}

void test_pwm16_read( struct connection * c )
{
  print_title("Pwm16 read on pin 13  :");
  pwm16_read(c, 13, NULL);
}

void test_digital_write( struct connection * c )
{
  print_title("Digital write of 'on' on pin 1 :");
  digital_write (c, 1, true);
}

void test_analogic_write( struct connection * c )
{
  print_title("Analogic write of '510' on pin 6 :");
  analogic_write (c, 6, 510);
}

void test_pwm8_write( struct connection * c )
{
  print_title("Pwm8 write of '24' on pin 3 :");
  pwm8_write (c, 3, 24);
}

void test_pwm16_write( struct connection * c )
{
  print_title("Pwm16 write of '515' on pin 9 :");
  pwm16_write (c, 9, 515);
}

void test_get_type( struct connection * c )
{
  printf("Get type of pin 8 : ");
  get_type(c, 8, NULL);
}

void test_get_type_mask( struct connection * c )
{
  print_title("Get type mask of pin 3,6,10:\n\t");
  mask m = new_mask(NB_PINS);
  m[3]  = MASK_PIN_ON;
  m[6]  = MASK_PIN_ON;
  m[10] = MASK_PIN_ON;
  get_type_mask(c, m, NULL);
  // Expected: |51|00|02|12|20|7a|;
  free(m);
}

void test_set_type( struct connection * c )
{
  print_title("Set type of pin 8 to digital :");
  set_type(c, 8,  PIN_TYPE_DIGITAL);
}

void test_set_type_mask( struct connection * c )
{
  print_title("Set type mask of pin 4,7,9 to digit, analog8, pwm16 :");
  mask m = new_mask(NB_PINS);
  m[4] = MASK_PIN_ON;
  m[7] = MASK_PIN_ON;
  m[9] = MASK_PIN_ON;
  uint16_t values[3] = { PIN_TYPE_DIGITAL, PIN_TYPE_ANALOG8, PIN_TYPE_PWM16 };
  set_type_mask(c, m,  values);
  // Expected Data :
  //      mask    |   values   |
  //     4  7  9  | 4 |  7 | 9 |
  // 00001001|0100-000-0|01-100-000|
  //    09   |    40    |   60
  // Expected    : |61|00|03|09|40|60|f2|
  free(m);
}

void test_get_failsafe( struct connection * c )
{
  print_title("Get failsafe on pin 7");
  get_failsafe(c, 7, NULL);
}

void test_get_failsafe_mask( struct connection * c )
{
  print_title("Get failsafe mask on pin 1, 3, 9");
  mask m = new_mask(c->caps.nb_pins);
  m[1] = true;
  m[3] = true;
  m[9] = true;
  get_failsafe_mask(c, m, NULL);
  destroy_mask(m);
}

void test_set_failsafe( struct connection * c )
{
  print_title("Test set failsafe on 3 to PWM8 5 with timeout 515 :");
  struct pin_failsafe fs;
  fs.pin_value = 5;
  fs.pin_state = PIN_TYPE_PWM8;
  set_failsafe( c, 3, 515, &fs);
}

/*
void set_failsafe_test( struct connection * c ) mask version!
{
  printf("Set failsafe : {1:DIGITAL:OFF, 3:PWM8:25, 9:ANALOG16:522} timeout 514\n\t");
  struct failsafe * f = new_failsafe(c->nb_pins);
  f->pins_used[1] = MASK_PIN_ON;
  f->pins_failsafe[1].pin_state = PIN_TYPE_DIGITAL;
  f->pins_failsafe[1].pin_value = false;
  f->pins_used[3] = MASK_PIN_ON;
  f->pins_failsafe[3].pin_state = PIN_TYPE_PWM8;
  f->pins_failsafe[3].pin_value = 25;
  f->pins_used[9] = MASK_PIN_ON;
  f->pins_failsafe[9].pin_state = PIN_TYPE_ANALOG16;
  f->pins_failsafe[9].pin_value = 522;
  f->timeout = 514;
  set_failsafe(c, f);
  // Expected Data :
  //      mask    |  1  |      3      |          9            |
  //  1 3      9  | D | | P8|   25    |A16|       522         |
  // 01010000|0100-010-0|011-00011|001-001-00|000010-00|001010-00|
  //    50   |    44    |   63    |    24    |    08   |   28    |
  printf("\tExpected    : |81|00|08|02|02|50|44|63|24|08|28|\n");
}
*/

int main( void )
{
  struct connection * c = malloc(sizeof(struct connection));
  c->fd_in = -1;
  c->fd_out = open("/dev/null", O_WRONLY);
  c->caps.nb_pins = NB_PINS;
  print_separator();
  test_get_caps(c);
  print_separator();
  test_reset(c);
  print_separator();
  test_ping(c);
  print_separator();
  test_digital_read(c);
  print_separator();
  test_analogic_read(c);
  print_separator();
  exit(EXIT_FAILURE);
  test_pwm8_read(c);
  test_pwm16_read(c);
  test_digital_write(c);
  test_analogic_write(c);
  test_pwm8_write(c);
  test_pwm16_write(c);
  test_get_type(c);
  test_get_type_mask(c);
  test_set_type(c);
  test_set_type_mask(c);
  test_get_failsafe(c);
  test_get_failsafe_mask(c);
  test_set_failsafe(c);
/* TODO discuss it
  test_set_failsafe_mask(c);
*/
  print_separator();
  free(c);
  exit(EXIT_SUCCESS);
}
