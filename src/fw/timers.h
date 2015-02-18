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

void timer0_setup(void);
void  timer0_mr0_interrupt(void);
uint32_t timer0_us(void);
void  timer0_delay_us(uint32_t us);


void timer1_setup(void);
void timer1_mr0_interrupt(void);
void timer1_set_sample_rate(uint32_t srate);

#endif /* TIMERS_H_ */
