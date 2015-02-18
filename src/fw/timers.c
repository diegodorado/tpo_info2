/*
 * timers.c
 *
 *  Created on: Oct 15, 2014
 *      Author: diego
 */

#include "timers.h"
#include "drivers.h"


void dummy_timer1_mr0_interrupt_handler(void){};

void timer_set_timer1_mr0_interrupt_handler(void (*handler)(void)){
  timer1_mr0_interrupt_handler = handler;
}


void timer_sleep_us (int us){
    volatile int    i;
    while (us--) {
        for (i = 0; i < US_TIME; i++) {
            ;    /* Burn cycles. */
        }
    }
}

void timer_sleep_ms (int ms){
    volatile int    i;
    while (ms--) {
        for (i = 0; i < MS_TIME; i++) {
            ;    /* Burn cycles. */
        }
    }
}







void timer1_setup(void)
{
  timer_set_timer1_mr0_interrupt_handler(dummy_timer1_mr0_interrupt_handler);


  PCONP |= (0x1<<2);   // Habilitar Timer 1 Pag 63
  PCLKSEL0 &= ~(3 << 4);  // Clock for timer: CCLK/4.... 25MHz


  TIMER1->TCR_ENABLED = 0;    // Apago el temporizador
  TIMER1->TCR_RESET = 1;      // Reseteo el temporizador

  TIMER1->PR = 0;         // 25MHz / 25 = 1MHz
  TIMER1->PC = 0;
  TIMER1->MR0 = 3125-1;   // match0
  TIMER1->TC = 0;

  TIMER1->MCR = 0x00;     // Aseguro la condición de arranque
  TIMER1->MCR_MR0I = 1;   // Interrupt on Match0
  TIMER1->MCR_MR0R = 1;   // Reset on Match0


  ISER0 |= (0x00000001)<<  2;  // Habilito interrupcion del Timer1


}




void timer1_set_sample_rate(uint32_t srate)
{

  TIMER1->TCR_ENABLED = 0;    // Apago el temporizador
  TIMER1->TCR_RESET = 1;      // Reseteo el temporizador

  TIMER1->MR0 = (25000000/srate)-1; // la cuenta es aproximada...
  TIMER1->TC = 0;

  TIMER1->TCR_RESET = 0;      // Apago el bit de RESET
  TIMER1->TCR_ENABLED = 1;    // Enciendo el temporizador

}
