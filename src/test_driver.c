#include <stdio.h>
#include <stdlib.h>

#include "driver.h"

#define NB_PINS 12

int main(void){
  printf("Get caps : ");
  get_caps(NULL);
  printf("Reset : ");
  reset(NULL);
  printf("Ping 3 : ");
  ping(NULL, 3);
  printf("Digital read on pin 5  : ");
  digital_read(NULL, 5, NULL);
  printf("Analogic read on pin 2 : ");
  analogic_read(NULL, 2, NULL);
  printf("PWM8 read on pin 7     : ");
  pwm8_read(NULL, 7, NULL);
  printf("PWM16 read on pin 13   : ");
  pwm16_read(NULL, 13, NULL);
  printf("Get type on pin 6      : ");
  get_type(NULL, 6, NULL);
  mask m = new_mask(NB_PINS);
  m[0] = true;
  m[5] = true;
  m[8] = true;
  get_type_mask(NULL, &m, NULL);
  free(m);
  exit(EXIT_SUCCESS);
}
