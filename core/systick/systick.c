#include "systick.h"


volatile static int us = 0;
void SysTick_Handler(void)
{
	us++;
}
