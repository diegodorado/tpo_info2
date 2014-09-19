/*
 * setup.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */
#include "setup.h"

void setup(void){
  //todo: implementar la inicializacion del equipo (cada llamada a funcion)

  setup_pll(); // inicializar PLL
  // LCD es inmediato para mostrar posibles errores
  setup_lcd(); // inicializar LCD

  setup_dac(); // inicializar DAC
  setup_kb(); //inicializar Teclado
  setup_sd(); //inicializar tarjeta SD
  setup_uart(); // inicializar puerto serie

}

