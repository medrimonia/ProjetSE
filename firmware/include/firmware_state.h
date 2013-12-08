#ifndef FIRMWARE_STATE_H
#define FIRMWARE_STATE_H

struct pin_state {
  uint8_t  pins_type;
  uint16_t pins_val;
};

struct device_caps {
  uint8_t            nb_pins;
  struct pin_state * pins_state;
};

#endif /*FIRMWARE_STATE_H*/
