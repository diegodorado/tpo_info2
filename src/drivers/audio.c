/*
 * audio.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#include "audio.h"
#include "drivers.h"
#include "fsm.h"

void audio_setup(void)
{
  dac_setup();
  timer1_setup(); // used for audio, interrupts at the defined sample rate of the current playback
  gpio_set_dir(0,22, 1);
  gpio_set_pin(0,22, 0);
}

void audio_set_sample_rate(uint32_t srate)
{
  timer1_set_sample_rate(srate);
}

void audio_play_sample()
{
  fsm_playback_sample_rate_tick();
}
