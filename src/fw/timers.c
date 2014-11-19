/*
 * timers.c
 *
 *  Created on: Oct 15, 2014
 *      Author: diego
 */

#include "timers.h"
#include "drivers.h"

void timer0_setup(void)
{

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


static volatile uint32_t us = 0;
uint32_t timer0_us()
{
  return us;
}


void  timer0_mr0_interrupt()
{
  us++;
  audio_us_tick();
}

// blocking delay... only for setup
void  timer0_delay_us(uint32_t us)
{
  uint32_t since = timer0_us();
  while ((uint32_t) (timer0_us() - since) < us );
}
