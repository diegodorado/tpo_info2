/*
 * storage.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#include "storage.h"



static volatile uint8_t buffer[512];
static volatile uint16_t buffer_in=0;
static volatile uint16_t buffer_out=0;
static volatile uint32_t block_index=0;


void storage_setup(void)
{
  //todo: implementar la inicializacion
}

uint8_t sd_card_test(void)
{
  uint8_t buffer1[N_BLOCKS * 512], buffer2[N_BLOCKS * 512] = { 0 };
  uint32_t i;
  uint8_t result = 0;

  if ( !sd_card_setup() )
    result = 1;

  for (i = 0; i < N_BLOCKS * 512; i++)
  {
    buffer1[i] = i % 512;
  }

  if ( sd_card_write(buffer1, N_BLOCKS, START_BLOCK) )
  {
    if ( sd_card_read(buffer2, N_BLOCKS, START_BLOCK) )
    {
      for (i = 0; i < N_BLOCKS * 512; i++)
      {
        if ( buffer1[i] != buffer2[i] )
        {
          result = 2;
          break;
        }
      }
    }
    else
      result = 3;
  }
  else
    result = 4;

  return result;

}

uint8_t sd_card_write_test(char * text)
{
  uint8_t buf[512];
  uint32_t i;

  // fill with text or zeros
  for (i = 0; i < 512; i++)
  {
    if ( *text )
      buf[i] = *text++;
    else
      buf[i] = 0;
  }

  if ( !sd_card_setup() )
    return 1; //error 1
  else if ( !sd_card_write(buf, 1, 0) )
    return 2; //error 2

  return 0; //no errors

}

uint8_t sd_card_read_test(char * text, uint8_t size)
{

  uint8_t buf[512];
  uint32_t i;

  if ( !sd_card_setup() )
    return 1; //error 1
  else if ( !sd_card_read(buf, 1, 0) )
    return 2; // error 2

  // fill text with returned buffer
  for (i = 0; i < size; i++)
  {
    *text++ = buf[i];
  }

  return 0; // no errors

}

