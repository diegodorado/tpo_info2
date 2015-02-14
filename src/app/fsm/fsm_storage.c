/*
 * fsm_storage.c
 *
 *  Created on: Feb 13, 2015
 *      Author: diego
 */


#include "fsm_storage.h"

static void check_card_detected( void);
static uint8_t status_entered(void);
static volatile uint8_t fetching_sd_status = 0;
static void fetching_sd_status_callback(void);
static volatile uint8_t formatting_sd = 0;
static void formatting_sd_callback(void);
static void check_card_detected( void);

static volatile int prev_tick_state = -1; // used to check first entry to state
static volatile fsm_storage_state_t state = FSM_STORAGE_STATE_BOOTING;

static void booting( void);
static void ok( void);
static void not_detected( void);
static void wrong_format( void);
static void formating( void);
static void error( void);

// definicion de la tabla de punteros a funcion
// que debe corresponderse con fsm_storage_state_t
static void (* const state_table[])(void) = {
  booting,
  ok,
  not_detected,
  wrong_format,
  formating,
  error,
};

void fsm_storage_update(void)
{
  // una llamada a fsm_storage_change puede cambiar el estado
  // por eso se almacena el valor antes
  fsm_storage_state_t prev_st = state;

  check_card_detected(); // parallel check
  // implementada con punteros a funcion
  (*state_table[ state ])();

  // guarda el estado para el siguiente tick
  prev_tick_state = prev_st;

}

void fsm_storage_change(fsm_storage_state_t st)
{
  if(st==state) return; //no es un cambio de estado
  //setea el estado nuevo
  state = st;
}

fsm_storage_state_t fsm_storage_state()
{
 return state ;
}


// utilities
static void check_card_detected( void){
  if(!storage_card_detected())
    fsm_storage_change(FSM_STORAGE_STATE_NOT_DETECTED);
}
static uint8_t status_entered(void)
{
  return prev_tick_state != state;
}


// implementacion de las funciones de los estados

static void booting( void){

  //start
  if(fetching_sd_status==0){
    lcd_clear();
    lcd_print_at("BOOTING...", 0, 0);

    fetching_sd_status = 1;
    systick_delay_async(1500, 0,fetching_sd_status_callback);
    storage_boot_sd();
    return;
  }

  //wait...
  if(fetching_sd_status==1)
   return;

  //finish
  if(fetching_sd_status==2){

    switch (storage_sd_status()) {
      case SD_STATUS_OK:
        fsm_storage_change(FSM_STORAGE_STATE_OK);
        break;
      case SD_STATUS_WRONG_FORMAT:
        fsm_storage_change(FSM_STORAGE_STATE_WRONG_FORMAT);
        break;
      case SD_STATUS_SETUP_FAILURE:
      case SD_STATUS_READ_FAILURE:
      case SD_STATUS_WRITE_FAILURE:
      case SD_STATUS_INVALID_SIZE:
        fsm_storage_change(FSM_STORAGE_STATE_ERROR);
        break;
      default:
        break;
    }
    fetching_sd_status = 0;
    return;
  }




}





static void ok( void){
  //do nothing
  if(status_entered()){
    lcd_clear();
    lcd_print_at("SD OK", 0, 0);
  }
}

static void not_detected( void){
  // print only once
  if(status_entered()){
    lcd_clear();
    lcd_print_at("INSERTE SD", 0, 0);
  }

  if ( storage_card_detected())
    fsm_storage_change(FSM_STORAGE_STATE_BOOTING);
}

static void wrong_format( void) {
  // print only once
  if(status_entered()){
    lcd_clear();
    lcd_print_at("SD WRONG FORMAT", 0, 0);
    lcd_print_at("* PLAY TO FORMAT", 1, 0);
  }


}

static void formating( void){

  //start
  if(formatting_sd==0){
    lcd_clear();
    lcd_print_at("FORMATING...", 0, 0);

    formatting_sd = 1;
    //format is not long...but lets provide visual feedback
    systick_delay_async(2000, 0,formatting_sd_callback);
    storage_format_sd();
    return;
  }

  //wait...
  if(formatting_sd==1)
   return;

  //finish
  if(formatting_sd==2){
    fsm_storage_change(FSM_STORAGE_STATE_BOOTING);
    formatting_sd = 0;
    return;
  }


}

static void error( void){
  // print only once
  if(status_entered()){
    lcd_clear();
    switch (storage_sd_status()) {
      case SD_STATUS_SETUP_FAILURE:
        lcd_print_at("SD SETUP FAILURE", 0, 0);
        break;
      case SD_STATUS_READ_FAILURE:
        lcd_print_at("SD READ FAILURE", 0, 0);
        break;
      case SD_STATUS_WRITE_FAILURE:
        lcd_print_at("SD WRITE FAILURE", 0, 0);
        break;
      case SD_STATUS_INVALID_SIZE:
        lcd_print_at("SD INVALID SIZE", 0, 0);
        break;
      default:
        break;
    }
  }

}


// callbacks

static void fetching_sd_status_callback(void){
  fetching_sd_status = 2;
}

static void formatting_sd_callback(void){
  formatting_sd = 2; // finished
}

// keyboard handler
void fsm_storage_keyboard_handler(uint8_t key){
  if(state==FSM_STORAGE_STATE_WRONG_FORMAT && key==2)
    fsm_storage_change(FSM_STORAGE_STATE_FORMATING);
}

