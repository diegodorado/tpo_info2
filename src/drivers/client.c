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

static void update_lcd_timer(void){
  char str_clck[] = "00:00";

  int aux = seconds++;

  str_clck[4] = '0' + (aux%10);  aux /=10;
  str_clck[3] = '0' + (aux%6) ;  aux /=6;
  str_clck[1] = '0' + (aux%10);  aux /=10;
  str_clck[0] = '0' + (aux%6) ;

  lcd_print_at(str_clck, 1,11);

}




uint8_t client_has_message()
{
  uint8_t data;
  // fill message buffer if there is data on serial buffer
  while( client_data_size()>0 )
  {
    data = client_rx_pop();
    messagesBufferPush( data );
  }

  // return 1 if meesage buffer has a complete message checked
  buffer_status_t bufferStatus;
  bufferStatus = messagesBufferProcess();
  return bufferStatus == BUFFER_MSG_OK;
}

message_hdr_t* client_get_message()
{
  uint8_t* raw_data = messagesBufferPop();
  message_hdr_t* message = (message_hdr_t*) raw_data;

  //perform some common validations

  if(message == NULL){
    free(message);
    lcd_print_char_at('3',1,8);//debug!
    return NULL;
  }

  if(message->msg_type >= MESSAGE_MAX_VALID_TYPE){
    free(message);
    lcd_print_char_at('4',1,9); //debug!
    return NULL;
  }

  return message;

}




void client_send_status_response(message_hdr_t* request, status_id_t status)
{
  message_hdr_t response;

  response.data_length = 1;
  response.msg_id = request->msg_id;
  response.msg_type = request->msg_type;
  response.is_response = 1;
  client_send_message_response(&response, (uint8_t*)&status);

}



void client_send_message_response(message_hdr_t* message, uint8_t* data)
{
  client_send_message(message, data);
}

void client_send_message(message_hdr_t* message, uint8_t* data)
{

  uint16_t i;
  uint8_t checksum = messageGetChecksum(message, data);

  client_tx_push(START_OF_FRAME);

  for(i = 0; i < sizeof(message_hdr_t); i++)
    client_tx_push( *( ((uint8_t*) message) +i) );

  for(i = 0; i < message->data_length ; i++)
    client_tx_push(*(data + i));

  client_tx_push(checksum);
  client_tx_push(END_OF_FRAME);

}






void client_setup(void)
{
#ifdef USE_UART0
  uart0_setup();
#else
  uart1_setup();
#endif


}



uint8_t client_data_size ( void)
{
#ifdef USE_UART0
  return uart0_rx_data_size();
#else
  return uart1_rx_data_size();
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

