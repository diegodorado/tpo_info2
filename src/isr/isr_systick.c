/*
 * isr_systick.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */


volatile static int us = 0;
void SysTick_Handler(void)
{
  // cuento microsegundos
  us++;
}
