#include "lcd.h"




void __delay(uint32_t t){
uint32_t x;
for(x=0;x<t*1000;x++);
//configurar ciclo a 1 microsegundo
}

void delayMicroseconds(int uSec)
{
	//us=0;
	//while(us*10<uSec){}
	__delay(uSec);
}


uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;

uint8_t _initialized;

uint8_t _numlines,_currline;


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
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).




void lcd_init(void)
{
	SetDIR(LCD_PIND4, 1);
	SetDIR(LCD_PIND5, 1);
	SetDIR(LCD_PIND6, 1);
	SetDIR(LCD_PIND7, 1);
	SetDIR(LCD_PINRS, 1);
	SetDIR(LCD_PINE, 1);
  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  lcd_begin(16, 2, LCD_5x8DOTS);
}

void lcd_begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;
  _currline = 0;


  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delayMicroseconds(50000);
  // Now we pull both RS and R/W low to begin commands
  SetPIN(LCD_PINRS, LOW);
  SetPIN(LCD_PINE, LOW);

  // we start in 8bit mode, try to set 4 bit mode
  lcd_write4bits(0x03);
  delayMicroseconds(4500); // wait min 4.1ms

  // second try
  lcd_write4bits(0x03);
  delayMicroseconds(4500); // wait min 4.1ms

  // third go!
  lcd_write4bits(0x03);
  delayMicroseconds(150);

  // finally, set to 8-bit interface
  lcd_write4bits(0x02);

  // finally, set # lines, font size, etc.
  lcd_command(LCD_FUNCTIONSET | _displayfunction);

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  lcd_display();

  // clear it off
  lcd_clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  lcd_command(LCD_ENTRYMODESET | _displaymode);

}

/********** high level commands, for the user! */
void lcd_clear()
{
  lcd_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void lcd_setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > _numlines ) {
    row = _numlines-1;    // we count rows starting w/0
  }

  lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_display() {
  _displaycontrol |= LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}


/*********** mid level commands, for sending data/cmds */

void lcd_command(uint8_t value) {
  lcd_send(value, LOW);
}

void lcd_write(uint8_t value) {
  lcd_send(value, HIGH);
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void lcd_send(uint8_t value, uint8_t mode) {
  SetPIN(LCD_PINRS, mode);
  lcd_write4bits(value>>4);
  lcd_write4bits(value);
}

void lcd_pulseEnable(void) {
  SetPIN(LCD_PINE, LOW);
  delayMicroseconds(1);
  SetPIN(LCD_PINE, HIGH);
  delayMicroseconds(1);    // enable pulse must be >450ns
  SetPIN(LCD_PINE, LOW);
  delayMicroseconds(100);   // commands need > 37us to settle
}

void lcd_write4bits(uint8_t value) {

  SetPIN(LCD_PIND4, (value >> 0) & 0x01);
  SetPIN(LCD_PIND5, (value >> 1) & 0x01);
  SetPIN(LCD_PIND6, (value >> 2) & 0x01);
  SetPIN(LCD_PIND7, (value >> 3) & 0x01);

  lcd_pulseEnable();
}



void lcd_print(char *str) {

  while (*str)
    lcd_write(*str++);
}




void run_lcd(){
	lcd_init();
	lcd_print("hello, world!");
	lcd_setCursor(0, 1);
	lcd_print("and EVA!");
}
