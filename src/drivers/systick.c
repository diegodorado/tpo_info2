/*
 * systick.c
 *
 *  Created on: Sep 21, 2014
 *      Author: diego
 */


#include "systick.h"


static volatile uint32_t millisecs;
static volatile uint32_t callbacks_millisecs[CALLBACKS_QUEUE_SIZE];
static volatile uint32_t callbacks_millisecs_load[CALLBACKS_QUEUE_SIZE]; //recarga para callbask periodicos
static volatile uint32_t callbacks_available_mask = 0xFF;
static volatile uint32_t callbacks_enabled_mask = 0x00;
static volatile uint32_t callbacks_periodic_mask = 0x00;

// cola de callbacks
void (* callbacks_queue[CALLBACKS_QUEUE_SIZE])(void);


void systick_setup(void){
  STRELOAD  =  STCALIB;   // configurado para interrumpir cada 10 ms
  STCURR = 0;
  ENABLE = 1;
  TICKINT = 1;
  CLKSOURCE = 1;
}

// llamada por isr_systick.c
// podria implementarse aqui directamente la atencion a la interrupcion
// evitando una llamada a funcion pero atenta contra la separacion de capas

void systick_handle_tick(void)
{
  int i;
  for (i=0; i< CALLBACKS_QUEUE_SIZE; i++){

    if( callbacks_enabled_mask & (0x01<<i) ){    // chequear si el callback esta habilitado

      callbacks_millisecs[i] -= MS_PER_TICK; // decrementar su cuenta
      if(callbacks_millisecs[i] <= 0){    // chequear si llego a su fin

        if( callbacks_periodic_mask & (0x01<<i) )   //chequear si el callback es periodico
          callbacks_millisecs[i] = callbacks_millisecs_load[i];   //recargar la cuenta
        else{
          callbacks_enabled_mask &= ~(0x01<<i); //deshabilito el callback
          callbacks_available_mask |= (0x01<<i); //dejo habilitado para otro callback

        }

        (*callbacks_queue[i])();  //llamada al callback


      }

    }

  }
  // incrementar la cuenta global de milisegundos
  // de uso exclusivo de systick_delay_sync()
  millisecs += MS_PER_TICK;
}

// delay bloqueante que toma control del codigo.
// utilizar preferentemente solo para configuraciones
void systick_delay_sync(uint32_t millis){
  millisecs = 0;  // resetea la cuenta global de milisegundos
  while ( millisecs < millis ){}; //bloquea el codigo hasta que se alcance la cuenta
}

// delay asincronico
// encola el callback para ser ejecutado cuando se alcance la cuenta
void systick_delay_async(uint32_t millis, char is_periodic, void (*callback)( void)){

  int i;

  for (i=0; i< CALLBACKS_QUEUE_SIZE; i++){
    if( callbacks_available_mask & (0x01<<i) ){

      callbacks_available_mask &= ~(0x01<<i); //ya no esta disponible

      callbacks_millisecs[i] = millis; //cargar la cuenta
      callbacks_enabled_mask |= (0x01<<i); //habilito el callback
      if(is_periodic){
        callbacks_millisecs_load[i] = millis;  //setear la recarga
        callbacks_periodic_mask |= (0x01<<i);  //periodica
      }else{
        callbacks_periodic_mask &= ~(0x01<<i); //no periodica
      }

      callbacks_queue[i] = callback; //seteo el callback
      callbacks_enabled_mask |= (0x01<<i); //habilito el callback

      return; // termino la ejecucion

    }
  }

  //todo: controlar la ecepcion de haber alcanzado el maximo permitido de callbacks
  return;


}
