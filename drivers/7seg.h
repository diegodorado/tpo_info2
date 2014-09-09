/*
 *
 * Hexadecimal encodings for displaying the digits 0 to F
 *
  Digit	gfedcba	abcdefg
	0	0×3F	0×7E
	1	0×06	0×30
	2	0×5B	0×6D
	3	0×4F	0×79
	4	0×66	0×33
	5	0×6D	0×5B
	6	0×7D	0×5F
	7	0×07	0×70
	8	0×7F	0×7F
	9	0×6F	0×7B
	A	0×77	0×77
	b	0×7C	0×1F
	C	0×39	0×4E
	d	0×5E	0×3D
	E	0×79	0×4F
	F	0×71	0×47

*/
#include "sysdefs.h"

#define		segmento_a		2,7
#define		segmento_b		2,6
#define		segmento_c		2,5
#define		segmento_d		2,4
#define		segmento_e		2,3
#define		segmento_f		2,2
#define		segmento_g		2,1
#define		segmento_dp		2,0

extern volatile uint8_t bufferDisplay;		//!< Buffer de display

void displayDigit(void);

