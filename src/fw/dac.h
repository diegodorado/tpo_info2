#ifndef DAC_H
#define DAC_H

#include "sysdefs.h"
#include "periph.h"

#define LOW 0
#define HIGH 1


#define DAC_PIN  0,26
#define DAC_FUNC  2


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


#define dac ((dac_t *) 0x4008C000UL)


void dac_init(void);
void dac_set_value(uint32_t);


#endif /* DAC_H */
