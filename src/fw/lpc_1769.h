/*
===============================================================================
RegsLPC1769
===============================================================================
*/
#ifndef REGS_H_
#define REGS_H_

#include "sysdefs.h"


// REGISTROS ------------------------------------------------------------------------------------------------------------------------
#define		PINSEL			( ( register_t  * ) 0x4002C000UL )		// Direccion de inicio de los registros PINSEL
#define		PINMODE		( ( register_t  * ) 0x4002C040UL )		// Direccion de inicio de los registros de modo de los pines del GPIO
#define		GPIO			  ( ( register_t  * ) 0x2009C000UL )		// Direccion de inicio de los registros de GPIOs



// NVIC ----------------------------------------------------------------------------------------------
// Nested Vectored Interrupt Controller (NVIC)
// 0xE000E100UL : Direccion de inicio de los registros de habilitación (set) de interrupciones en el NVIC:
#define		ISER		( ( register_t  * ) 0xE000E100UL )
//!< 0xE000E180UL : Direccion de inicio de los registros de deshabilitacion (clear) de interrupciones en el NVIC:
#define		ICER		( ( register_t  * ) 0xE000E180UL )

// Registros ISER:
#define		ISER0		ISER[0]
#define		ISER1		ISER[1]

// Registros ICER:
#define		ICER0		ICER[0]
#define		ICER1		ICER[1]

//  Power Control for Peripherals register (PCONP - 0x400F C0C4) [pag. 62 user manual LPC1769]
// 0x400FC0C4UL : Direccion de inicio del registro de habilitación de dispositivos:
#define 	PCONP	(* ( ( register_t  * ) 0x400FC0C4UL ))

// Peripheral Clock Selection registers 0 and 1 (PCLKSEL0 -0x400F C1A8 and PCLKSEL1 - 0x400F C1AC) [pag. 56 user manual]
// 0x400FC1A8UL : Direccion de inicio de los registros de seleccion de los CLKs de los dispositivos:
#define		PCLKSEL		( ( register_t  * ) 0x400FC1A8UL )
// Registros PCLKSEL
#define		PCLKSEL0	PCLKSEL[0]
#define		PCLKSEL1	PCLKSEL[1]






/**
 * Inicio de Interrumpciones Externas
 */
//!<Registro EXTMODE : Para seleccionar si la ISR Externa activa por flanco ó nivel
#define  EXTMODE   ( (uint32_t  * ) 0x400FC148UL )


#define  EXTMODE3_F  EXTMODE[0] |= 0x00000001 << 3  // EINT3 por flanco
#define  EXTMODE2_F  EXTMODE[0] |= 0x00000001 << 2  // EINT2 por flanco
#define  EXTMODE1_F  EXTMODE[0] |= 0x00000001 << 1  // EINT1 por flanco
#define  EXTMODE0_F  EXTMODE[0] |= 0x00000001       // EINT0 por flanco

//!<Registro EXTPOLAR : selecciona Polaridad del EXTMODE
#define    EXTPOLAR        ( (uint32_t  * ) 0x400FC14CUL )
#define    EXTPOLAR3_P      EXTPOLAR[0] |= 0X00000001 << 3 // Flanco ó Nivel Positivo
#define    EXTPOLAR2_P      EXTPOLAR[0] |= 0X00000001 << 2 // Flanco ó Nivel Positivo
#define    EXTPOLAR1_P      EXTPOLAR[0] |= 0X00000001 << 1 // Flanco ó Nivel Positivo
#define    EXTPOLAR0_P      EXTPOLAR[0] |= 0X00000001      // Flanco ó Nivel Positivo

//!<Registros ICER: Para deshabilitar las Interupciones Se desactivan con 1 Escribiendo un 0 no hace nada
#define  ICER0  ICER[0]
#define  ICER1  ICER[1]
#define  ICE_EINT3 ICER0 |= (0x00000001 << 21) // deshabilito a EINT3
#define  ICE_EINT2 ICER0 |= (0x00000001 << 20) // deshabilito a EINT2
#define  ICE_EINT1 ICER0 |= (0x00000001 << 19) // deshabilito a EINT1
#define  ICE_EINT0 ICER0 |= (0x00000001 << 18) // deshabilito a EINT0

//!<Registro EXINT: Registro de Flags para limpiar la ISR
#define  EXTINT   ( (uint32_t  * ) 0x400FC140UL )


//!<_____________________________________ Macros ISR _____________________________________
#define  CLR_EINT3  EXTINT[0] |= 0x00000001 << 3 // bajo el flag de EINT3
#define  CLR_EINT2  EXTINT[0] |= 0x00000001 << 2 // bajo el flag de EINT2
#define  CLR_EINT1  EXTINT[0] |= 0x00000001 << 1 // bajo el flag de EINT1
#define  CLR_EINT0  EXTINT[0] |= 0x00000001      // bajo el flag de EINT0


//!<_____________________________________ Registros de GPIO para usarse como ISR _____________________________________
//!< Configuración
#define      IO0IntEnR  ( (uint32_t  * ) 0x40028090UL ) // Puerto 0 como flanco ascendente
#define      IO2IntEnR  ( (uint32_t  * ) 0x400280B0UL )//  Puerto 2 como flanco ascendente
#define      IO0IntEnF  ( (uint32_t  * ) 0x40028094UL )// Puerto 0 como flanco descendente
#define      IO2IntEnF  ( (uint32_t  * ) 0x400280B4UL )// Puerto 0 como flanco ascendente

//!< Estado
#define     IO0IntStatR  ( (uint32_t  * ) 0x40028084UL ) //Estado de los flags de interr flanco ascendente bits Puerto 0
#define     IO2IntStatR  ( (uint32_t  * ) 0x400280A4UL ) //Estado de los flags de interr flanco ascendente bits Puerto 2
#define     IO0IntStatF  ( (uint32_t  * ) 0x40028088UL ) //Estado de los flags de interr flanco descendente bits Puerto 0
#define     IO2IntStatF  ( (uint32_t  * ) 0x400280A8UL ) //Estado de los flags de interr flanco descendente bits Puerto 2
#define     IOIntStatus  ( (uint32_t  * ) 0x40028080UL ) //Estado de los flags de interr de bits Puerto 2 y Puerto 0

//!<Bajo flags de Interr por GPIO
#define     IO0IntClr  ( (uint32_t  * ) 0x4002808CUL ) //Bajo flags de Interr Puerto 0
#define     IO2IntClr  ( (uint32_t  * ) 0x400280ACUL ) //Bajo flags de Interr Puerto 2



/**
 * Fin de Interrumpciones Externas
 */





/*
 * Inicio de Timers
 * */

// Table 425. TIMER/COUNTER0-3 register map
typedef struct
{
  union{
    __RW uint32_t IR;                // Interrupt Register
    struct{
      // Table 426. Interrupt Register
      __RW uint32_t IR_MR0:1;    // Interrupt Interrupt flag for match channel 0. 0
      __RW uint32_t IR_MR1:1;    // Interrupt flag for match channel 1. 0
      __RW uint32_t IR_MR2:1;    // Interrupt flag for match channel 2. 0
      __RW uint32_t IR_MR3:1;    // Interrupt flag for match channel 3. 0
      __RW uint32_t IR_CR0:1;    // Interrupt flag for capture channel 0 event. 0
      __RW uint32_t IR_CR1:1;    // Interrupt flag for capture channel 1 event. 0
    };
  };
  union{
    __RW uint32_t TCR;               // Timer Control Register
    struct{
      // Table 427. Timer Control Register
      __RW uint32_t TCR_ENABLED:1;    // Counter Enable
      __RW uint32_t TCR_RESET:1;    //  Counter Reset
    };
  };

  __RW uint32_t TC;                // Timer Counter
  __RW uint32_t PR;                // Prescale Register
  __RW uint32_t PC;                // Prescale Counter

  union{
    __RW uint32_t MCR;               // Match Control Register
    struct{
      // Table 429. Match Control Register
      __RW uint32_t MCR_MR0I:1;     // Interrupt on MR0
      __RW uint32_t MCR_MR0R:1;     //  Reset on MR0
      __RW uint32_t MCR_MR0S:1;     //  Stop on MR0
      __RW uint32_t MCR_MR1I:1;     // Interrupt on MR1
      __RW uint32_t MCR_MR1R:1;     //  Reset on MR1
      __RW uint32_t MCR_MR1S:1;     //  Stop on MR1
      __RW uint32_t MCR_MR2I:1;     // Interrupt on MR2
      __RW uint32_t MCR_MR2R:1;     //  Reset on MR2
      __RW uint32_t MCR_MR2S:1;     //  Stop on MR2
      __RW uint32_t MCR_MR3I:1;     // Interrupt on MR3
      __RW uint32_t MCR_MR3R:1;     //  Reset on MR3
      __RW uint32_t MCR_MR3S:1;     //  Stop on MR3
    };
  };

  __RW uint32_t MR0;               // Match Register 0
  __RW uint32_t MR1;               // Match Register 1
  __RW uint32_t MR2;               // Match Register 2
  __RW uint32_t MR3;               // Match Register 3
  __RW uint32_t CCR;               // Capture Control Register
  __RW uint32_t CR0;               // Capture Register 0
  __RW uint32_t CR1;               // Capture Register 1
  __RW uint32_t RESERVED0[2];
  __RW uint32_t EMR;               // External Match Register
  __RW uint32_t RESERVED1[12];
  __RW uint32_t CTCR;              // Count Control Register
} timer_t;


#define  TIMER0 ( ( timer_t  * ) 0x40004000UL )


/*
 * Fin de Timers
 * */




/*
 * Inicio de Systick
 * */

typedef struct
{
 union
 {
  __RW uint32_t  STCTRL ;                         /*!< Offset: 0x00  SysTick Control and Status Register */
  struct
  {
   __RW uint32_t ENABLE:1;
   __RW uint32_t TICKINT:1;
   __RW uint32_t CLKSOURCE:1;
   __RW uint32_t RESERVED0:14;
   __RW uint32_t COUNTFLAG:1;
   __RW uint32_t RESERVED1:14;
  };
 };
 __RW uint32_t STRELOAD;                   /*!< Offset: 0x04  SysTick Reload Value Register       */
 __RW uint32_t STCURR ;                    /*!< Offset: 0x08  SysTick Current Value Register      */
 __R  uint32_t STCALIB ;                   /*!< Offset: 0x0C  SysTick Calibration Register        */
} systick_t;

#define SYSTICK   ((systick_t *) 0xE000E010UL)


/*
 * Fin de Systick
 * */



/*
 * Inicio de UART
 **/



// Table 280: UARTn Line Status Register bit description
typedef struct{
  __RW uint32_t RDR:1;           //Receiver Data Ready
  __RW uint32_t OE:1;            // Overrun Error
  __RW uint32_t PE:1;            // Parity Error
  __RW uint32_t FE:1;            // Framing Error
  __RW uint32_t BI:1;            // Break Interrupt
  __RW uint32_t THRE:1;          // Transmitter Holding Register Empty
  __RW uint32_t TEMT:1;          // Transmitter Empty
  __RW uint32_t RXFE:1;          // Error in RX FIFO
  __RW uint32_t RESERVED0:24;    // For alignement purposes
} uart_lsr_t;


//Table 295: UART1 Interrupt Identification Register (U1IIR - address 0x4001 0008) bit description
typedef struct{
  __RW uint32_t INT_STATUS:1;  //Interrupt status. Note that it is active low ( 1 : No interrupt is pending.)
  __RW uint32_t INT_ID:3;      //Interrupt identification.
  __RW uint32_t NOT_USED0:28;  //not used in this project
} uart_iir_t;


typedef struct
{
  union{
    __R  uint32_t RESERVED0;         // For alignement purposes
    __RW uint8_t DLL;                // Parte baja del divisor de la UART1
    __R  uint8_t RBR;                // Registro de recepcion de la UART1
    __W  uint8_t THR;                // Registro de transmision de la UART1
  };
  union{
    __RW uint8_t DLM;                // Parte alta del divisor de la UART1
    __W  uint32_t IER;               // Registro habilitacion de interrupciones de la UART1
  };
  union{
    __R  uart_iir_t IIR;             // Registro de identificación de la interrupción de la UART1
    __W  uint32_t FCR;               // Registro de control de la FIFO de la UART1
  };

  __RW uint32_t LCR;                 // Line CONTROL Register de la UART1
  __R  uint32_t RESERVED1;           // For alignement purposes
  __R  uart_lsr_t LSR;                 // Line STATUS Register de la UART1

} uart_t;



#define UART_INT_ID_RLS  0b011 //Receive Line Status
#define UART_INT_ID_RDA  0b010 //Receive Data Available
#define UART_INT_ID_CTI  0b110 //Character Time-out Indicator
#define UART_INT_ID_THRE 0b001 //THRE Interrupt.
//End Table 295



#define  UART0 ( ( uart_t  * ) 0x4000C000UL )  // UART0
#define  UART1 ( ( uart_t  * ) 0x40010000UL )  // UART1



/*
 * Fin de UART
 **/


/*
 * Inicio de DAC
 **/


/*Table 538. DAC registers*/
typedef struct
{
 union
 {
  /* D/A Converter Register. This register contains the digital value to be R/W 0 converted to analog and a power control bit. */
  __RW uint32_t DACR;

  /* Table 539: D/A Converter Register (DACR - address 0x4008 C000) bit description */

  struct
  {
   /* Reserved, user software should not write ones to reserved bits. The value read from a NA reserved bit is not defined. */
   __R uint32_t RESERVED0:6;
   //After the selected settling time after this field is written with a new VALUE, the voltage on 0 the AOUT pin (with respect to VSSA) is VALUE × ((VREFP - VREFN)/1024) + VREFN.
   __RW  uint32_t VALUE:10;
   //The settling time of the DAC is 1 μs max, and the maximum current is 700 μA. This allows 0 a maximum update rate of 1 MHz.
   //The settling time of the DAC is 2.5 μs and the maximum current is 350 μA. This allows a maximum update rate of 400 kHz.
   __RW uint32_t BIAS:1;
   //Reserved, user software should not write ones to reserved bits. The value read from a NA reserved bit is not defined.
   __R uint32_t RESERVED1:15;

  };
 };
 //DAC Control register. This register controls DMA and timer operation. R/W 0
 __RW uint32_t DACCTRL;
 //DAC Counter Value register. This register contains the reload value for R/W 0 the DAC DMA/Interrupt timer.
 __RW uint32_t DACCNTVAL;
} dac_t;


#define DAC ((dac_t *) 0x4008C000UL)



/*
 * Fin de DAC
 **/



#endif /* REGS_H_ */
