/*
 * uart.h
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#ifndef UART_H_
#define UART_H_

#include "sysdefs.h"
#include "periph.h"






//!<_____________________________________ Registros SERIE _____________________________________
//!<UART0:
//!<0x4001000CUL : Registro de control de la UART0:
#define  DIR_U0LCR ( ( uint32_t  * ) 0x4000C00CUL )
//!<0x40010014UL : Registro de recepcion de la UART0:
#define  DIR_U0LSR  ( ( uint32_t  * ) 0x4000C014UL )
//!<0x40010000UL : Parte baja del divisor de la UART0:
#define  DIR_U0DLL ( ( uint32_t  * ) 0x4000C000UL )
//!<0x40010004UL : Parte alta del divisor de la UART0:
#define  DIR_U0DLM ( ( uint32_t  * ) 0x4000C004UL )
//!<0x40010000UL : Registro de recepcion de la UART0:
#define  DIR_U0RBR  ( ( uint32_t  * ) 0x4000C000UL )
//!<0x40010000UL : Registro de transmision de la UART0:
#define  DIR_U0THR  ( ( uint32_t  * ) 0x4000C000UL )

//!<UART1:
//!<0x4001000CUL : Registro de control de la UART1:
#define  DIR_U1LCR ( ( uint32_t  * ) 0x4001000CUL )
//!<0x40010014UL : Registro de recepcion de la UART1:
#define  DIR_U1LSR  ( ( uint32_t  * ) 0x40010014UL )
//!<0x40010000UL : Parte baja del divisor de la UART1:
#define  DIR_U1DLL ( ( uint32_t  * ) 0x40010000UL )
//!<0x40010004UL : Parte alta del divisor de la UART1:
#define  DIR_U1DLM ( ( uint32_t  * ) 0x40010004UL )
//!<0x40010000UL : Registro de recepcion de la UART1:
#define  DIR_U1RBR ( ( uint32_t  * ) 0x40010000UL )
//!<0x40010000UL : Registro de transmision de la UART1:
#define  DIR_U1THR ( ( uint32_t  * ) 0x40010000UL )
//!<0x40010004UL : Registro habilitacion de interrupciones de la UART1:
#define  DIR_U1IER ( ( uint32_t  * ) 0x40010004UL )
//!<0x40010008UL : Registro de identificación de la interrupción de la UART1:
#define  DIR_U1IIR ( ( uint32_t  * ) 0x40010008UL )
//!<0x40010008UL : Registro de control de la FIFO de la UART1:
#define  DIR_U1FCR ( ( uint32_t  * ) 0x40010008UL )



 //!< /////////////  UART0  ///////////////////////////
  //0x40010000UL : Registro de recepcion de la UART0:
  #define  DIR_UART0  ( ( __RW uint32_t  * ) 0x4000C000UL )

  #define  U0RBR  DIR_UART0[0] // Registro de Recepción RBR
  #define  U0THR  DIR_UART0[0] // Registro de Transmisión THR
  #define  U0DLL  DIR_UART0[0] // Parte baja del divisor de la UART0:
  #define  U0IER  DIR_UART0[1] // Registro de Habilitación de interrupciones de la UART0:
  #define  U0DLM  DIR_UART0[1] // Parte Alta del divisor de la UART0:
  #define  U0IIR  DIR_UART0[2] // Registro de Identificación de interrupciones de la UART0:
  #define  U0LCR  DIR_UART0[3] // Line CONTROL Register de la UART0:
  #define  U0LSR  DIR_UART0[5] // Line STATUS Register de la UART0:
  #define  U0FCR  DIR_UART0[8]
  //Macros UART0
  #define  U0DLAB_OFF (U0LCR & 0xEF)
  #define  U0DLAB_ON (U0LCR | 0x80)




//!<Registros de la UART1:
#define  U1THR  DIR_U1THR[0]
#define  U1RBR  DIR_U1RBR[0]
#define  U1LCR  DIR_U1LCR[0]
#define  U1LSR  DIR_U1LSR[0]
#define  U1DLL  DIR_U1DLL[0]
#define  U1DLM  DIR_U1DLM[0]
#define  U1IER  DIR_U1IER[0]
#define  U1IIR  DIR_U1IIR[0]
#define  U1FCR  DIR_U1FCR[0]
#define  U1RDR  (U1LSR & 0x01)
#define  U1THRE  ((U1LSR & 0x20)>>5)
#define  U1RS485CTRL  DIR_U1RS485CTRL[0]
#define  U1RS485ADRMATCH DIR_U1RS485ADRMATCH[0]
#define  U1RS485DLY  DIR_U1RS485DLY[0]



#define  U1TX_PIN  0,15
#define  U1RX_PIN  0,16



#define UART_BUFFER_SIZE 100

uint8_t buf_rx_UART1[UART_BUFFER_SIZE];
uint8_t buf_tx_UART1[UART_BUFFER_SIZE];

uint8_t out_tx_UART1;
uint8_t in_tx_UART1;
uint8_t out_rx_UART1;
uint8_t in_rx_UART1;



void uart1_setup(void);

void uart1_tx_push ( uint8_t);
uint8_t uart1_rx_pop ( void);


#endif /* UART_H_ */
