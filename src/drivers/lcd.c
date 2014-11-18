#include "lcd.h"


//definicion de funciones publicas
static volatile uint8_t has_to_clear = 0; //if set to 1 will clear lcd on lcd_refresh call
static volatile char text_buffer[32]; //holds the representation of lcd text
static volatile uint32_t dirty_mask = 0x00; //holds wich char needs to be drawed
static volatile uint8_t cursor = 0; //holds cursor index, to see if there is need of setting cursor before write


//declaracion de funciones privadas
static void write_4_bits(uint8_t value);
static void pulse_enable_low(void);
static void pulse_enable_high(void);
static void mode_low(void);
static void mode_high(void);


static void clear();
static uint8_t set_cursor(uint8_t row, uint8_t col);
static void refresh_chars(void);
static uint8_t command(uint8_t value);
static uint8_t write(uint8_t value);
static uint8_t write4bits( uint8_t value);









void lcd_setup(void)
{

  timer0_setup();

  // When the display powers up, it is configured as follows:
  //
  // 1. Display clear
  // 2. Function set:
  //    DL = 1; 8-bit interface data
  //    N = 0; 1-line display
  //    F = 0; 5x8 dot character font
  // 3. Display on/off control:
  //    D = 0; Display off
  //    C = 0; Cursor off
  //    B = 0; Blinking off
  // 4. Entry mode set:
  //    I/D = 1; Increment by 1
  //    S = 0; No shift
  //
  gpio_set_dir(LCD_PIND4, 1);
  gpio_set_dir(LCD_PIND5, 1);
  gpio_set_dir(LCD_PIND6, 1);
  gpio_set_dir(LCD_PIND7, 1);
  gpio_set_dir(LCD_PINRS, 1);
  gpio_set_dir(LCD_PINE, 1);


  timer0_delay_us(20000);  //20ms de startup


  // Now we pull both RS and R/W low to begin commands
  gpio_set_pin(LCD_PINRS, LCD_LOW);
  gpio_set_pin(LCD_PINE, LCD_LOW);

  // we start in 8bit mode, try to set 4 bit mode
  write_4_bits(0x03);
  timer0_delay_us(4100); // wait min 4.1ms

  // second try
  write_4_bits(0x03);
  timer0_delay_us(4100); // wait min 4.1ms

  // third go!
  write_4_bits(0x03);
  timer0_delay_us(150); //150 us

  // finally, set to 8-bit interface
  write_4_bits(0x02);

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS | LCD_2LINE);

  // turn the display on with no cursor or blinking default
  command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);

  // clear it off
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  timer0_delay_us(2000);  // this command takes a long time!

  // Initialize to default text direction (for romance languages)
  // set the entry mode
  command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);



}



void lcd_refresh(void)
{

  if(has_to_clear)
  {
    clear();
  }
  else
  {
    refresh_chars();
  }


}


static uint8_t clear()
{
  static uint32_t wait_until;
  static uint8_t cmd_result;

  crBegin;
  while (1) {
    cmd_result = command(LCD_CLEARDISPLAY);
    if (cmd_result == 0)
    {
      crReturn(0);
    }
    else
    {
      wait_until =  timer0_us() + 2000;
      crReturn(0);
      if (wait_until == timer0_us())
      {
        has_to_clear = 0;
        break;
      }

    }

  }
  crReturn(1);
  crFinish;

  //command(LCD_CLEARDISPLAY);
  //has_to_clear = 0;
  //timer0_delay_us(2000);

}


static void clear_old()
{
  static int last_us = 0;
  static uint8_t phase = 0;

  switch(phase) {

    case 0:
      if(command(LCD_CLEARDISPLAY))
      {
        phase++;
        last_us = timer0_us();
      }
      break;
    case 1:
      // this command takes a long time!
      if(last_us + 2000 == timer0_us())
        phase++;
      break;
    case 2:
      phase = 0;
      has_to_clear = 0;
  }

}



static uint8_t set_cursor(uint8_t row, uint8_t col)
{
  int offset = 0x00;
  if ( row > 0 )
    offset = 0x40;

  return command(LCD_SETDDRAMADDR | (offset + col));
}


//refresca el lcd con el buffer
//solo donde sea necesario
// refresca un caracter por llamada
static void refresh_chars(void){

  static uint8_t current_cursor;
  static uint8_t cursor_not_set = 1;

  if(dirty_mask == 0x00)
    return; // nothing to do!

  if(dirty_mask & (0x01<<current_cursor )){ //matches a dirty char
    //should_set_cursor
    if(cursor_not_set && cursor!=current_cursor )
      if(!set_cursor(current_cursor >15,current_cursor%16))
      {
        cursor_not_set = 0;
        return;
      }


    if(!write(text_buffer[current_cursor]))
      return;

    dirty_mask &= ~(0x01<<current_cursor ); //clears dirty mask at current_cursor position
  }

  current_cursor++;
  cursor_not_set = 1;
  current_cursor %= 32;


}



static uint8_t command(uint8_t value) {
      mode_low();
      write4bits(value>>4);
      write4bits(value);
      return 1;
}

static uint8_t command_old(uint8_t value) {
  static uint8_t phase = 0;

  switch(phase) {

    case 0:
      mode_low();
      phase++;
      break;
    case 1:
      if(write4bits(value>>4))
        phase++;
      break;
    case 2:
      if(write4bits(value))
        phase++;
      break;
    case 3:
      phase = 0;
      return 1;

  }

  return 0;

}


static uint8_t write(uint8_t value) {
      mode_high();
      write4bits(value>>4);
      write4bits(value);
      return 1;
}

static uint8_t write__(uint8_t value) {
  static uint8_t phase = 0;

  switch(phase) {

    case 0:
      mode_high();
      phase++;
      break;
    case 1:
      if(write4bits(value>>4))
        phase++;
      break;
    case 2:
      if(write4bits(value))
        phase++;
      break;
    case 3:
      phase = 0;
      return 1;

  }

  return 0;

}


static uint8_t write4bits( uint8_t value)
{
  write_4_bits(value);
  pulse_enable_low();
  timer0_delay_us(40);
  pulse_enable_high();
  timer0_delay_us(1);
  pulse_enable_low();
  timer0_delay_us(40);
  return 1;

}



static uint8_t write4bits_old( uint8_t value)
{
  static uint8_t phase = 0;
  static int last_us = 0;

  switch(phase) {

    case 0:
      write_4_bits(value);
      phase++;
      break;
    case 1:
      pulse_enable_low();
      phase++;
      last_us = timer0_us();
      break;
    case 2:
      if(last_us + 400 == timer0_us())
        phase++;
      break;
    case 3:
      pulse_enable_high();
      phase++;
      last_us = timer0_us();
      break;
    case 4:
      // enable pulse must be >450ns
      if(last_us + 100 == timer0_us())
        phase++;
      break;
    case 5:
      pulse_enable_low();
      phase++;
      last_us = timer0_us();
      break;
    case 6:
      // commands need > 37us to settle
      if(last_us + 400 == timer0_us())
        phase++;
      break;
    case 7:
      phase = 0;
      return 1;


  }


  return 0;

}







void lcd_clear()
{
  uint8_t i;
  cursor = 0;
  for (i=0; i<32;i++)
    text_buffer[i] = 0;

  has_to_clear = 1;
}


void lcd_print_int_at(  uint32_t value, uint8_t size,uint8_t row,uint8_t col)
{
  for(;size-->0;value/=10)
    lcd_print_char_at('0' + (value%10),row, col--);

  // int was too big? ***mark it***
  if(value>10)
    lcd_print_char('*');

}

void lcd_print_char(char c)
{
  if(text_buffer[cursor]!=c)
    dirty_mask |= (0x01<<cursor); //marca ese char como dirty
  text_buffer[cursor++]=c;
}


void lcd_print(char *str)
{
  //from cursor trhough size of buffer or end of string
  for(;(cursor<32) && *str!='\0';)
    lcd_print_char(*str++);
}

void lcd_print_char_at(char c,uint8_t row,uint8_t col)
{
  cursor = row * 16 + col;
  lcd_print_char(c);
}

void lcd_print_at(char *str,uint8_t row,uint8_t col)
{
  cursor = row * 16 + col;
  lcd_print(str);
}




static void write_4_bits(uint8_t value) {

  gpio_set_pin(LCD_PIND4, (value >> 0) & 0x01);
  gpio_set_pin(LCD_PIND5, (value >> 1) & 0x01);
  gpio_set_pin(LCD_PIND6, (value >> 2) & 0x01);
  gpio_set_pin(LCD_PIND7, (value >> 3) & 0x01);

}

static void pulse_enable_low(void) {
  gpio_set_pin(LCD_PINE, LCD_LOW);
}

static void pulse_enable_high(void) {
  gpio_set_pin(LCD_PINE, LCD_HIGH);
}


static void mode_low(void) {
  gpio_set_pin(LCD_PINRS, LCD_LOW);
}

static void mode_high(void) {
  gpio_set_pin(LCD_PINRS, LCD_HIGH);
}


