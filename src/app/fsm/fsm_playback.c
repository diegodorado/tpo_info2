/*
 * fsm_playback.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#include "fsm_playback.h"

fsm_playback_state_t fsm_playback_state = FSM_PLAYBACK_STATE_PLAY;

// definicion de la tabla de punteros a funcion
void (* const fsm_playback_state_table[])(void) = {
  fsm_playback_play,
  fsm_playback_pause,
  fsm_playback_stop
};

void fsm_playback(void)
{
  // Implementada con punteros a funcion
  fsm_playback_state_table[ fsm_playback_state ]();
}


void fsm_playback_play( void){
  //todo: implementar estado
}

void fsm_playback_pause( void){
  //todo: implementar estado
}

void fsm_playback_stop( void){
  //todo: implementar estado
}

