/*
 * keyboard.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "keyboard.h"
#include "fsm.h"


static void keyboard_refresh(void);


void keyboard_setup(void){

#ifdef USE_SW1_WITH_INTERRUPTS
  // Configuro el SW1 para que trabaje como EINT0
  // Esto debe hacerse con la interrupcion deshabilitada
  set_pin_sel(EINT0_PIN, 1);
  // Configuro la interrupcion externa 0 por flanco
  EXTMODE0_F;
  // Habilito Interrupcion Externa 0
  ISE_EINT0;
#endif

#ifdef USE_SW4_WITH_INTERRUPTS
  // Configuro el SW4 para que trabaje como EINT3
  // Esto debe hacerse con la interrupcion deshabilitada
  set_pin_sel(EINT3_PIN, 1);
  // Configuro la interrupcion externa 0 por flanco
  EXTMODE0_F;
  // Habilito Interrupcion Externa 0
  ISE_EINT3;
#endif

#ifdef USE_SW1_WITH_POLLING
  set_pin_sel( KEY_PIN(0),0);
  gpio_set_dir(KEY_PIN(0),0);
#endif
#ifdef USE_SW2_WITH_POLLING
  set_pin_sel( KEY_PIN(1),0);
  gpio_set_dir(KEY_PIN(1),0);
#endif
#ifdef USE_SW3_WITH_POLLING
  set_pin_sel( KEY_PIN(2),0);
  gpio_set_dir(KEY_PIN(2),0);
#endif
#ifdef USE_SW4_WITH_POLLING
  set_pin_sel( KEY_PIN(3),0);
  gpio_set_dir(KEY_PIN(3),0);
#endif


#ifdef USE_SW_WITH_POLLING
  systick_delay_async(20, 1,keyboard_refresh);
#endif

}


#ifdef USE_SW_WITH_POLLING
static void keyboard_refresh(void){

  static char check[4] = {1,1,1,1};

#ifdef USE_SW1_WITH_POLLING
  if(gpio_get_pin(KEY_PIN(0),0)){
   if (check[0]){
      keyboard_handle_key(0);
      check[0] = 0;
    }
  }
  else{
    check[0] = 1;
  }
#endif

#ifdef USE_SW2_WITH_POLLING
  if(gpio_get_pin(KEY_PIN(1),0)){
   if (check[1]){
      keyboard_handle_key(1);
      check[1] = 0;
    }
  }
  else{
    check[1] = 1;
  }
#endif

#ifdef USE_SW3_WITH_POLLING
  if(gpio_get_pin(KEY_PIN(2),0)){
   if (check[2]){
      keyboard_handle_key(2);
      check[2] = 0;
    }
  }
  else{
    check[2] = 1;
  }
#endif

#ifdef USE_SW4_WITH_POLLING

  if(gpio_get_pin(KEY_PIN(3),0)){
   if (check[3]){
      keyboard_handle_key(3);
      check[3] = 0;
    }
  }
  else{
    check[3] = 1;
  }

#endif


}
#endif  /* USE_SW_WITH_POLLING */


void keyboard_handle_key(uint8_t key){

  if(key == 3){
    fsm_playback_change(FSM_PLAYBACK_STATE_PREVIOUS);
  }
  else if(key == 2){
	  if(fsm_playback_state()==FSM_PLAYBACK_STATE_PLAYING)
	  {
		  fsm_playback_change(FSM_PLAYBACK_STATE_PAUSE);
	  }
	  else
	  {
		  fsm_playback_change(FSM_PLAYBACK_STATE_PLAY);
	  }
  }
  else if(key == 1){

	  fsm_playback_change(FSM_PLAYBACK_STATE_STOP);
  }
  else if(key == 0){
		fsm_playback_change(FSM_PLAYBACK_STATE_NEXT);
  }
}
