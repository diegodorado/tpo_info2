/*
 * audio.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "audio.h"

void audio_setup(void){
  dac_setup();
  timer0_setup();
}
