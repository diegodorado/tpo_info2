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


#define PLAYBACK_BUFFER_LENGTH (512*32)
#define PLAYBACK_BUFFER_HALF_LENGTH (PLAYBACK_BUFFER_LENGTH/2)

void audio_setup(void);
void audio_set_sample_rate(uint32_t srate);

#endif /* AUDIO_H_ */
