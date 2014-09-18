#include "../../sysdefs.h"
#include "../lpc_1769.h"

typedef struct
{
	union
	{
		__RW uint32_t CTRL;
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
	__RW uint32_t RELOAD;
	__RW uint32_t CURR;
	__R uint32_t CALIB;
} systick_t;


#define systick ((systick_t *) SYSTICK)
