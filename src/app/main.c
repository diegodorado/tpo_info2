#include "setup/setup.h"
#include "fsm.h"


int main(void)
{
  //inicializacion del equipo
  setup();

  // inicializacion de las maquinas
  fsm_storage_init();
  fsm_client_init();
  fsm_playback_init();

 	while(1){
 	  lcd_refresh(); // otro formato... pero es una FSM (con corutinas)
    fsm_storage_update();
    fsm_client_update();
    fsm_playback_update();
 	}

	 //esta devolucion es para satisfacer al compilador, aunque nunca suceda
 	return 1;
}
