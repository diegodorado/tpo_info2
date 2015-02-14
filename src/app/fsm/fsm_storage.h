/*
 * fsm_storage.h
 *
 *  Created on: Feb 13, 2015
 *      Author: diego
 */

#ifndef FSM_STORAGE_H_
#define FSM_STORAGE_H_

#include "drivers.h"


typedef enum {
  FSM_STORAGE_STATE_BOOTING,
  FSM_STORAGE_STATE_OK,
  FSM_STORAGE_STATE_NOT_DETECTED,
  FSM_STORAGE_STATE_WRONG_FORMAT,
  FSM_STORAGE_STATE_FORMATING,
  FSM_STORAGE_STATE_ERROR,
} fsm_storage_state_t;

void fsm_storage_update(void);
void fsm_storage_change(fsm_storage_state_t st);
fsm_storage_state_t fsm_storage_state();
void fsm_storage_keyboard_handler(uint8_t key);

#endif /* FSM_STORAGE_H_ */
