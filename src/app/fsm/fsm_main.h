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


typedef enum {
  FSM_MAIN_STATE_IDLE,
  FSM_MAIN_STATE_IDLE_UPDATE,
  FSM_MAIN_STATE_PLAYBACK,
  FSM_MAIN_STATE_PLAYBACK_UPDATE,
  FSM_MAIN_STATE_WRITE,
  FSM_MAIN_STATE_WRITE_UPDATE
} fsm_main_state_t;

extern fsm_main_state_t fsm_main_state;

void fsm_main(void);


void fsm_main_idle( void);
void fsm_main_idle_update( void);
void fsm_main_playback( void);
void fsm_main_playback_update( void);
void fsm_main_write( void);
void fsm_main_write_update( void);




#endif /* FSM_MAIN_H_ */
