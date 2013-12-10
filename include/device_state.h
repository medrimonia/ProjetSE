#ifndef DEVICE_STATE_H
#define DEVICE_STATE_H

struct device_caps {
  uint8_t          nb_pins;
  unsigned char *  pins_mask_type;
};

struct pin_state {
  uint8_t  pins_type;
  uint16_t pins_val;
};

struct device_state {
  struct pin_state * pins_state;
};

uint8_t get_id( void );

void device_caps_init  ( struct device_caps * dc, uint8_t nb_pins,
                         unsigned char * pins_mask_type );
void device_state_init( struct device_state * ds, struct device_caps * dc );

#endif /*DEVICE_STATE_H*/
