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
  timer0_setup(); // used for LCD, configured at 1us
  timer1_setup(); // used for audio, interrupts at the defined sample rate of the current playback
  // LCD es inmediato para mostrar posibles errores
  lcd_setup(); // inicializar LCD
  keyboard_setup(); //inicializar Teclado
  //storage_setup(); //inicializar almacenamiento (tarjeta SD)
  client_setup(); // inicializar cliente remoto (UART)

  lcd_print_at("STARTED.",0,0);
  audio_setup(); // inicializar AUDIO (DAC/DMA)
  lcd_print_at("READY.",1,0);

}

