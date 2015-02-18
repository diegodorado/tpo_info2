/*
 * setup.c
 *
 *  Created on: Sep 18, 2014
 *      Author: diego
 */
#include "setup.h"

void setup(void){
  //todo: implementar la inicializacion del equipo (cada llamada a funcion)
  device_setup(); // inicializar micro (PLL)
  systick_setup();
  // LCD es inmediato para mostrar posibles errores
  lcd_setup(); // inicializar LCD
  keyboard_setup(); //inicializar Teclado
  storage_setup(); //inicializar almacenamiento (tarjeta SD)
  client_setup(); // inicializar cliente remoto (UART)
  audio_setup(); // inicializar AUDIO (DAC/DMA)

}

