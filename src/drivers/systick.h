/*
 * systick.h
 *
 *  Created on: Sep 21, 2014
 *      Author: diego
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

//include firmware
#include "fw.h"

#define CALLBACKS_QUEUE_SIZE 32 // 32 bits de control

void systick_setup(void);
void systick_handle_tick(void);
void systick_delay_sync(uint32_t);
void systick_delay_async(uint32_t, char, void (*)( void));

#endif /* SYSTICK_H_ */
