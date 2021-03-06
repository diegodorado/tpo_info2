#ifndef LCD_H_
#define LCD_H_

//include firmware
#include "fw.h"
#include "sysdefs.h"


#define LCD_LOW 0
#define LCD_HIGH 1

#define LCD_PIND4  0,5
#define LCD_PIND5  0,10
#define LCD_PIND6  2,4
#define LCD_PIND7  2,5
#define LCD_PINRS 2,6		//Pin RS
#define LCD_PINE  0,4		//Pin E

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

void lcd_setup(void);
void lcd_clear(void);
void lcd_print(char *);
void lcd_print_at(char *,uint8_t, uint8_t);
void lcd_print_char(char);
void lcd_print_char_at(char ,uint8_t,uint8_t);
void lcd_refresh(void);





#endif /* LCD_H_ */
