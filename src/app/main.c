#include "sysinit.h"
#include "fsm/fsm_main.h"

int main(void)
{
	Inicializar ( ) ;

	//pasa el control del ciclo a la
	//maquina de estados principal
	fsm_main();

	//aunque no es estrictamente necesario
	//ya que la maquina de estados principal nunca termina
	//este while indica que el programa no debe terminar nunca
	while(1){};

	//esta devolucion es para satisfacer al compilador aunque nunca suceda
	return 1;
}
