#ifndef SYSDEFS_H
#define SYSDEFS_H

#define __R volatile const
#define __W volatile
#define __RW volatile

typedef 	unsigned int 		uint32_t;
typedef 	unsigned short 		uint16_t;
typedef 	unsigned char 		uint8_t;
typedef 	__RW uint32_t 		register_t;  //!< defino un tipo 'registro'.


/*
 * SYSTEM CONFIGURATION...
 * */

//#define DEBUG_ON

//UART
//uncomment to use uart0 or comment to use uart1
//#define USE_UART0
#define USE_UART1

//KEYBOARD
//#define USE_SW1_WITH_INTERRUPTS
//#define USE_SW4_WITH_INTERRUPTS
#define USE_SW1_WITH_POLLING
#define USE_SW2_WITH_POLLING
#define USE_SW3_WITH_POLLING
#define USE_SW4_WITH_POLLING


#define USE_SW_WITH_POLLING defined(USE_SW1_WITH_POLLING) || defined(USE_SW2_WITH_POLLING) || defined(USE_SW3_WITH_POLLING) || defined(USE_SW4_WITH_POLLING)



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
