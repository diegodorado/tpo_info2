/*
 * fsm_write.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */


#include "fsm_write.h"

fsm_write_state_t fsm_write_state = FSM_WRITE_STATE_IDLE;

// definicion de la tabla de punteros a funcion
void (* const fsm_write_state_table[])(void) = {
  fsm_write_idle,
  fsm_write_write
};

void fsm_write(void)
{
  // Implementada con punteros a funcion
  fsm_write_state_table[ fsm_write_state ]();
}


void fsm_write_idle( void){
  //todo: implementar estado
  // estado de espera de recepcion de nuevos
  // bloques de datos para grabar en la SD
}

void fsm_write_write( void){
  //todo: implementar estado
  // aqui se debe grabar el bloque de datos
  // recibidos via UART
}
