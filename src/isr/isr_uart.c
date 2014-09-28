/*
 * isr_uart.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "fw.h"
#include "drivers.h"

#ifdef USE_UART0

void UART0_IRQHandler(void){

  uint32_t iir;

  do
  {
    iir = U0IIR; // guardo el valor ya que se borra al leerlo

    switch((iir & 0x07)>>1) {

      //001--> Interr por TX
      case (0x01):
        if(uart0_rx_data_size()>0)
          U0THR = uart0_tx_pop();
        break;

      //010--> Interr por RX
      case (0x02):
        uart0_rx_push(U0RBR);
        break;

      //011--> Interr por LINE STATUS
      case (0x03):
        //todo: implementar Interr por LINE STATUS
        break;

    }


  } while( ! ( iir & 0x01 ) ); // continuar mientras haya interrupciones pendientes


}
#endif

#ifdef USE_UART1

void UART1_IRQHandler(void){

  uint32_t iir;

  do
  {
    iir = U1IIR; // guardo el valor ya que se borra al leerlo

    switch((iir & 0x07)>>1) {

      //001--> Interr por TX
      case (0x01):
        if(uart1_rx_data_size()>0)
          U1THR = uart1_tx_pop();
        break;

      //010--> Interr por RX
      case (0x02):
        uart1_rx_push(U1RBR);
        break;

      //011--> Interr por LINE STATUS
      case (0x03):
        //todo: implementar Interr por LINE STATUS
        break;

    }


  } while( ! ( iir & 0x01 ) ); // continuar mientras haya interrupciones pendientes


}
#endif
