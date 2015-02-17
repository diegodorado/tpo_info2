#include "setup/setup.h"
#include "fsm.h"


int main(void)
{
  volatile uint8_t kb_storage = 0;
  //inicializacion del equipo
  setup();

 	while(1){
 	  lcd_refresh(); // otro formato... pero es una FSM (con corutinas)
    fsm_storage_update();
    if ( fsm_storage_state() == FSM_STORAGE_STATE_OK){
      fsm_client_update();
      if(fsm_client_state()== FSM_CLIENT_STATE_IDLE || fsm_client_state()== FSM_CLIENT_STATE_CONNECTED){

        fsm_playback_update();

        // set keyboard handler
        if (kb_storage){
          keyboard_set_handler(fsm_playback_keyboard_handler);
          kb_storage = 0;
        }

      }
    }else{
      // set keyboard handler
      if (!kb_storage){
        keyboard_set_handler(fsm_storage_keyboard_handler);
        kb_storage = 1;
      }
    }




 	}

	 //esta devolucion es para satisfacer al compilador, aunque nunca suceda
 	return 1;
}
