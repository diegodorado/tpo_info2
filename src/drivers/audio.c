/*
 * audio.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "audio.h"

#define SOUND_DATA_LENGTH 24000
static volatile uint8_t SOUND_DATA_DATA[SOUND_DATA_LENGTH];
static volatile uint32_t sample_rate;
static volatile uint8_t playing = 0;

void audio_setup(void){
  dac_setup();
  gpio_set_dir(0,22, 1);
}


void audio_set_sample_rate(uint32_t srate)
{
  sample_rate = srate;
}


//return 1 if buffer is full
uint8_t audio_fill_audio_buffer(uint8_t* sample_start, uint8_t length)
{
  static volatile int sample_index = 0;

  while(length-- >0)
  {
    if (sample_index >= SOUND_DATA_LENGTH)
      return 1;

    SOUND_DATA_DATA[sample_index++] = *sample_start++;
  }

  return 0;

}

void audio_play()
{
  playing = 1;
}

void audio_stop()
{
  playing = 0;
}


// called every 1us
// sends the last sample to the dac
// increment the sample index if sample_rate is reached
void audio_us_tick()
{
  static volatile uint32_t counter = 0;
  static volatile uint32_t sample_index = 0;
  static volatile uint8_t last_sample;


  if(!playing)
    return;

  if(counter++ > 40){
    counter = 0;
    last_sample = SOUND_DATA_DATA[sample_index];

    if (++sample_index >= SOUND_DATA_LENGTH){
      sample_index = 0;
      gpio_set_pin(0,22,!gpio_get_pin(0,22,1));
    }
  }

  dac_set_value(last_sample);
}


