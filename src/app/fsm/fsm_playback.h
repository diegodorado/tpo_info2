/*
 * fsn_playback.h
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#ifndef FSM_PLAYBACK_H_
#define FSM_PLAYBACK_H_


typedef enum { FSM_PLAYBACK_STATE_PLAY, FSM_PLAYBACK_STATE_PAUSE, FSM_PLAYBACK_STATE_STOP } fsm_playback_state_t;

extern fsm_playback_state_t fsm_playback_state;

void fsm_playback(void);


void fsm_playback_play( void);
void fsm_playback_pause( void);
void fsm_playback_stop( void);



#endif /* FSM_PLAYBACK_H_ */
