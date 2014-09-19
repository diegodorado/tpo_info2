#ifndef GPIO_H
#define GPIO_H

#include "sysdefs.h"
#include "lpc_1769.h"

void SetDIR( uint8_t ,uint8_t ,uint8_t );
void gpio_set_pin( uint8_t , uint8_t , uint8_t );
uint8_t gpio_get_pin( uint8_t , uint8_t , uint8_t );

#endif /* GPIO_H */
