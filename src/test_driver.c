#include <stdio.h>
#include <stdlib.h>

#include "driver.h"

#define NB_PINS 12

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
  m[0] = true;
  m[5] = true;
  m[8] = true;
  get_type_mask(c, &m, NULL);
  free(m);
  destroy_connection(c);
  exit(EXIT_SUCCESS);
}
