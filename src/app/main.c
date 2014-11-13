#include "setup/setup.h"
#include "fsm/fsm_main.h"

int main(void)
{
  //inicializacion del equipo
  setup();

  // inicia la maquina principal
  //fsm_main_start();

 	while(1){
	   //pasa el control del ciclo a la
	   //maquina de estados principal
	   //fsm_main_update();
 	}

	 //esta devolucion es para satisfacer al compilador, aunque nunca suceda
 	return 1;
}
