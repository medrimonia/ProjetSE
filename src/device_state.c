#include <stdlib.h>
#include <string.h>

#include "bit_utils.h"
#include "device_state.h"
#include "protocol.h"

static uint8_t ID = 0;

uint8_t get_reply_id( void )
{
  uint8_t id = ID;
  ID++;
  if ( ID == 0 ) {
    ID = 16;
  }
  return id;
}

void device_caps_init( struct device_caps * dc, uint8_t nb_pins,
                       unsigned char * pins_mask_type )
{
  dc->nb_pins        = nb_pins;
  dc->pins_mask_type = pins_mask_type;
}

void device_state_init( struct device_state * ds, struct device_caps * dc )
{
  uint8_t i;
  ds->pins_state = malloc( dc->nb_pins * sizeof(struct pin_state) );

  for ( i = 0; i < dc->nb_pins; i++ ) {
    ds->pins_state[i].pins_type = PIN_TYPE_DISABLED;
    ds->pins_state[i].pins_val  = 0;
  }
}

void read_device_caps( struct device_caps * dc, struct packet * p )
{
  dc->nb_pins = read_bit_value( p->data,
                                REPLY_ID_BITS_NB, PINS_NO_BITS_NB );
  if (dc->pins_mask_type != NULL){
    free( dc->pins_mask_type );
  }
  dc->pins_mask_type = malloc(dc->nb_pins);
  //TODO hard coded value
  uint8_t i;
  for (i = 0; i < dc->nb_pins; i++){
    dc->pins_mask_type[i] = p->data[i + 2];
  }
}
