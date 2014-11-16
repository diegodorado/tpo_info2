#include "lcd.h"

//declaracion de funciones privadas
static void set_cursor(uint8_t, uint8_t);
static void command(uint8_t);
static void write(uint8_t);
static void send(uint8_t, uint8_t);
static void pulse_enable(void);
static void write_4_bits(uint8_t);
static void delay(uint32_t);
static void lcd_do_clear(void);


static volatile char text_buffer[32]; //holds the representation of lcd text
static volatile uint32_t dirty_mask = 0x00; //holds wich char needs to be drawed
static volatile char must_clear = 0; //if set to 1 will clear lcd on lcd_refresh call
static volatile uint8_t cursor = 0; //holds cursor index, to see if there is need of setting cursor before write

//definicion de funciones publicas


void lcd_setup(void)
{

  timer1_setup();

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


  delay(20); //20ms de startup

  // Now we pull both RS and R/W low to begin commands
  gpio_set_pin(LCD_PINRS, LCD_LOW);
  gpio_set_pin(LCD_PINE, LCD_LOW);

  // we start in 8bit mode, try to set 4 bit mode
  write_4_bits(0x03);
  delay(5); // wait min 4.1ms

  // second try
  write_4_bits(0x03);
  delay(5); // wait min 4.1ms

  // third go!
  write_4_bits(0x03);
  delay(1);

  // finally, set to 8-bit interface
  write_4_bits(0x02);

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS | LCD_2LINE);

  // turn the display on with no cursor or blinking default
  command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);

  // clear it off
  lcd_do_clear();

  // Initialize to default text direction (for romance languages)
  // set the entry mode
  command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);


  // refresco periodico del lcd
  // es llamado por las interrupciones del systick
  // cada 200 milisegundos
  systick_delay_async(200, 1,lcd_refresh);

}

void lcd_clear()
{
  must_clear = 1;
}

void lcd_do_clear()
{
  uint8_t i;
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  cursor = 0;
  for (i=0; i<32;i++)
    text_buffer[i] = 0;

  delay(2);  // this command takes a long time!
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

//refresca el lcd con el buffer
//solo donde sea necesario
void lcd_refresh(void){

  uint8_t i;

  // clears takes precedence
  if (must_clear){
    must_clear = 0;
    lcd_do_clear();
  }

  if (dirty_mask!=0x00){
    for (i=0; i<32;i++){
      if(dirty_mask & (0x01<<i)){ //matches a dirty char
        //should_set_cursor
        if(cursor!=i)
          set_cursor(i>15,i%16);
        write(text_buffer[i]);
      }
    }
    dirty_mask=0x00;
    return;
  }


}


//definicion de funciones privadas
static void delay(uint32_t ms)
{
  timer1_delay_ms(ms);
}



static void set_cursor(uint8_t row, uint8_t col)
{
  int offset = 0x00;;
  if ( row > 0 )
    offset = 0x40;

  command(LCD_SETDDRAMADDR | (offset + col));
}

static void command(uint8_t value) {
  send(value, LCD_LOW);
}

static void write(uint8_t value) {
  send(value, LCD_HIGH);
}

// write either command or data, with automatic 4/8-bit selection
static void send(uint8_t value, uint8_t mode) {
  gpio_set_pin(LCD_PINRS, mode);
  write_4_bits(value>>4);
  write_4_bits(value);
}

static void write_4_bits(uint8_t value) {

  gpio_set_pin(LCD_PIND4, (value >> 0) & 0x01);
  gpio_set_pin(LCD_PIND5, (value >> 1) & 0x01);
  gpio_set_pin(LCD_PIND6, (value >> 2) & 0x01);
  gpio_set_pin(LCD_PIND7, (value >> 3) & 0x01);

  pulse_enable();
}

static void pulse_enable(void) {
  gpio_set_pin(LCD_PINE, LCD_LOW);
  delay(1);
  gpio_set_pin(LCD_PINE, LCD_HIGH);
  delay(1);    // enable pulse must be >450ns
  gpio_set_pin(LCD_PINE, LCD_LOW);
  delay(1);   // commands need > 37us to settle
}

