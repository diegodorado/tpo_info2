/*
===============================================================================
RegsLPC1769
===============================================================================
*/
#ifndef REGS_H_
#define REGS_H_

#include "sysdefs.h"


// REGISTROS ------------------------------------------------------------------------------------------------------------------------
#define		PINSEL			( ( registro_t  * ) 0x4002C000UL )		//!< Direccion de inicio de los registros PINSEL
#define		PINMODE			( ( registro_t  * ) 0x4002C040UL )		//!< Direccion de inicio de los registros de modo de los pines del GPIO
#define		GPIO			( ( registro_t  * ) 0x2009C000UL )		//!< Direccion de inicio de los registros de GPIOs
#define		SYSTICK			( ( registro_t  * ) 0xE000E010UL )		//!< Direccion de inicio de los registros de SYSTICK


#define		TIMER0			( ( registro_t  * ) 0x40004000UL )

#define		T0IR			TIMER0[ 0 ]			/** IR - INTERRUPT REGISTER */
	#define		IRMR0		0
	#define		IRMR1		1
	#define		IRMR2		2
	#define		IRMR3		3
	#define		IRCR0		4
	#define		IRCR1		5
#define		T0TCR			TIMER0[ 1 ]			/** TCR - TIMER CONTROL REGISTER */
	#define		CE			0
	#define		CR			1
#define		T0TC			TIMER0[ 2 ]			/** TC - TIMER COUNTER REGISTER */
#define		T0PR			TIMER0[ 3 ]			/** PR - PRESCALE REGISTER */
#define		T0PC			TIMER0[ 4 ]			/** PC - PRESCALE COUNTER REGISTER */
#define		T0MCR			TIMER0[ 5 ]			/** MCR - MATCH CONTROL REGISTER */
	#define		MR0I			0
	#define		MR0R			1
	#define		MR0S			2
	#define		MR1I			3
	#define		MR1R			4
	#define		MR1S			5
	#define		MR2I			6
	#define		MR2R			7
	#define		MR2S			8
	#define		MR3I			9
	#define		MR3R			10
	#define		MR3S			11
#define		T0MR0			TIMER0[ 6 ]			/** MR - MATCH CONTROL REGISTER */
#define		T0MR1			TIMER0[ 7 ]
#define		T0MR2			TIMER0[ 8 ]
#define		T0MR3			TIMER0[ 9 ]
#define		T0CCR			TIMER0[ 10 ]
#define		T0CR0			TIMER0[ 11 ]
#define		T0CR1			TIMER0[ 12 ]
//los siguientes dos registros NO estan contigüos. Por ende no se continúa con
//el offset
#define		T0EMR			( * ( ( registro_t  * ) 0x4000403CUL ) )
#define		T0CTCR			( * ( ( registro_t  * ) 0x40004070UL ) )/** CTCR - COUNT CONTROL REGISTER */
	#define		TCM			0
	#define		CIS			2


// NVIC ----------------------------------------------------------------------------------------------
#define		NVIC_TIMER0		1
// Nested Vectored Interrupt Controller (NVIC)
// 0xE000E100UL : Direccion de inicio de los registros de habilitación (set) de interrupciones en el NVIC:
#define		ISER		( ( registro_t  * ) 0xE000E100UL )
//!< 0xE000E180UL : Direccion de inicio de los registros de deshabilitacion (clear) de interrupciones en el NVIC:
#define		ICER		( ( registro_t  * ) 0xE000E180UL )

// Registros ISER:
#define		ISER0		ISER[0]
#define		ISER1		ISER[1]

#define  ISE_EINT3 ISER[0] |= (0x00000001 << 21)  //ISER0->bit21 pongo un 1 en el bit 21 para habilitar la INT EINT3
#define  ISE_EINT2 ISER[0] |= (0x00000001 << 20)  //ISER0->bit20 pongo un 1 en el bit 20 para habilitar la INT EINT2
#define  ISE_EINT1 ISER[0] |= (0x00000001 << 19)  //ISER0->bit19 pongo un 1 en el bit 19 para habilitar la INT EINT1
#define  ISE_EINT0 ISER[0] |= (0x00000001 << 18)  //ISER0->bit18 pongo un 1 en el bit 18 para habilitar la INT EINT0


// Registros ICER:
#define		ICER0		ICER[0]
#define		ICER1		ICER[1]

//  Power Control for Peripherals register (PCONP - 0x400F C0C4) [pag. 62 user manual LPC1769]
// 0x400FC0C4UL : Direccion de inicio del registro de habilitación de dispositivos:
#define 	PCONP	(* ( ( registro_t  * ) 0x400FC0C4UL ))

// Peripheral Clock Selection registers 0 and 1 (PCLKSEL0 -0x400F C1A8 and PCLKSEL1 - 0x400F C1AC) [pag. 56 user manual]
// 0x400FC1A8UL : Direccion de inicio de los registros de seleccion de los CLKs de los dispositivos:
#define		PCLKSEL		( ( registro_t  * ) 0x400FC1A8UL )
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
#define    EXTPOLAR        ( (uint32_t  * ) 0x400FC14C )
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
 * Inicio de Systick
 * */

typedef struct
{
 union
 {
  __RW uint32_t  _STCTRL ;                         /*!< Offset: 0x00  SysTick Control and Status Register */
  struct
  {
   __RW uint32_t _ENABLE:1;
   __RW uint32_t _TICKINT:1;
   __RW uint32_t _CLKSOURCE:1;
   __RW uint32_t _RESERVED0:14;
   __RW uint32_t _COUNTFLAG:1;
   __RW uint32_t _RESERVED1:14;
  };
 };
 __RW uint32_t _STRELOAD;                   /*!< Offset: 0x04  SysTick Reload Value Register       */
 __RW uint32_t _STCURR ;                    /*!< Offset: 0x08  SysTick Current Value Register      */
 __R  uint32_t _STCALIB ;                   /*!< Offset: 0x0C  SysTick Calibration Register        */
} systick_t;

#define systick             ((systick_t *)       (0xE000E010))

#define MAX_TICKS 0x00fffffful
#define STCTRL  systick->_STCTRL
 #define ENABLE  systick->_ENABLE
 #define TICKINT  systick->_TICKINT
 #define CLKSOURCE systick->_CLKSOURCE
 #define COUNTFLAG systick->_COUNTFLAG
#define STRELOAD systick->_STRELOAD
#define STCURR      systick->_STCURR
#define STCALIB  systick->_STCALIB

/*
 * Fin de Systick
 * */

#endif /* REGS_H_ */
