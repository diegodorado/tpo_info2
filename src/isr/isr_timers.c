/*
 * isr_timers.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

//todo: utilizar el minimo de timers que sea posible
#include "drivers.h"

void TIMER0_IRQHandler(void){
  if ( TIMER0->IR_MR0 )
  {
    TIMER0->IR_MR0 = 1;  /* Writing a HIGH clears the interrupt flag */
    timer0_mr0_interrupt();
  }
}

void TIMER1_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);

}

void TIMER2_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);

}

void TIMER3_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);

}
