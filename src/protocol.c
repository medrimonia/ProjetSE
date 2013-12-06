#include "protocol.h"

uint8_t get_type_bits_nb(uint8_t pin_type){
  switch(pin_type){
  case PIN_TYPE_DIGITAL:
    return 1;
  case PIN_TYPE_ANALOG8:
  case PIN_TYPE_PWM8:
    return 8;
  case PIN_TYPE_ANALOG16:
  case PIN_TYPE_PWM16:
    return 16;
  }
  // Invalid mode
  return 0;
}
