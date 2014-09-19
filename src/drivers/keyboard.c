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


  // EINT0
  set_pin_sel(EINT0_PIN, 1); // Configuro el P2[10] SW1 para que trabaje como EINT0 esto debe hacerse con
                                       // la Interr deshabilitada Pag 25
   EXTMODE0_F;                         // Configuro la interrupcion externa 0 por flanco
                                          //EXTPOLAR0_P; // si quiero por flanco positivo debo cambiarlo en EXTPOLAR
   ISE_EINT0;                        // Habilito Interrupcion Externa 0


}

void keyboard_handle_key(uint8_t key){
  if(fsm_main_state == FSM_MAIN_STATE_IDLE){
    fsm_main_state = FSM_MAIN_STATE_PLAYBACK;
  }else if(fsm_main_state == FSM_MAIN_STATE_PLAYBACK){
    fsm_main_state = FSM_MAIN_STATE_WRITE;
  }else if(fsm_main_state == FSM_MAIN_STATE_WRITE){
    fsm_main_state = FSM_MAIN_STATE_IDLE;
  }
}
