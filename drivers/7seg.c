#include "7seg.h"
#include "core/gpio/gpio.h"
volatile uint8_t bufferDisplay = 0;

uint8_t bcd_7seg[] = { 0x3f, 0x06, 0x5B, 0x4f, 0x66, 0x6D, 0x7C, 0x07, 0x7f, 0x67};

void displayDigit(void)
{
	//negado, porque el display es catodo comun
	uint32_t aux = ~bcd_7seg[bufferDisplay];

	//armo el dato a mostrar
	SetPIN(segmento_a, (aux & 0x01));
	SetPIN(segmento_b, (( aux >> 1 ) & 0x01));
	SetPIN(segmento_c, (( aux >> 2 ) & 0x01));
	SetPIN(segmento_d, (( aux >> 3 ) & 0x01));
	SetPIN(segmento_e, (( aux >> 4 ) & 0x01));
	SetPIN(segmento_f, (( aux >> 5 ) & 0x01));
	SetPIN(segmento_g, (( aux >> 6 ) & 0x01));
	SetPIN(segmento_dp,(( aux >> 7 ) & 0x01));


}
