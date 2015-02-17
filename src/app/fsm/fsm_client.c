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
static void processing_file( void);
static void saving_headers(void);
static void timeout( void);


static uint8_t status_entered(void);
static volatile int prev_tick_state = -1; // used to check first entry to state


//utils
static volatile uint8_t timeout_counter = 0;
static volatile uint8_t timeout_started = 0;
static volatile uint8_t timeout_feedback = 0;
static void check_timeout( void);
static void timeout_countdown( void);
static void timeout_reset( void);

static volatile uint8_t saving_headers_feedback = 0;
static void saving_headers_callback( void);


static message_hdr_t* fetch_message(void);
static void process_handshake( message_hdr_t* message);
static void process_status( message_hdr_t* message);
static void process_command( message_hdr_t* message);
static void process_fileheader( message_hdr_t* message);
static void process_filechunk( message_hdr_t* message);

static volatile fileheader_data_t file_header;


// definicion de la tabla de punteros a funcion
// que debe corresponderse con fsm_client_state_t
static void (* const state_table[])(void) = {
  idle,
  connected,
  processing_file,
  saving_headers,
  timeout,
};


fsm_client_state_t fsm_client_state()
{
  return state;
}

void fsm_client_update(void)
{
  // una llamada a fsm_client_change puede cambiar el estado
  // por eso se almacena el valor antes
  fsm_client_state_t prev_st = state;

  check_timeout();
  // implementada con punteros a funcion
  (*state_table[ state ])();

  // guarda el estado para el siguiente tick
  prev_tick_state = prev_st;


}



void fsm_client_change(fsm_client_state_t st)
{
  if(st==state) return; //no es un cambio de estado

  //setea el estado nuevo
  state = st;
}

static uint8_t status_entered(void)
{
  return prev_tick_state != state;
}


// implementacion de las funciones de los estados


static void idle( void)
{
  message_hdr_t* message = fetch_message();

  if(status_entered())
    device_rgb_set(DEVICE_RGB_NONE);

  if( message != NULL )
  {
      if( message->msg_type== MESSAGE_HANDSHAKE){
        process_handshake(message);
        fsm_client_change(FSM_CLIENT_STATE_CONNECTED);
      }
      free(message);
  }

}


static void connected( void)
{
  message_hdr_t* message = fetch_message();

  if(status_entered())
    device_rgb_set(DEVICE_RGB_GREEN);


  if( message != NULL )
  {
      switch(message->msg_type){
        case MESSAGE_HANDSHAKE:
          // client is checking the connection is still alive
          process_handshake(message);
          break;
        case MESSAGE_INFO_STATUS:
          process_status(message);
          break;
        case MESSAGE_COMMAND:
          process_command(message);
          break;
        case MESSAGE_FILEHEADER:
          process_fileheader(message);
          break;
        default:
          lcd_print_at("NEXPT2",1,0);
          break;
      }
      free(message);

    }

}


static void processing_file( void)
{
  message_hdr_t* message = fetch_message();

  if(status_entered())
    device_rgb_set(DEVICE_RGB_BLUE);


  if( message != NULL )
  {
      if( message->msg_type== MESSAGE_FILECHUNK)
        process_filechunk(message);

      free(message);
  }

}


static void saving_headers(void){

  if(status_entered())
    device_rgb_set(DEVICE_RGB_RED);


  //start
  if(saving_headers_feedback==0){
    saving_headers_feedback = 1;
    lcd_clear();
    lcd_print_at("TRANSFER DONE", 0, 0);
    lcd_print_at("SAVING...", 1, 0);

    storage_save_file_header(file_header);

    //provide visual feedback
    systick_delay_async(3000, 0,saving_headers_callback);
    return;
  }

  //wait...
  if(saving_headers_feedback==1)
   return;

  //finish
  if(saving_headers_feedback==2){
    lcd_clear();
    fsm_client_change(FSM_CLIENT_STATE_IDLE);
    saving_headers_feedback = 0;
    return;
  }


}



static void timeout( void){

  if(status_entered())
    device_rgb_set(DEVICE_RGB_RED);

  //start
  if(timeout_feedback==0){
    lcd_clear();
    lcd_print_at("CONNECTION", 0, 0);
    lcd_print_at("TIMEOUT", 1, 0);

    timeout_feedback = 1;
    //provide visual feedback
    systick_delay_async(3000, 0,timeout_reset);
    return;
  }

  //wait...
  if(timeout_feedback==1)
   return;

  //finish
  if(timeout_feedback==2){
    lcd_clear();
    fsm_client_change(FSM_CLIENT_STATE_IDLE);
    timeout_feedback = 0;
    return;
  }


}

static message_hdr_t* fetch_message(void){

  if( client_has_message() )
  {
    timeout_counter = FSM_CLIENT_TIMEOUT;
    return client_get_message();
  }

  return NULL;

}

static void process_handshake( message_hdr_t* message)
{
  client_send_message_response(message, NULL);
}


static void process_status( message_hdr_t* message)
{

  status_hdr_t status;
  fileheader_data_t file_headers[STORAGE_FILE_HEADERS_MAX_COUNT];
  int i,j;
  uint8_t data[ sizeof(status_hdr_t) + 8 * STORAGE_FILE_HEADERS_MAX_COUNT];
  uint8_t* data_ptr;

  data_ptr = (uint8_t*) &data;
  status = storage_status();

  for(i = 0; i < sizeof(status_hdr_t) ; i++)
    *data_ptr++ = *( ( (uint8_t*) &status ) + i);

  storage_get_file_headers(file_headers);

  for(i = 0; i < status.files_count ; i++)
    for(j = 0; j < 8 ; j++)
      *data_ptr++ = file_headers[i].filename[j];

  message->data_length = sizeof(status_hdr_t) + 8 * status.files_count ;

  client_send_message_response(message, (uint8_t*) &data);
}

static void process_command( message_hdr_t* message)
{
  fileheader_data_t file_header;

  file_header.filename[0] = 'A';
  file_header.filename[1] = 'B';
  file_header.filename[2] = 'C';
  file_header.filename[3] = 'D';
  file_header.filename[4] = 'e';
  file_header.filename[5] = 'f';
  file_header.filename[6] = 'g';
  file_header.filename[7] = 'h';
  file_header.chunks_count = 40;

  storage_save_file_header(file_header);
  lcd_print_at("CMD RX:",1,0);
  lcd_print_char( '0' + *messageData(message) );
  client_send_status_response(message, STATUS_OK);
  fsm_client_change(FSM_CLIENT_STATE_IDLE);

}




static void process_fileheader( message_hdr_t* message){
  int i;
  file_header = *(fileheader_data_t*) messageData(message);
  lcd_clear();
  lcd_print_at("F:",0,0);
  for(i=0;i<sizeof(file_header.filename);i++)
    lcd_print_char(file_header.filename[i]);

  lcd_print_at("SR:",1,0);
  lcd_print_int_at(file_header.sample_rate,5,1,15);

  if(file_header.length<1024*1024*100) //less than 100mb
  {
    client_send_status_response(message, STATUS_OK);
    fsm_client_change(FSM_CLIENT_STATE_PROCESSING_FILE);
  }
  else
    client_send_status_response(message, STATUS_ERROR);

}

static void process_filechunk( message_hdr_t* message)
{
  filechunk_hdr_t chunk;
  uint8_t* buffer;

  chunk.status = 0; //no error
  chunk.chunk_id = *(uint32_t*) messageData(message);

  message->data_length = sizeof(chunk);


  buffer = messageData(message) + sizeof(chunk.chunk_id);
  if ( sd_card_write(buffer, 1, file_header.block_start + chunk.chunk_id) )
  {
    if(chunk.chunk_id == file_header.chunks_count - 1)
      fsm_client_change(FSM_CLIENT_STATE_SAVING_HEADERS);
    else
    {
      //lcd_print_int_at((file_header.chunks_count-chunk.chunk_id),5,1,15);
      lcd_print_int_at((chunk.chunk_id*100),5,1,15);

      //lcd_print_int_at(chunk.chunk_id,5,0,15);
      lcd_print_int_at((file_header.chunks_count-1),5,0,15);
      lcd_print_at("        ",0,0);
      lcd_print_int_at(chunk.chunk_id,3,0,7);
      lcd_print_char_at('%',1,0);
      lcd_print_int_at((chunk.chunk_id*100)/(file_header.chunks_count-1),3,1,3);
    }

    client_send_message_response(message, (uint8_t*)  &chunk);

  }
  else
  {
    chunk.status = 1;
    client_send_message_response(message, (uint8_t*)  &chunk);
  }




}



static void saving_headers_callback( void){
  saving_headers_feedback = 2;
}


// timout handling

static void check_timeout( void){
  //start countdown only once
  if(!timeout_started){
    systick_delay_async(1000, 1, timeout_countdown);
    timeout_started = 1;
  }

  if(state!=FSM_CLIENT_STATE_IDLE && timeout_counter<=0)
    fsm_client_change(FSM_CLIENT_STATE_TIMEOUT);

}

static void timeout_countdown( void){
  timeout_counter--;
}

static void timeout_reset( void){
  timeout_feedback = 2;
}

