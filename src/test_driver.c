#include <stdio.h>
#include <stdlib.h>

#include "driver.h"
#include "protocol.h"

#define NB_PINS 12

void set_type_mask_test(struct connection * c){
  printf("Set type mask of pin 4,7,9 to digit, analog8, pwm16 :\n\t");
  mask m = new_mask(NB_PINS);
  m[4] = MASK_PIN_ON;
  m[7] = MASK_PIN_ON;
  m[9] = MASK_PIN_ON;
  uint16_t values[3] = { PIN_TYPE_DIGITAL, PIN_TYPE_ANALOG8, PIN_TYPE_PWM16};
  set_type_mask(c, &m,  values, 3);
  // Expected Data :
  //      mask    |   values   |
  //     4  7  9  | 4 |  7 | 9 |
  // 00001001|0100-010-0|00-100-000|
  //    09   |    44    |   20
  printf("\tExpected    : |61|00|03|09|44|20|\n");
  free(m);
}

int main(void){
  struct connection * c = new_connection();
  c->nb_pins = NB_PINS;
  printf("Get caps : ");
  get_caps(c);
  printf("Reset : ");
  reset(c);
  printf("Ping 3 : ");
  ping(c, 3);
  printf("Digital read on pin 5  : ");
  digital_read(c, 5, NULL);
  printf("Analogic read on pin 2 : ");
  analogic_read(c, 2, NULL);
  printf("PWM8 read on pin 7     : ");
  pwm8_read(c, 7, NULL);
  printf("PWM16 read on pin 13   : ");
  pwm16_read(c, 13, NULL);
  printf("Get type on pin 6      : ");
  get_type(c, 6, NULL);
  printf("Get type mask (1,5,8)  : ");
  mask m = new_mask(NB_PINS);
  m[0] = MASK_PIN_ON;
  m[5] = MASK_PIN_ON;
  m[8] = MASK_PIN_ON;
  get_type_mask(c, &m, NULL);
  free(m);
  printf("Digital write of  'on' on pin 1 : ");
  digital_write (c, 1, true);
  printf("Analog  write of '510' on pin 6 : ");
  analogic_write(c, 6,  510);
  printf("PWM8    write of  '24' on pin 3 : ");
  pwm8_write    (c, 3,   24);
  printf("PWM16   write of '515' on pin 9 : ");
  pwm16_write   (c, 9,  515);
  printf("Set type of pin 8 to digital    : ");
  set_type(c, 8,  PIN_TYPE_DIGITAL);
  set_type_mask_test(c);
  destroy_connection(c);
  exit(EXIT_SUCCESS);
}
