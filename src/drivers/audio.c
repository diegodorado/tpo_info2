/*
 * audio.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "audio.h"
#include <math.h>

#define PI 3.14159265
#define MAX_SAMPLE_VALUE 128



#define SOUND_DATA_LENGTH 24000
static volatile uint8_t SOUND_DATA_DATA[SOUND_DATA_LENGTH];
static volatile uint32_t sample_rate;
static volatile uint8_t playing = 0;

static void audio_gen_sine_wave(void);
static void audio_sd_read_test(void);


void audio_setup(void)
{
  dac_setup();
  gpio_set_dir(0,22, 1);
  //audio_gen_sine_wave();

  audio_sd_read_test();

}

static void audio_gen_sine_wave(void)
{
  uint32_t freq,sample_index = 0;
  int i;
  uint8_t sample;
  double t,sine, period_sample_rate;

  sample_rate = 8000;
  freq = 600;


  while (sample_index < SOUND_DATA_LENGTH)
  {


    if(sample_index%200 == 0)
    {
      freq -= 5;
    }
    period_sample_rate = sample_rate * 1.0 / freq;

    t =  i * 1.0 / period_sample_rate;
    i++;
    i %=(int)period_sample_rate;

    sine = sin(t*2.0*PI);
    sample = ((sine + 1.0) * 0.5 * MAX_SAMPLE_VALUE);
    SOUND_DATA_DATA[sample_index++] = sample;
  }

}




static void audio_sd_read_test(void)
{
  uint32_t sample_index = 0;
  int i, ssd_result, block=0 ;
  uint8_t data_buf[512];

  sd_card_setup();

  //leo solo 44 bloques de la sd
  for (block=0;block<44;block++)
  {
    if(sd_card_read(data_buf, 512, block) ==0)
    {
      for(i=0;i<512;i++)
        SOUND_DATA_DATA[sample_index++] = data_buf[i];

    }
    else{
      lcd_print("error sd");
    }



  }


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
void audio_play_sample()
{
  static volatile uint32_t sample_index = 0;
  uint8_t sample;

  if(!playing)
    return;


  sample = SOUND_DATA_DATA[sample_index];

  if (++sample_index >= SOUND_DATA_LENGTH){
    sample_index = 0;
    gpio_set_pin(0,22,!gpio_get_pin(0,22,1)); // blink when buffer restart
  }
  dac_set_value(sample*4); // shift volume, since DAC is 10bit, and data is 8bit


}


