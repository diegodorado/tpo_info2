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
  //audio_setup(); // inicializar AUDIO (DAC/DMA)
  //keyboard_setup(); //inicializar Teclado
  //storage_setup(); //inicializar almacenamiento (tarjeta SD)
  client_setup(); // inicializar cliente remoto (UART)

  lcd_print_at("ALo",0,0);
  //lcd_print_at("->",1,1);

}

