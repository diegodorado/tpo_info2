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

#define FSM_CLIENT_TIMEOUT 5 //seconds


typedef enum {
  FSM_CLIENT_STATE_IDLE,
  FSM_CLIENT_STATE_CONNECTED,
  FSM_CLIENT_STATE_PROCESSING_FILE,
  FSM_CLIENT_STATE_SAVING_HEADERS,
  FSM_CLIENT_STATE_TIMEOUT,
} fsm_client_state_t;

void fsm_client_init(void);
void fsm_client_update(void);
void fsm_client_change(fsm_client_state_t);
fsm_client_state_t fsm_client_state();

#endif /* FSM_CLIENT_H_ */
