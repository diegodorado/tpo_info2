/*
 * isr_systick.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "drivers.h"

void SysTick_Handler(void)
{
  systick_handle_tick();
}
