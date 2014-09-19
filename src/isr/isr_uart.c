/*
 * isr_uart.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "fw.h"
#include "drivers.h"

//todo: decidir cual UART utilizar

void UART0_IRQHandler(void){
  //todo: implementar interrupcion
  while(1);

}


void UART1_IRQHandler(void){
  //todo: implementar interrupcion

  uint8_t iir,data;

  do
  {
    iir = U1IIR; // guardo el valor ya que se borra al leerlo

    switch((iir & 0x07)>>1) {

      //001--> Interr por TX
      case (0x01):
        lcd_set_cursor(1, 1);
        lcd_print("TX");
        data = uart1_rx_pop();
        if(data>0)
          U1THR = data;
        break;

      //010--> Interr por RX
      case (0x02):
        lcd_set_cursor(6, 1);
        lcd_print("RX");
        data = U1RBR;
        uart1_tx_push(data);
        break;

      //011--> Interr por LINE STATUS
      case (0x03):
        lcd_set_cursor(1, 1);
        lcd_print("LSR");
        break;

    }


  } while( ! ( iir & 0x01 ) ); // continuar mientras haya interrupciones pendientes


}
