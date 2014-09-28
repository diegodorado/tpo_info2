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


#define  UART_BUFFER_SIZE 100
#define  U0RX_PIN  0,2
#define  U0TX_PIN  0,3
#define  U1RX_PIN  0,16
#define  U1TX_PIN  0,15

//buffer size macro
#define BUFFER_SIZE(data_array) ( sizeof(data_array) / sizeof(data_array[0]) )
//macro que devuelve la cantidad de datos que hay en el buffer
//en base a la posicion de entrada y salida + buffer_size
#define BUFFER_DATA_SIZE(buffer) ( BUFFER_SIZE(buffer.data) + buffer.in_index - buffer.out_index ) % BUFFER_SIZE(buffer.data)

typedef struct{
  uint8_t data[UART_BUFFER_SIZE];
  uint8_t in_index;
  uint8_t out_index;
} uart_buffer_t;


void uart0_setup(void);
void uart1_setup(void);

void uart0_rx_push ( uint8_t);
uint8_t uart0_rx_pop ( void);
uint8_t uart0_rx_data_size (void);

void uart0_tx_push ( uint8_t);
uint8_t uart0_tx_pop ( void);
uint8_t uart0_tx_data_size (void);

void uart1_rx_push ( uint8_t);
uint8_t uart1_rx_pop ( void);
uint8_t uart1_rx_data_size (void);

void uart1_tx_push ( uint8_t);
uint8_t uart1_tx_pop ( void);
uint8_t uart1_tx_data_size (void);


#endif /* UART_H_ */
