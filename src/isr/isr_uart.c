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

  uint32_t iir,data;
  char received[2];

  do
  {
    iir = U1IIR; // guardo el valor ya que se borra al leerlo

    switch((iir & 0x07)>>1) {

      //001--> Interr por TX
      case (0x01):
        data = uart1_tx_pop();
        if(data>0)
          U1THR = data;
        break;

      //010--> Interr por RX
      case (0x02):
        data = U1RBR;
        received[0] = data;
        received[1] = '\0';
        lcd_print_at(received,0,8);

        break;

      //011--> Interr por LINE STATUS
      case (0x03):
        break;

    }


  } while( ! ( iir & 0x01 ) ); // continuar mientras haya interrupciones pendientes


}
