/*
 * timers.h
 *
 *  Created on: Oct 15, 2014
 *      Author: diego
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "sysdefs.h"
#include "periph.h"



#define  MATCH0_0   1000
#define  MATCH0_1   (MATCH0_0 + 100)
#define  MATCH0_2   (MATCH0_0 + 500)
#define  MATCH0_3   (MATCH0_0 + 2000)
#define  PREESCALER0 10000

void timer0_setup(void);
void  timer0_mr0_interrupt(void);


#endif /* TIMERS_H_ */
