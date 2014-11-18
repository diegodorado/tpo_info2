/*
 * fsm_lcd.h
 *
 *  Created on: Nov 16, 2014
 *      Author: diego
 */

#ifndef FSM_LCD_H_
#define FSM_LCD_H_


//include drivers
#include "drivers.h"

typedef enum {
  FSM_LCD_STATE_IDLE,
  FSM_LCD_STATE_REFRESHING,
} fsm_lcd_state_t;


void fsm_lcd_update(void);
void fsm_lcd_change(fsm_lcd_state_t);


#endif /* FSM_LCD_H_ */
