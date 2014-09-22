/*
 * uart.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "uart.h"

void uart1_setup(void)
{
  //TODO: Completar Inicializacion de UART1
  //1.- Registro PCONP: Energizo la UART:
  //1.- Registro PCONP (0x400FC0C4) - bit 4 en 1 prende la UART:
  PCONP |= 0x10;

  //2.- Registro PCLKSEL0 (0x400FC1A8) - selecciono el clk de la UART1
  // bits 8 y 9 en 0 seleccionan que el clk de la UART1 sea 25MHz--> DIVIDE POR 4
  PCLKSEL0 &= ~(0x03<<8);

  //3.- Registro U1LCR (0x4001000C)
  // transmision de 8 bits, 1 bit de stop, sin paridad, sin break cond, DLAB = 1
  U1LCR =0x83;

  //4.- Registros U1DLL (0x40010000) y U1DLM (0x40010004) en 0xA2:
  U1DLM = 0x00;   // es el resultado de 25Mhz/(9600*16)---> 162
  U1DLL = 0xA2;

  //5. habilitan las funciones especiales de los pines:
  set_pin_sel(U1TX_PIN,1);
  set_pin_sel(U1RX_PIN,1);

  //6. Habilito las interrupciones (En la UART -IER- y en el NVIC -ISER)
  // hay que poner el DLAB=0 para habilitar las interrupciones por TX RX;
  U1LCR &= ~(0x01<<7);// pongo en cero el bit 7 DLAB=0
  U1IER |= 0X03 ; // bit 0 y 1 del registro U1Ier Habilia int por TX y RX
  ISER0 |= (0x01<< 6);//  Habilita Interrupcion por UART1 del NVIC  Pag77


 }




extern uint8_t buf_rx_UART1[UART_BUFFER_SIZE];
extern uint8_t buf_tx_UART1[UART_BUFFER_SIZE];

extern uint8_t out_tx_UART1 = 0;
extern uint8_t in_tx_UART1 = 0;
extern uint8_t out_rx_UART1 = 0;
extern uint8_t in_rx_UART1 = 0;




void uart1_tx_push ( uint8_t data )
{
  buf_tx_UART1 [in_tx_UART1] = data;
  in_tx_UART1++;
  in_tx_UART1 %= UART_BUFFER_SIZE;

  //Si esta vacio el THR
  if ( U1LSR & 0x20 ) {
    data = uart1_tx_pop();
    U1THR = data;
  }

}



uint8_t uart1_tx_pop ( void)
{

  uint8_t aux;

  aux = buf_tx_UART1[out_tx_UART1];
  out_tx_UART1++;
  out_tx_UART1 %= UART_BUFFER_SIZE;

  return aux;

}

