/*
 * timers.c
 *
 *  Created on: Oct 15, 2014
 *      Author: diego
 */

#include "timers.h"
#include "drivers.h"

#define SOUND_DATA_LENGTH 16000
static char SOUND_DATA_DATA[SOUND_DATA_LENGTH];

void timer0_setup(void)
{

  gpio_set_dir(0,22, 1);
  gpio_set_pin(0,22,1);


  PCONP |= (1 << 1);   // Habilitar Timer 0 Pag 63
  PCLKSEL0 &= ~(3 << 2);  // Clock for timer: CCLK/4.... 25MHz


  TIMER0->TCR_ENABLED = 0;    // Apago el temporizador
  TIMER0->TCR_RESET = 1;      // Reseteo el temporizador

  TIMER0->PR = 25; //1Mhz    3125;      //25MHz/3125 = 8kHz
  TIMER0->PC = 0;
  TIMER0->MR0 = 1;      // match0 a 8kHz
  TIMER0->TC = 0;

  TIMER0->MCR = 0x00;     // Aseguro la condición de arranque
  TIMER0->MCR_MR0I = 1;   // Interrupt on Match0
  TIMER0->MCR_MR0R = 1;   // Reset on Match0


  ISER0 |= (0x00000001)<<  1;  // Habilito interrupcion del Timer0

  TIMER0->TCR_RESET = 0;      // Apago el bit de RESET
  TIMER0->TCR_ENABLED = 1;    // Enciendo el temporizador

}



void  timer0_mr0_interrupt()
{
  static volatile int counter = 0;
  static volatile int sample_index = 0;
  static volatile unsigned char last_sample;

  if(!counter--){
    counter = 124;
    last_sample = SOUND_DATA_DATA[sample_index];
    if (++sample_index >= SOUND_DATA_LENGTH){
      sample_index = 0;
      gpio_set_pin(0,22,!gpio_get_pin(0,22,1));
    }
  }

  dac_set_value(last_sample);

}





//for now, it is just timer0
void timer1_setup(void)
{

  PCONP |= (1 << 1);   // Habilitar Timer 0 Pag 63
  PCLKSEL0 &= ~(3 << 2);  // Clock for timer: CCLK/4.... 25MHz

  TIMER0->TCR_ENABLED = 0;    // Apago el temporizador
  TIMER0->TCR_RESET = 1;      // Reseteo el temporizador

  TIMER0->PR = 25000;   // 1ms
  TIMER0->PC = 0;
  TIMER0->TC = 0;

  TIMER0->TCR_RESET = 0;      // Apago el bit de RESET

}


void  timer1_delay_ms(uint8_t ms)
{
  TIMER0->TC = 0;
  TIMER0->TCR_ENABLED = 1;    // Enciendo el temporizador

  //wait until timer counter reaches the desired dela1y
  while(TIMER0->TC < ms);


  TIMER0->TCR_ENABLED = 0;    //disable timer

}
