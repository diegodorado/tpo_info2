#include "dac.h"

void dac_setup(void){
	set_pin_sel(DAC_PIN, DAC_FUNC);
	DAC->BIAS = 0;
}

void dac_set_value(uint32_t val){
	DAC->VALUE = val;
}
