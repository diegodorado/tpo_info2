#ifndef PERIPH_H_
#define PERIPH_H_

#include "sysdefs.h"
#include "lpc_1769.h"

void set_pin_sel ( uint8_t , uint8_t , uint8_t );
void set_pin_mode ( uint8_t , uint8_t , uint8_t );
void gpio_set_dir( uint8_t ,uint8_t ,uint8_t );
void gpio_set_pin( uint8_t , uint8_t , uint8_t );
uint8_t gpio_get_pin( uint8_t , uint8_t , uint8_t );

#endif /* PERIPH_H_ */
