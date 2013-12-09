#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "driver.h"
#include "protocol.h"
#include "test_utils.h"

#define NB_PINS 12

void test_get_caps( struct connection * c )
{
  print_title("Get caps :");
  get_caps(c);
  //TODO treat answer
}

void test_reset( struct connection * c )
{
  print_title("Reset :");
  reset(c);
}

void test_ping( struct connection * c )
{
  print_title("Ping 3 :");
  ping(c, 3);
}

void test_digital_read( struct connection * c )
{
  print_title("Digital read on pin 5  :");
  digital_read(c, 5, NULL);
}

void test_analogic_read( struct connection * c )
{
  print_title("Analogic read on pin 2  :");
  analogic_read(c, 2, NULL);
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



/*
void set_type_mask_test( struct connection * c )
{
  printf("Set type mask of pin 4,7,9 to digit, analog8, pwm16 :\n\t");
  mask m = new_mask(NB_PINS);
  m[4] = MASK_PIN_ON;
  m[7] = MASK_PIN_ON;
  m[9] = MASK_PIN_ON;
  uint16_t values[3] = { PIN_TYPE_DIGITAL, PIN_TYPE_ANALOG8, PIN_TYPE_PWM16 };
  set_type_mask(c, &m,  values, 3);//TODO length is not needed (read in mask)
  // Expected Data :
  //      mask    |   values   |
  //     4  7  9  | 4 |  7 | 9 |
  // 00001001|0100-010-0|00-100-000|
  //    09   |    44    |   20
  printf("\tExpected    : |61|00|03|09|44|20|\n");
  free(m);
}

void get_type_test( struct connection * c )
{
  printf("Get type of pin 8 : ");
  get_type(c, 8, NULL);
}

void get_type_mask_test( struct connection * c )
{
  printf("Get type mask of pin 3,6,10:\n\t");
  mask m = new_mask(NB_PINS);
  m[3]  = MASK_PIN_ON;
  m[6]  = MASK_PIN_ON;
  m[10] = MASK_PIN_ON;
  get_type_mask(c, &m, NULL);
  printf("\tExpected    : |51|00|02|12|20|\n");
  free(m);
}

void set_failsafe_test( struct connection * c )
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
  struct connection * c = new_connection();
  c->fd_out = STDERR_FILENO;
  c->nb_pins = NB_PINS;
  print_separator();
  test_get_caps(c);
  test_reset(c);
  test_ping(c);
  test_digital_read(c);
  test_analogic_read(c);
  test_pwm8_read(c);
  test_pwm16_read(c);
/*
  printf("Digital read on pin 5  : ");
  digital_read(c, 5, NULL);
  printf("Analogic read on pin 2 : ");
  analogic_read(c, 2, NULL);
  printf("PWM8 read on pin 7     : ");
  pwm8_read(c, 7, NULL);
  printf("PWM16 read on pin 13   : ");
  pwm16_read(c, 13, NULL);
  printf("Digital write of  'on' on pin 1 : ");
  digital_write (c, 1, true);
  printf("Analog  write of '510' on pin 6 : ");
  analogic_write(c, 6,  510);
  printf("PWM8    write of  '24' on pin 3 : ");
  pwm8_write    (c, 3,   24);
  printf("PWM16   write of '515' on pin 9 : ");
  pwm16_write   (c, 9,  515);
  get_type_test(c);
  get_type_mask_test(c);
  printf("Set type of pin 8 to digital    : ");
  set_type(c, 8,  PIN_TYPE_DIGITAL);
  set_type_mask_test(c);
  set_failsafe_test(c);
*/
  print_separator();
  destroy_connection(c);
  exit(EXIT_SUCCESS);
}
