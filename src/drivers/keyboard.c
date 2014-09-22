/*
 * keyboard.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "keyboard.h"
#include "fsm.h"
#include "drivers.h"


static void keyboard_setup_with_polling(void);
static void keyboard_refresh(void);
static void keyboard_setup_with_interrupts(void);


void keyboard_setup(void){
  keyboard_setup_with_polling();
}

static void keyboard_setup_with_polling(void){
  set_pin_sel( KEY_PIN(0),0);
  gpio_set_dir(KEY_PIN(0),0);
  set_pin_sel(KEY_PIN(1),0);
  gpio_set_dir(KEY_PIN(1),0);
  set_pin_sel(KEY_PIN(2),0);
  gpio_set_dir(KEY_PIN(2),0);
  set_pin_sel(KEY_PIN(3),0);
  gpio_set_dir(KEY_PIN(3),0);

  systick_delay_async(20, 1,keyboard_refresh);

}

static void keyboard_refresh(void){

  static char check[4] = {1,1,1,1};

  if(gpio_get_pin(KEY_PIN(0),0)){
   if (check[0]){
      keyboard_handle_key(0);
      check[0] = 0;
    }
  }
  else{
    check[0] = 1;
  }

  if(gpio_get_pin(KEY_PIN(1),0)){
   if (check[1]){
      keyboard_handle_key(1);
      check[1] = 0;
    }
  }
  else{
    check[1] = 1;
  }

  if(gpio_get_pin(KEY_PIN(2),0)){
   if (check[2]){
      keyboard_handle_key(2);
      check[2] = 0;
    }
  }
  else{
    check[2] = 1;
  }

  if(gpio_get_pin(KEY_PIN(3),0)){
   if (check[3]){
      keyboard_handle_key(3);
      check[3] = 0;
    }
  }
  else{
    check[3] = 1;
  }




}

static void keyboard_setup_with_interrupts(void){
  //todo: implementar la inicializacion

  // Configuro el SW1 para que trabaje como EINT0
  // Esto debe hacerse con la interrupcion deshabilitada
  set_pin_sel(EINT0_PIN, 1);
  // Configuro la interrupcion externa 0 por flanco
  EXTMODE0_F;
  // Habilito Interrupcion Externa 0
  ISE_EINT0;


  // Configuro el SW4 para que trabaje como EINT3
  // Esto debe hacerse con la interrupcion deshabilitada
  set_pin_sel(EINT3_PIN, 1);
  // Configuro la interrupcion externa 0 por flanco
  EXTMODE0_F;
  // Habilito Interrupcion Externa 0
  ISE_EINT3;

}

static void i_to_str(uint32_t value, char* result){
  result[2] = '\0';
  result[1] = '0'+ value%10;
  value /=10;
  result[0] = '0'+ value%10;
}

void keyboard_handle_key(uint8_t key){
  static char value=0;
  char result[3];

  if(key == 0){
    lcd_print_at("SEND",0,0);
  }
  else if(key == 1){
    value +=10;
    i_to_str(value,result);
    lcd_print_at(result,1,0);
  }
  else if(key == 2){
    i_to_str(++value,result);
    lcd_print_at(result,1,0);
  }
  else if(key == 3){
    i_to_str(--value,result);
    lcd_print_at(result,1,0);
  }
}
