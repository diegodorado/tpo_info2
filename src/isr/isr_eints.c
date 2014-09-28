/*
 * isr_eints.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "drivers.h"


#ifdef USE_SW1_WITH_INTERRUPTS
void EINT0_IRQHandler(void){
  // borro el flag EINT0 del registro EXTINT
  CLR_EINT0;
  keyboard_handle_key(0);


}
#endif

#ifdef USE_SW4_WITH_INTERRUPTS
void EINT3_IRQHandler(void){
  // borro el flag EINT3 del registro EXTINT
  CLR_EINT3;
  keyboard_handle_key(3);
}
#endif

