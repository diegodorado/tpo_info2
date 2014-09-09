#ifndef GPIO_H
#define GPIO_H

#include "sysdefs.h"
#include "core/lpc_1769.h"

void SetDIR( uint8_t ,uint8_t ,uint8_t );
void SetPIN( uint8_t , uint8_t , uint8_t );
uint8_t GetPIN( uint8_t , uint8_t , uint8_t );

#endif /* GPIO_H */
