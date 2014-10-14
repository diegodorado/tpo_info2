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



static volatile uint32_t seconds = 0;

static uint8_t minutes_only(void){
  int aux = seconds;
  aux /= 60;
  return aux % 60;
}

static uint8_t seconds_only(void){
  return seconds % 60;
}

static void update_lcd_timer(void){
  char str_clck[] = "00:00";

  int aux = seconds++;

  str_clck[4] = '0' + (aux%10);  aux /=10;
  str_clck[3] = '0' + (aux%6) ;  aux /=6;
  str_clck[1] = '0' + (aux%10);  aux /=10;
  str_clck[0] = '0' + (aux%6) ;

  lcd_print_at(str_clck, 1,11);

}



/**
 * TRAMA SERIE
 * [(tiempo)#velocidad#%evento%checksum]
 * evento: 0 a 4 (numero de pulsador)
 * ejemplo: [(22:39)#87#%numero_pulsador%xor_total]
 *
 * */

static uint8_t checksum(tp4_data_frame_t data)
{
  uint8_t* data_ptr = (uint8_t*) &data;  //cast data struct as uint8_t pointer
  uint8_t result = 0;
  for(; data_ptr - (uint8_t*)&data < sizeof(data) - 1 ; ) // -1, since last uint8_t is the checksum itself
    result ^= *data_ptr++;

  return result;
}

uint8_t client_is_checksum_ok(tp4_data_frame_t data)
{
  return (data.checksum == checksum(data));
}



void client_send_data_frame (uint8_t velocity,uint8_t event, uint8_t fake_checksum)
{
  tp4_data_frame_t data;
  uint8_t* data_ptr = (uint8_t*) &data;  //cast data struct as uint8_t pointer

  data.minutes = minutes_only();
  data.seconds = seconds_only();
  data.velocity = velocity;
  data.event = event;
  data.checksum = checksum(data);

  if(fake_checksum)
    data.checksum = fake_checksum;

  //uso de punteros para serializar la estructura
  // y aritmetica de punteros para recorrerla
  for(; data_ptr - (uint8_t*)&data < sizeof(data); ){
    client_tx_push(*data_ptr++);
  }


}

tp4_data_frame_t client_decode_data_frame(void)
{
  tp4_data_frame_t data;
  uint8_t* data_ptr = (uint8_t*) &data;  //cast data struct as uint8_t pointer

  //uso de punteros para serializar la estructura
  // y aritmetica de punteros para recorrerla
  for(; data_ptr - (uint8_t*)&data < sizeof(data); )
    *data_ptr++ = client_rx_pop();

  return data;
}



void client_setup(void)
{
#ifdef USE_UART0
  uart0_setup();
#else
  uart1_setup();
#endif

  lcd_clear();
  systick_delay_async(1000, 1,update_lcd_timer);

}



uint8_t client_data_frame_received ( void)
{
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

