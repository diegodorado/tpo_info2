/*
 * fsm_write.h
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */

#ifndef FSM_WRITE_H_
#define FSM_WRITE_H_


typedef enum { FSM_WRITE_STATE_IDLE, FSM_WRITE_STATE_WRITE } fsm_write_state_t;

extern fsm_write_state_t fsm_write_state;

void fsm_write(void);


void fsm_write_idle( void);
void fsm_write_write( void);


#endif /* FSM_WRITE_H_ */
