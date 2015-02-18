/*
 * isr_timers.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

//todo: utilizar el minimo de timers que sea posible
#include "drivers.h"

void TIMER0_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);
}

void TIMER1_IRQHandler(void){
  if ( TIMER1->IR_MR0 )
  {
    TIMER1->IR_MR0 = 1;  /* Writing a HIGH clears the interrupt flag */
    (*timer1_mr0_interrupt_handler)();
  }

}

void TIMER2_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);

}

void TIMER3_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);

}
