#ifndef SYSDEFS_H
#define SYSDEFS_H

#include <stdint.h>

/*
 * SYSTEM CONFIGURATION
 * */

#define DEBUG_ON

//UART
//uncomment to use uart0 or comment to use uart1
#define USE_UART0
//#define USE_UART1

//#define BAUDRATE_2400
//#define BAUDRATE_9600
//#define BAUDRATE_19200
#define BAUDRATE_38400
//#define BAUDRATE_115200


//KEYBOARD
//#define USE_SW1_WITH_INTERRUPTS
//#define USE_SW4_WITH_INTERRUPTS
#define USE_SW1_WITH_POLLING
#define USE_SW2_WITH_POLLING
#define USE_SW3_WITH_POLLING
#define USE_SW4_WITH_POLLING


#define USE_SW_WITH_POLLING defined(USE_SW1_WITH_POLLING) || defined(USE_SW2_WITH_POLLING) || defined(USE_SW3_WITH_POLLING) || defined(USE_SW4_WITH_POLLING)




#define __R volatile const
#define __W volatile
#define __RW volatile

typedef 	__RW uint32_t 		register_t;  //!< defino un tipo 'registro'.




typedef union {
  __RW uint32_t dword;
  __R uint32_t dWord;
  struct {
    __RW uint16_t wordL;
    __RW uint16_t wordH;
  };

  struct {
    __RW uint8_t byte0;
    __RW uint8_t byte1;
    __RW uint8_t byte2;
    __RW uint8_t byte3;
  };

  struct {
    __RW uint32_t bit00 :1;
    __RW uint32_t bit01 :1;
    __RW uint32_t bit02 :1;
    __RW uint32_t bit03 :1;
    __RW uint32_t bit04 :1;
    __RW uint32_t bit05 :1;
    __RW uint32_t bit06 :1;
    __RW uint32_t bit07 :1;
    __RW uint32_t bit08 :1;
    __RW uint32_t bit09 :1;
    __RW uint32_t bit10 :1;
    __RW uint32_t bit11 :1;
    __RW uint32_t bit12 :1;
    __RW uint32_t bit13 :1;
    __RW uint32_t bit14 :1;
    __RW uint32_t bit15 :1;
    __RW uint32_t bit16 :1;
    __RW uint32_t bit17 :1;
    __RW uint32_t bit18 :1;
    __RW uint32_t bit19 :1;
    __RW uint32_t bit20 :1;
    __RW uint32_t bit21 :1;
    __RW uint32_t bit22 :1;
    __RW uint32_t bit23 :1;
    __RW uint32_t bit24 :1;
    __RW uint32_t bit25 :1;
    __RW uint32_t bit26 :1;
    __RW uint32_t bit27 :1;
    __RW uint32_t bit28 :1;
    __RW uint32_t bit29 :1;
    __RW uint32_t bit30 :1;
    __RW uint32_t bit31 :1;
  };

  struct {
    __R uint32_t bit00 :1;
    __R uint32_t bit01 :1;
    __R uint32_t bit02 :1;
    __R uint32_t bit03 :1;
    __R uint32_t bit04 :1;
    __R uint32_t bit05 :1;
    __R uint32_t bit06 :1;
    __R uint32_t bit07 :1;
    __R uint32_t bit08 :1;
    __R uint32_t bit09 :1;
    __R uint32_t bit10 :1;
    __R uint32_t bit11 :1;
    __R uint32_t bit12 :1;
    __R uint32_t bit13 :1;
    __R uint32_t bit14 :1;
    __R uint32_t bit15 :1;
    __R uint32_t bit16 :1;
    __R uint32_t bit17 :1;
    __R uint32_t bit18 :1;
    __R uint32_t bit19 :1;
    __R uint32_t bit20 :1;
    __R uint32_t bit21 :1;
    __R uint32_t bit22 :1;
    __R uint32_t bit23 :1;
    __R uint32_t bit24 :1;
    __R uint32_t bit25 :1;
    __R uint32_t bit26 :1;
    __R uint32_t bit27 :1;
    __R uint32_t bit28 :1;
    __R uint32_t bit29 :1;
    __R uint32_t bit30 :1;
    __R uint32_t bit31 :1;
  } R;

  struct {
    __RW uint32_t bits0y1 :2;
    __RW uint32_t bits2y3 :2;
    __RW uint32_t bits4y5 :2;
    __RW uint32_t bits6y7 :2;
    __RW uint32_t bits8y9 :2;
    __RW uint32_t bits10y11 :2;
    __RW uint32_t bits12y13 :2;
    __RW uint32_t bits14y15 :2;
    __RW uint32_t bits16y17 :2;
    __RW uint32_t bits18y19 :2;
    __RW uint32_t bits20y21 :2;
    __RW uint32_t bits22y23 :2;
    __RW uint32_t bits24y25 :2;
    __RW uint32_t bits26y27 :2;
    __RW uint32_t bits28y29 :2;
    __RW uint32_t bits30y31 :2;
  };

  // Estructuras para el ADC
  struct {
    __RW uint32_t bits0a7 :8;
    __RW uint32_t bits8a15 :8;
    __RW uint32_t bits16 :1;
    __RW uint32_t bits17a20 :4;
    __RW uint32_t bits21 :1;
    __RW uint32_t bits22a23 :2;
    __RW uint32_t bits24a26 :3;
    __RW uint32_t bits27 :1;
    __RW uint32_t bits28a31 :4;
  };

  struct {
    __RW uint32_t bits0a3 :4;
    __RW uint32_t bits4a15 :12;
    __RW uint32_t bits16a23 :8;
    __RW uint32_t bits24a26 :3;
    __RW uint32_t bits27a29 :3;
    __RW uint32_t bits30 :1;
    __RW uint32_t bits31 :1;
  } ADC2;

  struct {
    __RW uint32_t bits0a3 :4;
    __RW uint32_t bits4a7 :4;
    __RW uint32_t bits8a11 :4;
  } ADC3;

} register_t2;












//CONFIG CHECK!
#if !( defined(USE_UART0) ^ defined(USE_UART1) )
#error "Seleccioner UART0 o UART1, una y no ambas"
#endif

#if !( defined(USE_SW1_WITH_INTERRUPTS) ^ defined(USE_SW1_WITH_POLLING) )
#error "Seleccioner SW1 con interrupciones o polling, una y no ambas"
#endif

#if !( defined(USE_SW4_WITH_INTERRUPTS) ^ defined(USE_SW4_WITH_POLLING) )
#error "Seleccioner SW4 con interrupciones o polling, una y no ambas"
#endif


/*
 * END SYSTEM CONFIGURATION
 * */


#endif /* SYSDEFS_H */
