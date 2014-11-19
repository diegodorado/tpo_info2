/*
 * fsn_playback.h
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#ifndef FSM_PLAYBACK_H_
#define FSM_PLAYBACK_H_

//include drivers
#include "drivers.h"

typedef enum {
  FSM_PLAYBACK_STATE_IDLE,
  FSM_PLAYBACK_STATE_PLAYING,
  FSM_PLAYBACK_STATE_PLAY,
	 FSM_PLAYBACK_STATE_PAUSE,
	 FSM_PLAYBACK_STATE_STOP,
	 FSM_PLAYBACK_STATE_NEXT,
	 FSM_PLAYBACK_STATE_PREVIOUS,
} fsm_playback_state_t;

void fsm_playback_update(void);
void fsm_playback_change(fsm_playback_state_t st);
fsm_playback_state_t fsm_playback_state();


#endif /* FSM_PLAYBACK_H_ */
