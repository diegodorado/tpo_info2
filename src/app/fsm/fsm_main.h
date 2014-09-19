/*
 * fsm_main.h
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#ifndef FSM_MAIN_H_
#define FSM_MAIN_H_

// include submachines
#include "fsm_playback.h"
#include "fsm_write.h"

//include drivers
#include "drivers.h"

//se exponen los estados posibles
//una funcion que ejecuta el update
//y otra para cambiar de estado de la maquina

#define ON_ENTER 0
#define ON_UPDATE 1
#define ON_EXIT 2

typedef enum {
  FSM_MAIN_STATE_IDLE,
  FSM_MAIN_STATE_PLAYBACK,
  FSM_MAIN_STATE_WRITE,
} fsm_main_state_t;

void fsm_main(void);
void fsm_main_change(fsm_main_state_t state);

#endif /* FSM_MAIN_H_ */
