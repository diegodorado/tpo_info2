/*
 * keyboard.h
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

//include firmware
#include "fw.h"



#define     EINT0_PIN       2,10
#define     EINT1_PIN       2,11
#define     EINT2_PIN       2,12
#define     EINT3_PIN       2,13

void keyboard_setup(void);
void keyboard_handle_key(uint8_t);

#endif /* KEYBOARD_H_ */
