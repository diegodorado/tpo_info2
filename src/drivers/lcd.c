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
static uint8_t command(uint8_t value);
static uint8_t write(uint8_t value);
static uint8_t write4bits( uint8_t value);









void lcd_setup(void)
{

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
  while(command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS | LCD_2LINE)==0);

  // turn the display on with no cursor or blinking default
  while(command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF)==0);

  // clear it off
  while(command(LCD_CLEARDISPLAY)==0);
  // clear display, set cursor position to zero
  timer0_delay_us(2000);  // this command takes a long time!

  // Initialize to default text direction (for romance languages)
  // set the entry mode
  while(command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT)==0);



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
  static uint32_t since;

  crBegin;
  while (!command(LCD_CLEARDISPLAY))
    crReturn(0);

  since =  timer0_us();
  while ((uint32_t) (timer0_us() - since) < 2000 )
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

  return command(LCD_SETDDRAMADDR | (offset + col));
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





static uint8_t command(uint8_t value) {

  crBegin;
  mode_low();

  while (!write4bits(value>>4))
    crReturn(0);

  while (!write4bits(value))
    crReturn(0);

  crFinish;
  return 1;
}


static uint8_t write(uint8_t value) {
  crBegin;
  mode_high();

  while (!write4bits(value>>4))
    crReturn(0);

  while (!write4bits(value))
    crReturn(0);

  crFinish;
  return 1;
}

static uint8_t write4bits( uint8_t value)
{

  static uint32_t since;

  crBegin;

  write_4_bits(value);

  pulse_enable_low();
  since =  timer0_us();
  while ((uint32_t) (timer0_us() - since) < 40 )
    crReturn(0);

  pulse_enable_high();
  since =  timer0_us();
  while ((uint32_t) (timer0_us() - since) < 1 )
    crReturn(0);



  pulse_enable_low();
  since =  timer0_us();
  while ((uint32_t) (timer0_us() - since) < 40 )
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


