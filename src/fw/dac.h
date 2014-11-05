#ifndef DAC_H
#define DAC_H

#include "sysdefs.h"
#include "periph.h"

#define LOW 0
#define HIGH 1


#define DAC_PIN  0,26
#define DAC_FUNC  2

void dac_setup(void);
void dac_set_value(uint32_t);


#endif /* DAC_H */
