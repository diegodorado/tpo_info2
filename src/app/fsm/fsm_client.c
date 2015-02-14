/*
 * fsm_client.c
 *
 *  Created on: Nov 13, 2014
 *      Author: diego
 */


#include "fsm_client.h"


static volatile  fsm_client_state_t state = FSM_CLIENT_STATE_IDLE; // estado inicial

// funciones primitivas de los estados
static void idle( void);
static void connected( void);
static void processing_status( void);
static void processing_command( void);
static void processing_fileheader( void);
static void processing_filechunks( void);

//utils
static void process_chunk( message_hdr_t* message);

static volatile message_hdr_t* last_request = NULL;
static volatile uint32_t chunks_count=0;
static volatile uint32_t chunks_left=0;
static volatile uint32_t start_block_index=0;


// definicion de la tabla de punteros a funcion
// que debe corresponderse con fsm_client_state_t
static void (* const state_table[])(void) = {
  idle,
  connected,
  processing_status,
  processing_command,
  processing_fileheader,
  processing_filechunks,
};


fsm_client_state_t fsm_client_state()
{
  return state;
}

void fsm_client_update(void)
{
  // implementada con punteros a funcion
  (*state_table[ state ])();
}



void fsm_client_change(fsm_client_state_t st)
{
  if(st==state) return; //no es un cambio de estado

  //setea el estado nuevo
  state = st;
}



// implementacion de las funciones de los estados


static void idle( void)
{
  message_hdr_t* message;

  if( client_has_message() )
  {
    message = client_get_message();
    if ( message ==NULL)
    {
      lcd_print_at("NULLMSG1",1,0);
    }
    else
    {
      if( message->msg_type== MESSAGE_HANDSHAKE)
      {
        lcd_print_at("HSH",1,0);

        message->is_response = 1;
        client_send_message_response(message, NULL);

        fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
      }
      else
      {
        lcd_print_at("MNEXPT!1",1,0);
      }
    }

    free(message);

  }




}


static void connected( void)
{
  message_hdr_t* message;

  if( client_has_message() )
  {
    message = client_get_message();
    if ( message ==NULL)
    {
      lcd_print_at("NULL2",1,0);
    }
    else
    {
      switch(message->msg_type){
        case MESSAGE_HANDSHAKE:
          // client is checking the connection is still alive
          // just respond, dont change the state
          message->is_response = 1;
          client_send_message_response(message, NULL);
          break;

        case MESSAGE_INFO_STATUS:
          last_request = message;
          fsm_client_change(FSM_CLIENT_STATE_PROCESSING_STATUS);
          break;
        case MESSAGE_COMMAND:
          last_request = message;
          fsm_client_change(FSM_CLIENT_STATE_PROCESSING_COMMAND);
          break;
        case MESSAGE_FILEHEADER:
          last_request = message;
          fsm_client_change(FSM_CLIENT_STATE_PROCESSING_FILEHEADER);
          break;
        default:
          free(message);
          lcd_print_at("NEXPT2",1,0);
          break;
      }

    }
  }



}

static void processing_status( void)
{
  //todo:
  //     *  recuperar espacio disponible
  //     *  recuperar espacio total


  message_hdr_t response;
  status_hdr_t status;
  fileheader_data_t file_headers[STORAGE_FILE_HEADERS_MAX_COUNT];
  int i,j;
  uint8_t data[sizeof(status_hdr_t)+sizeof(file_headers)];
  uint8_t* data_ptr;

  data_ptr = (uint8_t*) &data;

  //storage_disk_status(&status);
  if( status.sd_status!=0)
    status.files_count = 0;

  for(i = 0; i < sizeof(status_hdr_t) ; i++)
    *data_ptr++ = *( ( (uint8_t*) &status ) + i);

  if( storage_get_file_headers(file_headers, status.files_count)!=0)
  {
    status.files_count = 0;
    status.sd_status = 4; //couldnt get files info
  }

  for(i = 0; i < sizeof(fileheader_data_t) * status.files_count ; i++)
    *data_ptr++ = *( ( (uint8_t*) file_headers ) + i);


  response.msg_id = last_request->msg_id;
  response.msg_type = last_request->msg_type;
  response.is_response = 1;
  response.data_length = sizeof(status_hdr_t) + sizeof(fileheader_data_t) * status.files_count ;

  client_send_message_response(&response, (uint8_t*) &data);
  free(last_request);
  fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
}

static void processing_command( void)
{
  lcd_print_at("CMD RX:",1,0);
  lcd_print_char( '0' + *messageData(last_request) );

  switch (*messageData(last_request)) {
 /*   case COMMAND_FORMAT_SD:
      if(storage_format_disk()==0)
        client_send_status_response(last_request, STATUS_OK);
      else
        client_send_status_response(last_request, STATUS_ERROR);

      break;*/
    default:
      client_send_status_response(last_request, STATUS_OK);
      break;
  }

  free(last_request);
  fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
}

static void processing_fileheader( void)
{
  int i;
  fileheader_data_t* file_header;
  file_header = (fileheader_data_t*) messageData(last_request);
  lcd_clear();
  lcd_print_at("F:",0,0);
  for(i=0;i<sizeof(file_header->filename);i++)
    lcd_print_char(file_header->filename[i]);

  lcd_print_at("SR:",1,0);
  lcd_print_int_at(file_header->sample_rate,5,1,15);

  if(file_header->length>1024*1024*100) //no more than 100mb
  {
    client_send_status_response(last_request, STATUS_ERROR);
    free(last_request);
    fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
  }
  else
  {

    if ( storage_save_file_header(file_header)==0 )
    {

      client_send_status_response(last_request, STATUS_OK);
      chunks_count = chunks_left = file_header->chunks_count;
      start_block_index = file_header->block_start;
      fsm_client_change(FSM_CLIENT_STATE_PROCESSING_FILECHUNKS);

    }
    else
    {
      client_send_status_response(last_request, STATUS_ERROR);
      fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
      lcd_print_at("SD NOT READY",1,0);

    }

    free(last_request);
  }

}

static void processing_filechunks( void)
{

  message_hdr_t* message;

  if( client_has_message() )
  {
    message = client_get_message();
    if ( message ==NULL)
    {
      lcd_print_at("NULL4",1,0);
    }
    else
    {
      if( message->msg_type== MESSAGE_FILECHUNK)
      {
        process_chunk(message);
      }
      else
      {
        lcd_print_at("NEXPT4",1,0);
      }

    }

    free(message);

  }


}



static void process_chunk( message_hdr_t* request)
{


  message_hdr_t response;
  filechunk_hdr_t chunk;
  uint8_t* buf;

  chunk.chunk_id = *(uint32_t*) messageData(request);

  response.msg_id = request->msg_id;
  response.msg_type = request->msg_type;
  response.is_response = 1;
  response.data_length = sizeof(chunk);


  buf = messageData(request) + sizeof(chunk.chunk_id);
  if ( sd_card_write(buf, 1, start_block_index + chunk.chunk_id) )
  {
    chunk.status = 0;
    chunks_left--;
    if(chunks_left>0)
    {
      lcd_print_int_at(chunks_left,5,1,15);
      lcd_print_int_at(chunk.chunk_id,5,0,15);
      lcd_print_char_at('%',1,0);
      lcd_print_int_at((chunks_count-chunks_left)*100/chunks_count,3,1,3);
    }
    else
    {
      lcd_clear();
      lcd_print_at("TRANSFER DONE",0,0);
      fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
    }
    client_send_message_response(&response, (uint8_t*)  &chunk);

  }
  else
  {
    chunk.status = 1;
    fsm_client_change(FSM_CLIENT_STATE_IDLE);
    client_send_message_response(&response, (uint8_t*)  &chunk);
    lcd_clear();
    lcd_print_at("TRANSFER ERROR",0,0);
  }






}
