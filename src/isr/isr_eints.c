/*
 * isr_eints.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "drivers.h"


void EINT0_IRQHandler(void){
  //todo: implementar interrupcion

  CLR_EINT0;     // borro el flag EINT0 del registro EXTINT
  keyboard_handle_key(1);


}

void EINT1_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);

}

void EINT2_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);

}

void EINT3_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);

}

