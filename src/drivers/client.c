/*
 * client.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

/*
 * Abstraccion de la comunicacion
 * Puede utilizar uart0 o uart1 segun este USE_UART0 definida o no
 */

#include "client.h"

/**
 * TRAMA SERIE
 * [(tiempo)#velocidad#%evento%checksum]
 * evento: 0 a 4 (numero de pulsador)
 * ejemplo: [(22:39)#87#%numero_pulsador%xor_total]
 *
 * */


void client_send_data_frame (uint8_t minutes,uint8_t seconds,uint8_t velocity,uint8_t event){
  tp4_data_frame_t data;
  //cast data struct as uint8_t pointer
  uint8_t* data_ptr = (uint8_t*) &data;

  data.minutes = minutes;
  data.seconds = seconds;
  data.velocity = velocity;
  data.event = event;
  data.checksum = 0; //todo: calc checksum

  //uso de punteros para serializar la estructura
  // y aritmetica de punteros para recorrerla
  for(; data_ptr - (uint8_t*)&data < sizeof(data); ){
    client_tx_push(*data_ptr++);
  }


}

tp4_data_frame_t client_decode_data_frame(void){

}



void client_setup(void)
{
#ifdef USE_UART0
  uart0_setup();
#else
  uart1_setup();
#endif
}



uint8_t client_data_frame_received ( void){
#ifdef USE_UART0
  return (uart0_rx_data_size()>=sizeof(tp4_data_frame_t));
#else
  return (uart1_rx_data_size()>=sizeof(tp4_data_frame_t));
#endif

}


uint8_t client_rx_pop ( void)
{
#ifdef USE_UART0
  return uart0_rx_pop();
#else
  return uart1_rx_pop();
#endif
}

void client_tx_push ( uint8_t data)
{
#ifdef USE_UART0
  uart0_tx_push(data);
#else
  uart1_tx_push(data);
#endif
}

