#include "dac.h"

void dac_init(void){
	SetPINSEL(DAC_PIN, DAC_FUNC);
	dac->BIAS = 0;
}

void dac_set_value(uint32_t val){
	dac->VALUE = val;
}
