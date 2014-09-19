// NOTA:
// Momentaneamente aceptamos la cascada de llamadas
// en favor de la modularizacion del codigo
// En una futura version podremos unificar las maquinas
// en una misma tabla de punteros a funcion

#include "fsm_main.h"
#include "fw.h"


static fsm_main_state_t state = -1; // -1 mean no state set yet

static void idle_enter( void);
static void idle_update( void);
static void idle_exit( void);

static void playback_enter( void);
static void playback_update( void);
static void playback_exit( void);

static void write_enter( void);
static void write_update( void);
static void write_exit( void);


// definicion de la tabla de punteros a funcion
// que debe corresponderse con fsm_main_state_t
void (* const state_table[][3])(void) = {
  {idle_enter    , idle_update    , idle_exit},
  {playback_enter, playback_update, playback_exit},
  {write_enter   , write_update   , write_exit}
};


void fsm_main(void)
{
  // es la maquina de estados principal
  // y por lo tanto toma el control del loop principal
  // sin retornar nunca
  while ( 1 ) {
    // implementada con punteros a funcion
    // cambiando fsm_main_state se cambia el estado de la maquina
    // En este caso, cada estado es una submaquina.
    state_table[ state ][ON_UPDATE]();
  }
}

void fsm_main_change(fsm_main_state_t st){
  if(st==state) return; //no es un cambio de estado

  // solo si viene de un estado previo
  // ejecutar on_exit.
  if(state!=-1)
    state_table[ state ][ON_EXIT]();

  //setea el estado nuevo
  state = st;

  //ahora, ejecuta on_enter
  state_table[ state ][ON_ENTER]();

}



static void idle_enter( void){
  // estado de reposo...
  // se puede usar este estado para tests
  //audio_test();
  //lcd_clear();
  lcd_set_cursor(0, 0);
  lcd_print("IDLE STATE ...zz");

}

static void idle_update( void){
  // estado de reposo...
  // no tiene submaquina
}

static void idle_exit( void){

}


static void playback_enter( void){
  uart1_tx_push(0x55);
}

static void playback_update( void){
  // se llama a la submaquina fsm_playback
  //fsm_playback();
}

static void playback_exit( void){

}


static void write_enter( void){
  lcd_set_cursor(0, 0);
  lcd_print("WRITE STATE **");
}

static void write_update( void){
  // se llama a la submaquina fsm_write
  //fsm_write();

}

static void write_exit( void){

}
