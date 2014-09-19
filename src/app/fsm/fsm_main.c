#include "fsm_main.h"


fsm_main_state_t fsm_main_state = FSM_MAIN_STATE_IDLE;

// definicion de la tabla de punteros a funcion
// que debe corresponderse con fsm_main_state_t
void (* const fsm_main_state_table[])(void) = {
  fsm_main_idle,
  fsm_main_playback,
  fsm_main_write
};


void fsm_main(void)
{
  // es la maquina de estados principal
  // y por lo tanto toma el control del loop principal
  // sin retornar nunca
  while ( 1 ) {
    // implementada con punteros a funcion
    // cambiando fsm_main_state se cambia el estado de la maquina
    // En este caso, cada estado es una submaquina.
    fsm_main_state_table[ fsm_main_state ]();
  }
}


void fsm_main_idle( void)
{
  // estado de reposo...
}

// NOTA:
// Momentaneamente aceptamos la cascada de llamadas
// en favor de la modularizacion del codigo
// En una futura version podremos unificar las maquinas
// en una misma tabla de punteros a funcion


void fsm_main_playback( void)
{
  // se llama a la submaquina fsm_playback
  fsm_playback();
}

void fsm_main_write( void){
  // se llama a la submaquina fsm_write
  fsm_write();
}
