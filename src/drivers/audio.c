/*
 * audio.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "audio.h"
#include "drivers.h"


static volatile uint8_t playback_buffer[PLAYBACK_BUFFER_LENGTH];
static volatile uint16_t playback_buffer_in=0;
static volatile uint16_t playback_buffer_out=0;
volatile static uint32_t sd_block_index=2;

static volatile uint32_t sample_rate;
static volatile uint8_t playing = 0;

static uint16_t buffer_data_size();
static uint16_t buffer_free_space();


void audio_setup(void)
{
  dac_setup();
  timer1_setup(); // used for audio, interrupts at the defined sample rate of the current playback

  gpio_set_dir(0,22, 1);
  gpio_set_pin(0,22, 0);



}


static uint16_t buffer_data_size()
{
  if (playback_buffer_in >= playback_buffer_out )
    return playback_buffer_in - playback_buffer_out;
  else
    return PLAYBACK_BUFFER_LENGTH - ( playback_buffer_out - playback_buffer_in);
}


static uint16_t buffer_free_space()
{
  return PLAYBACK_BUFFER_LENGTH - buffer_data_size();
}




void audio_try_load_sample_block(void)
{

  if (buffer_free_space()>=(512*8))
  {

    if(sd_card_read(playback_buffer+playback_buffer_in, 8, sd_block_index))
    {
      gpio_set_pin(0,22,!gpio_get_pin(0,22,1));
      sd_block_index += 8;
      playback_buffer_in += (512*8);
      playback_buffer_in %= PLAYBACK_BUFFER_LENGTH;

    }

  }

}



void audio_set_sample_rate(uint32_t srate)
{
  sample_rate = srate;
}


void audio_play()
{
  if (sd_card_setup() )
  {
    lcd_print_at("sd ok",0,0);
    playing = 1;
  }
  else
  {
    lcd_print_at("sd failed",0,0);
  }



}

void audio_stop()
{
  playing = 0;
  sd_block_index = 1;
  playback_buffer_in = playback_buffer_out = 0;
}


// send a single audio sample to DAC
void audio_play_sample()
{
  uint8_t sample;

  if(!playing)
    return;

  if (buffer_data_size()>0)
  {
    sample = playback_buffer[playback_buffer_out];
    playback_buffer_out++;
    playback_buffer_out %= PLAYBACK_BUFFER_LENGTH;


    dac_set_value(sample*4); // shift volume, since DAC is 10bit, and data is 8bit

    //if (++sample_index >= PLAYBACK_BUFFER_LENGTH){
    //  gpio_set_pin(0,22,!gpio_get_pin(0,22,1)); // blink when buffer restart
    //}

  }




}


