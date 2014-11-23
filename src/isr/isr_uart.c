/*
 * isr_uart.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "drivers.h"


#ifdef USE_UART0

void UART0_IRQHandler(void){
  uart_iir_t iir; // iir auxiliar, ya que el registro se borra al leerlo
  uart_lsr_t lsr; // RLS interrupt is cleared upon an LSR read
  uint8_t data;

  do{
    iir = UART0->IIR;
    switch(iir.INT_ID) {
          case (UART_INT_ID_RDA):
                                              uart0_rx_push(UART0->RBR);
                                              break;
          case (UART_INT_ID_CTI):
                                              uart0_rx_push(UART0->RBR);
                                              break;
          case (UART_INT_ID_RLS):
                                              lsr = UART0->LSR; //reading clears the interrupt
                                              if(lsr.BI)
                                              puts_lcd("BI",0,3);
                                              if(lsr.FE)
                                              puts_lcd("FE",1,0);
                                              if(lsr.OE)
                                              puts_lcd("OE",0,0);
                                              if(lsr.PE)
                                              puts_lcd("PE",1,3);
                                              break;
          case (UART_INT_ID_THRE):
                                              if(uart0_tx_data_size()>0)
                                              UART0->THR = uart0_tx_pop();
                                              break;
        }
  } while( ! iir.INT_STATUS ); // continuar mientras haya interrupciones pendientes
  /*//      code OLD
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
                      printf("%d RLS INT, tx_size: %d  LSR: %d INT_STATUS: %d\n",x,uart0_tx_data_size(), UART0->LSR, iir.INT_STATUS);
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
                  printf("%d CTI INT ID nro %d  LSR.RDR: %d INT_STATUS: %d\n",x,rx_int_counter, UART0->LSR.RDR, iir.INT_STATUS);
                  }
                  break;
      case (UART_INT_ID_THRE):
                  printf("%d THRE INT, tx_size: %d  LSR.RDR: %d INT_STATUS: %d\n",x,uart0_tx_data_size(), UART0->LSR.RDR, iir.INT_STATUS);
                  if(uart0_tx_data_size()>0)
                  UART0->THR = uart0_tx_pop();
                  break;
                  }
  } while( ! iir.INT_STATUS ); // continuar mientras haya interrupciones pendientes
*/
}
#endif

#ifdef USE_UART1


void UART1_IRQHandler(void){

  uart_iir_t iir; // iir auxiliar, ya que el registro se borra al leerlo
  uart_lsr_t lsr; // RLS interrupt is cleared upon an LSR read

  uint8_t data;

  do
  {
    iir = UART1->IIR;

    switch(iir.INT_ID) {

      case (UART_INT_ID_RDA):
        uart1_rx_push(UART1->RBR);
        break;

      case (UART_INT_ID_CTI):
        uart1_rx_push(UART1->RBR);
        break;

      case (UART_INT_ID_RLS):
        lsr = UART1->LSR; //reading clears the interrupt
        if(lsr.BI)
          lcd_print_at("BI",0,3);
        if(lsr.FE)
          lcd_print_at("FE",1,0);
        if(lsr.OE)
          lcd_print_at("OE",0,0);
        if(lsr.PE)
          lcd_print_at("PE",1,3);

        break;


      case (UART_INT_ID_THRE):
        if(uart1_tx_data_size()>0)
          UART1->THR = uart1_tx_pop();
        break;


    }

  } while( ! iir.INT_STATUS ); // continuar mientras haya interrupciones pendientes

}

#endif
