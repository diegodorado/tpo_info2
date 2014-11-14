/*
 * fsm_client.c
 *
 *  Created on: Nov 13, 2014
 *      Author: diego
 */


#include "fsm_client.h"


static fsm_client_state_t state = FSM_CLIENT_STATE_IDLE; // estado inicial

// funciones primitivas de los estados
static void idle( void);
static void connected( void);
static void processing_status( void);
static void processing_playback_command( void);
static void processing_fileheader( void);
static void processing_filechunks( void);

//utils
static void process_chunk( message_hdr_t* message);

static message_hdr_t* last_request = NULL;
static uint32_t chunks_count=0;
static uint32_t chunks_left=0;


// definicion de la tabla de punteros a funcion
// que debe corresponderse con fsm_client_state_t
static void (* const state_table[])(void) = {
  idle,
  connected,
  processing_status,
  processing_playback_command,
  processing_fileheader,
  processing_filechunks,
};


void fsm_client_update(void)
{
  // implementada con punteros a funcion
  // cambiando fsm_main_state se cambia el estado de la maquina
  // En este caso, cada estado es una submaquina.
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
      lcd_print_at("ERROR NULL MSG",1,0);
    }
    else
    {
      if( message->msg_type== MESSAGE_HANDSHAKE)
      {
        lcd_print_at("HANDSHAKE!",1,0);
        client_send_status_response(message, STATUS_OK);
        fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
      }
      else
      {
        lcd_print_at("MSG NOT EXPT!",1,0);
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
      lcd_print_at("ERROR NULL MSG",1,0);
    }
    else
    {
      switch(message->msg_type){
        case MESSAGE_INFO_STATUS:
          last_request = message;
          fsm_client_change(FSM_CLIENT_STATE_PROCESSING_STATUS);
          break;
        case MESSAGE_PLAYBACK_COMMAND:
          last_request = message;
          fsm_client_change(FSM_CLIENT_STATE_PROCESSING_PLAYBACK_COMMAND);
          break;
        case MESSAGE_FILEHEADER:
          last_request = message;
          fsm_client_change(FSM_CLIENT_STATE_PROCESSING_FILEHEADER);
          break;
        default:
          free(message);
          lcd_print_at("MSG NOT EXPT!",1,0);
          break;
      }

    }
  }



}

static void processing_status( void)
{
  //todo:
  //     *  recuperar estado SD
  //     *  recuperar espacio disponible
  //     *  recuperar espacio total
  //     *  recuperar lista de audio
  //     *  componer mensaje de respuesta
  //


  message_hdr_t response;
  status_hdr_t status;
  fileheader_data_t files[4];
  int i,j;
  uint8_t data[sizeof(status_hdr_t)+sizeof(files)];
  uint8_t* data_ptr;

  data_ptr = (uint8_t*) &data;

  status.sd_connected = 1;
  status.total_space = 150;
  status.available_space = 75;
  status.files_count = 4;


  for(i = 0; i < sizeof(status_hdr_t) ; i++)
    *(data_ptr++) = *( ( (uint8_t*) &status ) + i);


  for(i = 0; i<status.files_count;i++)
  {
    files[i].filesize = 1000;
    files[i].chunks_count = 100;
    files[i].filename[0] = 'A';
    files[i].filename[1] = 'u';
    files[i].filename[2] = 'd';
    files[i].filename[3] = 'i';
    files[i].filename[4] = 'o';
    files[i].filename[5] = '_';
    files[i].filename[6] = '0';
    files[i].filename[7] = '0' + i;

    for(j = 0; j < sizeof(fileheader_data_t) ; j++)
      *(data_ptr++) = *( ( (uint8_t*) &files[i] ) + j);

  }


  response.msg_id = last_request->msg_id;
  response.msg_type = last_request->msg_type;
  response.is_response = 1;
  response.data_length = sizeof(data);

  client_send_message_response(&response, (uint8_t*) &data);
  free(last_request);
  fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
}

static void processing_playback_command( void)
{
  lcd_print_at("CMD RX:",1,0);
  lcd_print_char( '0' + *messageData(last_request) );
  client_send_status_response(last_request, STATUS_OK);
  free(last_request);
  fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
}

static void processing_fileheader( void)
{
  int i;
  fileheader_data_t* header;
  header = (fileheader_data_t*) messageData(last_request);
  lcd_print_at("F.Req: ",0,0);
  for(i=0;i<sizeof(header->filename);i++)
    lcd_print_char(header->filename[i]);

  lcd_print_at("Chunks: ",1,0);
  lcd_print_int_at(header->chunks_count,8,1,15);

  if(header->chunks_count>700)
  {
    client_send_status_response(last_request, STATUS_ERROR);
    free(last_request);
    fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
  }
  else
  {
    client_send_status_response(last_request, STATUS_OK);
    chunks_count = chunks_left = header->chunks_count;
    free(last_request);
    fsm_client_change(FSM_CLIENT_STATE_PROCESSING_FILECHUNKS);
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
      lcd_print_at("ERROR NULL MSG",1,0);
    }
    else
    {
      if( message->msg_type== MESSAGE_FILECHUNK)
      {
        lcd_print_at("pCH-",0,0);

        process_chunk(message);
      }
      else
      {
        lcd_print_at("MSG NOT EXPT!",1,0);
      }

    }

    free(message);

  }


}



static void process_chunk( message_hdr_t* request)
{
  message_hdr_t response;
  filechunk_hdr_t chunk;

  chunk.status = 0;
  chunk.chunk_id = *(uint32_t*) messageData(request);

  response.msg_id = request->msg_id;
  response.msg_type = request->msg_type;
  response.is_response = 1;
  response.data_length = sizeof(chunk);

  client_send_message_response(&response, (uint8_t*)  &chunk);

  lcd_print_int_at(chunk.chunk_id,5,0,15);

  lcd_print_char_at('%',1,0);
  chunks_left--;
  lcd_print_int_at((chunks_count-chunks_left)*100/chunks_count,3,1,3);

  if(chunks_left<500)
  {
    lcd_print_at("DONE!",1,0);
    fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
  }



}
