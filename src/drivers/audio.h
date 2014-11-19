/*
 * audio.h
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#ifndef AUDIO_H_
#define AUDIO_H_

//include firmware
#include "fw.h"

void audio_setup(void);
void audio_set_sample_rate(uint32_t srate);
uint8_t audio_fill_audio_buffer(uint8_t* sample_start, uint8_t length);
void audio_play();
void audio_stop();
void audio_us_tick();

#endif /* AUDIO_H_ */
