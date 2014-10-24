/*
 * isr_uart.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "drivers.h"


#ifdef USE_UART0

void UART0_IRQHandler(void){

  int x=0; // iir auxiliar, ya que el registro se borra al leerlo
  uart_iir_t iir; // iir auxiliar, ya que el registro se borra al leerlo
  static int rx_int_counter = 0;

  do
  {
    x++;
    iir = UART0->IIR;

    switch(iir.INT_ID) {

      case (UART_INT_ID_RLS):
        //todo: implementar Interr por LINE STATUS
        //printf("%d RLS INT, tx_size: %d  LSR: %d INT_STATUS: %d\n",x,uart0_tx_data_size(), UART0->LSR, iir.INT_STATUS);
        break;

      case (UART_INT_ID_RDA):
        while(UART0->LSR.RDR){
          rx_int_counter++;
          uart0_rx_push(UART0->RBR);
          //printf("%d RDA INT ID nro %d  LSR.RDR: %d INT_STATUS: %d\n",x,rx_int_counter, UART0->LSR.RDR, iir.INT_STATUS);
        }
        break;

      case (UART_INT_ID_CTI):
        while(UART0->LSR.RDR){
          uart0_rx_push(UART0->RBR);
          //printf("%d CTI INT ID nro %d  LSR.RDR: %d INT_STATUS: %d\n",x,rx_int_counter, UART0->LSR.RDR, iir.INT_STATUS);
        }
        break;

      case (UART_INT_ID_THRE):
        //printf("%d THRE INT, tx_size: %d  LSR.RDR: %d INT_STATUS: %d\n",x,uart0_tx_data_size(), UART0->LSR.RDR, iir.INT_STATUS);
        if(uart0_tx_data_size()>0)
          UART0->THR = uart0_tx_pop();
        break;


    }


  } while( ! iir.INT_STATUS ); // continuar mientras haya interrupciones pendientes


}
#endif

#ifdef USE_UART1

void UART1_IRQHandler(void){

  int x=0; // iir auxiliar, ya que el registro se borra al leerlo
  uart_iir_t iir; // iir auxiliar, ya que el registro se borra al leerlo
  static int rx_int_counter = 0;

  do
  {
    x++;
    iir = UART1->IIR;

    switch(iir.INT_ID) {

      case (UART_INT_ID_RLS):
        //todo: implementar Interr por LINE STATUS
#ifdef DEBUG_ON
        printf("%d RLS INT, tx_size: %d  LSR: %d INT_STATUS: %d\n",x,uart1_tx_data_size(), UART1->LSR, iir.INT_STATUS);
#endif
        break;

      case (UART_INT_ID_RDA):
        while(UART1->LSR.RDR){
          rx_int_counter++;
          uart1_rx_push(UART1->RBR);
#ifdef DEBUG_ON
          printf("%d RDA INT ID nro %d  LSR.RDR: %d INT_STATUS: %d\n",x,rx_int_counter, UART1->LSR.RDR, iir.INT_STATUS);
#endif
        }
        break;

      case (UART_INT_ID_CTI):
        while(UART1->LSR.RDR){
          uart1_rx_push(UART1->RBR);
#ifdef DEBUG_ON
          printf("%d CTI INT ID nro %d  LSR.RDR: %d INT_STATUS: %d\n",x,rx_int_counter, UART1->LSR.RDR, iir.INT_STATUS);
#endif
        }
        break;

      case (UART_INT_ID_THRE):
#ifdef DEBUG_ON
        printf("%d THRE INT, tx_size: %d  LSR.RDR: %d INT_STATUS: %d\n",x,uart1_tx_data_size(), UART1->LSR.RDR, iir.INT_STATUS);
#endif
        if(uart1_tx_data_size()>0)
          UART1->THR = uart1_tx_pop();
        break;


    }


  } while( ! iir.INT_STATUS ); // continuar mientras haya interrupciones pendientes


}
#endif
