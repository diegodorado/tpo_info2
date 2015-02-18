/*
 * fsm_playback.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "fsm.h"

static volatile fsm_playback_state_t state = FSM_PLAYBACK_STATE_IDLE;
static volatile int prev_tick_state = -1; // used to check first entry to state

//states
static void idle( void);
static void stopped( void);
static void playing( void);
static void paused( void);

static uint8_t status_entered(void);


//actions
static void stop( void);
static void play( void);
static void pause( void);
static void toggle( void);
static void next( void);
static void previous( void);



static void seek(uint8_t index);

static volatile uint8_t buffer[PLAYBACK_BUFFER_LENGTH];
static volatile uint16_t buffer_in=0;
static volatile uint16_t buffer_out=0;
static volatile uint32_t block_index;
static volatile uint8_t file_index=0;
static volatile fileheader_data_t current_file_header;
static uint16_t buffer_data_size();
static uint16_t buffer_free_space();

static volatile uint32_t seconds_left = 0;
static void seconds_left_countdown(void);
static void display_seconds_left(void);

// definicion de la tabla de punteros a funcion
// que debe corresponderse con fsm_playback_state_t
static void (* const state_table[])(void) = {
  idle,
  stopped,
  playing,
  paused,
};

void fsm_playback_init(void){
  systick_delay_async(1000, 1, seconds_left_countdown);
  timer_set_timer1_mr0_interrupt_handler(fsm_playback_sample_rate_tick);
}

void fsm_playback_update(void)
{
  // una llamada a fsm_playback_change puede cambiar el estado
  // por eso se almacena el valor antes
  fsm_playback_state_t prev_st = state;

  //conditions to go idle
  if   ( fsm_storage_state() != FSM_STORAGE_STATE_OK ||
       (fsm_client_state()!= FSM_CLIENT_STATE_IDLE && fsm_client_state()!= FSM_CLIENT_STATE_CONNECTED) ){
    fsm_playback_change(FSM_PLAYBACK_STATE_IDLE);

  }
  else{
    //wake up
    if(state==FSM_PLAYBACK_STATE_IDLE){
      fsm_playback_change(FSM_PLAYBACK_STATE_STOPPED);
      // gain control over keyboard
      keyboard_set_handler(fsm_playback_keyboard_handler);
    }


  }


  // implementada con punteros a funcion
  (*state_table[ state ])();

  // guarda el estado para el siguiente tick
  prev_tick_state = prev_st;
}



void fsm_playback_change(fsm_playback_state_t st)
{
  if(st==state) return; //no es un cambio de estado

  //setea el estado nuevo
  state = st;
}

static uint8_t status_entered(void)
{
  return prev_tick_state != state;
}



fsm_playback_state_t fsm_playback_state()
{
	return state ;
}

// implementacion de las funciones de los estados

static void idle( void){
  file_index = 0;
}


static void stopped( void){
  if(status_entered())
    seek(file_index);
}


static void playing( void)
{

  if( block_index >= current_file_header.block_start + current_file_header.chunks_count )
    next();


  //loads buffer
  if (buffer_free_space()>=(512*8))
  {
    if(sd_card_read(buffer+buffer_in , 8, block_index))
    {
      gpio_set_pin(0,22,!gpio_get_pin(0,22,1));
      block_index += 8;
      buffer_in += (512*8);
      buffer_in %= PLAYBACK_BUFFER_LENGTH;

    }

  }

}

static void paused( void){
  //just wait
}




static void seek(uint8_t index){

  int i;
  fileheader_data_t file_headers[STORAGE_FILE_HEADERS_MAX_COUNT];

  if(index > storage_sd_files_count()-1){
    lcd_print_at("NO FILES",0,0);

    return;
  }

  //clears buffer
  buffer_in = buffer_out = 0;



  storage_get_file_headers(file_headers);

  file_index = index;
  current_file_header = file_headers[file_index];


  lcd_clear();
  //display name
  for(i=0;i<sizeof(current_file_header.filename);i++)
    lcd_print_char_at(current_file_header.filename[i],0,i);

  //display sample rate
  lcd_print_at("SR:",1,8);
  lcd_print_int_at(current_file_header.sample_rate,1,11,5);
  //set sample rate
  audio_set_sample_rate(current_file_header.sample_rate);


  //display file #
  lcd_print_int_at(file_index+1,1,0,3);
  lcd_print_char_at('/',1,3);
  lcd_print_int_at(storage_sd_files_count(),1,4,3);


  seconds_left = (current_file_header.chunks_count*512)/current_file_header.sample_rate;

  block_index = current_file_header.block_start;

  display_seconds_left();

}



//actions
static void stop( void){
  fsm_playback_change(FSM_PLAYBACK_STATE_STOPPED);
}

static void play( void){
  fsm_playback_change(FSM_PLAYBACK_STATE_PLAYING);
}

static void pause( void){
  fsm_playback_change(FSM_PLAYBACK_STATE_PAUSED);
}

static void toggle( void){

  if(fsm_playback_state()==FSM_PLAYBACK_STATE_PLAYING)
    pause();
  else
    play();

}

static void next( void){
  if(file_index <  storage_sd_files_count()-1)
    seek(file_index+1);
  else
    seek(0);
}

static void previous( void){
  if(file_index == 0)
    seek(storage_sd_files_count()-1);
  else
    seek(file_index-1);
}




// callbacks




// keyboard handler
void fsm_playback_keyboard_handler(uint8_t key){

  if(key == 3)
    previous();
  else if(key == 2)
    toggle();
  else if(key == 1)
   stop();
  else if(key == 0)
    next();

}

void fsm_playback_command_handler(command_type_t command){
  switch (command) {
    case COMMAND_STOP:
      stop();
      break;
    case COMMAND_PLAY:
      play();
      break;
    case COMMAND_PAUSE:
      pause();
      break;
    case COMMAND_NEXT:
      next();
      break;
    case COMMAND_PREVIOUS:
      previous();
      break;
  }
}



static void seconds_left_countdown(void){
  if(state==FSM_PLAYBACK_STATE_PLAYING && seconds_left>0){
    seconds_left--;
    display_seconds_left();
  }
}

static void display_seconds_left(void){
  int aux = seconds_left;

  lcd_print_char_at('0' + (aux%10), 0,14);  aux /=10;
  lcd_print_char_at('0' + (aux%6) , 0,13);  aux /=6;
  lcd_print_char_at(':'           , 0,12);
  lcd_print_char_at('0' + (aux%10), 0,11);  aux /=10;
  lcd_print_char_at('0' + (aux%6) , 0,10);

}











static uint16_t buffer_data_size()
{
  return (PLAYBACK_BUFFER_LENGTH + buffer_in - buffer_out)%PLAYBACK_BUFFER_LENGTH;
}


static uint16_t buffer_free_space()
{
  return PLAYBACK_BUFFER_LENGTH - buffer_data_size();
}





void fsm_playback_sample_rate_tick(void){
  uint8_t sample;

  if (state==FSM_PLAYBACK_STATE_PLAYING && buffer_data_size()>0)
  {
    sample = buffer[buffer_out];
    buffer_out++;
    buffer_out %= PLAYBACK_BUFFER_LENGTH;
    dac_set_value(sample*4); // shift volume, since DAC is 10bit, and data is 8bit
  }

}




