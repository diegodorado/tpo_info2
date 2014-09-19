/*
 * audio.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "audio.h"

//todo: implementar con timer
void audio_delay(uint32_t t){
uint32_t x;
for(x=0;x<t*1000;x++);
//configurar ciclo a 1 microsegundo
}

void audio_setup(void){
  dac_init();
}

void audio_test(void){
//onda cuadrada??
 audio_delay(1);
 dac_set_value(1);
 audio_delay(1);
 dac_set_value(0);
}
