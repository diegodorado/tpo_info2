#include "dac.h"

void dac_delay(uint32_t t){
uint32_t x;
for(x=0;x<t*1000;x++);
//configurar ciclo a 1 microsegundo
}

void dac_init(void){
	SetPINSEL(DAC_PIN, DAC_FUNC);

	dac->BIAS = 0;

}

void dac_set_value(uint32_t val){
	dac->VALUE = val;
}


void dac_run(void){
	dac_init();
	while(1){
		//onda cuadrada??
		dac_delay(1);
		dac_set_value(1);
		dac_delay(1);
		dac_set_value(0);
	}
}
