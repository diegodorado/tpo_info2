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


static uint8_t clear();
static uint8_t set_cursor(uint8_t row, uint8_t col);
static uint8_t set_cursor_i(uint8_t i);
static uint8_t refresh_chars(void);
static uint8_t command(uint8_t value, uint32_t settleTime);
static uint8_t write(uint8_t value);
static uint8_t write4bits( uint8_t value, uint32_t settleTime);

// for initialization purposes
static void write_4_bits_sync(uint8_t value, uint32_t settleTime);
static void command_sync(uint8_t value, uint32_t settleTime);


void lcd_setup(void)
{


  gpio_set_dir(LCD_PIND4, 1);
  gpio_set_dir(LCD_PIND5, 1);
  gpio_set_dir(LCD_PIND6, 1);
  gpio_set_dir(LCD_PIND7, 1);
  gpio_set_dir(LCD_PINRS, 1);
  gpio_set_dir(LCD_PINE, 1);

  // Now we pull both RS and R/W low to begin commands
  mode_low();
  pulse_enable_low();
  timer0_delay_us(100000);  //100ms de startup

  write_4_bits_sync(0x03 , 4100); // we start in 8bit mode, try to set 4 bit mode and wait min 4.1ms
  write_4_bits_sync(0x03 , 100); // second try
  write_4_bits_sync(0x03 , 100); // third go!
  write_4_bits_sync(0x02 , 100); // finally, set to 4-bit interface

  command_sync(0x28,100); // finally, set # lines, font size, etc.
  command_sync(0x08,100); // turn the display off with no cursor or blinking default
  command_sync(0x01,1000);  // clear it off:  this command takes a long time!
  command_sync(0x06,100); // Initialize to default text direction
  command_sync(0x0C,100); // turn display on


}

static void write_4_bits_sync(uint8_t value, uint32_t settleTime){
  write_4_bits(value);
  pulse_enable_low();
  timer0_delay_us(1);
  pulse_enable_high();
  timer0_delay_us(1); // enable pulse must be >450ns
  pulse_enable_low();
  timer0_delay_us(settleTime);
}

static void command_sync(uint8_t value, uint32_t settleTime) {
  write_4_bits_sync(value>>4 , 1);
  write_4_bits_sync(value , settleTime);
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
  crBegin;
  while (!command(LCD_CLEARDISPLAY,1000))
    crReturn(0);

  has_to_clear = 0;
  crFinish;

  return 1;

}



static uint8_t set_cursor(uint8_t row, uint8_t col)
{
  int offset = 0x00;
  if ( row > 0 )
    offset = 0x40;

  return command(LCD_SETDDRAMADDR | (offset + col),20);
}

static uint8_t set_cursor_i(uint8_t i)
{
  if(cursor==i)
    return 1;

  return set_cursor(i>15,i%16);
}






//refresca el lcd con el buffer
//solo donde sea necesario
// refresca un caracter por llamada
static uint8_t refresh_chars(void){
  static uint8_t i;

  if(dirty_mask == 0x00)
    return 1; // nothing to do!


  crBegin;

  for(i=0;i<32;i++)
  {
    //matches a dirty char
    if(dirty_mask & (0x01<<i ))
    {

      //set_cursor
      while (!set_cursor_i(i))
        crReturn(0);

      while (!write(text_buffer[i]))
        crReturn(0);

      dirty_mask &= ~(0x01<<i); //clears dirty mask at current_cursor position
    }

  }

  crFinish;
  return 1;

}





static uint8_t command(uint8_t value, uint32_t settleTime) {

  crBegin;
  mode_low();

  while (!write4bits(value>>4,1))
    crReturn(0);

  while (!write4bits(value,settleTime))
    crReturn(0);

  crFinish;
  return 1;
}


static uint8_t write(uint8_t value) {
  crBegin;
  mode_high();

  while (!write4bits(value>>4,1))
    crReturn(0);

  while (!write4bits(value,20))
    crReturn(0);

  crFinish;
  return 1;
}

static uint8_t write4bits( uint8_t value, uint32_t settleTime)
{

  static uint32_t since;

  crBegin;

  write_4_bits(value);

  pulse_enable_low();
  since =  timer0_us();
  while ((uint32_t) (timer0_us() - since) < 1 )
    crReturn(0);

  pulse_enable_high();
  since =  timer0_us();
  while ((uint32_t) (timer0_us() - since) < 1 )
    crReturn(0);


  pulse_enable_low();
  since =  timer0_us();
  while ((uint32_t) (timer0_us() - since) < settleTime )
    crReturn(0);

  crFinish;
  return 1;

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








// interface functions: only touches buffer... doesnt really play with LCD directly

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


