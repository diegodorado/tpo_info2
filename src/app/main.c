#include "setup/setup.h"
#include "fsm.h"

int main(void)
{
  uint8_t ssd_result;
  char data_read[16+1] = "abcdefghijklmnop";
  //inicializacion del equipo
  setup();
 draw_example();
  puts_lcd("                ",0,0);
  puts_lcd("                ",1,0);
  // test de escritura en sd: escribe "0123456789" en los primeros bytes de la sd
  // devuelve 0 si no hubieron errores
  //ssd_result = sd_card_write_test("0123456789");
  //lcd_print_at("WE:",1,12);
  //lcd_print_char(ssd_result + '0');

  // test de lectura: lee los primeros 16 bytes de la sd y lo guarda en dataread
  // devuelve 0 si no hay errores
  // imprime el resultado en la fila superior del LCD

ssd_result = sd_card_read_test(data_read,16);

#if defined (LCD_2X16_NO_FSM)

  puts_lcd(data_read,0,0);
  putc_lcd(ssd_result+48,1,5);
    #if defined(UART0)
      uart0_string_tx("Debug Kit info2\n");
      uart0_string_tx("ssd result =");
      uart0_char_tx ( ssd_result+48 );
      uart0_string_tx ("\r\n" );
    #endif

#else
  lcd_print_at(data_read,0,0);
  lcd_print_at(ssd_result,1,0);
#endif

 	while(1){
 	  	  lcd_refresh(); // otro formato... pero es una FSM (con corutinas)
 	  fsm_client_update();  // ejecuta un tick de la maquina de estados "cliente"
 	  fsm_playback_update();  // ejecuta un tick de la maquina de estados "playback"
 	}

	 //esta devolucion es para satisfacer al compilador, aunque nunca suceda
 	return 1;
}
