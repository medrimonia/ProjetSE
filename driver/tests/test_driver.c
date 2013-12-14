#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bit_utils.h"
#include "driver.h"
#include "protocol.h"
#include "test_utils.h"

#define NB_PINS 16

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
  // Recovering the number of pins
  c->caps.nb_pins = NB_PINS;
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
  bool stored_val = c->state.pins_state[5].pins_val;
  printf("Stored state   : %s\n", DIGPIN2STR(stored_val));
  printf("Pin state      : %s\n", DIGPIN2STR(val));
  printf("Expected state : %s\n", DIGPIN2STR(  1));
}

void test_analogic_read( struct connection * c )
{
  print_title("Analogic read on pin 2  :");
  set_input( c, "driver/tests/test_analogic_read_reply" );
  uint16_t val;
  analogic_read(c, 2, &val);
  uint16_t stored_val = c->state.pins_state[2].pins_val;
  printf("Stored state   : %u\n", stored_val);
  printf("Pin state      : %u\n", val);
  printf("Expected state : %u\n", 510);
}

void test_pwm8_read( struct connection * c )
{
  print_title("Pwm8 read on pin 7  :");
  set_input( c, "driver/tests/test_pwm8_read_reply" );
  uint8_t val;
  pwm8_read(c, 7, &val);
  uint8_t stored_val = c->state.pins_state[7].pins_val;
  printf("Stored state   : %u\n", stored_val);
  printf("Pin state      : %u\n", val);
  printf("Expected state : %u\n", 42);
}

void test_pwm16_read( struct connection * c )
{
  print_title("Pwm16 read on pin 13  :");
  set_input( c, "driver/tests/test_pwm16_read_reply" );
  uint16_t val;
  pwm16_read(c, 13, &val);
  uint16_t stored_val = c->state.pins_state[13].pins_val;
  printf("Stored state   : %u\n", stored_val);
  printf("Pin state      : %u\n", val);
  printf("Expected state : %u\n", 515);
}

void test_digital_write( struct connection * c )
{
  print_title("Digital write of 'on' on pin 1 :");
  set_input( c, "driver/tests/test_digital_write_reply" );
  digital_write (c, 1, true);
  printf("Transmission Successful\n");
}

void test_analogic_write( struct connection * c )
{
  print_title("Analogic write of '510' on pin 6 :");
  set_input( c, "driver/tests/test_analogic_write_reply" );
  analogic_write (c, 6, 510);
  printf("Transmission Successful\n");
}

void test_pwm8_write( struct connection * c )
{
  print_title("Pwm8 write of '24' on pin 3 :");
  set_input( c, "driver/tests/test_pwm8_write_reply" );
  pwm8_write (c, 3, 24);
  printf("Transmission Successful\n");
}

void test_pwm16_write( struct connection * c )
{
  print_title("Pwm16 write of '515' on pin 9 :");
  set_input( c, "driver/tests/test_pwm16_write_reply" );
  pwm16_write (c, 9, 515);
  printf("Transmission Successful\n");
}

void test_get_type( struct connection * c )
{
  print_title( "Get type of pin 8 : " );
  set_input( c, "driver/tests/test_get_type_reply" );
  // get type reply : |50|00|02|08|60|45|
  uint8_t type;
  get_type( c, 8, &type );
  uint16_t stored_type = c->state.pins_state[8].pins_type;
  printf("Stored type   : %u\n", stored_type);
  printf("Pin type      : %u\n", type);
  printf("Expected type : %u\n", 3);

}

void test_get_type_mask( struct connection * c )
{
  print_title("Get type mask of pin 3,6,10:\n\t");
  set_input( c, "driver/tests/test_get_type_mask_reply" );
  mask m = new_mask(NB_PINS);
  m[3]  = MASK_PIN_ON;
  m[6]  = MASK_PIN_ON;
  m[10] = MASK_PIN_ON;
  uint16_t types[3];
  uint8_t  pins_no[3]  = {3, 6, 10};
  uint8_t  expected[3] = {2, 3, 4};
  get_type_mask(c, m, types);
  // Expected sent packet: |51|00|02|12|20|7a|;
  // Expected received data :
  // |010-011-10|0-0000000|
  // |   4e     |    00   |
  // Verifying :
  unsigned int i;
  for (i = 0; i < 3; i++){
    uint8_t stored = c->state.pins_state[pins_no[i]].pins_type;
    printf("pin %d -> Stored   : %u\n", i, stored);
    printf("pin %d -> Received : %u\n", i, types[i]);
    printf("pin %d -> Expected : %u\n", i, expected[i]);
  }
  free(m);
}

void test_set_type( struct connection * c )
{
  print_title("Set type of pin 8 to digital :");
  set_input( c, "driver/tests/test_set_type_reply" );
  set_type(c, 8,  PIN_TYPE_DIGITAL);
  printf("Transmission Successful\n");
}

void test_set_type_mask( struct connection * c )
{
  print_title("Set type mask of pin 4,7,9 to digit, analog8, pwm16 :");
  set_input( c, "driver/tests/test_set_type_mask_reply" );
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
  printf("Transmission Successful\n");
  free(m);
}

void test_get_failsafe( struct connection * c )
{
  print_title("Get failsafe on pin 7");
  set_input( c, "driver/tests/test_get_failsafe_reply" );
  struct pin_failsafe f;
  get_failsafe(c, 7, &f);
  // Expected data :
  // Reply id : ...
  // Timeout : 5 s = 500 hundredth of seconds
  // type : PWM8 -> 3 -> 011
  // val : 42    -> 00101010
  // data = |..|01|f4|011-00101|010-00000|
  //                     65        40
  //TODO compute store (needs to add a field to connection)
  //printf( "type -> Stored   : %u\n", storedType  );
  printf( "type -> Received : %u\n", f.pin_state );
  printf( "type -> Expected : %u\n", 3           );
  //printf( "type -> Stored   : %u\n", storedVal      );
  printf( "type -> Received : %u\n", f.pin_value );
  printf( "type -> Expected : %u\n", 42          );
}

void test_get_failsafe_mask( struct connection * c )
{
  print_title( "Get failsafe mask on pin 1, 3, 9" );
  set_input( c, "driver/tests/test_get_failsafe_mask_reply" );
  mask m = new_mask( c->caps.nb_pins );
  m[1] = true;
  m[3] = true;
  m[9] = true;
  struct failsafe * f = new_failsafe( 3 );
  get_failsafe_mask( c, m, f );
  // Expected Data in Reply :
  // | Timeout |  1  |      3      |           9              | Pad  |
  // |    |    | D | | P8|   25    |A16 |       522           |      |
  // |   515   |000-0-011-0|0011001-0|10-0000-00|10-0000-10|10-000000|
  // | 02 | 03 |    06     |    32   |    80    |    82    |   80    |
  // header : |70|00|07|00| <- id de message de 0
  // checksum = xy :
  // y : 8 + 2 + 3 + 6 + 2 + 2 = 23 -> y = 8 (f - 7) (reste 1 pour le x)
  // x : 1 + 7 + 3 + 8 + 8 + 8 = 35 -> y = c (f - 3)
  unsigned int i;
  uint8_t pins_no[3] = {1, 3, 9};
  uint16_t expected_type[3] = {0, 3, 2};
  uint16_t expected_val[3] = {0, 25, 522};
  for ( i = 0; i < 3; i++ ){
    uint16_t pin_id = pins_no[i];
    uint8_t stored_type = c->failsafe->pins_failsafe[pin_id].pin_state;
    uint16_t stored_val = c->failsafe->pins_failsafe[pin_id].pin_value;
    printf( "%d,type -> Stored   : %3u\n", i, stored_type                   );
    printf( "%d,type -> Received : %3u\n", i, f->pins_failsafe[i].pin_state );
    printf( "%d,type -> Expected : %3u\n", i, expected_type[i]              );
    printf( "%d,val  -> Stored   : %3u\n", i, stored_val                    );
    printf( "%d,val  -> Received : %3u\n", i, f->pins_failsafe[i].pin_value );
    printf( "%d,val  -> Expected : %3u\n", i, expected_val[i]               );
  }
  destroy_failsafe( f );
  destroy_mask( m );
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
  //      mask    |  1  |      3      |          9               |
  //  1 3      9  | D | | P8|   25    |A16|       522            |
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
  c->caps.pins_mask_type = NULL;
  c->caps.nb_pins = NB_PINS;
  c->state.pins_state = NULL;
  c->failsafe = NULL;
  connection_init_resources( c );
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
  test_pwm8_read(c);
  print_separator();
  test_pwm16_read(c);
  print_separator();
  test_digital_write(c);
  print_separator();
  test_analogic_write(c);
  print_separator();
  test_pwm8_write(c);
  print_separator();
  test_pwm16_write(c);
  print_separator();
  test_get_type(c);
  print_separator();
  test_get_type_mask(c);
  print_separator();
  test_set_type(c);
  print_separator();
  test_set_type_mask(c);
  print_separator();
  test_get_failsafe(c);//get_failsafe will need to be verified
  print_separator();
  test_get_failsafe_mask(c);//get_failsafe_mask will need to be verified
  print_separator();
  connection_close( c );
  exit(EXIT_FAILURE);
  test_set_failsafe(c);
/* TODO discuss it
  test_set_failsafe_mask(c);
*/
  print_separator();
}
