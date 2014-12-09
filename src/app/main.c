#include "setup/setup.h"
#include "fsm.h"


static uint32_t chunks_count=2000;
static uint32_t chunks_left=1000;


int main(void)
{

  //inicializacion del equipo
  setup();
  lcd_print_at("TPO INFO 2",0,0);

 	while(1){
 	  lcd_refresh(); // otro formato... pero es una FSM (con corutinas)
 	  fsm_client_update();  // ejecuta un tick de la maquina de estados "cliente"
 	  fsm_playback_update();  // ejecuta un tick de la maquina de estados "playback"
 	}

	 //esta devolucion es para satisfacer al compilador, aunque nunca suceda
 	return 1;
}
