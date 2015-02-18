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
  FSM_PLAYBACK_STATE_STOPPED,
  FSM_PLAYBACK_STATE_PLAYING,
  FSM_PLAYBACK_STATE_PAUSED,
} fsm_playback_state_t;

void fsm_playback_init(void);
void fsm_playback_update(void);
void fsm_playback_change(fsm_playback_state_t st);
fsm_playback_state_t fsm_playback_state();
void fsm_playback_keyboard_handler(uint8_t key);
void fsm_playback_command_handler(command_type_t command);
void fsm_playback_sample_rate_tick(void);

#endif /* FSM_PLAYBACK_H_ */
