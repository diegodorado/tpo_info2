#ifndef LCD_H_
#define LCD_H_

//include firmware
#include "fw.h"
#include "sysdefs.h"
#include "drivers.h"


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
#define LCD_SETDDRAMADDR 0x80


void lcd_setup(void);
void lcd_clear(void);
void lcd_print(char *);
void lcd_print_at(char *,uint8_t, uint8_t);
void lcd_print_char(char);
void lcd_print_char_at(char ,uint8_t,uint8_t);
void lcd_print_int_at(uint32_t value, uint8_t size,uint8_t row,uint8_t col);
void lcd_refresh(void);


//coroutines should be elsewhere
#define crBegin static int state=0; switch(state) { case 0:
#define crReturn(x) do { state=__LINE__; return x; case __LINE__:; } while (0)
#define crFinish state=0;}




#endif /* LCD_H_ */
