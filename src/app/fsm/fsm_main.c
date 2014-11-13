// NOTA:
// Momentaneamente aceptamos la cascada de llamadas
// en favor de la modularizacion del codigo
// En una futura version podremos unificar las maquinas
// en una misma tabla de punteros a funcion

#include "fsm_main.h"


static fsm_main_state_t state = FSM_MAIN_STATE_IDLE; // estado inicial

// funciones primitivas de los estados y
// sus transiciones
static void idle_enter( void);
static void idle_update( void);
static void idle_exit( void);

static void playback_enter( void);
static void playback_update( void);
static void playback_exit( void);

static void write_enter( void);
static void write_update( void);
static void write_exit( void);


static void receiving_enter( void);
static void receiving_update( void);
static void receiving_exit( void);

static void error_enter( void);
static void error_update( void);
static void error_exit( void);


// definicion de la tabla de punteros a funcion
// que debe corresponderse con fsm_main_state_t
void (* const state_table[][3])(void) = {
  {idle_enter    , idle_update    , idle_exit},
  {playback_enter, playback_update, playback_exit},
  {write_enter   , write_update   , write_exit},
  {receiving_enter   , receiving_update   , receiving_exit},
  {error_enter   , error_update   , error_exit},
};


void fsm_main_update(void)
{
  // implementada con punteros a funcion
  // cambiando fsm_main_state se cambia el estado de la maquina
  // En este caso, cada estado es una submaquina.
  (*state_table[ state ][ON_UPDATE])();
}


void fsm_main_start()
{
  //ejecuta el on enter del estado inicial
  (*state_table[ state ][ON_ENTER])();
}


void fsm_main_change(fsm_main_state_t st)
{
  if(st==state) return; //no es un cambio de estado

  // ejecutar on_exit.
  (*state_table[ state ][ON_EXIT]) ();

  //setea el estado nuevo
  state = st;

  //ahora, ejecuta on_enter
  (*state_table[ state ][ON_ENTER]) ();

}



// implementacion de las funciones de los estados






static void playback_enter( void)
{
}

static void playback_update( void)
{
  // se llama a la submaquina fsm_playback
  //fsm_playback();
}

static void playback_exit( void)
{

}


static void write_enter( void)
{
  lcd_print_at("WRITE STATE",0,0);
}

static void write_update( void)
{
  // se llama a la submaquina fsm_write
  //fsm_write();

}

static void write_exit( void)
{

}



static void idle_enter( void)
{
  lcd_print_at("IDLE STATE",0,0);
}

static void idle_update( void)
{
  return;

  uint8_t data;
  buffer_status_t status;
  //if data frame available, receive it!
  if(client_data_frame_received())
  {
    uint8_t data = client_rx_pop();
    printf("client_rx_pop : %d \n" , data);
    messagesBufferPush(data);
    status = messagesBufferProcess();
    printf(" buffer sstatus : %d \n" , status );
  }
    //fsm_main_change(FSM_MAIN_STATE_RECEIVING);

}

static void idle_exit( void)
{

}


static void receiving_enter( void)
{
  tp4_data_frame_t data;

  lcd_print_at("RX: ",0,0);
  data = client_decode_data_frame();

  if (client_is_checksum_ok(data)){
    //lcd_clear();
    lcd_print_char(data.event + '0');
    lcd_print(" @ (");
    lcd_print_char(data.minutes / 10 + '0');
    lcd_print_char(data.minutes % 10 + '0');
    lcd_print_char(':');
    lcd_print_char(data.seconds / 10 + '0');
    lcd_print_char(data.seconds % 10 + '0');
    lcd_print_char(')');
    fsm_main_change(FSM_MAIN_STATE_IDLE);
  }else{
    fsm_main_change(FSM_MAIN_STATE_ERROR);
  }

}

static void receiving_update( void)
{
}

static void receiving_exit( void)
{

}


static void error_enter( void)
{
  lcd_print_at("CHKSUM ERROR",0,0);
  lcd_print_at("RESTART DEVICE...",1,0);
  while(1){}; //app is hung
}

static void error_update( void)
{
}

static void error_exit( void)
{
}
