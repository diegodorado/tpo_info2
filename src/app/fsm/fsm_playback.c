/*
 * fsm_playback.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "fsm_playback.h"

fsm_playback_state_t state = FSM_PLAYBACK_STATE_PLAY;




static void play( void);
static void pause( void);
static void stop( void);
static void next( void);
static void previous( void);



// definicion de la tabla de punteros a funcion
// que debe corresponderse con fsm_client_state_t
static void (* const state_table[])(void) = {
  play,
  pause,
  stop,
  next,
  previous,
};


void fsm_playback_update(void)
{
  // implementada con punteros a funcion
  // cambiando fsm_main_state se cambia el estado de la maquina
  // En este caso, cada estado es una submaquina.
  (*state_table[ state ])();
}



void fsm_playback_change(fsm_playback_state_t st)
{
  if(st==state) return; //no es un cambio de estado

  //setea el estado nuevo
  state = st;
}

fsm_playback_state_t fsm_playback_state()
{
	return state ;
}


// implementacion de las funciones de los estados


static void play( void){
  //todo: implementar estado
  lcd_print_at("play",1,0);

}

static void pause( void){
  //todo: implementar estado
	  lcd_print_at("pause",1,0);
}

static void stop( void){
  //todo: implementar estado
	  lcd_print_at("stop",1,0);

}

static void next( void){
  //todo: implementar estado
	  lcd_print_at("next",1,0);

}

static void previous( void){
  //todo: implementar estado
	  lcd_print_at("previous",1,0);

}



