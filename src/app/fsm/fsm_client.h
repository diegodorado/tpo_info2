/*
 * fsm_client.h
 *
 *  Created on: Nov 13, 2014
 *      Author: diego
 */

#ifndef FSM_CLIENT_H_
#define FSM_CLIENT_H_

//include drivers
#include "drivers.h"

typedef enum {
  FSM_CLIENT_STATE_IDLE,
  FSM_CLIENT_STATE_CONNECTED,
  FSM_CLIENT_STATE_PROCESSING_STATUS,
  FSM_CLIENT_STATE_PROCESSING_PLAYBACK_COMMAND,
  FSM_CLIENT_STATE_PROCESSING_FILEHEADER,
  FSM_CLIENT_STATE_PROCESSING_FILECHUNKS,
} fsm_client_state_t;

void fsm_client_update(void);
void fsm_client_change(fsm_client_state_t);

#endif /* FSM_CLIENT_H_ */
