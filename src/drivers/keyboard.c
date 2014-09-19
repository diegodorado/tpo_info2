/*
 * keyboard.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "keyboard.h"
#include "fsm.h"

void keyboard_setup(void){
  //todo: implementar la inicializacion

  // Configuro el SW1 para que trabaje como EINT0
  // Esto debe hacerse con la interrupcion deshabilitada
  set_pin_sel(EINT0_PIN, 1);
  // Configuro la interrupcion externa 0 por flanco
  EXTMODE0_F;
  // Habilito Interrupcion Externa 0
  ISE_EINT0;


  // Configuro el SW4 para que trabaje como EINT3
  // Esto debe hacerse con la interrupcion deshabilitada
  set_pin_sel(EINT3_PIN, 1);
  // Configuro la interrupcion externa 0 por flanco
  EXTMODE0_F;
  // Habilito Interrupcion Externa 0
  ISE_EINT3;

}

void keyboard_handle_key(uint8_t key){
  if(key == 1)
    fsm_main_change(FSM_MAIN_STATE_PLAYBACK);
  else if(key == 4)
    fsm_main_change( FSM_MAIN_STATE_WRITE);
}
